#include "karma.h"        // shared
#include "systems.h"      // windows
#include "gfx_renderer.h" // rendering
#include "length_string.h"
#include "lin_maths.h"
#include "imgui/imgui.h"
#include "debug_service.h"
#include "particle_system.h"
#include "stream.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <time.h>
#include <stdlib.h>
#define NUM_RIGID_BODIES 20

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

Mat4 make_transform(Vec3 p, Vec3 s, Quat q) {
	Mat4 m0 = mat4_scalar(s);
	Mat4 m1 = quat_get_mat4(q);
	Mat4 m2 = mat4_translation(p);
	return m2 * m1 * m0;
}

Mat4 make_camera_transform(Vec3 p, Vec3 s, Quat q) {
	s.x = 1.0f / s.x;
	s.y = 1.0f / s.y;
	s.z = 1.0f / s.z;
	q   = quat_conjugate(q);
	p   = -p;
	return make_transform(p, s, q);
}

struct Camera {
	Vec3 position;
	Vec3 scale;
	Quat rotation;

	Camera_View view;
};

void ImGui_ItemDisplay(Camera *camera) {
	ImGui::DragFloat3("Position", camera->position.m, 0.1f);
	//	ImGui::DragFloat("Scale", camera->scale.m, 0.1f);
	ImGui::DragFloat4("Rotation", camera->rotation.m, 0.1f);
	camera->scale.y = camera->scale.z = camera->scale.x;
}

struct Entity_Controller {
	r32 horizontal;
	r32 jump;
};

struct Entity {
	Vec3 position;
	Vec2 scale;

	r32  mass;
	Vec2 force;
	Vec2 velocity;
};

void ImGui_ItemDisplay(Entity *entity) {
	ImGui::DragFloat3("Position", entity->position.m, 0.1f);
	ImGui::DragFloat2("Scale", entity->scale.m, 0.1f);
	ImGui::DragFloat("Mass", &entity->mass, 0.1f);
	ImGui::DragFloat2("Force", entity->force.m, 0.1f);
	ImGui::DragFloat2("Velocity", entity->velocity.m, 0.1f);
}

void PrintRigidBodies() {
	for (int i = 0; i < NUM_RIGID_BODIES; i++) {
		RigidBody *rigidBody = &rigidBodies[i];
		im_rect(vec2(rigidBody->position.x, rigidBody->position.y), vec2(rigidBody->shape.width, rigidBody->shape.height), vec4(vec3(1.1f, 1.2f, 1.5f), 1.0f));
	}
}

bool load_debug_font(Monospaced_Font *font) {
	String content = system_read_entire_file("data/debug.font");
	defer {
		memory_free(content.data);
	};

	if (content) {
		Istream in = istream(content);

		u32 min_c   = *istream_consume(&in, u32);
		u32 max_c   = *istream_consume(&in, u32);
		r32 advance = *istream_consume(&in, r32);
		u32 size    = *istream_consume(&in, u32);

		font->info.range = (Monospaced_Font_Glyph_Range *)memory_allocate(size);
		memcpy(font->info.range, istream_consume_size(&in, size), size);
		font->info.first   = (s32)min_c;
		font->info.count   = (s32)(max_c - min_c + 2);
		font->info.advance = advance;

		int w      = *istream_consume(&in, int);
		int h      = *istream_consume(&in, int);
		int n      = *istream_consume(&in, int);
		u8 *pixels = (u8 *)istream_consume_size(&in, w * h * n);

		font->texture = gfx_create_texture2d((u32)w, (u32)h, (u32)n, Data_Format_RGBA8_UNORM_SRGB, (const u8 **)&pixels, Buffer_Usage_IMMUTABLE, 1);

		return true;
	}

	return false;
}

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

const int FREQUENCY				= 128;
const int SAMPLES_PER_SEC		= 48000;
const int SAMPLES_COUNT			= SAMPLES_PER_SEC * FREQUENCY;

static s16 sine_wave[2 * SAMPLES_COUNT * sizeof(r32)];

#pragma pack(push, 1)

