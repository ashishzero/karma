#include "karma.h"       
#include "systems.h"     
#include "gfx_renderer.h"
#include "length_string.h"
#include "lin_maths.h"
#include "imgui/imgui.h"
#include "debug.h"
#include "particle_system.h"
#include "stream.h"
#include "audio.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

constexpr s32 MAX_SPEED_FACTOR = 64;

struct Time_Speed_Factor {
	s32 numerator   = 1;
	s32 demonimator = 1;
	r32 ratio       = 1;
};

void increase_game_speed(Time_Speed_Factor *factor) {
	if (factor->demonimator == 1) {
		factor->numerator = min_value(factor->numerator * 2, MAX_SPEED_FACTOR);
	} else {
		factor->demonimator = max_value(factor->demonimator / 2, 1);
	}
	factor->ratio = (r32)factor->numerator / (r32)factor->demonimator;
}

void decrease_game_speed(Time_Speed_Factor *factor) {
	if (factor->numerator == 1) {
		factor->demonimator = min_value(factor->demonimator * 2, MAX_SPEED_FACTOR);
	} else {
		factor->numerator = max_value(factor->numerator / 2, 1);
	}
	factor->ratio = (r32)factor->numerator / (r32)factor->demonimator;
}

enum Time_State {
	Time_State_RESUME,
	Time_State_PAUSE,
};

struct Audio {
	Audio_Stream *stream; // TODO: Use somekind of reference
	booli         playing;
	booli         loop;
	r32           pitch_factor;
	r32           volume; // TODO: Seperate channel volume?
	r32           samples_played;
	r32           buffered_pitch_factor;
};

struct Audio_Node {
	Audio audio;

	Audio_Node *next;
};

struct Audio_Mixer {
	u32 samples_rate;
	u32 channel_count;

	Handle     mutex;
	Audio_Node list; // NOTE: This is sentinel (first indicator), real node start from list.next
	u32        buffer_size_in_sample_count;
	u32        buffer_size_in_bytes;
	u32        buffer_consumed_bytes;
	r32 *      buffer;

	r32 volume_a;
	r32 volume_b;
	r32 volume_position; // in samples
	r32 volume_span;     // in samples
	r32 pitch_factor;
};

Audio_Mixer audio_mixer() {
	Audio_Mixer mixer;

	mixer.samples_rate  = system_audio_sample_rate();
	mixer.channel_count = system_audio_channel_count();

	// TODO: We only support these currently
	assert(mixer.samples_rate == 48000);
	assert(mixer.channel_count == 2);

	mixer.mutex                       = system_create_mutex();
	mixer.list                        = {};
	mixer.buffer_size_in_sample_count = (mixer.samples_rate * SYSTEM_AUDIO_BUFFER_SIZE_IN_MILLISECS) / 1000;
	mixer.buffer_size_in_bytes        = sizeof(r32) * mixer.buffer_size_in_sample_count * mixer.channel_count;
	mixer.buffer                      = (r32 *)tallocate(mixer.buffer_size_in_bytes);
	mixer.buffer_consumed_bytes       = 0;

	mixer.volume_a        = 0.5f;
	mixer.volume_b        = 0.5f;
	mixer.volume_position = 1;
	mixer.volume_span     = 1;
	mixer.pitch_factor    = 1;

	return mixer;
}

void audio_mixer_set_volume(Audio_Mixer *mixer, r32 volume) {
	mixer->volume_a = mixer->volume_b = volume;
}

void audio_mixer_animate_volume_to(Audio_Mixer *mixer, r32 volume, r32 secs) {
	mixer->volume_a        = lerp(mixer->volume_a, mixer->volume_b, clamp01(mixer->volume_position / mixer->volume_span));
	mixer->volume_b        = volume;
	mixer->volume_position = 0;
	mixer->volume_span     = mixer->samples_rate * secs;
}

void audio_mixer_animate_volume(Audio_Mixer *mixer, r32 volume_a, r32 volume_b, r32 secs) {
	mixer->volume_a        = volume_a;
	mixer->volume_b        = volume_b;
	mixer->volume_position = 0;
	mixer->volume_span     = mixer->samples_rate * secs;
}

Audio *play_audio(Audio_Mixer *mixer, Audio_Stream *stream, bool loop) {
	// TODO: Do allocations properly!!
	Audio_Node *node                  = new Audio_Node;
	node->audio.stream                = stream;
	node->audio.playing               = true;
	node->audio.loop                  = loop;
	node->audio.pitch_factor          = 1;
	node->audio.volume                = 1;
	node->audio.samples_played        = 0;
	node->audio.buffered_pitch_factor = 1;

	node->next       = mixer->list.next;
	mixer->list.next = node;

	assert(node == (Audio_Node *)&node->audio); // This has to be same so as to remove audio from list easily

	return &node->audio;
}

void system_refresh_audio_device(u32 sample_rate, u32 channel_count, void *user_data) {
	Audio_Mixer *mixer = (Audio_Mixer *)user_data;

	system_lock_mutex(mixer->mutex, WAIT_INFINITE);

	mixer->samples_rate  = sample_rate;
	mixer->channel_count = channel_count;

	// TODO: We only support these currently
	assert(mixer->samples_rate == 48000);
	assert(mixer->channel_count == 2);

	mixer->buffer_size_in_sample_count = (mixer->samples_rate * SYSTEM_AUDIO_BUFFER_SIZE_IN_MILLISECS) / 1000;
	mixer->buffer_size_in_bytes        = sizeof(r32) * mixer->buffer_size_in_sample_count * mixer->channel_count;
	mixer->buffer                      = (r32 *)tallocate(mixer->buffer_size_in_bytes);
	mixer->buffer_consumed_bytes       = 0;

	system_unlock_mutex(mixer->mutex);
}

void system_update_audio(const System_Audio &sys_audio, void *user_data) {
	Audio_Mixer *mixer = (Audio_Mixer *)user_data;
	system_lock_mutex(mixer->mutex, WAIT_INFINITE);

	// TODO: Don't hard-code this!, Query and use!!
	u32 channel_count = 2;

	u32 sample_count;
	u8 *sys_buffer = sys_audio.lock(sys_audio.handle, &sample_count);
	if (sys_buffer) {
		u32 buffer_size  = sizeof(r32) * channel_count * sample_count;
		u8 *src_ptr      = (u8 *)mixer->buffer + mixer->buffer_consumed_bytes;
		u8 *one_past_end = (u8 *)mixer->buffer + mixer->buffer_size_in_bytes;
		assert(one_past_end >= src_ptr);
		u32 copy_size = min_value((u32)(one_past_end - src_ptr), buffer_size);
		memcpy(sys_buffer, src_ptr, copy_size);
		memset(sys_buffer + copy_size, 0, buffer_size - copy_size);
		sys_audio.unlock(sys_audio.handle, sample_count);

		Debug_AudioFeedback((r32 *)src_ptr, copy_size, channel_count, buffer_size - copy_size);

		mixer->buffer_consumed_bytes += copy_size;
		mixer->volume_position += sample_count * mixer->pitch_factor;

		for (Audio_Node *node = mixer->list.next; node; node = node->next) {
			Audio &audio = node->audio;
			if (audio.playing) {
				audio.samples_played += audio.buffered_pitch_factor * (r32)sample_count;
				while ((u32)lroundf(audio.samples_played) >= audio.stream->sample_count) {
					audio.samples_played -= audio.stream->sample_count;
					audio.playing = audio.loop;
				}
			}
		}
	}

	system_unlock_mutex(mixer->mutex);
}

