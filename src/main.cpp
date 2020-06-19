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

constexpr s32 MAX_SPEED_FACTOR = 8;

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

enum Cell_Type : u32 {
	Cell_Type_NULL,
	Cell_Type_PATH,
};

enum Cell_Visibility {
	Cell_Visibility_DISABLED,
	Cell_Visibility_VISITED,
	Cell_Visibility_VISIBLE,
	Cell_Visibility_LIGHT,
};

struct World_Region_Cell {
	Cell_Type type;
	Cell_Visibility visibility = Cell_Visibility_DISABLED;

	World_Region_Cell(u32 t) {
		type = (Cell_Type)t;
	}
};

constexpr int  MAP_REGION_X_CELL_COUNT			= 25;
constexpr int  MAP_REGION_Y_CELL_COUNT			= 15;
constexpr int  MAP_REGION_HALF_X_CELL_COUNT		= MAP_REGION_X_CELL_COUNT / 2;
constexpr int  MAP_REGION_HALF_Y_CELL_COUNT		= MAP_REGION_Y_CELL_COUNT / 2;

static World_Region_Cell map_cells0[MAP_REGION_Y_CELL_COUNT][MAP_REGION_X_CELL_COUNT] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

struct World_Position {
	Vec3s region;
	Vec3  tile;
};

Vec2s world_map_tile_to_cell(Vec2 tile) {
	Vec2s cell_index;
	cell_index.x = (int)(tile.x + MAP_REGION_HALF_X_CELL_COUNT);
	cell_index.y = (int)(tile.y + MAP_REGION_HALF_Y_CELL_COUNT);
	return cell_index;
}

Vec2 world_map_cell_to_tile(int cell_index_x, int cell_index_y) {
	Vec2 tile;
	tile.x = (r32)(cell_index_x - MAP_REGION_HALF_X_CELL_COUNT);
	tile.y = (r32)(cell_index_y - MAP_REGION_HALF_Y_CELL_COUNT);
	return tile;
}

Vec2 world_map_cell_to_tile(Vec2s cell_index) {
	return world_map_cell_to_tile(cell_index.x, cell_index.y);
}

r32 world_map_cell_to_tile_horizontal(int cell_index) {
	return (r32)(cell_index - MAP_REGION_HALF_X_CELL_COUNT);
}

r32 world_map_cell_to_tile_vertical(int cell_index) {
	return (r32)(cell_index - MAP_REGION_HALF_Y_CELL_COUNT);
}

Vec3 world_position_unpack(World_Position &position, World_Position &center) {
	Vec3 result;
	result.x = (position.region.x - center.region.x) * MAP_REGION_X_CELL_COUNT + (position.tile.x - center.tile.x);
	result.y = (position.region.y - center.region.y) * MAP_REGION_Y_CELL_COUNT + (position.tile.y - center.tile.y);
	result.z = (r32)(position.region.z - center.region.z);
	return result;
}

void world_position_normalize(World_Position *position) {
	Vec2s cell_index = world_map_tile_to_cell(position->tile.xy);

	if (cell_index.x >= MAP_REGION_X_CELL_COUNT) {
		position->region.x += cell_index.x / MAP_REGION_X_CELL_COUNT;
		position->tile.x = world_map_cell_to_tile_horizontal(cell_index.x % MAP_REGION_X_CELL_COUNT);
	}

	if (cell_index.x < 0) {
		int dec = abs(cell_index.x + 1) / MAP_REGION_X_CELL_COUNT + 1;
		position->region.x -= dec;
		position->tile.x = world_map_cell_to_tile_horizontal(MAP_REGION_X_CELL_COUNT * dec + cell_index.x);
	}

	if (cell_index.y >= MAP_REGION_Y_CELL_COUNT) {
		position->region.y += cell_index.y / MAP_REGION_Y_CELL_COUNT;
		position->tile.y = world_map_cell_to_tile_vertical(cell_index.y % MAP_REGION_Y_CELL_COUNT);
	}

	if (cell_index.y < 0) {
		int dec = abs(cell_index.y + 1) / MAP_REGION_Y_CELL_COUNT + 1;
		position->region.y -= dec;
		position->tile.y = world_map_cell_to_tile_vertical(MAP_REGION_Y_CELL_COUNT * dec + cell_index.y);
	}
}

bool world_position_are_same(World_Position &a, World_Position &b) {
	return  (a.tile.x == b.tile.x) && (a.tile.y == b.tile.y) &&
		(a.region.z == b.region.z) && (a.region.x == b.region.x) && (a.region.y == b.region.y);
}

bool world_position_are_in_same_region(World_Position& a, World_Position& b) {
	return (a.region.z == b.region.z) && (a.region.x == b.region.x) && (a.region.y == b.region.y);
}

struct World_Map_Region {
	World_Region_Cell (*cells)[MAP_REGION_X_CELL_COUNT];
};

World_Region_Cell* world_map_region_get_cell(World_Map_Region* map_region, int cell_x, int cell_y) {
	assert(cell_x >= 0 && cell_x < MAP_REGION_X_CELL_COUNT);
	assert(cell_y >= 0 && cell_y < MAP_REGION_Y_CELL_COUNT);
	return &map_region->cells[cell_y][cell_x];
}