struct Riff_Header {
	u8	id[4];
	u32 size;
	u8	format[4];
};

struct Wave_Fmt {
	u8	id[4];
	u32 size;
	u16 audio_format;
	u16 channels_count;
	u32 sample_rate;
	u32 byte_rate;
	u16 block_align;
	u16 bits_per_sample;
};

struct Wave_Data {
	u8 id[4];
	u32 size;
};

#pragma pack(pop)

struct Audio_Stream {
	Riff_Header *header; // TODO: do we need this?
	Wave_Fmt	*fmt;	 //	TODO: do we need this?
	Wave_Data	*data;	 // TODO: do we need this?
	s16			*samples;
	u32			sample_count;
};

Audio_Stream make_sine_audio() {
	float t = 0;
	int sample_index = 0;
	for (int i = 0; i < SAMPLES_COUNT; ++i) {
		s16 sample_value = (s16)roundf(sinf(2 * MATH_PI * FREQUENCY * t) * MAX_INT16);
		sine_wave[sample_index + 0] = sample_value;
		sine_wave[sample_index + 1] = sample_value;
		t += 1.0f / (r32)SAMPLES_PER_SEC;
		sample_index += 2;
	}

	Audio_Stream stream = {};
	stream.samples = sine_wave;
	stream.sample_count = SAMPLES_COUNT;
	return stream;
}

Audio_Stream load_wave(String content) {
	Istream in				= istream(content);
	Riff_Header *wav_header = istream_consume(&in, Riff_Header);
	Wave_Fmt	*wav_fmt	= istream_consume(&in, Wave_Fmt);
	Wave_Data	*wav_data	= istream_consume(&in, Wave_Data);
	s16			*data		= (s16 *)istream_consume_size(&in, wav_data->size);

	Audio_Stream stream;
	stream.header		= wav_header;
	stream.fmt			= wav_fmt;
	stream.data			= wav_data;
	stream.samples		= data;
	stream.sample_count	= wav_data->size / sizeof(s16) / wav_fmt->channels_count;

	return stream;
}

struct Audio {
	Audio_Stream	*stream;	// TODO: Use somekind of reference
	booli			playing;
	booli			loop;
	r32				pitch_factor;
	r32				volume;		// TODO: Seperate channel volume?
	r32				samples_played;
	r32				buffered_pitch_factor;
};

struct Audio_Node {
	Audio audio;

	Audio_Node *next;
};

struct Audio_Mixer {
	u32 samples_rate;
	u32 channel_count;

	Handle				mutex;
	Audio_Node			list; // NOTE: This is sentinel (first indicator), real node start from list.next
	u32					buffer_size_in_sample_count;
	u32					buffer_size_in_bytes;
	u32					buffer_consumed_bytes;
	r32					*buffer;

	r32 volume_a;
	r32 volume_b;
	r32 volume_position; // in samples
	r32 volume_span;	 // in samples
	r32 pitch_factor;
};

Audio_Mixer audio_mixer() {
	Audio_Mixer mixer;

	mixer.samples_rate = system_audio_sample_rate();
	mixer.channel_count = system_audio_channel_count();

	// TODO: We only support these currently
	assert(mixer.samples_rate == 48000);
	assert(mixer.channel_count == 2);

	mixer.mutex = system_create_mutex();
	mixer.list = {};
	mixer.buffer_size_in_sample_count = (mixer.samples_rate * SYSTEM_AUDIO_BUFFER_SIZE_IN_MILLISECS) / 1000;
	mixer.buffer_size_in_bytes = sizeof(r32) * mixer.buffer_size_in_sample_count * mixer.channel_count;
	mixer.buffer = (r32 *)tallocate(mixer.buffer_size_in_bytes);
	mixer.buffer_consumed_bytes = 0;

	mixer.volume_a = 0.5f;
	mixer.volume_b = 0.5f;
	mixer.volume_position = 1;
	mixer.volume_span = 1;
	mixer.pitch_factor = 1;

	return mixer;
}

void audio_mixer_set_volume(Audio_Mixer *mixer, r32 volume) {
	mixer->volume_a = mixer->volume_b = volume;
}