void audio_mixer_update(Audio_Mixer *mixer) {
	constexpr r32 INVERSE_RANGE_S16 = 2.0f / (r32)((r32)MAX_INT16 - (r32)MIN_INT16);

	system_lock_mutex(mixer->mutex, WAIT_INFINITE);

	// TODO: Fix me!! Don't hardcode this!!!!!!
	u32 channel_count = 2;
	memset(mixer->buffer, 0, sizeof(r32) * channel_count * mixer->buffer_size_in_sample_count);
	mixer->buffer_consumed_bytes = 0;

	for (Audio_Node *node = mixer->list.next; node; node = node->next) {
		assert(node->audio.stream->fmt->channels_count == 2);

		if (node->audio.playing) {
			Audio &audio                = node->audio;
			audio.buffered_pitch_factor = audio.pitch_factor * mixer->pitch_factor;

			r32 *write_ptr       = mixer->buffer;
			r32  read_cursor     = audio.samples_played;
			r32  volume_position = mixer->volume_position;

			for (u32 sample_counter = 0; sample_counter < mixer->buffer_size_in_sample_count;) {
				u32 more_samples_required = (mixer->buffer_size_in_sample_count - sample_counter);
				u32 samples_available     = (u32)lroundf(((r32)audio.stream->sample_count - read_cursor) / audio.buffered_pitch_factor);
				u32 samples_to_mix        = min_value(samples_available, more_samples_required);

				r32 effective_volume;

				for (u32 sample_mix_index = 0; sample_mix_index < samples_to_mix; ++sample_mix_index) {
					// TODO: Here we expect both input audio and output buffer to be stero audio

					effective_volume = audio.volume * lerp(mixer->volume_a, mixer->volume_b, clamp01((volume_position + sample_mix_index * mixer->pitch_factor) / mixer->volume_span));

#define AUDIO_APPLY_PITCH_FILTERING

#ifdef AUDIO_APPLY_PITCH_FILTERING

					r32 real_sample_index = read_cursor + sample_mix_index * audio.buffered_pitch_factor;
					u32 sample_index_0    = (u32)(real_sample_index);
					assert(sample_index_0 < audio.stream->sample_count);
					u32 sample_index_1 = (sample_index_0 + 1 == audio.stream->sample_count) ? sample_index_0 : sample_index_0 + 1;

					r32 sampled_left_0  = INVERSE_RANGE_S16 * ((r32)audio.stream->samples[2 * sample_index_0 + 0] - (r32)MIN_INT16) - 1.0f;
					r32 sampled_right_0 = INVERSE_RANGE_S16 * ((r32)audio.stream->samples[2 * sample_index_0 + 1] - (r32)MIN_INT16) - 1.0f;

					r32 sampled_left_1  = INVERSE_RANGE_S16 * ((r32)audio.stream->samples[2 * sample_index_1 + 0] - (r32)MIN_INT16) - 1.0f;
					r32 sampled_right_1 = INVERSE_RANGE_S16 * ((r32)audio.stream->samples[2 * sample_index_1 + 1] - (r32)MIN_INT16) - 1.0f;

					r32 sample_t = real_sample_index - (r32)sample_index_0;

					write_ptr[0] += lerp(sampled_left_0, sampled_left_1, sample_t) * effective_volume;
					write_ptr[1] += lerp(sampled_right_0, sampled_right_1, sample_t) * effective_volume;

#else

					u32 sample_index = lroundf(read_cursor + sample_mix_index * audio.buffered_pitch_factor);
					assert(sample_index < audio.stream->sample_count);

					r32 sampled_left  = INVERSE_RANGE_S16 * ((r32)audio.stream->samples[2 * sample_index + 0] - (r32)MIN_INT16) - 1.0f;
					r32 sampled_right = INVERSE_RANGE_S16 * ((r32)audio.stream->samples[2 * sample_index + 1] - (r32)MIN_INT16) - 1.0f;

					write_ptr[0] += sampled_left * effective_volume;
					write_ptr[1] += sampled_right * effective_volume;

#endif

					write_ptr[0] = clamp(-1.0f, 1.0f, write_ptr[0]);
					write_ptr[1] = clamp(-1.0f, 1.0f, write_ptr[1]);

					write_ptr += channel_count;
				}

				r32 samples_progressed = samples_to_mix * audio.buffered_pitch_factor;

				sample_counter += samples_to_mix;
				read_cursor += samples_progressed;
				volume_position += samples_progressed;

				if (!audio.loop) {
					break;
				} else {
					while ((u32)lroundf(read_cursor) >= audio.stream->sample_count) {
						read_cursor -= audio.stream->sample_count;
					}
				}
			}
		}
	}

	system_unlock_mutex(mixer->mutex);
}

struct Editor {
	bool display = false;
	bool guide   = true;
};

static Editor editor;

Texture2d_Handle debug_load_texture(const char *filepath) {
	stbi_set_flip_vertically_on_load(1);

	int            w, h, n;
	unsigned char *pixels  = stbi_load(filepath, &w, &h, &n, 4);
	auto           texture = gfx_create_texture2d(u32(w), u32(h), u32(n), Data_Format_RGBA8_UNORM_SRGB, (const u8 **)&pixels, Buffer_Usage_IMMUTABLE, 1);
	stbi_image_free(pixels);
	return texture;
}

Monospaced_Font debug_load_font(const String filepath) {
	Monospaced_Font font = {};

	String content = system_read_entire_file(filepath);
	defer{ memory_free(content.data); };

	if (content.count) {
		Istream in = istream(content);

		u32 min_c = *istream_consume(&in, u32);
		u32 max_c = *istream_consume(&in, u32);
		r32 advance = *istream_consume(&in, r32);
		u32 size = *istream_consume(&in, u32);

		font.info.range = (Monospaced_Font_Glyph_Range *)memory_allocate(size);
		memcpy(font.info.range, istream_consume_size(&in, size), size);
		font.info.first = (s32)min_c;
		font.info.count = (s32)(max_c - min_c + 2);
		font.info.advance = advance;

		int w = *istream_consume(&in, int);
		int h = *istream_consume(&in, int);
		int n = *istream_consume(&in, int);
		u8 *pixels = (u8 *)istream_consume_size(&in, w * h * n);

		font.texture = gfx_create_texture2d((u32)w, (u32)h, (u32)n, Data_Format_RGBA8_UNORM_SRGB, (const u8 **)&pixels, Buffer_Usage_IMMUTABLE, 1);
	}

	return font;
}

bool debug_load_mesh(String file, Im_Mesh *mesh) {
	String content = system_read_entire_file(file);

	if (content.count) {
		Istream in = istream(content);

		mesh->vertex_count = *istream_consume(&in, u32);
		mesh->texture_coord_count = *istream_consume(&in, u32);
		mesh->normal_count = *istream_consume(&in, u32);
		mesh->index_count = *istream_consume(&in, u32);

		ptrsize vertex_size = sizeof(Vec3) * mesh->vertex_count;
		ptrsize tex_coord_size = sizeof(Vec2) * mesh->texture_coord_count;
		ptrsize normal_size = sizeof(Vec3) * mesh->normal_count;
		ptrsize index_size = sizeof(Im_Mesh::Index) * mesh->index_count;

		ptrsize mesh_size = 0;
		mesh_size += vertex_size;
		mesh_size += tex_coord_size;
		mesh_size += normal_size;
		mesh_size += index_size;

		mesh->ptr = content.data;

		mesh->vertices = (Vec3 *)istream_consume_size(&in, vertex_size);
		mesh->texture_coords = (Vec2 *)istream_consume_size(&in, tex_coord_size);
		mesh->normals = (Vec3 *)istream_consume_size(&in, normal_size);
		mesh->indices = (Im_Mesh::Index *)istream_consume_size(&in, index_size);

		return true;
	}

	return false;
}