World_Region_Cell* world_map_region_get_cell_ranged(World_Map_Region* map_region, int cell_x, int cell_y) {
	if (cell_x >= 0 && cell_x < MAP_REGION_X_CELL_COUNT && cell_y >= 0 && cell_y < MAP_REGION_Y_CELL_COUNT) {
		return &map_region->cells[cell_y][cell_x];
	}
	return nullptr;
}

World_Region_Cell* world_map_region_get_cell(World_Map_Region* map_region, Vec2s cell_index) {
	return world_map_region_get_cell(map_region, cell_index.x, cell_index.y);
}

struct World_Map {
	World_Map_Region **regions;
	int x_count;
	int y_count;
	int z_count;

	int x_index_offset;
	int y_index_offset;
	int z_index_offset;
};

World_Map_Region* world_map_get_region(World_Map *world_map, int region_x, int region_y, int region_z) {
	region_x += world_map->x_index_offset;
	region_y += world_map->y_index_offset;
	region_z += world_map->z_index_offset;

	World_Map_Region* map_region = nullptr;

	if ((region_x >= 0 && region_x < world_map->x_count) && 
		(region_y >= 0 && region_y < world_map->y_count) && 
		(region_z >= 0 && region_z < world_map->z_count)) {
		return  *(world_map->regions + region_x +
				region_y * world_map->x_count +
				region_z * world_map->y_count * world_map->x_count);
	}

	return map_region;
}

bool world_map_has_region(World_Map *world_map, int region_x, int region_y, int region_z) {
	return world_map_get_region(world_map, region_x, region_y, region_z) != nullptr;
}

World_Map create_test_world() {
	static World_Map_Region test_map_regions[1];
	test_map_regions[0].cells = map_cells0;

	World_Map map;
	map.x_count = 1;
	map.y_count = 1;
	map.z_count = 1;

	map.x_index_offset = map.x_count / 2;
	map.y_index_offset = map.y_count / 2;
	map.z_index_offset = map.z_count / 2;

	if (map.x_count % 2 == 0) map.x_index_offset -= 1;
	if (map.y_count % 2 == 0) map.y_index_offset -= 1;
	if (map.z_count % 2 == 0) map.z_index_offset -= 1;

	ptrsize total_size = sizeof(World_Map_Region *) * map.x_count * map.y_count * map.z_count;
	map.regions = (World_Map_Region **)memory_allocate(total_size);
	memset(map.regions, 0, total_size);

	map.regions[0] = &test_map_regions[0];

	return map;
}

enum Face {
	Face_EAST,
	Face_WEST,
	Face_NORTH,
	Face_SOUTH,
};

Quat face_quaternion(Face f) {
	static const Quat quats[] = {
		quat_angle_axis(vec3(0, 0, 1), MATH_TAU),
		quat_angle_axis(vec3(0, 0, 1), -MATH_TAU),
		quat_angle_axis(vec3(0, 0, 1), 0),
		quat_angle_axis(vec3(0, 0, 1), MATH_PI),
	};

	return quats[f];
}

enum Camera_Control {
	Camera_Control_GAME,
	Camera_Control_FREE
};

struct Player {
	enum State {
		IDEL,
		WALKING
	};

	World_Position position			  = { vec3s(0, 0, 0) };
	World_Position render_position    = position;

	Face face				   = Face_NORTH;
	Quat face_direction        = quat_identity();

	State state = IDEL;

	Vec2 movement_direction = vec2(0);
	r32  movement_force		= 1400;

	r32  mass                 = 50;
	r32  drag                 = 7;

	r32  turn_velocity      = 7;
	r32  dp					= 0;

	struct Block *grabbed = nullptr;
};

struct Block {
	World_Position position;
};

struct Light {
	World_Position position;
	int radius;
};

struct Game_Controller {
	r32 x;
	r32 y;
	bool grab;
};

struct Camera_Controller {
	r32 x;
	r32 y;
	bool is_valid;
};

struct Camera_Bounds {
	r32 left, right;
	r32 top, bottom;
};

struct Camera {
	World_Position position;
	World_Position position_target;

	Quat orientation = quat_identity();
	Quat orientation_target = orientation;

	r32 aspect_ratio;

	Camera_View view;
};

Camera_Bounds camera_get_bounds(Camera &camera) {
	Camera_Bounds bounds;

	if (camera.view.kind == Camera_View_Kind::ORTHOGRAPHIC) {
		r32 half_height = MAP_REGION_HALF_Y_CELL_COUNT;
		r32 half_width  = half_height * camera.aspect_ratio;

		bounds.left   = -half_width;
		bounds.right  = half_width;
		bounds.top    = half_height;
		bounds.bottom = -half_height;
	} else {
		r32 half_width  = camera.position.tile.z;
		r32 half_height = camera.position.tile.z / camera.aspect_ratio;

		bounds.left = -half_width;
		bounds.right = half_width;
		bounds.top = half_height;
		bounds.bottom = -half_height;
	}

	return bounds;
}

Mat4 camera_get_transform(Camera &camera) {
	return quat_get_mat4(quat_conjugate(camera.orientation)) * mat4_translation(-camera.position.tile);
}

void camera_set_view(Camera *camera, Camera_View_Kind kind) {
	if (kind == Camera_View_Kind::ORTHOGRAPHIC) {
		r32 half_height = MAP_REGION_HALF_Y_CELL_COUNT;
		r32 half_width  = half_height * camera->aspect_ratio;
		camera->view = orthographic_view(-half_width, half_width, half_height, -half_height, -1.0f, 100.0f);
	} else {
		camera->view = perspective_view(to_radians(60), camera->aspect_ratio, 0.1f, 100.0f);
	}
}