void audio_mixer_animate_volume_to(Audio_Mixer *mixer, r32 volume, r32 secs) {
	mixer->volume_a = lerp(mixer->volume_a, mixer->volume_b, clamp01(mixer->volume_position / mixer->volume_span));
	mixer->volume_b = volume;
	mixer->volume_position = 0;
	mixer->volume_span = mixer->samples_rate * secs;
}

void audio_mixer_animate_volume(Audio_Mixer *mixer, r32 volume_a, r32 volume_b, r32 secs) {
	mixer->volume_a = volume_a;
	mixer->volume_b = volume_b;
	mixer->volume_position = 0;
	mixer->volume_span = mixer->samples_rate * secs;
}

Audio *play_audio(Audio_Mixer *mixer, Audio_Stream *stream, bool loop) {
	// TODO: Do allocations properly!!
	Audio_Node *node = new Audio_Node;
	node->audio.stream = stream;
	node->audio.playing = true;
	node->audio.loop = loop;
	node->audio.pitch_factor = 1;
	node->audio.volume = 1;
	node->audio.samples_played = 0;
	node->audio.buffered_pitch_factor = 1;

	node->next = mixer->list.next;
	mixer->list.next = node;

	assert(node == (Audio_Node *)&node->audio); // This has to be same so as to remove audio from list easily

	return &node->audio;
}