constexpr int  LEVEL_REGION_X_CELL_COUNT			= 25;
constexpr int  LEVEL_REGION_Y_CELL_COUNT			= 15;
constexpr int  LEVEL_REGION_HALF_X_CELL_COUNT		= LEVEL_REGION_X_CELL_COUNT / 2;
constexpr int  LEVEL_REGION_HALF_Y_CELL_COUNT		= LEVEL_REGION_Y_CELL_COUNT / 2;

struct Level_Position {
	Vec2s region;
	Vec2 tile;
};

struct Level_Map {
	struct Cell {
		enum Type {
			EMPTY, WALKABLE
		};

		struct Entity *entity;
		booli beam;

		Type type;
	};

	struct Region {
		Cell cells[LEVEL_REGION_X_CELL_COUNT * LEVEL_REGION_Y_CELL_COUNT];
	};

	Region **regions;
	int x_count, y_count;
};

inline Vec2 level_map_tile_to_cell_r32(Vec2 tile) {
	return tile + vec2((r32)LEVEL_REGION_HALF_X_CELL_COUNT, (r32)LEVEL_REGION_HALF_Y_CELL_COUNT);
}

inline Vec2 level_map_cell_to_tile_r32(Vec2 tile) {
	return tile - vec2((r32)LEVEL_REGION_HALF_X_CELL_COUNT, (r32)LEVEL_REGION_HALF_Y_CELL_COUNT);
}

inline Vec2s level_map_tile_to_cell(r32 tile_x, r32 tile_y) {
	Vec2s cell_index;
	cell_index.x = (int)(tile_x + LEVEL_REGION_HALF_X_CELL_COUNT);
	cell_index.y = (int)(tile_y + LEVEL_REGION_HALF_Y_CELL_COUNT);
	return cell_index;
}

inline Vec2s level_map_tile_to_cell(Vec2 tile) {
	return level_map_tile_to_cell(tile.x, tile.y);
}

inline Vec2 level_map_cell_to_tile(int cell_index_x, int cell_index_y) {
	Vec2 tile;
	tile.x = (r32)(cell_index_x - LEVEL_REGION_HALF_X_CELL_COUNT);
	tile.y = (r32)(cell_index_y - LEVEL_REGION_HALF_Y_CELL_COUNT);
	return tile;
}

inline Vec2 level_map_cell_to_tile(Vec2s cell_index) {
	return level_map_cell_to_tile(cell_index.x, cell_index.y);
}

inline r32 level_map_cell_to_tile_horizontal(int cell_index) {
	return (r32)(cell_index - LEVEL_REGION_HALF_X_CELL_COUNT);
}

inline r32 level_map_cell_to_tile_vertical(int cell_index) {
	return (r32)(cell_index - LEVEL_REGION_HALF_Y_CELL_COUNT);
}

Vec2 level_position_unpack(Level_Position &position, Level_Position &center) {
	Vec2 result;
	result.x = (position.region.x - center.region.x) * LEVEL_REGION_X_CELL_COUNT + (position.tile.x - center.tile.x);
	result.y = (position.region.y - center.region.y) * LEVEL_REGION_Y_CELL_COUNT + (position.tile.y - center.tile.y);
	return result;
}

void level_position_normalize(Level_Position *position) {
	Vec2s cell_index = level_map_tile_to_cell(position->tile);

	if (cell_index.x >= LEVEL_REGION_X_CELL_COUNT) {
		position->region.x += cell_index.x / LEVEL_REGION_X_CELL_COUNT;
		position->tile.x = level_map_cell_to_tile_horizontal(cell_index.x % LEVEL_REGION_X_CELL_COUNT);
	}

	if (cell_index.x < 0) {
		int dec = abs(cell_index.x + 1) / LEVEL_REGION_X_CELL_COUNT + 1;
		position->region.x -= dec;
		position->tile.x = level_map_cell_to_tile_horizontal(LEVEL_REGION_X_CELL_COUNT * dec + cell_index.x);
	}

	if (cell_index.y >= LEVEL_REGION_Y_CELL_COUNT) {
		position->region.y += cell_index.y / LEVEL_REGION_Y_CELL_COUNT;
		position->tile.y = level_map_cell_to_tile_vertical(cell_index.y % LEVEL_REGION_Y_CELL_COUNT);
	}

	if (cell_index.y < 0) {
		int dec = abs(cell_index.y + 1) / LEVEL_REGION_Y_CELL_COUNT + 1;
		position->region.y -= dec;
		position->tile.y = level_map_cell_to_tile_vertical(LEVEL_REGION_Y_CELL_COUNT * dec + cell_index.y);
	}
}

inline bool level_position_are_same(Level_Position &a, Level_Position &b) {
	return  ((a.region.x == b.region.x) && (a.region.y == b.region.y)) && ((a.tile.x == b.tile.x) && (a.tile.y == b.tile.y));
}

inline bool level_position_are_same_region(Level_Position& a, Level_Position& b) {
	return (a.region.x == b.region.x) && (a.region.y == b.region.y);
}

inline Level_Map::Cell *level_map_region_get_cell(Level_Map::Region *region, int cell_x, int cell_y) {
	assert(cell_x >= 0 && cell_x < LEVEL_REGION_X_CELL_COUNT && cell_y >= 0 && cell_y < LEVEL_REGION_Y_CELL_COUNT);
	return region->cells + cell_x + cell_y * LEVEL_REGION_X_CELL_COUNT;
}
inline Level_Map::Cell *level_map_region_get_cell(Level_Map::Region *region, Vec2s cell) {
	return level_map_region_get_cell(region, cell.x, cell.y);
}

inline Level_Map::Cell *level_map_region_get_cell_bounded(Level_Map::Region *region, int cell_x, int cell_y) {
	if (cell_x >= 0 && cell_x < LEVEL_REGION_X_CELL_COUNT && cell_y >= 0 && cell_y < LEVEL_REGION_Y_CELL_COUNT) {
		return region->cells + cell_x + cell_y * LEVEL_REGION_X_CELL_COUNT;
	}
	return nullptr;
}
inline Level_Map::Cell *level_map_region_get_cell_bounded(Level_Map::Region *region, Vec2s cell) {
	return level_map_region_get_cell_bounded(region, cell.x, cell.y);
}

inline Level_Map::Region *level_map_get_region(Level_Map *level_map, int region_x, int region_y) {
	return *(level_map->regions + region_x + region_y * level_map->x_count);
}
inline Level_Map::Region *level_map_get_region(Level_Map *level_map, Vec2s region) {
	return level_map_get_region(level_map, region.x, region.y);
}

inline Level_Map::Region *level_map_get_region_bounded(Level_Map *level_map, int region_x, int region_y) {
	Level_Map::Region *region = nullptr;

	if ((region_x >= 0 && region_x < level_map->x_count) && 
		(region_y >= 0 && region_y < level_map->y_count)) {
		region = *(level_map->regions + region_x + region_y * level_map->x_count);
	}

	return region;
}
inline Level_Map::Region *level_map_get_region_bounded(Level_Map *level_map, Vec2s region) {
	return level_map_get_region_bounded(level_map, region.x, region.y);
}

