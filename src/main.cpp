#include "karma.h"        // shared
#include "systems.h"      // windows
#include "gfx_renderer.h" // rendering
#include "length_string.h"
#include "lin_maths.h"
#include "imgui/imgui.h"
#include "debug.h"
#include "particle_system.h"
#include "stream.h"
#include "audio.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#if 0
#	include <time.h>
#	include <stdlib.h>
#	define NUM_RIGID_BODIES 20

int initial = 0;
int xy      = 2000;

typedef struct {
	float width;
	float height;
	float mass;
	float momentofInertia;
} BoxShape;

//=========two dimensional rigid body=========#

typedef struct {
	Vec2     position;
	Vec2     velocity;
	Vec2     linear_velocity;
	Vec2     linear_acceleration;
	float    angular_velocity;
	float    angle;
	Vec2     force;
	float    torque;
	BoxShape shape;
	Mm_Rect  min_max_points;
	bool     collided;
} RigidBody;

RigidBody rigidBodies[NUM_RIGID_BODIES];

int random_number_generator(int n) {
	//srand(int(time(NULL)));
	return (rand() % n + 1);
}

int neg_random_number_generator(int n) {
	//srand(int(time(NULL)));
	return (rand() % n - n + 1);
}

void CalculateBoxInertia(BoxShape *boxShape) {
	float m                   = boxShape->mass;
	float h                   = boxShape->height;
	float w                   = boxShape->width;
	boxShape->momentofInertia = m * (w * w + h * h) / 2;
}

void ComputeForceandTorque(RigidBody *rigidBody) {
	Vec2 f;
	//f = { r32(2000), r32(2000) };
	f = vec2(r32(neg_random_number_generator(2 + xy)), r32(neg_random_number_generator(1 + xy)));
	//if(initial==1)
	//	 f = { r32(1500), r32(2000) };
	//else
	//	 f = { r32(1500), r32(-2000) };

	rigidBody->force  = f;
	Vec2 r            = vec2(rigidBody->shape.width / 2, rigidBody->shape.height / 2);
	rigidBody->torque = r.x * f.y - r.y * f.x;
	xy += 1;
	initial += 1;
}

void InitializeRigidBodies() {
	int x = 0;
	for (int i = 0; i < NUM_RIGID_BODIES; i++) {
		RigidBody *rigidBody = &rigidBodies[i];
		ComputeForceandTorque(rigidBody);
		rigidBody->position = vec2(r32(random_number_generator(350 + x)), r32(random_number_generator(650 + x)));
		//if (i == 0)
		//	rigidBody->position = { 500,100 };
		//if (i==1)
		//	rigidBody->position = { 500,500 };
		rigidBody->angle            = float(random_number_generator(360) / 360.0f * MATH_PI * 2);
		rigidBody->linear_velocity  = vec2(0);
		rigidBody->angular_velocity = 0;
		rigidBody->collided         = false;

		BoxShape shape;
		shape.mass = float(random_number_generator(5000));

		shape.width  = 20 * float(random_number_generator(2));
		shape.height = 20 * float(random_number_generator(4));
		CalculateBoxInertia(&shape);
		rigidBody->shape = shape;

		//rigidBody->linear_acceleration = { rigidBody->force.x / rigidBody->shape.mass, rigidBody->force.y / rigidBody->shape.mass };
		rigidBody->linear_acceleration.x = rigidBody->force.x / rigidBody->shape.mass;
		rigidBody->linear_acceleration.y = rigidBody->force.y / rigidBody->shape.mass;

		rigidBody->min_max_points.min = vec2(rigidBody->position.x, rigidBody->position.y);
		rigidBody->min_max_points.max = vec2(rigidBody->position.x + rigidBody->shape.width, rigidBody->position.y + rigidBody->shape.height);
	}
}

void update_min_max(RigidBody *rigidBody) {
	for (int i = 0; i < NUM_RIGID_BODIES; i++) {
		RigidBody *rigidBody          = &rigidBodies[i];
		rigidBody->min_max_points.min = vec2(rigidBody->position.x, rigidBody->position.y);
		rigidBody->min_max_points.max = vec2(rigidBody->position.x + rigidBody->shape.width, rigidBody->position.y + rigidBody->shape.height);
	}
}

