#include "karma.h"        // shared
#include "systems.h"      // windows
#include "gfx_renderer.h" // rendering
#include "length_string.h"
#include "lin_maths.h"
#include "imgui/imgui.h"
#include "debug_service.h"
#include "particle_system.h"
#include "stream.h"

#include <Windows.h>
#include <Mmdeviceapi.h>
#include <Functiondiscoverykeys_devpkey.h>
#include <Audioclient.h>
#include <avrt.h>

static void win32_check_for_error_info(const Compile_Info &compile_info) {
	DWORD error = GetLastError();
	if (error) {
		LPSTR message = 0;
		FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPSTR)&message, 0, NULL);
		system_log(LOG_ERROR, "Platform", "File: %s, Line: %d, Function: %s", compile_info.file, compile_info.line, compile_info.procedure);
		system_log(LOG_ERROR, "Platform", "Message: %s", message);
		LocalFree(message);
	}
}

#if defined(BUILD_DEBUG)
#	define win32_check_for_error() win32_check_for_error_info(COMPILE_INFO)
#else
#	define win32_check_for_error()
#endif

const CLSID CLSID_MMDeviceEnumerator	= __uuidof(MMDeviceEnumerator);
const IID IID_IMMDeviceEnumerator		= __uuidof(IMMDeviceEnumerator);
const IID IID_IAudioClient				= __uuidof(IAudioClient);
const IID IID_IAudioRenderClient		= __uuidof(IAudioRenderClient);
const IID IID_IAudioClock				= __uuidof(IAudioClock);

constexpr u32 REFTIMES_PER_SEC			= 10000000;
constexpr u32 REFTIMES_PER_MILLISEC		= 10000;

constexpr u32 SYSTEM_AUDIO_BUFFER_SIZE_IN_MILLISECS			= 100;
constexpr u32 SYSTEM_AUDIO_BUFFER_UPDATE_MAX_WAIT_MILLISECS = 1000; // wait for max one sec

struct Audio_Master_Buffer {
	u64 playing_sample_index;
	u64 next_sample_index;
	r32 *buffer;
	u32 buffer_size_in_sample_count;
};

class Audio_Client : public IMMNotificationClient {
public:
	LONG				reference_count					= 1;
	IMMDeviceEnumerator *enumerator						= nullptr;
	IMMDevice			*device							= nullptr;
	IAudioClient		*client							= nullptr;
	IAudioClock			*clock							= nullptr;
	IAudioRenderClient	*renderer						= nullptr;
	WAVEFORMATEX		*format							= nullptr;
	HANDLE				thread							= nullptr;
	HANDLE				write_mutex						= nullptr;
	HANDLE				write_event						= nullptr;
	LONG volatile		playing							= false;
	r32 *				buffer							= nullptr;
	r32 *				buffer_ptr						= nullptr;
	r32 *				buffer_one_past_end				= nullptr;
	u32					buffer_size_in_sample_count		= 0;
	u32					total_samples					= 0;
	u32					channel_count					= 0;
	u32					samples_per_sec					= 0;
	u64					clock_frequency					= 0;
	u64					previous_master_sample_index	= 0;
	u64					master_sample_index				= 0;

	void Cleanup() {
		if (enumerator)		enumerator->Release();
		if (device)			device->Release();
		if (client)			client->Release();
		if (renderer)		renderer->Release();
		if (format)			CoTaskMemFree(format);

		memory_free(buffer);

		enumerator					= nullptr;
		device						= nullptr;
		client						= nullptr;
		renderer					= nullptr;
		format						= nullptr;
		buffer						= nullptr;
		buffer_ptr					= nullptr;
		buffer_one_past_end			= nullptr;
		buffer_size_in_sample_count = 0;
		total_samples				= 0;
	}

	void Destroy() {
		Cleanup();

		if (thread)			CloseHandle(thread);
		if (write_event)	CloseHandle(write_event);
		if (write_mutex)	CloseHandle(write_mutex);
	}

	bool FindDevice() {
		// TODO: Audio output device selection
		auto hr = enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &device);
		if (FAILED(hr)) {
			// NOTE: I don't know why there are two return codes when device is not available
			// In the docs both these errors are returned when device is not found
			if (hr == E_NOTFOUND || hr == ERROR_NOT_FOUND) {
				system_display_critical_message("Audio Device is not present!");
			} else {
				// TODO: Logging
				win32_check_for_error();
				system_display_critical_message("Audio Thread could not be initialized");
			}

			Destroy();
			return false;
		}