bool level_map_region_is_present(Level_Map *level_map, int region_x, int region_y) {
	return level_map_get_region_bounded(level_map, region_x, region_y) != nullptr;
}
bool level_map_region_is_present(Level_Map *level_map, Vec2s region) {
	return level_map_region_is_present(level_map, region.x, region.y);
}

struct Camera_Bounds {
	r32 left, right;
	r32 top, bottom;
};

struct Camera {
	Level_Position position;
	Level_Position position_target;

	r32 distance;
	r32 distance_target;

	Quat orientation		= quat_identity();
	Quat orientation_target = orientation;

	r32			aspect_ratio;
	Camera_View view;

	r32 move_speed;
	r32 reach_speed;
	r32 turn_speed;

	// NOTE: Valid only after camera_update is called
	Vec2			render_position;
	Camera_Bounds	bounds;
};

void camera_update(Camera *camera, r32 dt, Level_Position target_position, Level_Map *level_map = nullptr) {
	Level_Position targetp	= camera->position_target;
	targetp.region			= target_position.region;
	targetp.tile			= target_position.tile;

	if (camera->view.kind == Camera_View_Kind::ORTHOGRAPHIC) {
		r32 icamera_zoom = powf(2.0f, camera->distance) / 512.0f;
		r32 half_height = LEVEL_REGION_HALF_Y_CELL_COUNT * icamera_zoom;
		r32 half_width  = half_height * camera->aspect_ratio;

		camera->bounds.left   = -half_width;
		camera->bounds.right  = half_width;
		camera->bounds.top    = half_height;
		camera->bounds.bottom = -half_height;
	} else {
		r32 half_width  = camera->distance;
		r32 half_height = camera->distance / camera->aspect_ratio;

		camera->bounds.left		= -half_width;
		camera->bounds.right	= half_width;
		camera->bounds.top		= half_height;
		camera->bounds.bottom	= -half_height;
	}

	if (level_map) {
		Vec2 camera_min = targetp.tile + vec2(camera->bounds.left,  camera->bounds.bottom);
		Vec2 camera_max = targetp.tile + vec2(camera->bounds.right, camera->bounds.top);

		Vec2 min_cell = level_map_tile_to_cell_r32(camera_min);
		Vec2 max_cell = level_map_tile_to_cell_r32(camera_max);

		if (min_cell.x < 0 && !level_map_region_is_present(level_map, targetp.region.x - 1, targetp.region.y)) {
			targetp.tile.x -= min_cell.x + 1;
		} else if (max_cell.x >= LEVEL_REGION_X_CELL_COUNT && !level_map_region_is_present(level_map, targetp.region.x + 1, targetp.region.y)) {
			targetp.tile.x += (LEVEL_REGION_X_CELL_COUNT - max_cell.x);
		}

		if (min_cell.y < 0 && !level_map_region_is_present(level_map, targetp.region.x, targetp.region.y - 1)) {
			targetp.tile.y -= min_cell.y + 1;
		} else if (max_cell.y >= LEVEL_REGION_Y_CELL_COUNT && !level_map_region_is_present(level_map, targetp.region.x, targetp.region.y + 1)) {
			targetp.tile.y += (LEVEL_REGION_Y_CELL_COUNT - max_cell.y);
		}
	}

	camera->position_target = targetp;

	Vec2 target;
	target.x = (r32)(LEVEL_REGION_X_CELL_COUNT * (camera->position_target.region.x - camera->position.region.x));
	target.x += camera->position_target.tile.x - camera->position.tile.x;
	target.y = (r32)(LEVEL_REGION_Y_CELL_COUNT * (camera->position_target.region.y - camera->position.region.y));
	target.y += camera->position_target.tile.y - camera->position.tile.y;

	camera->position.tile += lerp(vec2(0), target, 1.0f - powf(1.0f - camera->move_speed, dt));
	level_position_normalize(&camera->position);

	camera->distance = lerp(camera->distance, camera->distance_target, 1.0f - powf(1.0f - camera->reach_speed, dt));

	camera->orientation = slerp(camera->orientation, camera->orientation_target, 1.0f - powf(1.0f - camera->turn_speed, dt));
}

Mat4 camera_get_transform(Camera &camera) {
	if (camera.view.kind == Camera_View_Kind::ORTHOGRAPHIC) {
		r32 camera_zoom = 512.0f / powf(2.0f, camera.distance);
		return quat_get_mat4(quat_conjugate(camera.orientation)) * 
			mat4_scalar(camera_zoom, camera_zoom, 1) *
			mat4_translation((r32)-camera.position.tile.x, (r32)-camera.position.tile.y, 0.0f);
	} else {
		return quat_get_mat4(quat_conjugate(camera.orientation)) * 
			mat4_translation((r32)-camera.position.tile.x, (r32)-camera.position.tile.y, -camera.distance);
	}
}

void camera_set_view(Camera *camera, r32 distance, r32 aspect_ratio, Camera_View_Kind kind) {
	camera->distance		 = distance;
	camera->distance_target	 = distance;
	camera->aspect_ratio	 = aspect_ratio;

	if (kind == Camera_View_Kind::ORTHOGRAPHIC) {
		r32 half_height = LEVEL_REGION_HALF_Y_CELL_COUNT;
		r32 half_width  = half_height * camera->aspect_ratio;
		camera->view = orthographic_view(-half_width, half_width, half_height, -half_height, -1.0f, 10.0f);
	} else {
		camera->view = perspective_view(to_radians(60), camera->aspect_ratio, 0.1f, 100.0f);
	}
}

Camera camera_create(Level_Position position, r32 distance = 9.0f, r32 aspect_ratio = 1280.0f / 720.0f, Camera_View_Kind kind = Camera_View_Kind::ORTHOGRAPHIC) {
	Camera camera;
	camera.position					 = position;
	camera.position_target			 = camera.position;
	camera.move_speed				 = 0.99f;
	camera.reach_speed				 = 0.8f;
	camera.turn_speed				 = 0.99f;
	camera_set_view(&camera, distance, aspect_ratio, kind);
	return camera;
}

void camera_set_target_distance(Camera *camera, r32 distance) {
	camera->distance_target = distance;
}

static constexpr r32 MOVE_FORCE = 50.0f;
static constexpr r32 DRAG = 7.0f;
static constexpr r32 TURN_VELOCITY = 7.0f;

struct Entity {
	enum Type {
		PLAYER, BEAM_EMITTER, BEAM_REFLECTOR, BEAM_RECEIVER, DOOR
	};

	enum Flag {
		MOVABLE = bit(0),
		CONTROLLABLE = bit(1),
		WALKABLE = bit(2)
	};

	enum Face {
		EAST, WEST, NORTH, SOUTH
	};
	
	enum class State {
		IDEL, WALKING
	};

	Type type;
	u32 flags;

	Level_Position	position;
	Level_Position	render_position;
	Face			face;
	State			state;

	r32  dp;
};

struct Player : public Entity {
	Quat face_direction     = quat_identity();
	Vec2 movement_direction = vec2(0);

	Player() {
		type = Type::PLAYER;
		flags = Entity::MOVABLE | Entity::CONTROLLABLE;
	}
};

struct Input {
	Vec2 move;
};

struct Beam_Emitter : public Entity {	
	Beam_Emitter() {
		type = Type::BEAM_EMITTER;
		flags = Entity::MOVABLE;
	}
};

struct Beam_Reflector : public Entity {
	Face face1;