void PrintRigidBodies() {
	for (int i = 0; i < NUM_RIGID_BODIES; i++) {
		RigidBody *rigidBody = &rigidBodies[i];
		im_rect(vec2(rigidBody->position.x, rigidBody->position.y), vec2(rigidBody->shape.width, rigidBody->shape.height), vec4(vec3(1.1f, 1.2f, 1.5f), 1.0f));
	}
}
#endif

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

enum {
	Cell_Type_PLACE  = 0,
	Cell_Type_VACUUM = 1,
};

struct World_Region_Cell {
	int id;
};

constexpr int  MAP_REGION_X_CELL_COUNT			= 25;
constexpr int  MAP_REGION_Y_CELL_COUNT			= 15;
constexpr int  MAP_REGION_HALF_X_CELL_COUNT		= MAP_REGION_X_CELL_COUNT / 2;
constexpr int  MAP_REGION_HALF_Y_CELL_COUNT		= MAP_REGION_Y_CELL_COUNT / 2;

static World_Region_Cell map_cells0[MAP_REGION_Y_CELL_COUNT][MAP_REGION_X_CELL_COUNT] = {
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0 },
	{ 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0 },
	{ 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1 },
	{ 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
};

static World_Region_Cell map_cells1[MAP_REGION_Y_CELL_COUNT][MAP_REGION_X_CELL_COUNT] = {
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
};

struct World_Position {
	Vec3s region;
	Vec2  tile;
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
	result.x = (position.region.x - center.region.x) * MAP_REGION_HALF_X_CELL_COUNT + (position.tile.x - center.tile.x);
	result.y = (position.region.y - center.region.y) * MAP_REGION_HALF_Y_CELL_COUNT + (position.tile.y - center.tile.y);
	result.z = (r32)(position.region.z - center.region.z);
	return result;
}

