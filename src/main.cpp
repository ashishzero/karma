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

constexpr r32 TOTAL_GRID_HORIZONTAL = 32;
constexpr r32 TOTAL_GRID_VERTICAL	= 16;
constexpr r32 HALF_GRID_HORIZONTAL	= TOTAL_GRID_HORIZONTAL / 2;
constexpr r32 HALF_GRID_VERTICAL	= TOTAL_GRID_VERTICAL / 2;

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#if 0
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

		Debug_AudioFeedback((r32 *)src_ptr, copy_size, channel_count, buffer_size - copy_size);

		mixer->buffer_consumed_bytes += copy_size;
		mixer->volume_position += sample_count * mixer->pitch_factor;

		for (Audio_Node *node = mixer->list.next ; node; node = node->next) {
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
			Audio &audio = node->audio;
			audio.buffered_pitch_factor	= audio.pitch_factor * mixer->pitch_factor;

			r32 *write_ptr = mixer->buffer;
			r32 read_cursor = audio.samples_played;
			r32 volume_position = mixer->volume_position;

			for (u32 sample_counter = 0; sample_counter < mixer->buffer_size_in_sample_count;) {
				u32 more_samples_required	= (mixer->buffer_size_in_sample_count - sample_counter);
				u32 samples_available		= (u32)lroundf(((r32)audio.stream->sample_count - read_cursor) / audio.buffered_pitch_factor);
				u32 samples_to_mix			= min_value(samples_available, more_samples_required);

				r32 effective_volume;

				for (u32 sample_mix_index = 0; sample_mix_index < samples_to_mix; ++sample_mix_index) {
					// TODO: Here we expect both input audio and output buffer to be stero audio

					effective_volume = audio.volume * lerp(mixer->volume_a, mixer->volume_b, clamp01((volume_position + sample_mix_index * mixer->pitch_factor) / mixer->volume_span));

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
};

static Editor editor;

Texture2d_Handle debug_load_texture(const char * filepath) {
	int w, h, n;
	unsigned char *pixels = stbi_load(filepath, &w, &h, &n, 4);
	auto texture = gfx_create_texture2d(u32(w), u32(h), u32(n), Data_Format_RGBA8_UNORM_SRGB, (const u8 **)&pixels, Buffer_Usage_IMMUTABLE, 1);
	stbi_image_free(pixels);
	return texture;
}

struct Player {
	Vec3 position = vec3(0, 0, 1);
	Vec3 direction = vec3(0, -1, 0);

	r32 movement_force = 500;

	r32 mass = 50;
	r32 drag = 3;
	r32 friction_coefficient = 0.5f;
	Vec3 velocity = vec3(0);
	Vec3 force = vec3(0);
};

struct Player_Controller {
	r32 x;
	r32 y;
};

struct Camera {
	Vec3 position;
	r32 distance;
	r32 distance_target;

	Camera_View view;
};

Camera camera_create(Vec3 position, r32 distance = 0.5f, r32 target_distance = 1.3f) {
	Camera camera;
	camera.position = position;
	camera.distance = distance;
	camera.distance_target = target_distance;
	camera.view = orthographic_view(-HALF_GRID_HORIZONTAL, HALF_GRID_HORIZONTAL, HALF_GRID_VERTICAL, -HALF_GRID_VERTICAL);
	return camera;
}

void editor_update(Audio_Mixer *mixer, Player *player) {
	if (!editor.display) return;

	ImGui::Begin("Editor");

	r32 volume = mixer->volume_b;
	ImGui::DragFloat("Volume", &volume, 0.01f, 0.0f, 1.0f);

	ImGui::Text("Player");
	ImGui::DragFloat3("Position: (%.3f, %.3f, %.3f)", player->position.m);
	ImGui::Text("Direction: (%.3f, %.3f, %.3f)", player->direction.x, player->direction.y, player->direction.z);
	ImGui::DragFloat("Mass: %.3f", &player->mass, 0.1f);
	ImGui::DragFloat("Friction: %.3f", &player->friction_coefficient, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("Drag: %.3f", &player->drag, 0.01f);
	ImGui::DragFloat("Movement Force: %.3f", &player->movement_force, 0.1f);
	ImGui::Text("Velocity: (%.3f, %.3f, %.3f)", player->velocity.x, player->velocity.y, player->velocity.z);
	ImGui::Text("Force: (%.3f, %.3f, %.3f)", player->force.x, player->force.y, player->force.z);

	ImGui::End();

	audio_mixer_set_volume(mixer, volume);
}


int system_main() {
	r32    framebuffer_w = 1280;
	r32    framebuffer_h = 720;
	Handle platform      = system_create_window(u8"Karma", 1280, 720, System_Window_Show_NORMAL);
	gfx_create_context(platform, Render_Backend_DIRECTX11, Vsync_ADAPTIVE, 2, (u32)framebuffer_w, (u32)framebuffer_h);

	auto audio = load_wave(system_read_entire_file("../res/misc/POL-course-of-nature-short.wav"));
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

	auto music = play_audio(&mixer, &audio, true);

	auto player_sprite = debug_load_texture("../res/misc/player.png");

	//
	//
	//

	Player player;
	Player_Controller controller = {};
	Camera camera = camera_create(player.position);

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
		Debug_TimedFrameBegin();

		Debug_TimedBlockBegin(EventHandling);
		auto events = system_poll_events();
		for (s64 event_index = 0; event_index < events.count; ++event_index) {
			Event &event = events[event_index];

			if (event.type & Event_Type_EXIT) {
				running = false;
				break;
			}
			
			if (Debug_HandleEvent(event)) continue;
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
				float value = (float)(event.key.state == State_DOWN);
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

		}

		Debug_TimedBlockEnd(EventHandling);

		const Controller &input = system_get_controller_state(0);
		controller.x = input.left_thumb.x;
		controller.y = input.left_thumb.y;

		Debug_TimedBlockBegin(Simulation);

		player.force = vec3(0);

		Vec2 movement_direction = vec2_normalize_check(vec2(controller.x, controller.y));
		r32 movement_magnitude = vec2_length(movement_direction);
		if (movement_magnitude) {
			r32 angle = vec3_signed_angle_between(player.direction, vec3(movement_direction, 0), vec3(0, 0, 1));
			player.direction = vec3_normalize(quat_rotate(quat_angle_axis(vec3(0, 0, 1), angle), player.direction));
		}

		const float gravity = 10.f;
		const float ground_friction_coefficient = 1;

		while (accumulator_t >= fixed_dt) {
			Debug_TimedScope(SimulationFrame);

			if (state == Time_State_RESUME) {

				float effective_friction_coeff = ground_friction_coefficient * player.friction_coefficient;
				player.force.xy += (player.movement_force * movement_magnitude) * player.direction.xy;

				r32 frictional_force_mag = effective_friction_coeff * gravity * player.mass;
				r32 force_mag = vec3_length(player.force);
				if (frictional_force_mag > force_mag) frictional_force_mag = force_mag;

				Vec3 friction_force_dir;
				if (force_mag) {
					friction_force_dir = player.force / force_mag;
				} else {
					friction_force_dir = {};
				}

				player.force -= friction_force_dir * frictional_force_mag;

				//
				//
				//

				Vec3 acceleration = (player.force / player.mass);
				player.velocity += dt * acceleration;
				player.velocity *= powf(0.5f, player.drag * dt);
				player.position += dt * player.velocity;

#if 0

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

		Vec2 camera_player_offset = vec2(-1.0f);
		camera.position.xy = lerp(camera.position.xy - camera_player_offset, player.position.xy, 0.5f);
		camera.distance = lerp(camera.distance, camera.distance_target, 1.0f - powf(1.0f - 0.9f, game_dt));

		r32 camera_zoom = 1.0f / powf(2.0f, camera.distance);

		r32 thickness = 8.0f / (TOTAL_GRID_HORIZONTAL * TOTAL_GRID_VERTICAL);

		Mat4 transform = mat4_inverse(mat4_translation(camera.position) * mat4_scalar(camera_zoom, camera_zoom, 1)) * mat4_translation(vec3(0.5f, 0.5f, 0));

		gfx_begin_drawing(Framebuffer_Type_HDR, Clear_COLOR | Clear_DEPTH, vec4(0.02f, 0.02f, 0.02f));
		gfx_viewport(0, 0, framebuffer_w, framebuffer_h);

		im_begin(camera.view, transform);
		im_unbind_texture();
		for (r32 x = -5; x < 5; ++x) {
			for (r32 y = -5; y < 5; ++y) {
				im_rect_centered_outline2d(vec3(x, y, 1), vec2(1), vec4(0, 0.3f, 0), 1.5f * thickness);
				im_rect_centered(vec3(x, y, 1), vec2(1), vec4(0.1f, 0.9f, 0.2f));
			}
		}

		im_bind_texture(player_sprite);
		im_rect_centered_rotated(player.position, vec2(1), vec3_signed_angle_between(vec3(0, 1, 0), player.direction, vec3(0, 0, 1)), vec4(1));

		im_unbind_texture();
		im_rect_centered_outline2d(player.position, vec2(1), vec4(1, 1, 0), thickness);

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