	Beam_Reflector() {
		type  = Type::BEAM_REFLECTOR;
		flags = Entity::MOVABLE;
	}
};

struct Beam_Receiver : public Entity {
	booli receiving;

	Beam_Receiver() {
		type = Type::BEAM_RECEIVER;
		flags = 0;
	}
};

struct Door : public Entity {
	Door() {
		type = Type::DOOR;
		flags = 0;
	}
};

void door_open(Door *door) {
	set_bit(door->flags, Entity::WALKABLE);
}

void door_shut(Door * door) {
	clear_bit(door->flags, Entity::WALKABLE);
}

struct World {
	Level_Map map;
	Camera camera;
	Player player;
	Beam_Emitter beam_emitters[3];
	Beam_Reflector beam_reflector;
	Beam_Receiver beam_receiver;
	Door door;
	Input input;
};

Quat face_quaternion(Entity::Face f) {
	static const Quat quats[] = {
		quat_angle_axis(vec3(0, 0, 1), MATH_TAU),
		quat_angle_axis(vec3(0, 0, 1), -MATH_TAU),
		quat_angle_axis(vec3(0, 0, 1), 0),
		quat_angle_axis(vec3(0, 0, 1), MATH_PI),
	};

	return quats[(int)f];
}

r32 face_rotation(Entity::Face f) {
	static const r32 angles[] = {
		MATH_TAU,
		-MATH_TAU,
		0,
		MATH_PI
	};

	return angles[(int)f];
}

World *create_default_world() {
	World *world = new World;

	world->map.x_count = 1;
	world->map.y_count = 1;
	world->map.regions = new Level_Map::Region *[world->map.x_count * world->map.y_count];

	world->map.regions[0] = new Level_Map::Region;

	for (int y = 0; y < LEVEL_REGION_Y_CELL_COUNT; ++y) {
		for (int x = 0; x < LEVEL_REGION_X_CELL_COUNT; ++x) {
			auto cell = level_map_region_get_cell(world->map.regions[0], x, y);

			if (x == 0 || y == 0 || x == LEVEL_REGION_X_CELL_COUNT - 1 || y == LEVEL_REGION_Y_CELL_COUNT - 1) {
				cell->type = Level_Map::Cell::EMPTY;
			} else {
				cell->type = Level_Map::Cell::WALKABLE;
			}
		}
	}

	world->input.move = vec2(0);

	world->camera = camera_create({vec2s(0), vec2(0)});
	camera_set_target_distance(&world->camera, 9.0f);

	world->player.position = { vec2s(0), vec2(0) };
	world->player.render_position = world->player.position;
	world->player.face = Entity::Face::NORTH;
	world->player.face_direction = face_quaternion(world->player.face);
	world->player.movement_direction = vec2(0);
	world->player.state = Entity::State::IDEL;
	world->player.dp = 0;

	world->beam_emitters[0].position.region = vec2s(0);
	world->beam_emitters[0].position.tile = vec2(3, 4);
	world->beam_emitters[0].render_position = world->beam_emitters[0].position;
	world->beam_emitters[0].face = Entity::Face::EAST;
	world->beam_emitters[0].state = Entity::State::IDEL;
	world->beam_emitters[0].dp = 0;
	
	world->beam_emitters[1].position.region = vec2s(0);
	world->beam_emitters[1].position.tile = vec2(-3, -4);
	world->beam_emitters[1].render_position = world->beam_emitters[1].position;
	world->beam_emitters[1].face = Entity::Face::NORTH;
	world->beam_emitters[1].state = Entity::State::IDEL;
	world->beam_emitters[1].dp = 0;

	world->beam_emitters[2].position.region = vec2s(0);
	world->beam_emitters[2].position.tile = vec2(0, -4);
	world->beam_emitters[2].render_position = world->beam_emitters[2].position;
	world->beam_emitters[2].face = Entity::Face::WEST;
	world->beam_emitters[2].state = Entity::State::IDEL;
	world->beam_emitters[2].dp = 0;

	world->beam_reflector.position.region = vec2s(0);
	world->beam_reflector.position.tile = vec2(5, -3);
	world->beam_reflector.render_position = world->beam_reflector.position;
	world->beam_reflector.face = Entity::Face::WEST;
	world->beam_reflector.face1 = Entity::Face::SOUTH;
	world->beam_reflector.state = Entity::State::IDEL;
	world->beam_reflector.dp = 0;

	world->beam_receiver.position.region = vec2s(0);
	world->beam_receiver.position.tile = vec2(-5, -3);
	world->beam_receiver.render_position = world->beam_receiver.position;
	world->beam_receiver.face = Entity::Face::NORTH;
	world->beam_receiver.state = Entity::State::IDEL;
	world->beam_receiver.dp = 0;

	world->door.position.region = vec2s(0);
	world->door.position.tile = vec2(5, 5);
	world->door.render_position = world->door.position;
	world->door.face = Entity::Face::NORTH;
	world->door.state = Entity::State::IDEL;
	world->door.dp = 0;

	return world;
}

static Texture2d_Handle	debug_player_sprite;
static Texture2d_Handle	debug_torch_sprite;
static Texture2d_Handle	debug_spiral_sprite;
static Texture2d_Handle	debug_stone_sprite;
static Texture2d_Handle	debug_diamond_sprite;
static Texture2d_Handle	debug_pipe_sprite;
static Monospaced_Font	debug_font;

void entity_update_movement(Entity *entity, r32 dt) {
	if (entity->state == Entity::State::WALKING) {
		Vec2 direction = level_position_unpack(entity->position, entity->render_position);
		r32 distance = vec2_length(direction);

		if (distance) {
			r32 acceleration = MOVE_FORCE;
			entity->dp += dt * acceleration;
			entity->dp *= powf(0.5f, DRAG * dt);
			r32 change_in_position = dt * entity->dp;

			if (change_in_position < distance) {
				direction /= distance;
				entity->render_position.tile += direction * change_in_position;
				level_position_normalize(&entity->render_position);
			} else {
				entity->render_position = entity->position;
			}

		} else {
			entity->state = Entity::State::IDEL;
		}
	}

	if (entity->type == Entity::Type::PLAYER) {
		Player *player = (Player *)entity;
		player->face_direction = rotate_toward(player->face_direction, face_quaternion(player->face), TURN_VELOCITY * dt);
	}
}

bool move_entity(World *world, Entity *entity, Entity::Face direction) {
	bool moved = false;

	if (entity->state == Entity::State::IDEL) {
		Level_Position target = entity->position;

		switch (direction) {
			case Entity::Face::EAST: target.tile.x -= 1; break;
			case Entity::Face::WEST: target.tile.x += 1; break;
			case Entity::Face::NORTH: target.tile.y += 1; break;
			case Entity::Face::SOUTH: target.tile.y -= 1; break;
		}

		level_position_normalize(&target);

		auto region = level_map_get_region_bounded(&world->map, target.region);

		if (region) {
			auto cell = level_map_region_get_cell(region, level_map_tile_to_cell(target.tile));

			if (cell->type == cell->WALKABLE) {
				if (!level_position_are_same(entity->position, target)) {
					if (cell->entity) {
						if ((cell->entity->flags & Entity::MOVABLE) && move_entity(world, cell->entity, direction)) {
							auto current_cell = level_map_region_get_cell(level_map_get_region(&world->map, entity->position.region), level_map_tile_to_cell(target.tile));
							current_cell->entity = nullptr;

							entity->state = Entity::State::WALKING;
							entity->position = target;
							cell->entity = entity;
							moved = true;
						} else if (cell->entity->flags & Entity::WALKABLE) {
							auto current_cell = level_map_region_get_cell(level_map_get_region(&world->map, entity->position.region), level_map_tile_to_cell(target.tile));
							current_cell->entity = nullptr;

							entity->state = Entity::State::WALKING;
							entity->position = target;
							cell->entity = entity;
							moved = true;
						}
					} else {
						auto current_cell = level_map_region_get_cell(level_map_get_region(&world->map, entity->position.region), level_map_tile_to_cell(target.tile));
						current_cell->entity = nullptr;

						entity->state = Entity::State::WALKING;
						entity->position = target;
						cell->entity = entity;
						moved = true;
					}
					
				} else {
					entity->dp = 0;
				}
			}
		}

		if (entity->type == Entity::Type::PLAYER) {
			entity->face = direction;
		}
	}

	return moved;
}