void world_position_normalize(World_Position *position) {
	Vec2s cell_index = world_map_tile_to_cell(position->tile);

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

bool world_map_is_position_available(World_Map *world_map, World_Position &position) {
	World_Map_Region* map_region = world_map_get_region(world_map, position.region.x, position.region.y, position.region.z);

	if (map_region) {
		Vec2s cell_index = world_map_tile_to_cell(position.tile);
		World_Region_Cell* region_cell = world_map_region_get_cell(map_region, cell_index.x, cell_index.y);
		return region_cell->id != Cell_Type_VACUUM;
	}

	return false;
}

World_Map create_test_world() {
	static World_Map_Region test_map_regions[2];
	test_map_regions[0].cells = map_cells0;
	test_map_regions[1].cells = map_cells1;

	World_Map map;
	map.x_count = 2;
	map.y_count = 1;
	map.z_count = 1;

	map.x_index_offset = map.x_count / 2;
	map.y_index_offset = map.y_count / 2;
	map.z_index_offset = map.z_count / 2;

	if (map.x_count % 2 == 0) map.x_index_offset -= 1;
	if (map.y_count % 2 == 0) map.y_index_offset -= 1;
	if (map.z_count % 2 == 0) map.z_index_offset -= 1;

	map.regions = (World_Map_Region **)memory_allocate(sizeof(World_Map_Region *) * map.x_count * map.y_count * map.z_count);

	map.regions[0] = &test_map_regions[0];
	map.regions[1] = &test_map_regions[1];

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

struct Player {
	enum State {
		IDEL,
		WALKING
	};

	World_Position position = {};
	World_Position render_position    = position;

	Face face				   = Face_NORTH;
	Quat face_direction        = quat_identity();

	State state = IDEL;

	r32 movement_force = 1;

	r32  mass                 = 50;
	r32  drag                 = 7;

	r32  turn_velocity      = 7;
	r32  dp					= 0;
};

struct Player_Controller {
	r32 x;
	r32 y;
};

struct Camera_Bounds {
	r32 left, right;
	r32 top, bottom;
};

struct Camera {
	World_Position position;
	World_Position position_target;

	r32 distance;
	r32 distance_target;

	r32 aspect_ratio;

	Camera_View view;
};

Camera_Bounds camera_get_bounds(Camera &camera) {
	r32 camera_zoom = 1.0f / powf(2.0f, camera.distance);

	r32 height = (r32)MAP_REGION_Y_CELL_COUNT;
	r32 half_height = 0.5f * height * camera_zoom;
	r32 half_width  = half_height * camera.aspect_ratio;

	Camera_Bounds bounds;
	bounds.left   = -half_width;
	bounds.right  = half_width;
	bounds.top    = half_height;
	bounds.bottom = -half_height;

	return bounds;
}

Camera camera_create(World_Position position, r32 distance = 0.5f, r32 target_distance = 1.0f, r32 aspect_ratio = 1280.0f / 720.0f) {
	r32 top = MAP_REGION_HALF_Y_CELL_COUNT + 0.5f;
	r32 bottom = -MAP_REGION_HALF_Y_CELL_COUNT - 0.5f;

	r32 height = top - bottom;
	r32 width = height * aspect_ratio;

	r32 right = 0.5f * width;
	r32 left = -0.5f * width;

	Camera camera;
	camera.position			 = position;
	camera.position_target   = camera.position;
	camera.distance			 = distance;
	camera.distance_target	 = target_distance;
	camera.aspect_ratio      = aspect_ratio;
	camera.view              = orthographic_view(left, right, top, bottom);

	return camera;
}

void editor_update(Audio_Mixer *mixer, Player *player) {
	if (!editor.display)
		return;

	ImGui::Begin("Editor");

	r32 volume = mixer->volume_b;
	ImGui::DragFloat("Volume", &volume, 0.01f, 0.0f, 1.0f);
	ImGui::Checkbox("Guide", &editor.guide);

#if 0
	ImGui::Text("Player");
	ImGui::DragFloat3("Target Position: (%.3f, %.3f, %.3f)", player->position.m);
	ImGui::Text("Face Direction: (%.3f, %.3f, %.3f)", player->face_direction.x, player->face_direction.y, player->face_direction.z);
	ImGui::DragFloat("Mass: %.3f", &player->mass, 0.1f);
	ImGui::DragFloat("Drag: %.3f", &player->drag, 0.01f);
	ImGui::DragFloat("Movement Force: %.3f", &player->movement_force, 0.1f);
	ImGui::DragFloat("Turn Speed: %.3f", &player->turn_velocity, 0.01f);
#endif

	ImGui::End();

	audio_mixer_set_volume(mixer, volume);
}

int system_main() {
	r32    framebuffer_w = 1280;
	r32    framebuffer_h = 720;
	Handle platform      = system_create_window(u8"Karma", 1280, 720, System_Window_Show_NORMAL);
	gfx_create_context(platform, Render_Backend_DIRECTX11, Vsync_ADAPTIVE, 2, (u32)framebuffer_w, (u32)framebuffer_h);

	auto audio        = load_wave(system_read_entire_file("../res/misc/POL-course-of-nature-short.wav"));
	auto bounce_sound = load_wave(system_read_entire_file("../res/misc/Boing Cartoonish-SoundBible.com-277290791.wav"));

	Audio_Mixer mixer = audio_mixer();

	if (!system_audio(system_update_audio, system_refresh_audio_device, &mixer)) {
		system_display_critical_message("Failed to load audio!");
	}

	audio_mixer_set_volume(&mixer, 0.0f);

	ImGui_Initialize();
	Debug_ModeEnable();

	Debug_SetPresentationState(false);

#if 0
	InitializeRigidBodies();
#endif

	World_Map world = create_test_world();

	auto music = play_audio(&mixer, &audio, true);

	auto player_sprite = debug_load_texture("../res/misc/player.png");

	//
	//
	//

	Player            player;
	Player_Controller controller = {};
	Camera            camera     = camera_create(player.position);

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

		Debug_TimedBlockEnd(EventHandling);

		Debug_TimedBlockBegin(Simulation);

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

		while (accumulator_t >= fixed_dt) {
			Debug_TimedScope(SimulationFrame);

			if (state == Time_State_RESUME) {
				player.face_direction = rotate_toward(player.face_direction, face_quaternion(player.face), player.turn_velocity * dt);

				bool accept_input = false;
				if (player.state == Player::IDEL) {
					accept_input = true;
				} else if (player.state == Player::WALKING) {
					Vec2 direction = player.position.tile - player.render_position.tile;
					r32 length = vec2_length(direction);

					if (length) {
						r32 acceleration = (player.movement_force / player.mass);
						player.dp += dt * acceleration;
						player.dp *= powf(0.5f, player.drag * dt);
						player.render_position.tile = move_toward(player.render_position.tile, player.position.tile, dt * player.dp);
						world_position_normalize(&player.render_position);

						player.render_position = player.position;
					} else {
						player.state = Player::IDEL;
					}

					if (length < 0.25f)
						accept_input = true;
				}

				if (accept_input) {
					World_Position new_position = player.position;
					new_position.tile += movement_direction;
					world_position_normalize(&new_position);


					Face new_face_direction;
					if (movement_direction.y > 0) {
						new_face_direction = Face_NORTH;
					}
					else if (movement_direction.y < 0) {
						new_face_direction = Face_SOUTH;
					}
					else if (movement_direction.x > 0) {
						new_face_direction = Face_WEST;
					}
					else if (movement_direction.x < 0) {
						new_face_direction = Face_EAST;
					}
					else {
						new_face_direction = player.face;
					}

					if (world_map_is_position_available(&world, new_position)) {
						if (!world_position_are_same(player.position, new_position)) {
							player.state = Player::WALKING;
							player.position = new_position;
						} else {
							player.dp = 0;
						}
						
					}

					player.face = new_face_direction;
				}

				//
				//
				//

				World_Position camera_target_new = player.render_position;

#if 0
				if (world_position_are_in_same_region(camera.position, camera_target_new)) {
					auto    cam_bounds = camera_get_bounds(camera);

					Mm_Rect camera_rect;
					camera_rect.min = camera_target_new.tile + vec2(cam_bounds.left, cam_bounds.bottom);
					camera_rect.max = camera_target_new.tile + vec2(cam_bounds.right, cam_bounds.top);

					camera_rect.min.x = camera_rect.min.x + MAP_REGION_HALF_X_CELL_COUNT;
					camera_rect.min.y = camera_rect.min.y + MAP_REGION_HALF_Y_CELL_COUNT;
					camera_rect.max.x = camera_rect.max.x + MAP_REGION_HALF_X_CELL_COUNT;
					camera_rect.max.y = camera_rect.max.y + MAP_REGION_HALF_Y_CELL_COUNT;

					if (camera_rect.min.x < 0) {
						camera_target_new.tile.x += camera_rect.min.x;
					} else if (camera_rect.max.x >= MAP_REGION_X_CELL_COUNT) {
						camera_target_new.tile.x += (MAP_REGION_X_CELL_COUNT - camera_rect.max.x);
					}

					if (camera_rect.min.y < 0) {
						camera_target_new.tile.y += camera_rect.min.y;
					} else if (camera_rect.max.y >= MAP_REGION_Y_CELL_COUNT) {
						camera_target_new.tile.y += (MAP_REGION_Y_CELL_COUNT - camera_rect.max.y);
					}
				}
#endif

				camera.position_target = camera_target_new;

				Vec2 camera_position_target;
				camera_position_target.x = (r32)(MAP_REGION_X_CELL_COUNT * (camera.position_target.region.x - camera.position.region.x));
				camera_position_target.x += camera.position_target.tile.x - camera.position.tile.x;

				camera_position_target.y = (r32)(MAP_REGION_Y_CELL_COUNT * (camera.position_target.region.y - camera.position.region.y));
				camera_position_target.y += camera.position_target.tile.y - camera.position.tile.y;

				camera.position.tile += lerp(vec2(0), camera_position_target, 1.0f - powf(1.0f - 0.99f, dt));
				world_position_normalize(&camera.position);

				camera.distance = lerp(camera.distance, camera.distance_target, 1.0f - powf(1.0f - 0.8f, dt));
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

		r32 camera_zoom = 1.0f / powf(2.0f, camera.distance);

		r32 thickness = 15.0f / framebuffer_h;

		Mat4 transform = mat4_inverse(mat4_translation(vec3(camera.position.tile, 0)) * mat4_scalar(camera_zoom, camera_zoom, 1));

		gfx_begin_drawing(Framebuffer_Type_HDR, Clear_COLOR | Clear_DEPTH, vec4(0));
		gfx_viewport(0, 0, framebuffer_w, framebuffer_h);

		im_begin(camera.view, transform);
		im_unbind_texture();

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
						for (int cell_index_y = 0; cell_index_y < MAP_REGION_Y_CELL_COUNT; ++cell_index_y) {
							for (int cell_index_x = 0; cell_index_x < MAP_REGION_X_CELL_COUNT; ++cell_index_x) {
								World_Region_Cell* region_cell = world_map_region_get_cell(map_region, cell_index_x, cell_index_y);

								Vec2 draw_pos;

								draw_pos.y = (r32)((region_index_y - camera.position.region.y) * MAP_REGION_Y_CELL_COUNT);
								draw_pos.x = (r32)((region_index_x - camera.position.region.x) * MAP_REGION_X_CELL_COUNT);

								draw_pos += world_map_cell_to_tile(cell_index_x, cell_index_y);

								if (region_cell->id == Cell_Type_PLACE) {
									im_rect_centered(vec3(draw_pos, 1), vec2(1), vec4(0, 0.3f, 0));
									im_rect_centered(vec3(draw_pos, 1), vec2(0.95f), vec4(0.1f, 0.7f, 0.3f));
								}
								else {
									im_rect_centered(vec3(draw_pos, 1), vec2(1), vec4(0.7f, 0.3f, 0));
									im_rect_centered(vec3(draw_pos, 1), vec2(0.95f), vec4(0.7f, 0.4f, 0.2f));
								}
							}
						}
					}

				}
			}
		}

		auto cam_bounds = camera_get_bounds(camera);
		Vec2 draw_dim   = vec2(cam_bounds.right - cam_bounds.left, cam_bounds.top - cam_bounds.bottom);

		Vec2 player_draw_pos;
		player_draw_pos.y = (r32)((player.render_position.region.y - camera.position.region.y) * MAP_REGION_Y_CELL_COUNT);
		player_draw_pos.x = (r32)((player.render_position.region.x - camera.position.region.x) * MAP_REGION_X_CELL_COUNT);
		player_draw_pos += player.render_position.tile;

		im_circle_outline(player_draw_pos, 0.5f, vec4(1, 1, 0), thickness);
		im_rect_centered_outline2d(player_draw_pos, draw_dim, vec4(1, 0, 1), thickness);

		r32  angle;
		Vec3 axis;
		quat_get_angle_axis(player.face_direction, &angle, &axis);

		im_bind_texture(player_sprite);
		im_rect_centered_rotated(player_draw_pos, vec2(1), axis.z * angle, vec4(1));

		im_end();

		gfx_end_drawing();

#if 0
		if (emitter.texture)
			im_bind_texture(*emitter.texture);

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
				im_rect_rotated(particle->position, vec2(lerp(particle->scale_a, particle->scale_b, t)), particle->rotation, particle_color);
			}

			im_unbind_texture();
		}

		PrintRigidBodies();
#endif

		gfx_apply_bloom(2);

		r32 render_w = window_w;
		r32 render_h = floorf(window_w / aspect_ratio);
		if (render_h > window_h) {
			render_h = window_h;
			render_w = floorf(window_h * aspect_ratio);
		}

		r32 render_x = floorf((window_w - render_w) * 0.5f);
		r32 render_y = floorf((window_h - render_h) * 0.5f);

		gfx_begin_drawing(Framebuffer_Type_DEFAULT, Clear_COLOR, vec4(0, 0, 0));

		gfx_blit_hdr(render_x, render_y, render_w, render_h);
		gfx_viewport(0, 0, window_w, window_h);

#if defined(BUILD_IMGUI)
		{
			Debug_TimedScope(ImGuiRender);
			editor_update(&mixer, &player);
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