void system_refresh_audio_device(u32 sample_rate, u32 channel_count, void *user_data) {
	Audio_Mixer *mixer = (Audio_Mixer *)user_data;

	system_lock_mutex(mixer->mutex, WAIT_INFINITE);

	mixer->samples_rate = sample_rate;
	mixer->channel_count = channel_count;

	// TODO: We only support these currently
	assert(mixer->samples_rate == 48000);
	assert(mixer->channel_count == 2);

	mixer->buffer_size_in_sample_count = (mixer->samples_rate * SYSTEM_AUDIO_BUFFER_SIZE_IN_MILLISECS) / 1000;
	mixer->buffer_size_in_bytes = sizeof(r32) * mixer->buffer_size_in_sample_count * mixer->channel_count;
	mixer->buffer = (r32 *)tallocate(mixer->buffer_size_in_bytes);
	mixer->buffer_consumed_bytes = 0;

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
		u32 buffer_size = sizeof(r32) * channel_count * sample_count;
		u8 *src_ptr = (u8 *)mixer->buffer + mixer->buffer_consumed_bytes;
		u8 *one_past_end = (u8 *)mixer->buffer + mixer->buffer_size_in_bytes;
		assert(one_past_end >= src_ptr);
		u32 copy_size = min_value((u32)(one_past_end - src_ptr), buffer_size);
		memcpy(sys_buffer, src_ptr, copy_size);
		memset(sys_buffer + copy_size, 0, buffer_size - copy_size);
		sys_audio.unlock(sys_audio.handle, sample_count);

		mixer->buffer_consumed_bytes += copy_size;
		mixer->volume_position += sample_count * mixer->pitch_factor;

		for (Audio_Node *node = mixer->list.next ; node; node = node->next) {
			Audio &audio = node->audio;
			if (audio.playing) {
				audio.samples_played += audio.buffered_pitch_factor * (r32)sample_count;
				if ((u32)lroundf(audio.samples_played) >= audio.stream->sample_count) {
					audio.samples_played = 0;
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
			Audio &audio = node->audio;
			audio.buffered_pitch_factor	= audio.pitch_factor * mixer->pitch_factor;
			//r32 inverse_audio_pitch_factor = 1.0f / audio.pitch_factor;

			r32 *write_ptr = mixer->buffer;
			r32 read_cursor = audio.samples_played;

			for (u32 sample_counter = 0; sample_counter < mixer->buffer_size_in_sample_count;) {
				u32 more_samples_required	= (mixer->buffer_size_in_sample_count - sample_counter);
				u32 samples_available		= (u32)lroundf(((r32)audio.stream->sample_count - read_cursor) / audio.buffered_pitch_factor);
				u32 samples_to_mix			= min_value(samples_available, more_samples_required);

				r32 effective_volume;
				r32 volume_position = mixer->volume_position;

				for (u32 sample_mix_index = 0; sample_mix_index < samples_to_mix; ++sample_mix_index) {
					// TODO: Here we expect both input audio and output buffer to be stero audio

					effective_volume = audio.volume * lerp(mixer->volume_a, mixer->volume_b, (volume_position + sample_mix_index * mixer->pitch_factor) / mixer->volume_span);

					#define AUDIO_APPLY_PITCH_FILTERING

					#ifdef AUDIO_APPLY_PITCH_FILTERING

					r32 real_sample_index = read_cursor + sample_mix_index * audio.buffered_pitch_factor;
					u32 sample_index_0 = (u32)(real_sample_index);
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

					write_ptr[0] += sampled_left  * effective_volume;
					write_ptr[1] += sampled_right * effective_volume;

					#endif

					write_ptr[0] = clamp(-1.0f, 1.0f, write_ptr[0]);
					write_ptr[1] = clamp(-1.0f, 1.0f, write_ptr[1]);

					write_ptr += channel_count;
				}

				sample_counter += samples_to_mix;
				read_cursor += samples_to_mix * audio.buffered_pitch_factor;

				if (!audio.loop) {
					break;
				} else if ((u32)lroundf(read_cursor) >= audio.stream->sample_count) {
					read_cursor = 0;
				}

			}
		}
	}

	system_unlock_mutex(mixer->mutex);
}

int system_main() {
	r32    framebuffer_w = 1280;
	r32    framebuffer_h = 720;
	Handle platform      = system_create_window(u8"Karma", 1280, 720, System_Window_Show_NORMAL);
	gfx_create_context(platform, Render_Backend_DIRECTX11, Vsync_ADAPTIVE, 2, (u32)framebuffer_w, (u32)framebuffer_h);

	auto sine_audio = make_sine_audio();

	auto audio = load_wave(system_read_entire_file("../res/misc/POL-course-of-nature-short.wav"));
	auto bounce_sound = load_wave(system_read_entire_file("../res/misc/Boing Cartoonish-SoundBible.com-277290791.wav"));

	Audio_Mixer mixer = audio_mixer();
	audio_mixer_animate_volume(&mixer, 0, 0.5f, 5);

	if (!system_audio(system_update_audio, system_refresh_audio_device, &mixer)) {
		system_display_critical_message("Failed to load audio!");
	}

	ImGui_Initialize();
	karma_debug_service_initialize();

	Monospaced_Font font;
	load_debug_font(&font);

	bool editor_on = false;

	Camera camera;
	camera.position = vec3(0);
	camera.scale    = vec3(1);
	camera.rotation = quat_identity();

	Entity_Controller controller;
	controller.horizontal = 0;
	controller.jump       = 0;

	Entity player;
	player.position = vec3(0, 0, 1);
	player.scale    = vec2(1);
	player.mass     = 1;
	player.force    = vec2(0);
	player.velocity = vec2(0);

	InitializeRigidBodies();

	int  w, h, n;
	auto pixels = stbi_load("../res/misc/circle.png", &w, &h, &n, 4);
	auto circle = gfx_create_texture2d(w, h, 4, Data_Format_RGBA8_UNORM_SRGB, (const u8 **)&pixels, Buffer_Usage_IMMUTABLE, 1);

	Particle_Emitter emitter = particle_emitter_create(&circle, mm_rect(0, 0, 1, 1), 1000, 250);

	auto music = play_audio(&mixer, &audio, true);

	//
	//
	//

	bool  running = true;

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
		karma_timed_frame_begin();

		karma_timed_block_begin(EventHandling);
		auto events = system_poll_events();
		for (s64 event_index = 0; event_index < events.count; ++event_index) {
			Event &event = events[event_index];

			if (event.type & Event_Type_EXIT) {
				running = false;
				break;
			}
			
			karma_debug_service_handle_event(event);
			if (ImGui_HandleEvent(event)) continue;

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
						} else {
							state   = Time_State_RESUME;
							game_dt = fixed_dt;
							system_audio_resume();
						}
						break;
					case Key_F2:
						decrease_game_speed(&factor);
						break;
					case Key_F3:
						increase_game_speed(&factor);
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

			if ((event.type & Event_Type_KEY_UP) && event.key.symbol == Key_TAB) {
				editor_on = !editor_on;
				continue;
			}
			
			if (event.type & Event_Type_KEYBOARD) {
				float value = (float)(event.key.state == State_DOWN);
				switch (event.key.symbol) {
					case Key_A:
					case Key_LEFT:
						controller.horizontal = -value;
						break;
					case Key_D:
					case Key_RIGHT:
						controller.horizontal = value;
						break;
				}

				if ((event.type & Event_Type_KEY_DOWN) && event.key.symbol == Key_SPACE && !event.key.repeat && player.position.y <= 0) {
					controller.jump = true;
					play_audio(&mixer, &bounce_sound, false);
				}
			}
		}

		karma_timed_block_end(EventHandling);

		karma_timed_block_begin(Simulation);

		while (accumulator_t >= fixed_dt) {
			karma_timed_scope(SimulationFrame);

			if (state == Time_State_RESUME) {
				const r32 gravity = 10;
				const r32 drag = 5;

				player.force = vec2(0);

				player.force.x += 600 * controller.horizontal * dt;

				Vec2 acceleration = (player.force / player.mass);
				acceleration.y -= gravity; // TODO: Make jump framerate independent!!!
				player.velocity += dt * acceleration;
				player.velocity.y += 10 * controller.jump;
				player.velocity *= powf(0.5f, drag * dt);
				player.position.xy += dt * player.velocity;

				if (player.position.y <= 0) {
					player.position.y = 0;
					player.velocity.y = 0;
				}

				controller.jump = false;

#if 1

				for (int i = 0; i < NUM_RIGID_BODIES; i++) {
					RigidBody *rigidBody = &rigidBodies[i];

					if (rigidBody->collided == false) {
						rigidBody->linear_velocity.x += rigidBody->linear_acceleration.x * dt;
						rigidBody->linear_velocity.y += rigidBody->linear_acceleration.y * dt;
					}

					for (int j = i + 1; j < NUM_RIGID_BODIES; j++) {
						RigidBody *rigidBody2 = &rigidBodies[j];
						if (aabb_collision(rigidBody->min_max_points, rigidBody2->min_max_points)) {
							Vec2 prev_velocity_rigidBody  = rigidBody->linear_velocity;
							Vec2 prev_velocity_rigidBody2 = rigidBody2->linear_velocity;

							rigidBody2->linear_velocity.x = ((2 * rigidBody->shape.mass * prev_velocity_rigidBody.x) / (rigidBody->shape.mass + rigidBody2->shape.mass) + ((rigidBody2->shape.mass - rigidBody->shape.mass) / (rigidBody2->shape.mass + rigidBody->shape.mass)) * prev_velocity_rigidBody2.x);
							rigidBody2->linear_velocity.y = ((2 * rigidBody->shape.mass * prev_velocity_rigidBody.y) / (rigidBody->shape.mass + rigidBody2->shape.mass) + ((rigidBody2->shape.mass - rigidBody->shape.mass) / (rigidBody2->shape.mass + rigidBody->shape.mass)) * prev_velocity_rigidBody2.y);

							rigidBody->linear_velocity.x = (((rigidBody->shape.mass - rigidBody2->shape.mass) / (rigidBody->shape.mass + rigidBody2->shape.mass)) * prev_velocity_rigidBody.x + (2 * rigidBody2->shape.mass * prev_velocity_rigidBody2.x) / (rigidBody->shape.mass + rigidBody2->shape.mass));
							rigidBody->linear_velocity.y = (((rigidBody->shape.mass - rigidBody2->shape.mass) / (rigidBody->shape.mass + rigidBody2->shape.mass)) * prev_velocity_rigidBody.y + (2 * rigidBody2->shape.mass * prev_velocity_rigidBody2.y) / (rigidBody->shape.mass + rigidBody2->shape.mass));

							if (rigidBody->linear_velocity.x > 80.0f)
								rigidBody->linear_velocity.x = 80.0f;
							if (rigidBody->linear_velocity.x < -80.0f)
								rigidBody->linear_velocity.x = -80.0f;
							if (rigidBody->linear_velocity.y > 80.0f)
								rigidBody->linear_velocity.y = 80.0f;
							if (rigidBody->linear_velocity.y < -80.0f)
								rigidBody->linear_velocity.y = -80.0f;

							if (rigidBody2->linear_velocity.x > 80.0f)
								rigidBody2->linear_velocity.x = 80.0f;
							if (rigidBody2->linear_velocity.x < -80.0f)
								rigidBody2->linear_velocity.x = -80.0f;
							if (rigidBody2->linear_velocity.y > 80.0f)
								rigidBody2->linear_velocity.y = 80.0f;
							if (rigidBody2->linear_velocity.y < -80.0f)
								rigidBody2->linear_velocity.y = -80.0f;

							rigidBody->linear_acceleration.x = rigidBody->linear_velocity.x / dt;
							rigidBody->linear_acceleration.y = rigidBody->linear_velocity.y / dt;

							rigidBody->force.x = rigidBody->linear_acceleration.x * rigidBody->shape.mass;
							rigidBody->force.y = rigidBody->linear_acceleration.y * rigidBody->shape.mass;

							rigidBody2->linear_acceleration.x = rigidBody2->linear_velocity.x / dt;
							rigidBody2->linear_acceleration.y = rigidBody2->linear_velocity.y / dt;

							rigidBody2->force.x = rigidBody2->linear_acceleration.x * rigidBody2->shape.mass;
							rigidBody2->force.y = rigidBody2->linear_acceleration.y * rigidBody2->shape.mass;

							rigidBody2->position.x += rigidBody2->linear_velocity.x * dt;
							rigidBody2->position.y += rigidBody2->linear_velocity.y * dt;
							update_min_max(rigidBody2);

							rigidBody2->collided = true;
						}
					}

					//if (rigidBody->collided == false) {
					rigidBody->position.x += rigidBody->linear_velocity.x * dt;
					rigidBody->position.y += rigidBody->linear_velocity.y * dt;
					//}
					float angularAcceleration = rigidBody->torque / rigidBody->shape.momentofInertia;
					rigidBody->angular_velocity += angularAcceleration * dt;
					rigidBody->angle += rigidBody->angular_velocity * dt;
					if (rigidBody->linear_velocity.x > 80.0f)
						rigidBody->linear_velocity.x = 80.0f;
					if (rigidBody->linear_velocity.x < -80.0f)
						rigidBody->linear_velocity.x = -80.0f;
					if (rigidBody->linear_velocity.y > 80.0f)
						rigidBody->linear_velocity.y = 80.0f;
					if (rigidBody->linear_velocity.y < -80.0f)
						rigidBody->linear_velocity.y = -80.0f;

					if (rigidBody->position.x <= 0) {
						rigidBody->force.x *= -1;
						rigidBody->linear_acceleration.x *= -1;
						rigidBody->linear_velocity.x *= -1;
					}
					if (rigidBody->position.x >= window_w) {
						rigidBody->force.x *= -1;
						rigidBody->linear_acceleration.x *= -1;
						rigidBody->linear_velocity.x *= -1;
					}
					if (rigidBody->position.y <= 0) {
						rigidBody->force.y *= -1;
						rigidBody->linear_acceleration.y *= -1;
						rigidBody->linear_velocity.y *= -1;
					}
					if (rigidBody->position.y >= window_h) {
						rigidBody->force.y *= -1;
						rigidBody->linear_acceleration.y *= -1;
						rigidBody->linear_velocity.y *= -1;
					}
					update_min_max(rigidBody);
				}

				for (int i = 0; i < NUM_RIGID_BODIES; i++) {
					RigidBody *rigidBody = &rigidBodies[i];
					rigidBody->collided  = false;
				}

#endif

#if 1
				particle_emitter_update_particles(&emitter, dt);
#endif
			}

			accumulator_t -= fixed_dt;
		}

		karma_timed_block_end(Simulation);

		karma_timed_block_begin(AudioUpdate);

		mixer.pitch_factor = factor.ratio;
		audio_mixer_update(&mixer);
		karma_timed_block_end(AudioUpdate);

		karma_timed_block_begin(Rendering);
		camera.position.x = player.position.x;

		r32 alpha = accumulator_t / fixed_dt; // TODO: Use this

		ImGui_UpdateFrame(real_dt);

		r32 world_height_half = 4.5f;
		r32 world_width_half  = aspect_ratio * world_height_half;

		auto transform = mat4_inverse(mat4_scalar(vec3(4.5f, 4.5f, 1)));
		camera.view    = perspective_view(to_radians(90), aspect_ratio, 0.1f, 2);

		auto view = orthographic_view(0, framebuffer_w, framebuffer_h, 0);
		//auto view              = orthographic_view(-world_width_half, world_width_half, world_height_half, -world_height_half, -2.0f, 2.0f);

#if 1
		gfx_begin_drawing(Framebuffer_Type_HDR, Clear_COLOR | Clear_DEPTH, vec4(0.02f, 0.02f, 0.02f));
		gfx_viewport(0, 0, framebuffer_w, framebuffer_h);

		im_begin(camera.view, transform);

		static r32 angle = 0.0f;

		if (state == Time_State_RESUME) {
			angle += game_dt;
		}

		im_rect_rotated(vec2(0), vec2(1), angle, vec4(0.6f, 0.2f, 0.3f));
		im_rect(player.position, player.scale, vec4(1));

		im_end();

		//im_begin(orthographic_view(-world_width_half, world_width_half, world_height_half, -world_height_half));
		{
			im_begin(view);

			if (emitter.texture)
				im_bind_texture(*emitter.texture);

			for (int i = emitter.emit_count; i >= 0; --i) {
				Particle *particle = emitter.particles + i;
				if (particle->life <= particle->life_span) {
					r32 t = particle->life / particle->life_span;

					r32 fade_t = 1;
					if (particle->life < emitter.properties.fade_in) {
						fade_t *= particle->life / emitter.properties.fade_in;
					} else if (particle->life_span - particle->life < emitter.properties.fade_out) {
						fade_t *= (particle->life_span - particle->life) / emitter.properties.fade_out;
					}

					auto particle_color = hsv_to_rgb(lerp(particle->color_a, particle->color_b, t));
					particle_color.xyz *= emitter.properties.intensity;
					particle_color.w *= (fade_t * emitter.properties.opacity);
					im_rect_rotated(particle->position, vec2(lerp(particle->scale_a, particle->scale_b, t)), particle->rotation, particle_color);
				}
			}
			im_unbind_texture();
			PrintRigidBodies();
			im_end();

			view = orthographic_view(-world_width_half, world_width_half, world_height_half, -world_height_half);

#	if 1
			{
				im_begin(view);
				const Vec4  line_color       = vec4(0.2f, 0.2f, 0.2f, 1.0f);
				const float x_line_thickness = world_width_half / framebuffer_w;
				const float y_line_thickness = world_height_half / framebuffer_h;
				for (float x = -world_width_half; x <= world_width_half; x += 1) {
					im_line2d(vec2(x, -world_height_half), vec2(x, world_height_half), line_color, x_line_thickness);
				}
				for (float y = -world_height_half; y <= world_height_half; y += 1) {
					im_line2d(vec2(-world_width_half, y), vec2(world_width_half, y), line_color, y_line_thickness);
				}
				im_end();
			}
#	endif
		}

		gfx_end_drawing();
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
		if (editor_on) {
			ImGui::Begin("Primary");
			ImGui::Text("Camera");
			ImGui_ItemDisplay(&camera);
			ImGui::End();

			ImGui::Begin("Player");
			ImGui_ItemDisplay(&player);
			ImGui::End();
		}

		r32 master_volume = mixer.volume_b;

		// ImGui Rendering here
		ImGui::Begin("Edit");
		static int index = 0;
		RigidBody &rb    = rigidBodies[index];
		ImGui::Text("Speed: %f", factor.ratio);
		ImGui::Text("Speed N: %d", factor.numerator);
		ImGui::Text("Speed D: %d", factor.demonimator);
		ImGui::DragInt("Index", &index);
		//ImGui::DragFloat("Master Volume", &master_volume, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Master Volume", &master_volume, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat3("Position", rb.position.m);
		ImGui::DragFloat2("Velocity", rb.velocity.m);
		ImGui::DragFloat2("Linear Velocity", rb.linear_velocity.m);
		ImGui::DragFloat2("Linear Acceleration", rb.linear_acceleration.m);
		ImGui::DragFloat("Angular Velocity", &rb.angular_velocity);
		ImGui::DragFloat("Angle", &rb.angle);
		ImGui::DragFloat2("Force", rb.force.m);
		ImGui::DragFloat("Torque", &rb.torque);

		ImGui::End();

		audio_mixer_animate_volume_to(&mixer, master_volume, 5);

		//ImGui::ShowDemoWindow();
#endif

		ImGui_RenderFrame();

		{
			karma_timed_scope(DebugCollation);
			karma_timed_frame_presentation(font, real_dt, window_w, window_h);
		}

#if 0
		im_debug_begin(0, window_w, window_h, 0);

		float audio_display_height = 80.0f;
		float audio_display_x = 100;
		float audio_display_y = 50;

		im_rect(vec2(audio_display_x, audio_display_y), vec2(500, audio_display_height), vec4(0, 0, 0, 0.6f));
		for (int i = 0; i < 500; ++i) {
			r32 height_factor = record_right_channel_audio[i + playing_cursor];
			if (height_factor < 0) {
				height_factor *= -1;
				im_rect(vec2(audio_display_x + i, audio_display_y + 0.5f * audio_display_height * (1.0f - height_factor)), vec2(1, height_factor * audio_display_height * 0.5f), vec4(0.1f, 0.1f, 0.6f));
			} else {
				im_rect(vec2(audio_display_x + i, audio_display_y + audio_display_height * 0.5f), vec2(1, height_factor * audio_display_height * 0.5f), vec4(0.1f, 0.1f, 0.6f));
			}
		}

		audio_display_y += audio_display_height + 20;

		im_rect(vec2(audio_display_x, audio_display_y), vec2(500, audio_display_height), vec4(0, 0, 0, 0.6f));

		for (int i = 0; i < 500; ++i) {
			r32 height_factor = record_left_channel_audio[i + playing_cursor];
			if (height_factor < 0) {
				height_factor *= -1;
				im_rect(vec2(audio_display_x + i, audio_display_y + 0.5f * audio_display_height * (1.0f - height_factor)), vec2(1, height_factor * audio_display_height * 0.5f), vec4(0.1f, 0.1f, 0.6f));
			} else {
				im_rect(vec2(audio_display_x + i, audio_display_y + audio_display_height * 0.5f), vec2(1, height_factor * audio_display_height * 0.5f), vec4(0.1f, 0.1f, 0.6f));
			}
		}

		playing_cursor += (u32)(real_dt * wave_fmt->nSamplesPerSec);

		im_bind_texture(font.texture);

		audio_display_y -= 20;
		im_text(vec2(audio_display_x, audio_display_y), 14.0f, font.info, "Right Channel", vec4(1));
		audio_display_y += audio_display_height + 20;
		im_text(vec2(audio_display_x, audio_display_y), 14.0f, font.info, "Left Channel", vec4(1));

		im_end();
#endif

		gfx_end_drawing();

		karma_timed_block_begin(Present);

		gfx_present();

		karma_timed_block_end(Present);

		karma_timed_block_end(Rendering);

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

		karma_timed_frame_end();
	}

	karma_debug_service_shutdown();

	ImGui_Shutdown();
	gfx_destroy_context();

	return 0;
}