void level_map_set_entity(Level_Map *level_map, Entity *entity) {
	auto region = level_map_get_region_bounded(level_map, entity->position.region);
	if (region) {
		auto cell = level_map_region_get_cell(region, level_map_tile_to_cell(entity->position.tile));
		cell->entity = entity;
	}
}

void world_update(World *world, r32 dt) {
	Camera &camera = world->camera;
	Player &player = world->player;

	constexpr int MAX_UPDATE_REGION_X = 1;
	constexpr int MAX_UPDATE_REGION_Y = 1;

	{
		int start_index_y = world->camera.position.region.y - MAX_UPDATE_REGION_Y / 2;
		int start_index_x = world->camera.position.region.x - MAX_UPDATE_REGION_X / 2;

		for (int region_y = start_index_y; region_y < MAX_UPDATE_REGION_Y; ++region_y) {
			for (int region_x = start_index_x; region_x < MAX_UPDATE_REGION_X; ++region_x) {
				auto *region = level_map_get_region_bounded(&world->map, region_x, region_y);

				if (region) {
					for (int cell_y = 0; cell_y < LEVEL_REGION_Y_CELL_COUNT; ++cell_y) {
						for (int cell_x = 0; cell_x < LEVEL_REGION_X_CELL_COUNT; ++cell_x) {
							auto cell = level_map_region_get_cell(region, cell_x, cell_y);
							cell->entity = nullptr;
							cell->beam = false;
						}
					}
				}

			}
		}
	}

	level_map_set_entity(&world->map, &player);
	level_map_set_entity(&world->map, world->beam_emitters + 0);
	level_map_set_entity(&world->map, world->beam_emitters + 1);
	level_map_set_entity(&world->map, world->beam_emitters + 2);
	level_map_set_entity(&world->map, &world->beam_reflector);
	level_map_set_entity(&world->map, &world->beam_receiver);
	level_map_set_entity(&world->map, &world->door);

	world->beam_receiver.receiving = false;

	if (fabsf(world->input.move.x) > fabsf(world->input.move.y)) {
		if (world->input.move.x > 0) {
			move_entity(world, &world->player, Entity::Face::WEST);
		} else {
			move_entity(world, &world->player, Entity::Face::EAST);
		}
	} else if (fabsf(world->input.move.y) > fabsf(world->input.move.x)) {
		if (world->input.move.y > 0) {
			move_entity(world, &world->player, Entity::Face::NORTH);
		} else {
			move_entity(world, &world->player, Entity::Face::SOUTH);
		}
	}

	entity_update_movement(&world->player, dt);
	entity_update_movement(world->beam_emitters + 0, dt);
	entity_update_movement(world->beam_emitters + 1, dt);
	entity_update_movement(world->beam_emitters + 2, dt);
	entity_update_movement(&world->beam_reflector, dt);
	entity_update_movement(&world->beam_receiver, dt);
	camera_update(&camera, dt, player.render_position, &world->map);

	for (int i = 0; i < static_count(world->beam_emitters); ++i) {
		Beam_Emitter &beam = world->beam_emitters[i];

		Entity::Face face = beam.face;
		Level_Position position = beam.position;

		while (true) {
			switch (face) {
				case Entity::Face::EAST: {
					position.tile.x -= 1;
				} break;
				
				case Entity::Face::WEST: {
					position.tile.x += 1;
				} break;

				case Entity::Face::NORTH: {
					position.tile.y += 1;
				} break;
				
				case Entity::Face::SOUTH: {
					position.tile.y -= 1;
				} break;

				invalid_default_case();
			}

			level_position_normalize(&position);

			auto region = level_map_get_region_bounded(&world->map, position.region);
			if (region) {
				auto cell = level_map_region_get_cell(region, level_map_tile_to_cell(position.tile));
				if (cell->type == cell->WALKABLE) {
					if (cell->entity == nullptr) {
						cell->beam = true;
					} else if (cell->entity->type == Entity::Type::BEAM_REFLECTOR) {
						Beam_Reflector *beam_reflector = (Beam_Reflector *)cell->entity;
						auto eface0 = beam_reflector->face;
						auto eface1 = beam_reflector->face1;

						auto blocked = false;
						switch (face) {
							case Entity::Face::EAST: {
								if (eface0 == Entity::Face::WEST) {
									face = eface1;
								} else if (eface1 == Entity::Face::WEST) {
									face = eface0;
								} else {
									blocked = true;
								}
							} break;

							case Entity::Face::WEST: {
								if (eface0 == Entity::Face::EAST) {
									face = eface1;
								} else if (eface1 == Entity::Face::EAST) {
									face = eface0;
								} else {
									blocked = true;
								}
							} break;

							case Entity::Face::NORTH: {
								if (eface0 == Entity::Face::SOUTH) {
									face = eface1;
								} else if (eface1 == Entity::Face::SOUTH) {
									face = eface0;
								} else {
									blocked = true;
								}
							} break;

							case Entity::Face::SOUTH: {
								if (eface0 == Entity::Face::NORTH) {
									face = eface1;
								} else if (eface1 == Entity::Face::NORTH) {
									face = eface0;
								} else {
									blocked = true;
								}
							} break;

							invalid_default_case();
						}

						if (blocked) {
							break;
						}
					} else if (cell->entity->type == Entity::Type::BEAM_RECEIVER) {
						Beam_Receiver *receiver = (Beam_Receiver *)cell->entity;
						if ((receiver->face == Entity::EAST && face == Entity::WEST) ||
							(receiver->face == Entity::WEST && face == Entity::EAST) ||
							(receiver->face == Entity::NORTH && face == Entity::SOUTH) ||
							(receiver->face == Entity::SOUTH && face == Entity::NORTH)) {
							receiver->receiving = true;
						}
						break;
					} else {
						break;
					}
				} else {
					break;
				}
			} else {
				break;
			}
		}

	}

	if (world->beam_receiver.receiving) {
		door_open(&world->door);
	} else {
		door_shut(&world->door);
	}

	static bool complete = false;
	if (level_position_are_same(world->player.position, world->door.position) && !complete) {
		Debug_NotifySuccess("Level Complete");
		complete = true;
	}
	
}