Camera camera_create(World_Position position, r32 distance = 5.0f, r32 target_distance = 15.0f, r32 aspect_ratio = 1280.0f / 720.0f, Camera_View_Kind kind = Camera_View_Kind::ORTHOGRAPHIC) {
	Camera camera;
	camera.position					 = position;
	camera.position_target			 = camera.position;
	camera.position.tile.z			 = distance;
	camera.position_target.tile.z	 = target_distance;
	camera.aspect_ratio				 = aspect_ratio;

	camera_set_view(&camera, kind);
	return camera;
}

void editor_update(Audio_Mixer *mixer, Player *player, Camera *camera) {
	if (!editor.display)
		return;

	ImGui::Begin("Editor");

	r32 volume = mixer->volume_b;
	ImGui::DragFloat("Volume", &volume, 0.01f, 0.0f, 1.0f);
	ImGui::Checkbox("Guide", &editor.guide);

#if 1
	ImGui::Text("Player");
	ImGui::DragInt3("Region Position", player->position.region.m);
	ImGui::DragFloat3("Tile Position", player->position.tile.m);
	ImGui::Text("Face Direction: (%.3f, %.3f, %.3f)", player->face_direction.x, player->face_direction.y, player->face_direction.z);
	ImGui::DragFloat("Mass", &player->mass, 0.1f);
	ImGui::DragFloat("Drag", &player->drag, 0.01f);
	ImGui::DragFloat("Movement Force", &player->movement_force, 0.1f);
	ImGui::DragFloat("Turn Speed", &player->turn_velocity, 0.01f);

	ImGui::DragFloat("Camera Distance", &camera->position_target.tile.z, 0.01f);
#endif

	ImGui::End();

	audio_mixer_set_volume(mixer, volume);
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

bool world_map_is_position_available(World_Map *world_map, Light *lights, int light_count, Block *blocks, int block_count, Face move_dir, World_Position &position, void *entity_ptr) {
	World_Map_Region* map_region = world_map_get_region(world_map, position.region.x, position.region.y, position.region.z);

	if (map_region) {
		Vec2s cell_index = world_map_tile_to_cell(position.tile.xy);
		World_Region_Cell* region_cell = world_map_region_get_cell(map_region, cell_index.x, cell_index.y);

		if (region_cell->type == Cell_Type_PATH) {
			for (int index = 0; index < light_count; ++index) {
				Light &light = lights[index];
				if (position.region.x == light.position.region.x && 
					position.region.y == light.position.region.y && 
					position.region.z == light.position.region.z) {
					Vec2s light_cell = world_map_tile_to_cell(light.position.tile.xy);
					if (light_cell.x == cell_index.x && light_cell.y == cell_index.y) {
						return false;
					}
				}
			}

			for (int index = 0; index < block_count; ++index) {
				void *ptr = blocks + index;
				if (ptr == entity_ptr) continue;

				Block &block = blocks[index];
				if (position.region.x == block.position.region.x && 
					position.region.y == block.position.region.y && 
					position.region.z == block.position.region.z) {
					Vec2s block_cell = world_map_tile_to_cell(block.position.tile.xy);
					if (block_cell.x == cell_index.x && block_cell.y == cell_index.y) {
						World_Position block_new_position = block.position;
						switch (move_dir) {
						case Face_EAST:  block_new_position.tile.x -= 1; break;
						case Face_WEST:	 block_new_position.tile.x += 1; break;
						case Face_NORTH: block_new_position.tile.y += 1; break;
						case Face_SOUTH: block_new_position.tile.y -= 1; break;
						}
						world_position_normalize(&block_new_position);

						if (world_map_is_position_available(world_map, lights, light_count, blocks, block_count, move_dir, block_new_position, ptr)) {
							block.position = block_new_position;
							return true;
						} else {
							return false;
						}
					}
				}
			}

			return true;
		} else {
			return region_cell->type != Cell_Type_NULL;
		}

	}

	return false;
}

void map_region_light_area(World_Map_Region *region, Vec3s region_index, Light &light) {
	if (region_index.x == light.position.region.x && 
		region_index.y == light.position.region.y && 
		region_index.z == light.position.region.z) {
		Vec2s index = world_map_tile_to_cell(light.position.tile.xy);

		int x_index = index.x - light.radius + 1;
		int y_index = index.y - light.radius + 1;
		int x_count = index.x + light.radius;
		int y_count = index.y + light.radius;

		for (int y = y_index; y < y_count; ++y) {
			for (int x = x_index; x < x_count; ++x) {
				World_Region_Cell *cell = world_map_region_get_cell_ranged(region, x, y);
				if (cell) {
					cell->visibility = Cell_Visibility_LIGHT;
				}
			}
		}
	}
}

void map_region_update(World_Map *world, World_Map_Region *region, Vec3s region_index, Player *player, Light *lights, int light_count) {
	for (int cell_index_y = 0; cell_index_y < MAP_REGION_Y_CELL_COUNT; ++cell_index_y) {
		for (int cell_index_x = 0; cell_index_x < MAP_REGION_X_CELL_COUNT; ++cell_index_x) {
			World_Region_Cell &cell = region->cells[cell_index_y][cell_index_x];
			if (cell.visibility == Cell_Visibility_VISIBLE) {
				cell.visibility = Cell_Visibility_VISITED;
			}
		}
	}

	if (region_index.x == player->position.region.x && 
		region_index.y == player->position.region.y && 
		region_index.z == player->position.region.z) {
		Vec2s index = world_map_tile_to_cell(player->position.tile.xy);

		enum {
			CENTER, LEFT, RIGHT, 
			FORWARD_CENTER, FORWARD_LEFT, FORWARD_RIGHT,
			FORWARD_CENTER2, FORWARD_LEFT2, FORWARD_RIGHT2,
			TOTAL_DIR_COUNT
		};

		World_Region_Cell *cells[TOTAL_DIR_COUNT];

		cells[CENTER] = &region->cells[index.y][index.x];

		switch (player->face) {
		case Face_EAST: {
			cells[LEFT]   = world_map_region_get_cell_ranged(region, index.x, index.y - 1);
			cells[RIGHT]  = world_map_region_get_cell_ranged(region, index.x, index.y + 1);
			cells[FORWARD_CENTER]  = world_map_region_get_cell_ranged(region, index.x - 1, index.y);
			cells[FORWARD_LEFT]  = world_map_region_get_cell_ranged(region, index.x - 1, index.y - 1);
			cells[FORWARD_RIGHT]  = world_map_region_get_cell_ranged(region, index.x - 1, index.y + 1);
			cells[FORWARD_CENTER2]  = world_map_region_get_cell_ranged(region, index.x - 2, index.y);
			cells[FORWARD_LEFT2]  = world_map_region_get_cell_ranged(region, index.x - 2, index.y - 1);
			cells[FORWARD_RIGHT2]  = world_map_region_get_cell_ranged(region, index.x - 2, index.y + 1);
		} break;

		case Face_WEST: {
			cells[LEFT]   = world_map_region_get_cell_ranged(region, index.x, index.y + 1);
			cells[RIGHT]  = world_map_region_get_cell_ranged(region, index.x, index.y - 1);
			cells[FORWARD_CENTER]  = world_map_region_get_cell_ranged(region, index.x + 1, index.y);
			cells[FORWARD_LEFT]  = world_map_region_get_cell_ranged(region, index.x + 1, index.y + 1);
			cells[FORWARD_RIGHT]  = world_map_region_get_cell_ranged(region, index.x + 1, index.y - 1);
			cells[FORWARD_CENTER2]  = world_map_region_get_cell_ranged(region, index.x + 2, index.y);
			cells[FORWARD_LEFT2]  = world_map_region_get_cell_ranged(region, index.x + 2, index.y + 1);
			cells[FORWARD_RIGHT2]  = world_map_region_get_cell_ranged(region, index.x + 2, index.y - 1);
		} break;

		case Face_NORTH: {
			cells[LEFT]   = world_map_region_get_cell_ranged(region, index.x - 1, index.y);
			cells[RIGHT]  = world_map_region_get_cell_ranged(region, index.x + 1, index.y);
			cells[FORWARD_CENTER]  = world_map_region_get_cell_ranged(region, index.x, index.y + 1);
			cells[FORWARD_LEFT]  = world_map_region_get_cell_ranged(region, index.x - 1, index.y + 1);
			cells[FORWARD_RIGHT]  = world_map_region_get_cell_ranged(region, index.x + 1, index.y + 1);
			cells[FORWARD_CENTER2]  = world_map_region_get_cell_ranged(region, index.x, index.y + 2);
			cells[FORWARD_LEFT2]  = world_map_region_get_cell_ranged(region, index.x - 1, index.y + 2);
			cells[FORWARD_RIGHT2]  = world_map_region_get_cell_ranged(region, index.x + 1, index.y + 2);
		} break;

		case Face_SOUTH: {
			cells[LEFT]   = world_map_region_get_cell_ranged(region, index.x + 1, index.y);
			cells[RIGHT]  = world_map_region_get_cell_ranged(region, index.x - 1, index.y);
			cells[FORWARD_CENTER]  = world_map_region_get_cell_ranged(region, index.x, index.y - 1);
			cells[FORWARD_LEFT]  = world_map_region_get_cell_ranged(region, index.x + 1, index.y - 1);
			cells[FORWARD_RIGHT]  = world_map_region_get_cell_ranged(region, index.x - 1, index.y - 1);
			cells[FORWARD_CENTER2]  = world_map_region_get_cell_ranged(region, index.x, index.y - 2);
			cells[FORWARD_LEFT2]  = world_map_region_get_cell_ranged(region, index.x + 1, index.y - 2);
			cells[FORWARD_RIGHT2]  = world_map_region_get_cell_ranged(region, index.x - 1, index.y - 2);
		} break;
		}

		cells[CENTER]->visibility = Cell_Visibility_VISIBLE;
		cells[LEFT]->visibility = Cell_Visibility_VISIBLE;
		cells[RIGHT]->visibility = Cell_Visibility_VISIBLE;
		cells[FORWARD_CENTER]->visibility = Cell_Visibility_VISIBLE;

		if (cells[FORWARD_CENTER]->type == Cell_Type_PATH || cells[LEFT]->type == Cell_Type_PATH ) {
			cells[FORWARD_LEFT]->visibility = Cell_Visibility_VISIBLE;
		}

		if (cells[FORWARD_CENTER]->type == Cell_Type_PATH || cells[RIGHT]->type == Cell_Type_PATH ) {
			cells[FORWARD_RIGHT]->visibility = Cell_Visibility_VISIBLE;
		}

		if (cells[FORWARD_CENTER]->type == Cell_Type_PATH) {
			cells[FORWARD_CENTER2]->visibility = Cell_Visibility_VISIBLE;
			cells[FORWARD_LEFT2]->visibility = Cell_Visibility_VISIBLE;
			cells[FORWARD_RIGHT2]->visibility = Cell_Visibility_VISIBLE;
		}
	}

	for (int index = 0; index < light_count; ++index) {
		map_region_light_area(region, region_index, lights[index]);
	}
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

	audio_mixer_set_volume(&mixer, 0.0f);

	Camera_Control camera_control = Camera_Control_GAME;

	ImGui_Initialize();
	Debug_ModeEnable();

	Debug_SetPresentationState(false);

	Im_Mesh player_mesh;
	debug_load_mesh("dev/base.model", &player_mesh);

	World_Map world = create_test_world();

	auto music = play_audio(&mixer, &audio, true);

	auto player_sprite = debug_load_texture("../res/misc/player.png");
	auto square_texture = debug_load_texture("../res/misc/square.png");
	auto debug_font = debug_load_font("dev/debug.font");

	//
	//
	//

	Player            player;
	Game_Controller controller			= {};
	Camera            camera			= camera_create(player.position);
	Camera_Controller camera_controller = {};

	Light light;
	light.position.region = player.position.region;
	light.position.tile = vec3(5, 5, 0);
	light.radius = 2;

	Block blocks[2];
	blocks[0].position.region = player.position.region;
	blocks[0].position.tile = vec3(-5, -2, 0);

	blocks[1].position.region = player.position.region;
	blocks[1].position.tile = vec3(2, -2, 0);

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

	while (running) {
		Debug_TimedFrameBegin();

		camera_controller.x = 0;
		camera_controller.y = 0;

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
					case Key_F6:
						// TODO: properly reset level!!
						Debug_NotifySuccess("Level Reset");
						player = Player{};
						camera = camera_create(player.position);
						break;
					case Key_F7:
						if (camera.view.kind == Camera_View_Kind::ORTHOGRAPHIC) {
							Debug_NotifySuccess("Perspective View");
							camera_set_view(&camera, Camera_View_Kind::PERSPECTIVE);
						} else {
							Debug_NotifySuccess("Orthographic View");
							camera_set_view(&camera, Camera_View_Kind::ORTHOGRAPHIC);
						}
						break;

					case Key_F8:
						if (camera_control == Camera_Control_FREE) {
							Debug_NotifySuccess("Game Camera");
							camera_control = Camera_Control_GAME;
						} else {
							Debug_NotifySuccess("Free Camera");
							camera_control = Camera_Control_FREE;
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

			if (event.type & Event_Type_MOUSE) {
				if ((event.type & Event_Type_MOUSE_BUTTON) && event.mouse_button.symbol == Button_LEFT) {
					camera_controller.is_valid = (event.mouse_button.state == Key_State_DOWN);
				}

				if (event.type == Event_Type_MOUSE_AXIS) {
					camera_controller.x = event.mouse_axis.x;
					camera_controller.y = event.mouse_axis.y;
				}
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
						controller.x = value;
						break;
					case Key_A:
					case Key_LEFT:
						controller.x = -value;
						break;

					case Key_W:
					case Key_UP:
						controller.y = value;
						break;
					case Key_S:
					case Key_DOWN:
						controller.y = -value;
						break;

					case Key_Q:
					case Key_PAD_0:
						controller.grab = (event.key.state == Key_State_DOWN);
						break;
				}
			}

			if (event.type == Event_Type_CONTROLLER_AXIS) {
				switch (event.controller_axis.symbol) {
					case Controller_Axis_LTHUMB_X:
						controller.x = event.controller_axis.value;
						break;

					case Controller_Axis_LTHUMB_Y:
						controller.y = event.controller_axis.value;
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

		player.movement_direction = vec2(0);

		if (camera_control == Camera_Control_GAME) {
			Vec2 movement_direction = vec2_normalize_check(vec2(controller.x, controller.y));
			r32 x_move_abs = fabsf(movement_direction.x);
			r32 y_move_abs = fabsf(movement_direction.y);
			if (y_move_abs > x_move_abs) {
				movement_direction = vec2(0, (r32)sgn(movement_direction.y));
			} else if (x_move_abs > y_move_abs) {
				movement_direction = vec2((r32)sgn(movement_direction.x), 0);
			} else {
				movement_direction = vec2(0);
			}
			player.movement_direction = movement_direction;
		}

		while (accumulator_t >= fixed_dt) {
			Debug_TimedScope(SimulationFrame);

			if (state == Time_State_RESUME) {
				player.face_direction = rotate_toward(player.face_direction, face_quaternion(player.face), player.turn_velocity * dt);

				if (controller.grab) {
					for (int i = 0; i < static_count(blocks); ++i) {
						auto *block = blocks + i;
						if (block->position.region.x == player.position.region.x &&
							block->position.region.y == player.position.region.y &&
							block->position.region.z == player.position.region.z) {
							if (fabsf(block->position.tile.x - player.position.tile.x) <= 1.0f &&
								fabsf(block->position.tile.y - player.position.tile.y) <= 1.0f &&
								fabsf(block->position.tile.z - player.position.tile.z) <= 1.0f) {
								player.grabbed = block;
								break;
							}
						}
					}

				} else {
					player.grabbed = nullptr;
				}

				bool accept_input = false;
				if (player.state == Player::IDEL) {
					accept_input = true;
				} else if (player.state == Player::WALKING) {
					Vec2 direction = world_position_unpack(player.position, player.render_position).xy;
					r32 distance = vec2_length(direction);

					if (distance) {
						r32 acceleration = (player.movement_force / player.mass);
						player.dp += dt * acceleration;
						player.dp *= powf(0.5f, player.drag * dt);
						r32 change_in_position = dt * player.dp;

						if (change_in_position < distance) {
							direction /= distance;
							player.render_position.tile.xy += direction * change_in_position;
							world_position_normalize(&player.render_position);
						} else {
							player.render_position = player.position;
						}

					} else {
						player.state = Player::IDEL;
					}

					if (distance < 0.25f)
						accept_input = true;
				}

				if (accept_input) {
					if (player.movement_direction.y > 0 && player.face != Face_NORTH) {
						player.face = Face_NORTH;
					} else if (player.movement_direction.y < 0 && player.face != Face_SOUTH) {
						player.face = Face_SOUTH;
					} else if (player.movement_direction.x > 0 && player.face != Face_WEST) {
						player.face = Face_WEST;
					} else if (player.movement_direction.x < 0 && player.face != Face_EAST) {
						player.face = Face_EAST;
					} else {
						World_Position new_position = player.position;
						new_position.tile.xy += player.movement_direction;
						world_position_normalize(&new_position);
						if (world_map_is_position_available(&world, &light, 1, blocks, static_count(blocks), player.face, new_position, &player)) {
							if (!world_position_are_same(player.position, new_position)) {
								player.state = Player::WALKING;
								player.position = new_position;

								if (player.grabbed) {
									new_position = player.grabbed->position;
									new_position.tile.xy += player.movement_direction;
									world_position_normalize(&new_position);
									if (world_map_is_position_available(&world, &light, 1, blocks, static_count(blocks), player.face, new_position, player.grabbed)) {
										player.grabbed->position = new_position;
									}
								}
							} else {
								player.dp = 0;
							}
						}
					}

				}

				//
				//
				//

				if (camera_control == Camera_Control_GAME) {
					World_Position camera_target_new = camera.position_target;
					camera_target_new.region = player.render_position.region;
					camera_target_new.tile.xy = player.render_position.tile.xy;

					bool fix_camera_to_regions_end = true;

					if (fix_camera_to_regions_end) {
						auto    cam_bounds = camera_get_bounds(camera);

						Vec2 camera_min = camera_target_new.tile.xy + vec2(cam_bounds.left, cam_bounds.bottom);
						Vec2 camera_max = camera_target_new.tile.xy + vec2(cam_bounds.right, cam_bounds.top);

						Vec2 min_cell = camera_min + vec2(MAP_REGION_HALF_X_CELL_COUNT, MAP_REGION_HALF_Y_CELL_COUNT);
						Vec2 max_cell = camera_max + vec2(MAP_REGION_HALF_X_CELL_COUNT, MAP_REGION_HALF_Y_CELL_COUNT);

						if (min_cell.x < 0 && 
							!world_map_has_region(&world, camera_target_new.region.x - 1, camera_target_new.region.y, camera_target_new.region.z)) {
							camera_target_new.tile.x -= min_cell.x + 1;
						}
						else if (max_cell.x >= MAP_REGION_X_CELL_COUNT &&
							!world_map_has_region(&world, camera_target_new.region.x + 1, camera_target_new.region.y, camera_target_new.region.z)) {
							camera_target_new.tile.x += (MAP_REGION_X_CELL_COUNT - max_cell.x);
						}

						if (min_cell.y < 0 &&
							!world_map_has_region(&world, camera_target_new.region.x, camera_target_new.region.y - 1, camera_target_new.region.z)) {
							camera_target_new.tile.y -= min_cell.y + 1;
						}
						else if (max_cell.y >= MAP_REGION_Y_CELL_COUNT &&
							!world_map_has_region(&world, camera_target_new.region.x, camera_target_new.region.y + 1, camera_target_new.region.z)) {
							camera_target_new.tile.y += (MAP_REGION_Y_CELL_COUNT - max_cell.y);
						}
					}
						
					camera.position_target = camera_target_new;

					Vec2 camera_position_target;
					camera_position_target.x = (r32)(MAP_REGION_X_CELL_COUNT * (camera.position_target.region.x - camera.position.region.x));
					camera_position_target.x += camera.position_target.tile.x - camera.position.tile.x;

					camera_position_target.y = (r32)(MAP_REGION_Y_CELL_COUNT * (camera.position_target.region.y - camera.position.region.y));
					camera_position_target.y += camera.position_target.tile.y - camera.position.tile.y;

					camera.position.tile.xy += lerp(vec2(0), camera_position_target, 1.0f - powf(1.0f - 0.99f, dt));
					camera.position.tile.z = lerp(camera.position.tile.z, camera.position_target.tile.z, 1.0f - powf(1.0f - 0.8f, dt));
					world_position_normalize(&camera.position);
					camera.orientation = slerp(camera.orientation, camera.orientation_target, 1.0f - powf(1.0f - 0.99f, dt));
				} else {
					const r32 CAMERA_MOVE_SPEED = 0.5f;
					const r32 CAMERA_TURN_SPEED = 2;

					if (camera_controller.is_valid) {
						camera.orientation = 
							quat_angle_axis(vec3(0, 0, -1), camera_controller.x) *
							quat_angle_axis(quat_right(camera.orientation), -camera_controller.y) *
							camera.orientation;
					}

					Vec2 cam_move = vec2_normalize_check(vec2(controller.x, controller.y));
					camera.position.tile += cam_move.x * CAMERA_MOVE_SPEED * quat_right(camera.orientation);
					camera.position.tile -= cam_move.y * CAMERA_MOVE_SPEED * quat_up(camera.orientation);
					world_position_normalize(&camera.position);
				}
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

		r32 thickness = 15.0f / framebuffer_h;

		Mat4 transform = camera_get_transform(camera);

		gfx_begin_drawing(Framebuffer_Type_HDR, Clear_COLOR | Clear_DEPTH, vec4(0));
		gfx_viewport(0, 0, framebuffer_w, framebuffer_h);

		Vec2 cursorp = system_get_cursor_position();
		if (cursorp.x < render_x) cursorp.x = render_x;
		if (cursorp.y < render_y) cursorp.y = render_y;
		if (cursorp.x > render_x + render_w) cursorp.x = render_x + render_w;
		if (cursorp.y > render_y + render_h) cursorp.y = render_y + render_h;
		cursorp.x -= render_x;
		cursorp.y -= render_y;
		cursorp.x /= render_w;
		cursorp.y /= render_h;
		cursorp.x *= 2;
		cursorp.y *= 2;
		cursorp.x -= 1;
		cursorp.y -= 1;

		Mat4 view_inv = mat4_inverse(gfx_view_transform(camera.view));
		cursorp = (view_inv * vec4(cursorp, 0, 0)).xy;

		World_Position cursor = camera.position;
		cursor.tile.xy += cursorp;
		cursor.tile.x = roundf(cursor.tile.x);
		cursor.tile.y = roundf(cursor.tile.y);

		if (system_key(Key_B) == Key_State_DOWN) {
			auto cell_index = world_map_tile_to_cell(cursor.tile.xy);
			auto region = world_map_get_region(&world, camera.position.region.x, camera.position.region.y, camera.position.region.z);
			auto cell = world_map_region_get_cell_ranged(region, cell_index.x, cell_index.y);
			if (cell) {
				cell->type = Cell_Type_PATH;
			}
		}
		if (system_key(Key_V) == Key_State_DOWN) {
			auto cell_index = world_map_tile_to_cell(cursor.tile.xy);
			auto region = world_map_get_region(&world, camera.position.region.x, camera.position.region.y, camera.position.region.z);
			auto cell = world_map_region_get_cell_ranged(region, cell_index.x, cell_index.y);
			if (cell) {
				cell->type = Cell_Type_NULL;
			}
		}

		im2d_begin(camera.view, transform);
		im2d_bind_texture(square_texture);

		constexpr int MAP_MAX_RENDER_REGION_X = 1;
		constexpr int MAP_MAX_RENDER_REGION_Y = 1;
		constexpr int MAP_MAX_RENDER_REGION_Z = 0;

		int map_region_start_index_z = camera.position.region.z - MAP_MAX_RENDER_REGION_Z;
		int map_region_start_index_y = camera.position.region.y - MAP_MAX_RENDER_REGION_Y;
		int map_region_start_index_x = camera.position.region.x - MAP_MAX_RENDER_REGION_X;

		for (int region_index_z = map_region_start_index_z; region_index_z < MAP_MAX_RENDER_REGION_Z + 1; ++region_index_z) {
			for (int region_index_y = map_region_start_index_y; region_index_y < MAP_MAX_RENDER_REGION_Y + 1; ++region_index_y) {
				for (int region_index_x = map_region_start_index_x; region_index_x < MAP_MAX_RENDER_REGION_X + 1; ++region_index_x) {
					World_Map_Region *map_region = world_map_get_region(&world, region_index_x, region_index_y, region_index_z);

					if (map_region) {
						map_region_update(&world, map_region, vec3s(region_index_x, region_index_y, region_index_z), &player, &light, 1);

						r32 region_y = (r32)((region_index_y - camera.position.region.y) * MAP_REGION_Y_CELL_COUNT);
						r32 region_x = (r32)((region_index_x - camera.position.region.x) * MAP_REGION_X_CELL_COUNT);
						//im3d_cube(vec3(region_x, region_y, 0.5f), quat_identity(), vec3(MAP_REGION_X_CELL_COUNT, MAP_REGION_Y_CELL_COUNT, 0.1f), 
						//	vec4(0.7f, 0.7f, 0.7f));

						for (int cell_index_y = 0; cell_index_y < MAP_REGION_Y_CELL_COUNT; ++cell_index_y) {
							for (int cell_index_x = 0; cell_index_x < MAP_REGION_X_CELL_COUNT; ++cell_index_x) {
								World_Region_Cell* region_cell = world_map_region_get_cell(map_region, cell_index_x, cell_index_y);

								Vec2 draw_pos;

								draw_pos.y = region_y;
								draw_pos.x = region_x;

								draw_pos += world_map_cell_to_tile(cell_index_x, cell_index_y);

								r32 factor;
								if (region_cell->visibility == Cell_Visibility_DISABLED) {
									factor = 0.2f;
								} else if (region_cell->visibility == Cell_Visibility_VISITED) {
									factor = 0.5f;
								} else {
									factor = 1;
								}

								if (region_cell->type == Cell_Type_NULL) {
									Vec4 color = vec4(factor * vec3(0.7f, 0.4f, 0.2f));
									im2d_rect_centered(draw_pos, vec2(1), color);
									//im3d_cube(vec3(draw_pos, 1), quat_identity(), vec3(1), vec4(0.7f, 0.4f, 0.2f));
								} else {
									im2d_rect_centered(draw_pos, vec2(1), vec4(factor * vec3(0.3f, 0.7f, 0.2f)));
								}
							}
						}
					}

				}
			}
		}

		for (int i = 0; i < static_count(blocks); ++i) {
			auto &block = blocks[i];
			Vec3 block_draw_pos;
			block_draw_pos.y = (r32)((block.position.region.y - camera.position.region.y) * MAP_REGION_Y_CELL_COUNT);
			block_draw_pos.x = (r32)((block.position.region.x - camera.position.region.x) * MAP_REGION_X_CELL_COUNT);
			block_draw_pos.xy += block.position.tile.xy;
			block_draw_pos.z = 0;

			im2d_rect_centered(block_draw_pos, vec2(1), vec4(0.9f, 0.6f, 0.5f));
		}

		auto cam_bounds = camera_get_bounds(camera);
		Vec2 draw_dim   = vec2(cam_bounds.right - cam_bounds.left, cam_bounds.top - cam_bounds.bottom);

		Vec3 player_draw_pos;
		player_draw_pos.y = (r32)((player.render_position.region.y - camera.position.region.y) * MAP_REGION_Y_CELL_COUNT);
		player_draw_pos.x = (r32)((player.render_position.region.x - camera.position.region.x) * MAP_REGION_X_CELL_COUNT);
		player_draw_pos.xy += player.render_position.tile.xy;
		player_draw_pos.z = 0;
		//player_draw_pos.z = 1.5f;

		r32  angle;
		Vec3 axis;
		quat_get_angle_axis(player.face_direction, &angle, &axis);

		im2d_bind_texture(player_sprite);
		im2d_rect_centered_rotated(player_draw_pos, vec2(1), axis.z * angle, vec4(1));
		//im3d_mesh(player_mesh, player_draw_pos, player.face_direction, vec3(0.5f), vec4(0.1f, 0.7f, 0.8f));

		im2d_unbind_texture();

		{
			Vec3 overlay_draw_pos;
			overlay_draw_pos.y = (r32)((cursor.region.y - camera.position.region.y) * MAP_REGION_Y_CELL_COUNT);
			overlay_draw_pos.x = (r32)((cursor.region.x - camera.position.region.x) * MAP_REGION_X_CELL_COUNT);
			overlay_draw_pos.xy += cursor.tile.xy;
			overlay_draw_pos.z = 0;

			im2d_rect_centered(overlay_draw_pos, vec2(1), vec4(0.8f, 0.8f, 0.2f, 0.7f));
		}

		{
			Vec3 light_draw_pos;
			light_draw_pos.y = (r32)((light.position.region.y - camera.position.region.y) * MAP_REGION_Y_CELL_COUNT);
			light_draw_pos.x = (r32)((light.position.region.x - camera.position.region.x) * MAP_REGION_X_CELL_COUNT);
			light_draw_pos.xy += light.position.tile.xy;
			light_draw_pos.z = 0;

			im2d_circle(light_draw_pos, 0.4f, vec4(0.3f, 0.8f, 0.7f, 1));
		}

		im2d_end();

		gfx_end_drawing();

#if 0
		if (emitter.texture)
			im3d_bind_texture(*emitter.texture);

		for (int i = emitter.emit_count; i >= 0; --i) {
			Particle *particle = emitter.particles + i;
			if (particle->life <= particle->life_span) {
				r32 t = particle->life / particle->life_span;

				r32 fade_t = 1;
				if (particle->life < emitter.properties.fade_in) {
					fade_t *= particle->life / emitter.properties.fade_in;
				}
				else if (particle->life_span - particle->life < emitter.properties.fade_out) {
					fade_t *= (particle->life_span - particle->life) / emitter.properties.fade_out;
				}

				auto particle_color = hsv_to_rgb(lerp(particle->color_a, particle->color_b, t));
				particle_color.xyz *= emitter.properties.intensity;
				particle_color.w *= (fade_t * emitter.properties.opacity);
				im3d_rect_rotated(particle->position, vec2(lerp(particle->scale_a, particle->scale_b, t)), particle->rotation, particle_color);
			}

			im3d_unbind_texture();
		}

		PrintRigidBodies();
#endif

		gfx_apply_bloom(2);

		gfx_begin_drawing(Framebuffer_Type_DEFAULT, Clear_COLOR, vec4(0, 0, 0));

		gfx_blit_hdr(render_x, render_y, render_w, render_h);
		gfx_viewport(0, 0, window_w, window_h);

#if defined(BUILD_IMGUI)
		{
			Debug_TimedScope(ImGuiRender);
			editor_update(&mixer, &player, &camera);
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