		return true;
	}

	bool RetriveInformation() {
		auto hr = client->GetBufferSize(&total_samples);
		if (FAILED(hr)) {
			// TODO: Logging
			win32_check_for_error();
			return false;
		}

		hr = clock->GetFrequency(&clock_frequency);
		if (FAILED(hr)) {
			// TODO: Logging
			win32_check_for_error();
			return false;
		}

		channel_count	= format->nChannels;
		samples_per_sec = format->nSamplesPerSec;

		return true;
	}

	inline u32 GetChannelCount() {
		return channel_count;
	}

	inline u32 GetSamplesPerSec() {
		return samples_per_sec;
	}

	bool Initialize() {
		auto hr = CoCreateInstance(CLSID_MMDeviceEnumerator, nullptr, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&enumerator);
		if (FAILED(hr)) {
			// TODO: Better logging
			win32_check_for_error();
			return false;
		}

		enumerator->RegisterEndpointNotificationCallback(this);

		if (FindDevice()) {
			hr = device->Activate(IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&client);
			if (FAILED(hr)) {
				if (hr == AUDCLNT_E_DEVICE_INVALIDATED) {
					system_display_critical_message("Audio Device disconnected!");
				}
				else {
					// TODO: Logging
					win32_check_for_error();
					system_display_critical_message("Audio Thread could not be initialized");
				}

				Destroy();
				return false;
			}

			hr = client->GetMixFormat(&format);
			if (FAILED(hr)) {
				if (hr == AUDCLNT_E_DEVICE_INVALIDATED) {
					system_display_critical_message("Audio Device disconnected!");
				}
				else if (hr == AUDCLNT_E_SERVICE_NOT_RUNNING) {
					system_display_critical_message("The Windows audio service is not running.");
				}
				else {
					// TODO: Logging
					win32_check_for_error();
					system_display_critical_message("Audio Thread could not be initialized");
				}
				return false;
			}

			REFERENCE_TIME requested_duration = 0;
			hr = client->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_EVENTCALLBACK, requested_duration, 0, format, nullptr);
			if (hr == AUDCLNT_E_BUFFER_SIZE_NOT_ALIGNED) {
				hr = client->GetBufferSize(&total_samples);
				if (FAILED(hr)) {
					// TODO: Logging
					win32_check_for_error();
				}

				requested_duration = (REFERENCE_TIME)((10000.0 * 1000 / format->nSamplesPerSec * total_samples) + 0.5);
				client->Release();
				CoTaskMemFree(format);

				hr = device->Activate(IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&client);
				if (FAILED(hr)) {
					// TODO: Logging
					win32_check_for_error();
					return false;
				}

				hr = client->GetMixFormat(&format);
				if (FAILED(hr)) {
					// TODO: Logging
					win32_check_for_error();
					return false;
				}

				hr = client->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_EVENTCALLBACK, requested_duration, requested_duration, format, nullptr);
				if (FAILED(hr)) {
					// TODO: Logging
					win32_check_for_error();
					return false;
				}
			}

			if (hr != S_OK) {
				// TODO: Logging
				win32_check_for_error();
				return false;
			}

			hr = client->GetService(IID_IAudioRenderClient, (void **)&renderer);
			if (FAILED(hr)) {
				// TODO: Logging
				win32_check_for_error();
				return false;
			}

			hr = client->GetService(IID_IAudioClock, (void **)&clock);
			if (FAILED(hr)) {
				// TODO: Logging
				win32_check_for_error();
				return false;
			}

			client->SetEventHandle(write_event);

			if (!RetriveInformation()) {
				return false;
			}

			ptrsize device_buffer_size	= sizeof(r32) * total_samples * format->nChannels;
			ptrsize buffer_size			= (sizeof(r32) * format->nSamplesPerSec * SYSTEM_AUDIO_BUFFER_SIZE_IN_MILLISECS * format->nChannels) / 1000;

			// TODO: Error or Log??
			// If Audio device buffer can't be made smaller, and can't be overidden, then there'll be large audio lag
			// What do we do in this case?
			assert(device_buffer_size < buffer_size);

			buffer							= (r32 *)memory_allocate(buffer_size);
			buffer_ptr						= buffer;
			buffer_one_past_end				= (r32 *)((u8 *)buffer + buffer_size);
			buffer_size_in_sample_count		= (format->nSamplesPerSec * SYSTEM_AUDIO_BUFFER_SIZE_IN_MILLISECS) / 1000;
			previous_master_sample_index	= 0;
			master_sample_index				= 0;
			memset(buffer, 0, buffer_size);

			return true;
		}

		return false;
	}

	static DWORD WINAPI AudioThreadProcedure(LPVOID param) {
		Audio_Client *audio = (Audio_Client *)param;

		DWORD task_index = 0;
		auto task = AvSetMmThreadCharacteristicsW(L"Audio", &task_index);
		if (task) {
			AvSetMmThreadPriority(task, AVRT_PRIORITY_HIGH);
		}

		InterlockedExchange(&audio->playing, true);

		u32 samples_padding		= 0;
		u32 samples_available	= 0;
		BYTE *data				= 0;
		u32 flags				= 0;

		auto hr = audio->client->Start();
		if (FAILED(hr)) {
			// TODO: Handle error
			win32_check_for_error();
			return 0;
		}

		while (audio->playing) {
			DWORD wait_res = WaitForSingleObject(audio->write_event, 2000);
			if (wait_res != WAIT_OBJECT_0) {
				// Event handle timed out after a 2-second wait.
				// TODO: Handle timed out
			}

			hr = audio->client->GetCurrentPadding(&samples_padding);
			if (SUCCEEDED(hr)) {
				samples_available = audio->total_samples - samples_padding;

				DWORD wait_result = WaitForSingleObject(audio->write_mutex, SYSTEM_AUDIO_BUFFER_UPDATE_MAX_WAIT_MILLISECS);
				if (wait_result == WAIT_ABANDONED) {
					// TODO: Handle error correctly
					return 0;
				} else if (wait_result == WAIT_TIMEOUT) {
					// TODO: Correctly handle timedout
					system_log(LOG_ERROR, "Audio", "Buffer timed-out");
					return 0; // Do we just kill audio??
				} else if (wait_result != WAIT_OBJECT_0) {
					// TODO: Handle error properly
					win32_check_for_error();
					return 0;
				}

				defer {
					ReleaseMutex(audio->write_mutex);
				};

				hr = audio->renderer->GetBuffer(samples_available, &data);
				if (SUCCEEDED(hr)) {
					// NOTE: This code assumes that Audio_Client::buffer is larger than the maximum buffer size of audio device
					u32 samples_to_write_in_channels = audio->channel_count * samples_available;
					if (audio->buffer_ptr + samples_to_write_in_channels < audio->buffer_one_past_end) {
						memcpy(data, audio->buffer_ptr, samples_to_write_in_channels * sizeof(r32));
						audio->buffer_ptr += samples_to_write_in_channels;

						if (audio->buffer_ptr == audio->buffer_one_past_end)
							audio->buffer_ptr = audio->buffer;
						flags = 0;
					} else if (audio->buffer_ptr < audio->buffer_one_past_end) {
						// NOTE: If audio data if not enough, we start putting silence
						u32 partial_data_size = (u32)((u8 *)audio->buffer_one_past_end - (u8 *)audio->buffer_ptr);
						memcpy(data, audio->buffer_ptr, partial_data_size);
						u32 remaining_size = samples_to_write_in_channels * sizeof(r32) - partial_data_size;
						memset(data + partial_data_size, 0, remaining_size);

						audio->buffer_ptr = audio->buffer_one_past_end;
						flags = 0;
					} else { // audio->buffer_ptr == audio->buffer_one_past_end
						assert(audio->buffer_ptr == audio->buffer_one_past_end);
						flags = AUDCLNT_BUFFERFLAGS_SILENT;
					}

					audio->renderer->ReleaseBuffer(samples_available, flags);
					audio->master_sample_index += samples_available;
				} else {
					win32_check_for_error();
				}
			} else {
				win32_check_for_error();
			}

		}

		return 0;
	}

	bool StartThread() {
		write_event = CreateEventW(nullptr, FALSE, FALSE, 0);
		if (write_event == NULL) {
			// TODO: Better logging
			win32_check_for_error();
			Destroy();
			return false;
		}

		write_mutex = CreateMutexW(nullptr, FALSE, 0);
		if (write_mutex == NULL) {
			// TODO: Better logging
			win32_check_for_error();
			Destroy();
			return false;
		}

		if (Initialize()) {
			thread = CreateThread(nullptr, 0, AudioThreadProcedure, this, 0, nullptr);
			if (thread == NULL) {
				win32_check_for_error();
				Destroy();
				return false;
			}
			return true;
		}

		return false;
	}

	void StopThread() {
		InterlockedExchange(&playing, false);
	}

	u64 GetNextSampleIndex() {
		return master_sample_index;
	}

	u32 GetSampleCount() {
		return buffer_size_in_sample_count;
	}

	bool LockBuffer(Audio_Master_Buffer *out) {
		clock->GetPosition(&out->playing_sample_index, nullptr);
		out->playing_sample_index /= clock_frequency;

		out->buffer_size_in_sample_count	= buffer_size_in_sample_count;
		out->buffer							= buffer;

		auto wait_result = WaitForSingleObject(write_mutex, SYSTEM_AUDIO_BUFFER_UPDATE_MAX_WAIT_MILLISECS);
		if (wait_result == WAIT_ABANDONED) {
			return false;
		} else if (wait_result == WAIT_TIMEOUT) {
			// TODO: Correctly handle timedout
			system_log(LOG_ERROR, "Audio", "Buffer timed-out");
			return false;
		} else if (wait_result != WAIT_OBJECT_0) {
			// TODO: Handle error properly
			win32_check_for_error();
			return false;
		}
		out->next_sample_index				= master_sample_index;

		return true;
	}
	
	void UnlockBuffer(u32 samples_written) {
		buffer_ptr			= buffer;
		buffer_one_past_end = buffer + samples_written * channel_count;
		ReleaseMutex(write_mutex);
	}

	ULONG STDMETHODCALLTYPE AddRef() {
		return InterlockedIncrement(&reference_count);
	}

	ULONG STDMETHODCALLTYPE Release() {
		ULONG ulRef = InterlockedDecrement(&reference_count);
		if (0 == ulRef) {} // we allocate this in stack
		return ulRef;
	}

	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, VOID **ppvInterface) {
		if (IID_IUnknown == riid) {
			AddRef();
			*ppvInterface = (IUnknown*)this;
		}
		else if (__uuidof(IMMNotificationClient) == riid) {
			AddRef();
			*ppvInterface = (IMMNotificationClient*)this;
		}
		else {
			*ppvInterface = NULL;
			return E_NOINTERFACE;
		}
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE OnDefaultDeviceChanged(EDataFlow flow, ERole role, LPCWSTR pwstrDeviceId) {
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE OnDeviceAdded(LPCWSTR pwstrDeviceId) {
		return S_OK;
	};

	HRESULT STDMETHODCALLTYPE OnDeviceRemoved(LPCWSTR pwstrDeviceId) {
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE OnDeviceStateChanged(LPCWSTR pwstrDeviceId, DWORD dwNewState) {
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE OnPropertyValueChanged(LPCWSTR pwstrDeviceId, const PROPERTYKEY key) {
		return S_OK;
	}
};

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

constexpr s32 MAX_SPEED_FACTOR = 6;

struct Time_Speed_Factor {
	s32 numerator   = 1;
	s32 demonimator = 1;
	r32 ratio       = 1;
};

void increase_game_speed(Time_Speed_Factor *factor) {
	if (factor->demonimator == 1) {
		factor->numerator = min_value(factor->numerator + 1, MAX_SPEED_FACTOR);
	} else {
		factor->demonimator = max_value(factor->demonimator - 1, 1);
	}
	factor->ratio = (r32)factor->numerator / (r32)factor->demonimator;
}

void decrease_game_speed(Time_Speed_Factor *factor) {
	if (factor->numerator == 1) {
		factor->demonimator = min_value(factor->demonimator + 1, MAX_SPEED_FACTOR);
	} else {
		factor->numerator = max_value(factor->numerator - 1, 1);
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
static r32 output_volume	= 0.5f;

bool mix_second_sound				= false;
u64 second_sound_sample_counter		= 0;


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

struct Audio {
	Riff_Header *header; // TODO: do we need this?
	Wave_Fmt	*fmt;	 //	TODO: do we need this?
	Wave_Data	*data;	 // TODO: do we need this?
	s16			*samples;
	u32			sample_count;
};

Audio make_sine_audio() {
	float t = 0;
	int sample_index = 0;
	for (int i = 0; i < SAMPLES_COUNT; ++i) {
		s16 sample_value = (s16)roundf(sinf(2 * MATH_PI * FREQUENCY * t) * MAX_INT16);
		sine_wave[sample_index + 0] = sample_value;
		sine_wave[sample_index + 1] = sample_value;
		t += 1.0f / (r32)SAMPLES_PER_SEC;
		sample_index += 2;
	}

	Audio audio = {};
	audio.samples = sine_wave;
	audio.sample_count = SAMPLES_COUNT;
	return audio;
}

r32 get_next_sample(Audio &audio, u64 sample_index) {
	if (sample_index > audio.sample_count) {
		sample_index = sample_index % audio.sample_count;
	}

	constexpr r32 imax = 1.0f / (r32)MAX_INT16;
	r32 sample_value = audio.samples[sample_index] * output_volume * imax;

	return sample_value;
}

void fill_silence(u32 samples_count_to_write, r32 *write_ptr) {
	memset(write_ptr, 0, sizeof(r32) * 2 * samples_count_to_write);
}

void fill_audio(Audio &audio, u64 write_sample_index, u32 samples_count_to_write, r32 *write_ptr) {
	for (u32 sample_index = 0; sample_index < samples_count_to_write; ++sample_index) {
		r32 sample_value = get_next_sample(audio, write_sample_index + sample_index);
		write_ptr[0] = sample_value;
		write_ptr[1] = sample_value;
		write_ptr += 2;
	}
}

void mix_audio(Audio &audio, u32 sample_index, u32 samples_count_to_write, r32 *write_ptr) {
	for (u32 sample_counter = 0; sample_counter < samples_count_to_write; ++sample_counter) {
		r32 sample_value = get_next_sample(audio, sample_counter + sample_index);
		write_ptr[0] += sample_value;
		write_ptr[1] += sample_value;
		write_ptr += 2;
	}
}

Audio load_wave(String content) {
	Istream stream			= istream(content);
	Riff_Header *wav_header = istream_consume(&stream, Riff_Header);
	Wave_Fmt	*wav_fmt	= istream_consume(&stream, Wave_Fmt);
	Wave_Data	*wav_data	= istream_consume(&stream, Wave_Data);
	s16			*data		= (s16 *)istream_consume_size(&stream, wav_data->size);

	Audio audio;
	audio.header		= wav_header;
	audio.fmt			= wav_fmt;
	audio.data			= wav_data;
	audio.samples		= data;
	audio.sample_count	= wav_data->size / sizeof(s16) / wav_fmt->channels_count;

	return audio;
}

int system_main() { // Entry point
	r32    framebuffer_w = 1280;
	r32    framebuffer_h = 720;
	Handle platform      = system_create_window(u8"Karma", 1280, 720, System_Window_Show_NORMAL);
	gfx_create_context(platform, Render_Backend_DIRECTX11, Vsync_ADAPTIVE, 2, (u32)framebuffer_w, (u32)framebuffer_h);

	auto sine_audio = make_sine_audio();

	auto audio = load_wave(system_read_entire_file("../res/misc/POL-course-of-nature-short.wav"));
	auto bounce_sound = load_wave(system_read_entire_file("../res/misc/Boing Cartoonish-SoundBible.com-277290791.wav"));

	Audio_Client audio_client;
	if (!audio_client.StartThread()) {
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
						} else {
							state   = Time_State_RESUME;
							game_dt = fixed_dt;
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
					mix_second_sound = true;
				}
			}
		}

		karma_timed_block_end(EventHandling);

		karma_timed_block_begin(Simulation);

		while (accumulator_t >= fixed_dt) {
			karma_timed_scope(SimulationFrame);

			const r32 gravity = 10;
			const r32 drag    = 5;

			player.force = vec2(0);

			player.force.x += 600 * controller.horizontal * dt;
			player.force.y += 500 * controller.jump; // TODO: Make jump framerate independent!!!

			Vec2 acceleration = (player.force / player.mass) - vec2(0, gravity);
			player.velocity += dt * acceleration;
			player.velocity *= powf(0.5f, drag * dt);
			player.position.xy += dt * player.velocity;

			if (player.position.y <= 0) {
				player.position.y = 0;
				player.velocity.y = 0;
			}

			controller.jump = false;

			if (state == Time_State_RESUME) {
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

		auto next_sample_index = audio_client.GetNextSampleIndex();
		r32 *audio_buffer = (r32 *)tallocate(sizeof(r32) * audio_client.channel_count * audio_client.format->nSamplesPerSec * 1);

		auto buffer_size_in_sample_count = audio_client.format->nSamplesPerSec * 1;
		fill_silence(buffer_size_in_sample_count, audio_buffer);
		fill_audio(audio, next_sample_index, buffer_size_in_sample_count, audio_buffer);

		if (mix_second_sound) {
			if (second_sound_sample_counter == 0)
				second_sound_sample_counter = next_sample_index;

			u32 write_sample_index = (u32)(next_sample_index - second_sound_sample_counter);
			if (write_sample_index < bounce_sound.sample_count) {
				auto samples_to_mix = min_value(bounce_sound.sample_count - write_sample_index, buffer_size_in_sample_count);
				mix_audio(bounce_sound, write_sample_index, samples_to_mix, audio_buffer);
			} else {
				mix_second_sound = false;
				second_sound_sample_counter = 0;
			}
		}
		
		Audio_Master_Buffer buffer;
		if (audio_client.LockBuffer(&buffer)) {
			u32 write_sample_index = (u32)(buffer.next_sample_index - next_sample_index);
			memcpy(buffer.buffer, audio_buffer + audio_client.channel_count * write_sample_index, audio_client.channel_count * buffer.buffer_size_in_sample_count * sizeof(r32));
			audio_client.UnlockBuffer(buffer.buffer_size_in_sample_count);
		}

#if 0
		Audio_Master_Buffer buffer;
		if (audio_client.LockBuffer(&buffer)) {
			u32 write_sample_index = (u32)(buffer.next_sample_index % SAMPLES_COUNT);
			fill_silence(buffer.buffer_size_in_sample_count, buffer.buffer);
			fill_audio(write_sample_index, buffer.buffer_size_in_sample_count, buffer.buffer);

			if (mix_second_sound) {
				if (second_sound_sample_counter == 0)
					second_sound_sample_counter = buffer.next_sample_index;

				write_sample_index = (u32)(buffer.next_sample_index - second_sound_sample_counter);
				if (write_sample_index < SAMPLES_PER_SEC) {
					auto samples_to_mix = min_value(SAMPLES_PER_SEC - write_sample_index, buffer.buffer_size_in_sample_count);
					mix_audio(write_sample_index, samples_to_mix, buffer.buffer);
				} else {
					mix_second_sound = false;
					second_sound_sample_counter = 0;
				}
			}

			audio_client.UnlockBuffer(buffer.buffer_size_in_sample_count);
		}
#endif

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

		angle += game_dt;

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

		// ImGui Rendering here
		ImGui::Begin("Edit");
		static int index = 0;
		RigidBody &rb    = rigidBodies[index];
		ImGui::Text("Speed: %f", factor.ratio);
		ImGui::Text("Speed N: %d", factor.numerator);
		ImGui::Text("Speed D: %d", factor.demonimator);
		ImGui::DragInt("Index", &index);
		ImGui::DragFloat3("Position", rb.position.m);
		ImGui::DragFloat2("Velocity", rb.velocity.m);
		ImGui::DragFloat2("Linear Velocity", rb.linear_velocity.m);
		ImGui::DragFloat2("Linear Acceleration", rb.linear_acceleration.m);
		ImGui::DragFloat("Angular Velocity", &rb.angular_velocity);
		ImGui::DragFloat("Angle", &rb.angle);
		ImGui::DragFloat2("Force", rb.force.m);
		ImGui::DragFloat("Torque", &rb.torque);

		ImGui::End();
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

	audio_client.StopThread();

	ImGui_Shutdown();
	gfx_destroy_context();

	return 0;
}