void world_render(World *world) {
	constexpr int MAX_RENDER_REGION_X = 1;
	constexpr int MAX_RENDER_REGION_Y = 1;

	Camera &camera = world->camera;
	Player &player = world->player;

	Mat4 cam_transform = camera_get_transform(camera);

	im2d_begin(camera.view, cam_transform);

	int start_index_y = world->camera.position.region.y - MAX_RENDER_REGION_Y / 2;
	int start_index_x = world->camera.position.region.x - MAX_RENDER_REGION_X / 2;

	im2d_unbind_texture();
	im2d_rect_centered(vec2(0), 2 * vec2(MAX_RENDER_REGION_X * LEVEL_REGION_X_CELL_COUNT, MAX_RENDER_REGION_Y * LEVEL_REGION_Y_CELL_COUNT), vec4(0.3f, 0.7f, 0.2f));

	for (int region_y = start_index_y; region_y < MAX_RENDER_REGION_Y; ++region_y) {
		for (int region_x = start_index_x; region_x < MAX_RENDER_REGION_X; ++region_x) {
			auto *region = level_map_get_region_bounded(&world->map, region_x, region_y);

			if (region) {
				r32 draw_y = (r32)((region_y - camera.position.region.y) * LEVEL_REGION_Y_CELL_COUNT);
				r32 draw_x = (r32)((region_x - camera.position.region.x) * LEVEL_REGION_X_CELL_COUNT);

				for (int cell_y = 0; cell_y < LEVEL_REGION_Y_CELL_COUNT; ++cell_y) {
					for (int cell_x = 0; cell_x < LEVEL_REGION_X_CELL_COUNT; ++cell_x) {
						auto cell = level_map_region_get_cell(region, cell_x, cell_y);

						Vec2 draw_pos;
						draw_pos.y = draw_y;
						draw_pos.x = draw_x;
						draw_pos += level_map_cell_to_tile(cell_x, cell_y);

						if (cell->type == Level_Map::Cell::EMPTY) {
							im2d_bind_texture(debug_stone_sprite);
							im2d_rect_centered(draw_pos, vec2(1), vec4(1));
						} else {
							if (cell->beam) {
								Color3 color = vec3(0, 1, 1);
								im2d_unbind_texture();
								im2d_rect_centered(draw_pos, vec2(1), vec4(3 * color, 0.5f));
							}
						}
					}
				}
			}

		}
	}

	im2d_bind_texture(debug_player_sprite);

	{
		Vec2 draw_pos;
		draw_pos.y = (r32)((player.render_position.region.y - camera.position.region.y) * LEVEL_REGION_Y_CELL_COUNT);
		draw_pos.x = (r32)((player.render_position.region.x - camera.position.region.x) * LEVEL_REGION_X_CELL_COUNT);
		draw_pos += player.render_position.tile;

		r32  angle;
		Vec3 axis;
		quat_get_angle_axis(player.face_direction, &angle, &axis);

		im2d_rect_centered_rotated(draw_pos, vec2(1), axis.z * angle, vec4(1));
	}

	im2d_unbind_texture();

	{
		im2d_bind_texture(debug_pipe_sprite);
		auto &mirror = world->beam_reflector;

		Vec2 draw_pos;
		draw_pos.y = (r32)((mirror.render_position.region.y - camera.position.region.y) * LEVEL_REGION_Y_CELL_COUNT);
		draw_pos.x = (r32)((mirror.render_position.region.x - camera.position.region.x) * LEVEL_REGION_X_CELL_COUNT);
		draw_pos += mirror.render_position.tile;

		im2d_rect_centered(draw_pos, vec2(1), vec4(1));
	}

	{
		im2d_bind_texture(debug_diamond_sprite);

		auto &receiver = world->beam_receiver;

		Vec2 draw_pos;
		draw_pos.y = (r32)((receiver.render_position.region.y - camera.position.region.y) * LEVEL_REGION_Y_CELL_COUNT);
		draw_pos.x = (r32)((receiver.render_position.region.x - camera.position.region.x) * LEVEL_REGION_X_CELL_COUNT);
		draw_pos += receiver.render_position.tile;

		Color4 color = vec4(1, 1, 1, 1);
		if (receiver.receiving) {
			color.xyz *= 15.0f;
		} else {
			color.xyz *= 0.8f;
		}

		im2d_rect_centered_rotated(draw_pos, vec2(1), face_rotation(receiver.face), color);
	}

	{
		im2d_bind_texture(debug_spiral_sprite);

		auto &door = world->door;

		Vec2 draw_pos;
		draw_pos.y = (r32)((door.render_position.region.y - camera.position.region.y) * LEVEL_REGION_Y_CELL_COUNT);
		draw_pos.x = (r32)((door.render_position.region.x - camera.position.region.x) * LEVEL_REGION_X_CELL_COUNT);
		draw_pos += door.render_position.tile;

		Color4 color = vec4(1, 1, 1, 1);
		if (door.flags & Entity::Flag::WALKABLE) {
			color.xyz *= 15.0f;
		} else {
			color.xyz *= 0.1f;
		}

		im2d_rect_centered(draw_pos, vec2(1), color);
	}

	im2d_bind_texture(debug_torch_sprite);

	for (int i = 0; i < static_count(world->beam_emitters); ++i) {
		auto &beam = world->beam_emitters[i];

		Vec2 draw_pos;
		draw_pos.y = (r32)((beam.render_position.region.y - camera.position.region.y) * LEVEL_REGION_Y_CELL_COUNT);
		draw_pos.x = (r32)((beam.render_position.region.x - camera.position.region.x) * LEVEL_REGION_X_CELL_COUNT);
		draw_pos += beam.render_position.tile;

		im2d_rect_centered_rotated(draw_pos, vec2(1), face_rotation(beam.face), vec4(1));
	}

	im2d_end();
}

int system_main() {
	r32    framebuffer_w = 1280;
	r32    framebuffer_h = 720;
	Handle platform      = system_create_window(u8"Karma", 1280, 720, System_Window_Show_NORMAL);
	gfx_create_context(platform, Render_Backend_DIRECTX11, Vsync_ADAPTIVE, 2, (u32)framebuffer_w, (u32)framebuffer_h);

	auto audio        = load_wave(system_read_entire_file("../res/misc/POL-course-of-nature-short.wav"));

	Audio_Mixer mixer = audio_mixer();

	if (!system_audio(system_update_audio, system_refresh_audio_device, &mixer)) {
		system_display_critical_message("Failed to load audio!");
	}

	audio_mixer_set_volume(&mixer, 0.2f);

	ImGui_Initialize();
	Debug_ModeEnable();

	Debug_SetPresentationState(false);

	Im_Mesh player_mesh;
	debug_load_mesh("dev/base.model", &player_mesh);

	auto music = play_audio(&mixer, &audio, true);

	debug_player_sprite = debug_load_texture("../res/misc/player.png");
	debug_torch_sprite = debug_load_texture("../res/misc/torch.png");
	debug_spiral_sprite = debug_load_texture("../res/misc/spiral.png");
	debug_stone_sprite = debug_load_texture("../res/misc/stone.png");
	debug_diamond_sprite = debug_load_texture("../res/misc/diamond.png");
	debug_pipe_sprite = debug_load_texture("../res/misc/pipe.png");
	debug_font			= debug_load_font("dev/debug.font");

	World *world = create_default_world();

	//
	//
	//

	bool running = true;

	const r32 aspect_ratio = framebuffer_w / framebuffer_h;

	Time_Speed_Factor factor;

	r32 const fixed_dt      = 1.0f / 30.0f;
	r32       dt            = fixed_dt * factor.ratio;
	r32       game_dt       = fixed_dt * factor.ratio;
	r32       real_dt       = fixed_dt;
	r32       game_t        = 0.0f;
	r32       real_t        = 0.0f;
	r32       accumulator_t = fixed_dt;

	Time_State state = Time_State_RESUME;

	r32 window_w = 0, window_h = 0;

	u64 frequency = system_get_frequency();
	u64 counter   = system_get_counter();

	Array<Mm_Rect> rects;

	array_add(&rects, mm_rect_from_dimension(vec2(700, 500), vec2(50)));
	array_add(&rects, mm_rect_from_dimension(vec2(900, 100), vec2(60)));
	array_add(&rects, mm_rect_from_dimension(vec2(200, 50), vec2(20)));
	array_add(&rects, mm_rect_from_dimension(vec2(50, 50), vec2(120)));
	array_add(&rects, mm_rect_from_dimension(vec2(10, 500), vec2(80)));
	array_add(&rects, mm_rect_from_dimension(vec2(1000, 300), vec2(45)));

	while (running) {
		Debug_TimedFrameBegin();

		Debug_TimedBlockBegin(EventHandling);
		auto events = system_poll_events();
		for (s64 event_index = 0; event_index < events.count; ++event_index) {
			Event &event = events[event_index];

			if (event.type & Event_Type_EXIT) {
				running = false;
				break;
			}

			if (Debug_HandleEvent(event))
				continue;
			if (ImGui_HandleEvent(event))
				continue;

				//
				// Developer Commands
				//

#if defined(BUILD_DEBUG) || defined(BUILD_DEBUG_FAST) || defined(BUILD_DEVELOPER)
			if (event.type == Event_Type_KEY_UP) {
				switch (event.key.symbol) {
					case Key_F1:
						if (state == Time_State_RESUME) {
							state = Time_State_PAUSE;
							system_audio_pause();
							Debug_Notify("Paused");
						} else {
							state   = Time_State_RESUME;
							game_dt = fixed_dt * factor.ratio;
							system_audio_resume();
							Debug_Notify("Resumed");
						}
						break;
					case Key_F2:
						decrease_game_speed(&factor);
						Debug_Notify("Speed x%.3f", factor.ratio);
						break;
					case Key_F3:
						increase_game_speed(&factor);
						Debug_Notify("Speed x%.3f", factor.ratio);
						break;
					case Key_F4:
						if (Debug_TogglePresentationState()) {
							Debug_Notify("Debug View: On");
						} else {
							Debug_Notify("Debug View: Off");
						}
						break;
					case Key_F5:
						editor.display = !editor.display;
						if (editor.display) {
							Debug_Notify("Interface: On");
						} else {
							Debug_Notify("Interface: Off");
						}
						break;
				}
			}
#endif

			//
			//
			//

			if (event.type & Event_Type_WINDOW_RESIZE) {
				s32 w = event.window.dimension.x;
				s32 h = event.window.dimension.y;

				gfx_on_client_resize(w, h);
				window_w = (r32)w;
				window_h = (r32)h;
				continue;
			}

			if ((event.type & Event_Type_KEY_UP) && event.key.symbol == Key_ESCAPE) {
				system_request_quit();
				break;
			}

			if ((event.type & Event_Type_KEY_UP) && event.key.symbol == Key_F11) {
				system_fullscreen_state(SYSTEM_TOGGLE);
				continue;
			}

			if (event.type & Event_Type_KEYBOARD) {
				float value = (float)(event.key.state == Key_State_DOWN);
				switch (event.key.symbol) {
				case Key_D:
				case Key_RIGHT:
					world->input.move.x = value;
					break;
				case Key_A:
				case Key_LEFT:
					world->input.move.x = -value;
					break;

				case Key_W:
				case Key_UP:
					world->input.move.y = value;
					break;
				case Key_S:
				case Key_DOWN:
					world->input.move.y = -value;
					break;
				}
			}
		}

		r32 render_w = window_w;
		r32 render_h = floorf(window_w / aspect_ratio);
		if (render_h > window_h) {
			render_h = window_h;
			render_w = floorf(window_h * aspect_ratio);
		}

		r32 render_x = floorf((window_w - render_w) * 0.5f);
		r32 render_y = floorf((window_h - render_h) * 0.5f);

		Debug_TimedBlockEnd(EventHandling);

		Debug_TimedBlockBegin(Simulation);

		while (accumulator_t >= fixed_dt) {
			Debug_TimedScope(SimulationFrame);

			if (state == Time_State_RESUME) {
				world_update(world, dt);
			}

			accumulator_t -= fixed_dt;
		}

		Debug_TimedBlockEnd(Simulation);

		Debug_TimedBlockBegin(AudioUpdate);

		mixer.pitch_factor = factor.ratio;
		audio_mixer_update(&mixer);
		Debug_TimedBlockEnd(AudioUpdate);

		Debug_TimedBlockBegin(Rendering);

		r32 alpha = accumulator_t / fixed_dt; // TODO: Use this

		ImGui_UpdateFrame(real_dt);

		gfx_begin_drawing(Framebuffer_Type_HDR, Clear_ALL, vec4(0.0f));
		gfx_viewport(0, 0, framebuffer_w, framebuffer_h);

		world_render(world);

		gfx_end_drawing();

		gfx_apply_bloom(2);

		gfx_begin_drawing(Framebuffer_Type_DEFAULT, Clear_COLOR, vec4(0));

		gfx_blit_hdr(render_x, render_y, render_w, render_h);
		gfx_viewport(0, 0, window_w, window_h);

		im2d_begin(orthographic_view(0, window_w, window_h, 0));
		im2d_unbind_texture();

		Vec2 ray_origin = vec2(500);
		Vec2 cursor_p = system_get_cursor_position();
		Vec2 ray_end = cursor_p;

		for (auto& rect : rects) {
			Vec2 rect_pos = rect.min;
			Vec2 rect_dim = rect.max - rect.min;

			im2d_rect(rect_pos, rect_dim, vec4(1));

			bool hitted = false;

			Ray_Hit hit;
			if (ray_vs_aabb(ray_origin, cursor_p - ray_origin, rect, &hit) && hit.t <= 1.0f) {
				ray_end = hit.point;
				im2d_rect_outline(rect_pos, rect_dim, vec4(1, 0, 0), 1);
				hitted = true;
			}


			if (hitted) {
				im2d_circle(hit.point, 5, vec4(1, 0, 1));
				im2d_line(hit.point, hit.point + 20.0f * hit.normal, vec4(1, 0, 1));
			}
		}

		im2d_line(ray_origin, ray_end, vec4(0, 1, 1), 1.0f);

		im2d_end();

#if defined(BUILD_IMGUI)
		{
			Debug_TimedScope(ImGuiRender);
			ImGui_RenderFrame();
		}
#endif

#if defined(BUILD_DEBUG_SERVICE)
		{
			Debug_TimedScope(DebugRender);
			Debug_RenderFrame(window_w, window_h);
		}
#endif

		gfx_end_drawing();

		Debug_TimedBlockBegin(Present);
		gfx_present();
		Debug_TimedBlockEnd(Present);
		Debug_TimedBlockEnd(Rendering);

		reset_temporary_memory();

		auto new_counter = system_get_counter();
		auto counts      = new_counter - counter;
		counter          = new_counter;

		real_dt = ((1000000.0f * (r32)counts) / (r32)frequency) / 1000000.0f;
		real_t += real_dt;

		game_dt = factor.ratio * real_dt;
		dt      = fixed_dt * factor.ratio;

		if (state == Time_State_RESUME) {
			game_t += game_dt;
		}

		accumulator_t += real_dt;
		accumulator_t = min_value(accumulator_t, 0.3f);

		Debug_TimedFrameEnd(real_dt);
	}

	ImGui_Shutdown();
	gfx_destroy_context();

	return 0;
}
