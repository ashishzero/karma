#define OEMRESOURCE
#include "../length_string.h"
#include "../systems.h"
#include "../stb_sprintf.h"
#include "resource.h"

#include <winsock2.h>

#include <ShlObj_core.h>
#include <Windows.h>
#include <shlwapi.h>
#include <strsafe.h>
#include <windowsx.h>
#include <winreg.h>
#include <Lmcons.h> // for UNLEN

#include <Mmdeviceapi.h>
#include <Functiondiscoverykeys_devpkey.h>
#include <Audioclient.h>
#include <audiopolicy.h>

#include <avrt.h>

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "Advapi32.lib")
#pragma comment(lib, "Ole32.lib")
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "Avrt.lib")
#pragma comment( lib, "wsock32.lib")

//
// Win32 Error
//

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

//
// XInput Header File
//

#define XINPUT_DEVTYPE_GAMEPAD    0x01
#define XINPUT_DEVSUBTYPE_GAMEPAD 0x01

#define XINPUT_DEVSUBTYPE_UNKNOWN          0x00
#define XINPUT_DEVSUBTYPE_WHEEL            0x02
#define XINPUT_DEVSUBTYPE_ARCADE_STICK     0x03
#define XINPUT_DEVSUBTYPE_FLIGHT_STICK     0x04
#define XINPUT_DEVSUBTYPE_DANCE_PAD        0x05
#define XINPUT_DEVSUBTYPE_GUITAR           0x06
#define XINPUT_DEVSUBTYPE_GUITAR_ALTERNATE 0x07
#define XINPUT_DEVSUBTYPE_DRUM_KIT         0x08
#define XINPUT_DEVSUBTYPE_GUITAR_BASS      0x0B
#define XINPUT_DEVSUBTYPE_ARCADE_PAD       0x13

#define XINPUT_CAPS_VOICE_SUPPORTED 0x0004
#define XINPUT_CAPS_FFB_SUPPORTED   0x0001
#define XINPUT_CAPS_WIRELESS        0x0002
#define XINPUT_CAPS_PMD_SUPPORTED   0x0008
#define XINPUT_CAPS_NO_NAVIGATION   0x0010

#define XINPUT_GAMEPAD_DPAD_UP        0x0001
#define XINPUT_GAMEPAD_DPAD_DOWN      0x0002
#define XINPUT_GAMEPAD_DPAD_LEFT      0x0004
#define XINPUT_GAMEPAD_DPAD_RIGHT     0x0008
#define XINPUT_GAMEPAD_START          0x0010
#define XINPUT_GAMEPAD_BACK           0x0020
#define XINPUT_GAMEPAD_LEFT_THUMB     0x0040
#define XINPUT_GAMEPAD_RIGHT_THUMB    0x0080
#define XINPUT_GAMEPAD_LEFT_SHOULDER  0x0100
#define XINPUT_GAMEPAD_RIGHT_SHOULDER 0x0200
#define XINPUT_GAMEPAD_A              0x1000
#define XINPUT_GAMEPAD_B              0x2000
#define XINPUT_GAMEPAD_X              0x4000
#define XINPUT_GAMEPAD_Y              0x8000

#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  7849
#define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689
#define XINPUT_GAMEPAD_TRIGGER_THRESHOLD    30

#define XINPUT_GAMEPAD_TRIGGER_MIN 0
#define XINPUT_GAMEPAD_TRIGGER_MAX 255
#define XINPUT_GAMEPAD_THUMB_MIN   -32768
#define XINPUT_GAMEPAD_THUMB_MAX   32767

#define XINPUT_FLAG_GAMEPAD 0x00000001

#define BATTERY_DEVTYPE_GAMEPAD 0x00
#define BATTERY_DEVTYPE_HEADSET 0x01

#define BATTERY_TYPE_DISCONNECTED 0x00
#define BATTERY_TYPE_WIRED        0x01
#define BATTERY_TYPE_ALKALINE     0x02
#define BATTERY_TYPE_NIMH         0x03
#define BATTERY_TYPE_UNKNOWN      0xFF

#define BATTERY_LEVEL_EMPTY  0x00
#define BATTERY_LEVEL_LOW    0x01
#define BATTERY_LEVEL_MEDIUM 0x02
#define BATTERY_LEVEL_FULL   0x03

#define XUSER_MAX_COUNT 4
#define XUSER_INDEX_ANY 0x000000FF

#define VK_PAD_A         0x5800
#define VK_PAD_B         0x5801
#define VK_PAD_X         0x5802
#define VK_PAD_Y         0x5803
#define VK_PAD_RSHOULDER 0x5804
#define VK_PAD_LSHOULDER 0x5805
#define VK_PAD_LTRIGGER  0x5806
#define VK_PAD_RTRIGGER  0x5807

#define VK_PAD_DPAD_UP      0x5810
#define VK_PAD_DPAD_DOWN    0x5811
#define VK_PAD_DPAD_LEFT    0x5812
#define VK_PAD_DPAD_RIGHT   0x5813
#define VK_PAD_START        0x5814
#define VK_PAD_BACK         0x5815
#define VK_PAD_LTHUMB_PRESS 0x5816
#define VK_PAD_RTHUMB_PRESS 0x5817

#define VK_PAD_LTHUMB_UP        0x5820
#define VK_PAD_LTHUMB_DOWN      0x5821
#define VK_PAD_LTHUMB_RIGHT     0x5822
#define VK_PAD_LTHUMB_LEFT      0x5823
#define VK_PAD_LTHUMB_UPLEFT    0x5824
#define VK_PAD_LTHUMB_UPRIGHT   0x5825
#define VK_PAD_LTHUMB_DOWNRIGHT 0x5826
#define VK_PAD_LTHUMB_DOWNLEFT  0x5827

#define VK_PAD_RTHUMB_UP        0x5830
#define VK_PAD_RTHUMB_DOWN      0x5831
#define VK_PAD_RTHUMB_RIGHT     0x5832
#define VK_PAD_RTHUMB_LEFT      0x5833
#define VK_PAD_RTHUMB_UPLEFT    0x5834
#define VK_PAD_RTHUMB_UPRIGHT   0x5835
#define VK_PAD_RTHUMB_DOWNRIGHT 0x5836
#define VK_PAD_RTHUMB_DOWNLEFT  0x5837

#define XINPUT_KEYSTROKE_KEYDOWN 0x0001
#define XINPUT_KEYSTROKE_KEYUP   0x0002
#define XINPUT_KEYSTROKE_REPEAT  0x0004

#define XINPUT_GAMEPAD_VIBRATION_MAX 65535

typedef struct _XINPUT_GAMEPAD {
	WORD  wButtons;
	BYTE  bLeftTrigger;
	BYTE  bRightTrigger;
	SHORT sThumbLX;
	SHORT sThumbLY;
	SHORT sThumbRX;
	SHORT sThumbRY;
} XINPUT_GAMEPAD, *PXINPUT_GAMEPAD;

typedef struct _XINPUT_STATE {
	DWORD          dwPacketNumber;
	XINPUT_GAMEPAD Gamepad;
} XINPUT_STATE, *PXINPUT_STATE;

typedef struct _XINPUT_VIBRATION {
	WORD wLeftMotorSpeed;
	WORD wRightMotorSpeed;
} XINPUT_VIBRATION, *PXINPUT_VIBRATION;

typedef struct _XINPUT_CAPABILITIES {
	BYTE             Type;
	BYTE             SubType;
	WORD             Flags;
	XINPUT_GAMEPAD   Gamepad;
	XINPUT_VIBRATION Vibration;
} XINPUT_CAPABILITIES, *PXINPUT_CAPABILITIES;

typedef struct _XINPUT_BATTERY_INFORMATION {
	BYTE BatteryType;
	BYTE BatteryLevel;
} XINPUT_BATTERY_INFORMATION, *PXINPUT_BATTERY_INFORMATION;

typedef struct _XINPUT_KEYSTROKE {
	WORD  VirtualKey;
	WCHAR Unicode;
	WORD  Flags;
	BYTE  UserIndex;
	BYTE  HidCode;
} XINPUT_KEYSTROKE, *PXINPUT_KEYSTROKE;

typedef DWORD(*Proc_XInput_Get_State)(DWORD dwUserIndex, XINPUT_STATE *pState);
typedef DWORD(*Proc_XInput_Set_State)(DWORD dwUserIndex, XINPUT_VIBRATION *pVibration);
typedef DWORD(*Proc_XInput_Get_Capabilities)(DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES *pCapabilities);
typedef DWORD(*Proc_XInput_Get_Audio_Device_Ids)(DWORD dwUserIndex, LPWSTR pRenderDeviceId, UINT *pRenderCount, LPWSTR pCaptureDeviceId, UINT *pCaptureCount);
typedef DWORD(*Proc_XInput_Get_Battery_Information)(DWORD dwUserIndex, BYTE devType, XINPUT_BATTERY_INFORMATION *pBatteryInformation);
typedef DWORD(*Proc_XInput_Get_Keystroke)(DWORD dwUserIndex, DWORD dwReserved, PXINPUT_KEYSTROKE pKeystroke);

static Proc_XInput_Get_State               XInputGetState;
static Proc_XInput_Set_State               XInputSetState;
static Proc_XInput_Get_Capabilities        XInputGetCapabilities;
static Proc_XInput_Get_Audio_Device_Ids    XInputGetAudioDeviceIds;
static Proc_XInput_Get_Battery_Information XInputGetBatteryInformation;
static Proc_XInput_Get_Keystroke           XInputGetKeystroke;

static DWORD xinput_get_state_stub(DWORD, XINPUT_STATE *) {
	return ERROR_DEVICE_NOT_CONNECTED;
}
static DWORD xinput_set_state_stub(DWORD, XINPUT_VIBRATION *) {
	return ERROR_DEVICE_NOT_CONNECTED;
}
static DWORD xinput_get_capacilities_stub(DWORD, DWORD, XINPUT_CAPABILITIES *) {
	return ERROR_DEVICE_NOT_CONNECTED;
}
static DWORD xinput_get_audio_device_ids_stub(DWORD, LPWSTR, UINT *, LPWSTR, UINT *) {
	return ERROR_DEVICE_NOT_CONNECTED;
}
static DWORD xinput_get_battery_information_stub(DWORD, BYTE, XINPUT_BATTERY_INFORMATION *) {
	return ERROR_EMPTY;
}
static DWORD xinput_get_keystroke_stub(DWORD, DWORD, PXINPUT_KEYSTROKE) {
	return ERROR_EMPTY;
}

struct Win32_Controller {
	XINPUT_STATE	state;
	bool			connected;
};

//
// XInput Header File
//

//
// Windows Audio Client
//

const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
const IID IID_IAudioClient = __uuidof(IAudioClient);
const IID IID_IAudioRenderClient = __uuidof(IAudioRenderClient);
const IID IID_IAudioClock = __uuidof(IAudioClock);
const IID IID_IAudioSessionControl = __uuidof(IAudioSessionControl);

constexpr u32 REFTIMES_PER_SEC = 10000000;
constexpr u32 REFTIMES_PER_MILLISEC = 10000;

class Audio_Client : public IAudioSessionEvents {
public:
	LONG					reference_count = 1;
	IMMDeviceEnumerator *enumerator = nullptr;
	IMMDevice *device = nullptr;
	IAudioClient *client = nullptr;
	IAudioSessionControl *control = nullptr;
	IAudioClock *clock = nullptr;
	IAudioRenderClient *renderer = nullptr;
	WAVEFORMATEXTENSIBLE	 format = {};
	HANDLE					thread = nullptr;
	HANDLE					write_event = nullptr;
	u32						total_samples = 0;
	u64						clock_frequency = 0;
	u64						write_cursor = 0;

	Audio_Callback			on_update = nullptr;
	Audio_Device_Refresh	on_refresh = nullptr;
	void *on_update_user_data = nullptr;

	void Cleanup() {
		if (enumerator)		enumerator->Release();
		if (device)			device->Release();
		if (client)			client->Release();
		if (renderer)		renderer->Release();

		enumerator = nullptr;
		device = nullptr;
		client = nullptr;
		renderer = nullptr;
		total_samples = 0;
	}

	void Destroy() {
		Cleanup();

		if (thread)			CloseHandle(thread);
		if (write_event)	CloseHandle(write_event);
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

		return true;
	}

	static void StubAudioCallback(const System_Audio &sys_audio, void *user_data) {
		(void)user_data;
	}

	bool Initialize() {
		auto hr = CoCreateInstance(CLSID_MMDeviceEnumerator, nullptr, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void **)&enumerator);
		if (FAILED(hr)) {
			// TODO: Better logging
			win32_check_for_error();
			return false;
		}

		if (FindDevice()) {
			hr = device->Activate(IID_IAudioClient, CLSCTX_ALL, NULL, (void **)&client);
			if (FAILED(hr)) {
				if (hr == AUDCLNT_E_DEVICE_INVALIDATED) {
					system_display_critical_message("Audio Device disconnected!");
				} else {
					// TODO: Logging
					win32_check_for_error();
					system_display_critical_message("Audio Thread could not be initialized");
				}

				Destroy();
				return false;
			}

			{
				WAVEFORMATEX *mix_format = nullptr;
				hr = client->GetMixFormat(&mix_format);
				if (FAILED(hr)) {
					if (hr == AUDCLNT_E_DEVICE_INVALIDATED) {
						system_display_critical_message("Audio Device disconnected!");
					} else if (hr == AUDCLNT_E_SERVICE_NOT_RUNNING) {
						system_display_critical_message("The Windows audio service is not running.");
					} else {
						// TODO: Logging
						win32_check_for_error();
						system_display_critical_message("Audio Thread could not be initialized");
					}
					Destroy();
					return false;
				}

				if (mix_format->wFormatTag == WAVE_FORMAT_EXTENSIBLE && mix_format->cbSize >= 22) {
					memcpy(&format, mix_format, sizeof(WAVEFORMATEXTENSIBLE));
				} else {
					memcpy(&format, mix_format, sizeof(WAVEFORMATEX));
				}
				CoTaskMemFree(mix_format);

				if (format.SubFormat != KSDATAFORMAT_SUBTYPE_IEEE_FLOAT || format.Format.wBitsPerSample != 32 || format.Samples.wValidBitsPerSample != 32) {
					format.SubFormat = KSDATAFORMAT_SUBTYPE_IEEE_FLOAT;
					format.Format.wBitsPerSample = 32;
					format.Samples.wValidBitsPerSample = 32;

					WAVEFORMATEX *closest_match = nullptr;
					if (client->IsFormatSupported(AUDCLNT_SHAREMODE_SHARED, (WAVEFORMATEX *)&format, &closest_match) != S_OK) {
						// TODO: We don't support IEEE 32 bit floating samples
						win32_check_for_error();
						Cleanup();
						CoTaskMemFree(closest_match);
						trigger_breakpoint();
						return false;
					}
					CoTaskMemFree(closest_match);
				}
			}

			REFERENCE_TIME requested_duration = 0;
			DWORD init_flags = AUDCLNT_STREAMFLAGS_EVENTCALLBACK | AUDCLNT_STREAMFLAGS_AUTOCONVERTPCM | AUDCLNT_STREAMFLAGS_SRC_DEFAULT_QUALITY;
			hr = client->Initialize(AUDCLNT_SHAREMODE_SHARED, init_flags, requested_duration, 0, (WAVEFORMATEX *)&format, nullptr);
			if (hr == AUDCLNT_E_BUFFER_SIZE_NOT_ALIGNED) {
				hr = client->GetBufferSize(&total_samples);
				if (FAILED(hr)) {
					// TODO: Logging
					win32_check_for_error();
				}

				requested_duration = (REFERENCE_TIME)((10000.0 * 1000 / format.Format.nSamplesPerSec * total_samples) + 0.5);
				client->Release();

				hr = device->Activate(IID_IAudioClient, CLSCTX_ALL, NULL, (void **)&client);
				if (FAILED(hr)) {
					// TODO: Logging
					win32_check_for_error();
					return false;
				}

				WAVEFORMATEX *mix_format = nullptr;
				hr = client->GetMixFormat(&mix_format);
				if (FAILED(hr)) {
					// TODO: Logging
					win32_check_for_error();
					Destroy();
					return false;
				}
				CoTaskMemFree(mix_format);

				if (format.SubFormat != KSDATAFORMAT_SUBTYPE_IEEE_FLOAT || format.Format.wBitsPerSample != 32 || format.Samples.wValidBitsPerSample != 32) {
					format.SubFormat = KSDATAFORMAT_SUBTYPE_IEEE_FLOAT;
					format.Format.wBitsPerSample = 32;
					format.Samples.wValidBitsPerSample = 32;

					WAVEFORMATEX *closest_match = nullptr;
					if (client->IsFormatSupported(AUDCLNT_SHAREMODE_SHARED, (WAVEFORMATEX *)&format, &closest_match) != S_OK) {
						// TODO: We don't support IEEE 32 bit floating samples
						win32_check_for_error();
						Destroy();
						CoTaskMemFree(closest_match);
						trigger_breakpoint();
						return false;
					}
					CoTaskMemFree(closest_match);
				}

				hr = client->Initialize(AUDCLNT_SHAREMODE_SHARED, init_flags, requested_duration, requested_duration, mix_format, nullptr);
				if (FAILED(hr)) {
					// TODO: Logging
					win32_check_for_error();
					Destroy();
					return false;
				}
			}

			if (hr != S_OK) {
				// TODO: Logging
				win32_check_for_error();
				Destroy();
				return false;
			}

			hr = client->GetService(IID_IAudioSessionControl, (void **)&control);
			if (FAILED(hr)) {
				// TODO: Logging
				win32_check_for_error();
				Destroy();
				return false;
			}

			control->RegisterAudioSessionNotification(this);

			hr = client->GetService(IID_IAudioRenderClient, (void **)&renderer);
			if (FAILED(hr)) {
				// TODO: Logging
				win32_check_for_error();
				Destroy();
				return false;
			}

			hr = client->GetService(IID_IAudioClock, (void **)&clock);
			if (FAILED(hr)) {
				// TODO: Logging
				win32_check_for_error();
				Destroy();
				return false;
			}

			if (write_event == NULL) {
				write_event = CreateEventW(nullptr, FALSE, FALSE, 0);
				if (write_event == NULL) {
					// TODO: Better logging
					win32_check_for_error();
					Destroy();
					return false;
				}
			}

			client->SetEventHandle(write_event);

			if (!RetriveInformation()) {
				Destroy();
				return false;
			}

			ptrsize device_buffer_size = sizeof(r32) * total_samples * format.Format.nChannels;
			ptrsize buffer_size = (sizeof(r32) * format.Format.nSamplesPerSec * SYSTEM_AUDIO_BUFFER_SIZE_IN_MILLISECS * format.Format.nChannels) / 1000;

			if (device_buffer_size >= buffer_size) {
				// TODO: Error or Log??
				// If Audio device buffer can't be made smaller, and can't be overidden, then there'll be large audio lag
				// What do we do in this case?
				trigger_breakpoint();
				Destroy();
				return false;
			}

			return true;
		}

		return false;
	}

	static u8 *LockBuffer(System_Audio_Handle sys_audio, u32 *sample_count) {
		Audio_Client *audio = (Audio_Client *)sys_audio;

		// TODO: Error handling!!!
		u32 samples_padding;
		auto hr = audio->client->GetCurrentPadding(&samples_padding);
		if (SUCCEEDED(hr)) {
			*sample_count = audio->total_samples - samples_padding;

#if defined(BUILD_DEBUG) || defined(BUILD_DEVELOPER)
			if (*sample_count == audio->total_samples) {
				system_log(LOG_WARNING, "Audio", "Missed frame");
			}
#endif

			BYTE *data;
			hr = audio->renderer->GetBuffer(*sample_count, &data);
			if (SUCCEEDED(hr)) {
				return data;
			}
		}

		return nullptr;
	}

	static void UnlockBuffer(System_Audio_Handle sys_audio, u32 samples_written) {
		Audio_Client *audio = (Audio_Client *)sys_audio;
		audio->renderer->ReleaseBuffer(samples_written, 0);
		audio->write_cursor += samples_written;
	}

	static DWORD WINAPI AudioThreadProcedure(LPVOID param) {
		Audio_Client *audio = (Audio_Client *)param;

		DWORD task_index = 0;
		auto task = AvSetMmThreadCharacteristicsW(L"Audio", &task_index);
		if (task) {
			AvSetMmThreadPriority(task, AVRT_PRIORITY_HIGH);
		}

		context.handle.hptr = audio->thread;
		context.id = GetCurrentThreadId();
		context.allocator = NULL_ALLOCATOR;
		context.temp_memory = Temporary_Memory(0, 0);
		context.proc = (Thread_Proc)AudioThreadProcedure;
		context.data = 0;

		SetThreadDescription(audio->thread, L"Platform Audio Thread");

		u32 samples_padding = 0;
		u32 samples_available = 0;
		BYTE *data = 0;
		u32 flags = 0;

		auto hr = audio->client->Start();
		if (FAILED(hr)) {
			// TODO: Handle error
			win32_check_for_error();
			return 0;
		}

		static const System_Audio sys_audio = { audio, LockBuffer, UnlockBuffer };

		while (true) {
			DWORD wait_res = WaitForSingleObject(audio->write_event, SYSTEM_AUDIO_BUFFER_SIZE_IN_MILLISECS);
			if (wait_res != WAIT_OBJECT_0) {
				// Event handle timed out after a SYSTEM_AUDIO_BUFFER_SIZE_IN_MILLISECS wait.
				// If event is not signaled even after SYSTEM_AUDIO_BUFFER_SIZE_IN_MILLISECS millisecs, audio buffer is may be lost
				// TODO: Handle timed out, (may be we delete and find default device again?)
			}

			audio->on_update(sys_audio, audio->on_update_user_data);
		}

		return 0;
	}

	bool StartThread(Audio_Callback callback, Audio_Device_Refresh refresh, void *user_ptr) {
		if (callback) {
			on_update = callback;
			on_update_user_data = user_ptr;
		} else {
			on_update = StubAudioCallback;
			on_update_user_data = 0;
		}
		on_refresh = refresh;

		thread = CreateThread(nullptr, 0, AudioThreadProcedure, this, 0, nullptr);
		if (thread == NULL) {
			win32_check_for_error();
			Destroy();
			return false;
		}

		return true;
	}

	void StopThread() {
		TerminateThread(thread, 0);
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
			*ppvInterface = (IUnknown *)this;
		} else if (__uuidof(IAudioSessionEvents) == riid) {
			AddRef();
			*ppvInterface = (IAudioSessionEvents *)this;
		} else {
			*ppvInterface = NULL;
			return E_NOINTERFACE;
		}
		return S_OK;
	}

	HRESULT OnChannelVolumeChanged(DWORD  ChannelCount, float NewChannelVolumeArray[], DWORD ChangedChannel, LPCGUID EventContext) {
		return S_OK;
	}

	HRESULT OnDisplayNameChanged(LPCWSTR NewDisplayName, LPCGUID EventContext) {
		return S_OK;
	}

	HRESULT OnGroupingParamChanged(LPCGUID NewGroupingParam, LPCGUID EventContext) {
		return S_OK;
	}

	HRESULT OnIconPathChanged(LPCWSTR NewIconPath, LPCGUID EventContext) {
		return S_OK;
	}

	HRESULT OnSimpleVolumeChanged(float NewVolume, BOOL NewMute, LPCGUID EventContext) {
		return S_OK;
	}

	HRESULT OnStateChanged(AudioSessionState NewState) {
		return S_OK;
	}

	HRESULT OnSessionDisconnected(AudioSessionDisconnectReason DisconnectReason) {
		Cleanup();
		if (Initialize()) {
			if (on_refresh) {
				on_refresh(format.Format.nSamplesPerSec, format.Format.nChannels, on_update_user_data);
			}
		} else {
			// TODO: Log error
			// Audio device removed and new device could not be initialized
		}
		return S_OK;
	}
};

//
//
//

thread_local Thread_Context context;

static HWND            window_handle;
static WINDOWPLACEMENT windowed_placement;

static constexpr int WINDOWS_MAX_KEYS = 256;
static Key           windows_key_map[WINDOWS_MAX_KEYS]; // This number should be enough
static WPARAM		 windows_vk_key_map[WINDOWS_MAX_KEYS]; // This number should be enough

static constexpr int WINDOWS_MAX_EVENTS = 128000; // This number should be enough
static Event         windows_event_buffer[WINDOWS_MAX_EVENTS];
static u32			 windows_event_count;

static int		window_width;
static int		window_height;

static Audio_Client		windows_audio_client;

static Win32_Controller windows_controllers_state[XUSER_MAX_COUNT];
static Controller       controllers[XUSER_MAX_COUNT];

Array_View<u8> system_read_entire_file(const String path) {
	Array_View<u8> result = {};

	auto     length = path.count + 1;
	wchar_t *wpath = (wchar_t *)tallocate(length * sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8, 0, (char *)path.data, (int)path.count, wpath, (int)length);
	wpath[path.count] = 0;

	auto handle = CreateFileW(wpath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	win32_check_for_error();

	if (handle != INVALID_HANDLE_VALUE) {
		defer{
			CloseHandle(handle);
		};

		LARGE_INTEGER file_size;
		GetFileSizeEx(handle, &file_size);
		void *buffer = memory_allocate(file_size.QuadPart);

		// this is done because ReadFile can with blocks of DWORD and not LARGE_INTEGER
		DWORD read_size = 0;
		if (file_size.QuadPart > MAX_UINT32) {
			read_size = MAX_UINT32;
		} else {
			read_size = (DWORD)file_size.QuadPart;
		}

		DWORD read_bytes = 0;
		u8 *end_ptr = (u8 *)buffer + file_size.QuadPart;
		for (u8 *read_ptr = (u8 *)buffer;
			read_ptr < end_ptr;
			read_ptr += read_bytes) {
			// resetting *read_bytes* because docs says to (for next loop)
			read_bytes = 0;
			BOOL res = ReadFile(handle, read_ptr, read_size, &read_bytes, 0);
			if (res && (read_bytes == 0 || read_bytes == file_size.QuadPart)) {
				// END OF FILE reached
				break;
			} else {
				if (GetLastError()) { // if no error is registered, it is regarded as success
					memory_free(buffer);
					break;
				} else {
					// *read_size* went past the file size, so it's a success
					break;
				}
			}
		}

		result.data = (u8 *)buffer;
		result.count = file_size.QuadPart;
	}

	return result;
}

bool system_write_entire_file(const String path, Array_View<u8> content) {
	auto     length = path.count + 1;
	wchar_t *wpath = (wchar_t *)tallocate(length * sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8, 0, (char *)path.data, (int)path.count, wpath, (int)length);
	wpath[path.count] = 0;

	auto handle = CreateFileW(wpath, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (handle != INVALID_HANDLE_VALUE) {
		defer{
			CloseHandle(handle);
		};

		// this is done because WriteFile can with blocks of DWORD and not LARGE_INTEGER
		DWORD write_size = 0;
		if (content.count > MAX_UINT32) {
			write_size = MAX_UINT32;
		} else {
			write_size = (DWORD)content.count;
		}

		DWORD written_bytes = 0;
		u8 *end_ptr = content.data + content.count;
		for (u8 *write_ptr = content.data;
			write_ptr < end_ptr;
			write_ptr += written_bytes) {
			DWORD left_size = (DWORD)(content.count - (s64)written_bytes);
			if (write_size > left_size) {
				write_size = left_size;
			}

			// resetting *written_bytes* because docs says to (for next loop)
			written_bytes = 0;
			BOOL res = WriteFile(handle, write_ptr, write_size, &written_bytes, 0);
			if (res) {
				if (written_bytes == content.count) break;
			} else {
				return false;
			}
		}
	}

	return true;
}

ptrsize isystem_get_file_pointer(Handle handle) {
	LARGE_INTEGER result;
	auto          res = GetFileSizeEx(handle.hptr, &result);
	assert(res); // invalid file may result assertion here
	return result.QuadPart;
}

bool isystem_set_file_pointer(Handle handle, ptrsize position, File_Position start_position) {
	LARGE_INTEGER move_distance = {};
	move_distance.QuadPart = position;
	DWORD method;
	if (start_position == File_Position_BEG)
		method = FILE_BEGIN;
	else if (start_position == File_Position_CUR)
		method = FILE_CURRENT;
	else
		method = FILE_END;
	return SetFilePointerEx(handle.hptr, move_distance, 0, method);
}

bool isystem_read_file(Handle handle, ptrsize size, u8 *buffer) {
	// this is done because ReadFile can with blocks of DWORD and not LARGE_INTEGER
	DWORD read_size = 0;
	if (size > MAX_UINT32) {
		read_size = MAX_UINT32;
	} else {
		read_size = (DWORD)size;
	}

	DWORD read_bytes = 0;
	u8 *end_ptr = buffer + size;
	for (u8 *read_ptr = buffer;
		read_ptr < end_ptr;
		read_ptr += read_bytes) {
		// resetting *read_bytes* because docs says to (for next loop)
		read_bytes = 0;
		BOOL res = ReadFile(handle.hptr, read_ptr, read_size, &read_bytes, 0);
		if (res && (read_bytes == 0 || read_bytes == size)) {
			// END OF FILE reached
			break;
		} else {
			if (GetLastError()) { // if no error is registered, it is regarded as success
				return false;
				break;
			} else {
				// *read_size* went past the file size, so it's a success
				break;
			}
		}
	}

	return true;
}

bool isystem_write_file(Handle handle, void *ptr, ptrsize size) {
	u8 *content = (u8 *)ptr;
	// this is done because WriteFile can with blocks of DWORD and not LARGE_INTEGER
	DWORD write_size = 0;
	if (size > MAX_UINT32) {
		write_size = MAX_UINT32;
	} else {
		write_size = (DWORD)size;
	}

	DWORD written_bytes = 0;
	u8 *end_ptr = content + size;
	for (u8 *write_ptr = content;
		write_ptr < end_ptr;
		write_ptr += written_bytes) {
		DWORD left_size = (DWORD)(size - (s64)written_bytes);
		if (write_size > left_size) {
			write_size = left_size;
		}

		// resetting *written_bytes* because docs says to (for next loop)
		written_bytes = 0;
		BOOL res = WriteFile(handle.hptr, write_ptr, write_size, &written_bytes, 0);
		if (res) {
			if (written_bytes == size) break;
		} else {
			return false;
		}
	}

	return true;
}

u64 isystem_get_file_size(Handle handle) {
	LARGE_INTEGER size;
	if (GetFileSizeEx(handle.hptr, &size)) {
		return size.QuadPart;
	}
	return 0;
}

u64 isystem_get_file_creation_time(Handle handle) {
	FILETIME t;
	if (GetFileTime(handle.hptr, &t, nullptr, nullptr)) {
		ULARGE_INTEGER r;
		r.HighPart = t.dwHighDateTime;
		r.LowPart = t.dwLowDateTime;
		return r.QuadPart;
	}
	return 0;
}

u64 isystem_get_file_last_access_time(Handle handle) {
	FILETIME t;
	if (GetFileTime(handle.hptr, nullptr, &t, nullptr)) {
		ULARGE_INTEGER r;
		r.HighPart = t.dwHighDateTime;
		r.LowPart = t.dwLowDateTime;
		return r.QuadPart;
	}
	return 0;
}

u64 isystem_get_file_last_modified_time(Handle handle) {
	FILETIME t;
	if (GetFileTime(handle.hptr, nullptr, nullptr, &t)) {
		ULARGE_INTEGER r;
		r.HighPart = t.dwHighDateTime;
		r.LowPart = t.dwLowDateTime;
		return r.QuadPart;
	}
	return 0;
}

bool system_open_file(const String path, File_Operation options, System_File *file) {
	DWORD access = 0;
	DWORD creation = 0;

	*file = {};
	if (options == File_Operation_READ) {
		access = GENERIC_READ;
		creation = OPEN_EXISTING;
		file->read = isystem_read_file;
	} else if (options == File_Operation_APPEND) {
		access = GENERIC_WRITE;
		creation = OPEN_EXISTING;
		file->write = isystem_write_file;
	} else if (options == File_Operation_READ_WRITE) {
		access = GENERIC_READ | GENERIC_WRITE;
		creation = OPEN_EXISTING;
		file->read = isystem_read_file;
		file->write = isystem_write_file;
	} else {
		access = GENERIC_WRITE;
		creation = CREATE_ALWAYS;
		file->write = isystem_write_file;
	}

	auto     length = path.count + 1;
	wchar_t *wpath = (wchar_t *)tallocate(length * sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8, 0, (char *)path.data, (int)path.count, wpath, (int)length);
	wpath[length] = 0;

	auto handle = CreateFileW(wpath, access, FILE_SHARE_WRITE | FILE_SHARE_READ, 0, creation, FILE_ATTRIBUTE_NORMAL, 0);
	if (handle == INVALID_HANDLE_VALUE) {
		auto error = GetLastError();
		win32_check_for_error();
		*file = {};
		return false;
	}

	file->handle.hptr = handle;
	file->get_cursor = isystem_get_file_pointer;
	file->set_cursor = isystem_set_file_pointer;
	file->get_size = isystem_get_file_size;
	file->get_creation_time = isystem_get_file_creation_time;
	file->get_last_modified_time = isystem_get_file_last_modified_time;
	file->get_last_access_time = isystem_get_file_last_access_time;

	return true;
}

void system_close_file(System_File *file) {
	CloseHandle(file->handle.hptr);
	*file = {};
}

inline wchar_t *windows_get_file_extension(wchar_t *file) {
	while (*file) {
		if (*file == L'.') return file;
		file += 1;
	}
	return L"";
}

u32 windows_find_files_count(const wchar_t *root_dir, int root_dir_len, const wchar_t *extension, bool recursive) {
	scoped_temporary_allocation();

	wchar_t *search = (wchar_t *)tallocate((root_dir_len + 3) * sizeof(wchar_t));
	memcpy(search, root_dir, root_dir_len * sizeof(wchar_t));
	search[root_dir_len + 0] = '/';
	search[root_dir_len + 1] = '*';
	search[root_dir_len + 2] = 0;

	WIN32_FIND_DATAW find_data;
	auto find = FindFirstFileW(search, &find_data);
	if (find == INVALID_HANDLE_VALUE) {
		win32_check_for_error();
		return 0;
	}

	u32 count = 0;

	do {
		if (recursive && (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
			if (wcscmp(find_data.cFileName, L".") != 0 && wcscmp(find_data.cFileName, L"..") != 0) {
				int next_dir_len = (int)(root_dir_len + wcslen(find_data.cFileName) + 1);
				wchar_t *next_dir = (wchar_t *)tallocate((next_dir_len + 1) * sizeof(wchar_t));
				wcscpy_s(next_dir, next_dir_len + 1, root_dir);
				wcscat_s(next_dir, next_dir_len + 1, L"/");
				wcscat_s(next_dir, next_dir_len + 1, find_data.cFileName);
				count += windows_find_files_count(next_dir, next_dir_len, extension, recursive);
			}
		} else if (wcscmp(extension, L"*") == 0 || wcscmp(windows_get_file_extension(find_data.cFileName), extension) == 0) {
			count += 1;
		}
	} while (FindNextFileW(find, &find_data) != 0);

	return count;
}

u32 windows_find_files_info(System_Find_File_Info *info, const System_Find_File_Info *info_one_past_end, const wchar_t *root_dir, int root_dir_len, const wchar_t *extension, bool recursive) {
	scoped_temporary_allocation();

	wchar_t *search = (wchar_t *)tallocate((root_dir_len + 3) * sizeof(wchar_t));
	memcpy(search, root_dir, root_dir_len * sizeof(wchar_t));
	search[root_dir_len + 0] = '/';
	search[root_dir_len + 1] = '*';
	search[root_dir_len + 2] = 0;

	WIN32_FIND_DATAW find_data;
	auto find = FindFirstFileW(search, &find_data);
	if (find == INVALID_HANDLE_VALUE) {
		win32_check_for_error();
		return 0;
	}

	u32 count = 0;

	do {
		if (recursive && (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
			if (wcscmp(find_data.cFileName, L".") != 0 && wcscmp(find_data.cFileName, L"..") != 0) {
				int next_dir_len = (int)(root_dir_len + wcslen(find_data.cFileName) + 1);
				wchar_t *next_dir = (wchar_t *)tallocate((next_dir_len + 1) * sizeof(wchar_t));
				wcscpy_s(next_dir, next_dir_len + 1, root_dir);
				wcscat_s(next_dir, next_dir_len + 1, L"/");
				wcscat_s(next_dir, next_dir_len + 1, find_data.cFileName);
				u32 next_count = windows_find_files_info(info, info_one_past_end, next_dir, next_dir_len, extension, recursive);;
				count += next_count;
				info += next_count;
			}
		} else if (wcscmp(extension, L"*") == 0 || wcscmp(windows_get_file_extension(find_data.cFileName), extension) == 0) {
			int found_file_len = (int)wcslen(find_data.cFileName);
			int found_full_file_len = root_dir_len + found_file_len + 1;
			wchar_t *full_file_name = (wchar_t *)tallocate((found_full_file_len + 1) * sizeof(wchar_t));
			wcscpy_s(full_file_name, found_full_file_len + 1, root_dir);
			wcscat_s(full_file_name, found_full_file_len + 1, L"/");
			wcscat_s(full_file_name, found_full_file_len + 1, find_data.cFileName);

			char *utf_full_file_name = (char *)memory_allocate(found_full_file_len + 1);
			WideCharToMultiByte(CP_UTF8, 0, full_file_name, found_full_file_len, utf_full_file_name, found_full_file_len, 0, 0);
			utf_full_file_name[found_full_file_len] = 0;

			info->path.data = (u8 *)utf_full_file_name;
			info->path.count = root_dir_len + 1;
			info->name.data = (u8 *)utf_full_file_name + root_dir_len + 1;
			info->name.count = found_file_len;

			auto find_result = string_isearch_reverse(info->name, ".");
			if (find_result.found) {
				info->extension = string_substring(info->name, find_result.start_index, info->name.count - find_result.start_index);
			} else {
				info->extension = String(info->name.data + info->name.count, 0);
			}

			info->size = (find_data.nFileSizeHigh * ((u64)MAXDWORD + 1)) + find_data.nFileSizeLow;

			ULARGE_INTEGER ul;
			ul.HighPart = find_data.ftLastWriteTime.dwHighDateTime;
			ul.LowPart = find_data.ftLastWriteTime.dwLowDateTime;
			info->modified = ul.QuadPart;

			count += 1;
			info += 1;
		}
	} while (FindNextFileW(find, &find_data) != 0 && info != info_one_past_end);

	return count;
}

Array_View<System_Find_File_Info> system_find_files(const String directory, const String extension, bool recursive) {
	scoped_temporary_allocation();

	int      length = MultiByteToWideChar(CP_UTF8, 0, (char *)directory.data, (int)directory.count, 0, 0);
	wchar_t *wsearch = (wchar_t *)tallocate((length + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8, 0, (char *)directory.data, (int)directory.count, wsearch, length);
	wsearch[length] = 0;

	wchar_t wextension[50];
	StringCbPrintfW(wextension, 50 * sizeof(wchar_t), L"%S", tto_cstring(extension));

	u32 items_count = windows_find_files_count(wsearch, length, wextension, recursive);

	Array_View<System_Find_File_Info> items;
	items.data = (System_Find_File_Info *)memory_allocate(items_count * sizeof(System_Find_File_Info));
	items.count = windows_find_files_info(items.data, items.data + items_count, wsearch, length, wextension, recursive);

	return items;
}

//
//
//

Vec2s system_get_primary_monitor_size() {
	Vec2s result;
	result.x = GetSystemMetricsForDpi(SM_CXFULLSCREEN, DPI_AWARENESS_PER_MONITOR_AWARE);
	result.y = GetSystemMetricsForDpi(SM_CYFULLSCREEN, DPI_AWARENESS_PER_MONITOR_AWARE);
	return result;
}

Vec2s system_get_client_size() {
	RECT rect = {};
	GetClientRect(window_handle, &rect);

	return vec2s(rect.right - rect.left, rect.bottom - rect.top);
}

int system_fullscreen_state(int toggle) {
	LONG_PTR style = GetWindowLongPtrW(window_handle, GWL_STYLE);
	bool     is_full = !(style & WS_OVERLAPPEDWINDOW);

	if (toggle == SYSTEM_TOGGLE) {
		if (is_full)
			toggle = SYSTEM_DISABLE;
		else
			toggle = SYSTEM_ENABLE;
	}

	windowed_placement.length = sizeof(WINDOWPLACEMENT);

	switch (toggle) {
	case SYSTEM_ENABLE:
	{
		if (!is_full) {
			MONITORINFO mi;
			mi.cbSize = sizeof(mi);
			if (GetWindowPlacement(window_handle, &windowed_placement) &&
				GetMonitorInfoW(MonitorFromWindow(window_handle,
					MONITOR_DEFAULTTONEAREST),
					&mi)) {
				SetWindowLongPtrW(window_handle, GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(window_handle, HWND_TOP,
					mi.rcMonitor.left, mi.rcMonitor.top,
					mi.rcMonitor.right - mi.rcMonitor.left,
					mi.rcMonitor.bottom - mi.rcMonitor.top,
					SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
			}
		}
		return SYSTEM_ENABLE;
	} break;

	case SYSTEM_DISABLE:
	{
		if (is_full) {
			SetWindowLongPtrW(window_handle, GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
			SetWindowPlacement(window_handle, &windowed_placement);
			SetWindowPos(window_handle, 0, 0, 0, 0, 0,
				SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
				SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		}
		return SYSTEM_DISABLE;
	} break;
	case SYSTEM_QUERY:
	{
		return (is_full ? SYSTEM_ENABLE : SYSTEM_DISABLE);
	} break;

	invalid_default_case();
	}

	return SYSTEM_QUERY; // i don't know what to return here so...
}

int system_maximize_state(int toggle) {
	WINDOWPLACEMENT wnd_placement = {};
	wnd_placement.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(window_handle, &wnd_placement);
	bool is_maximized = (wnd_placement.showCmd == SW_SHOWMAXIMIZED);

	if (toggle == SYSTEM_TOGGLE) {
		if (is_maximized)
			toggle = SYSTEM_DISABLE;
		else
			toggle = SYSTEM_ENABLE;
	}

	switch (toggle) {
	case SYSTEM_ENABLE:
	{
		if (!is_maximized) {
			wnd_placement.showCmd = SW_SHOWMAXIMIZED;
			SetWindowPlacement(window_handle, &wnd_placement);
		}
		return SYSTEM_ENABLE;
	} break;

	case SYSTEM_DISABLE:
	{
		if (is_maximized) {
			wnd_placement.showCmd = SW_SHOWNORMAL;
			SetWindowPlacement(window_handle, &wnd_placement);
		}
		return SYSTEM_DISABLE;
	} break;
	case SYSTEM_QUERY:
	{
		return (is_maximized ? SYSTEM_ENABLE : SYSTEM_DISABLE);
	} break;

	invalid_default_case();
	}

	return SYSTEM_QUERY; // i don't know what to return here so...
}

System_Window_State system_get_window_state() {
	System_Window_State state = {};

	WINDOWPLACEMENT *placement = (WINDOWPLACEMENT *)memory_allocate(sizeof(*placement));
	placement->length = sizeof(*placement);
	if (GetWindowPlacement(window_handle, placement)) {
		state.handle = placement;
		state.size = sizeof(*placement);
	} else {
		memory_free(placement);
	}

	return state;
}

bool system_set_window_state(System_Window_State state) {
	if (state.handle && state.size == sizeof(WINDOWPLACEMENT)) {
		return SetWindowPlacement(window_handle, (WINDOWPLACEMENT *)state.handle);
	}

	return false;
}

void system_set_capture() {
	SetCapture(window_handle);
}

void system_release_capture() {
	ReleaseCapture();
}

void system_show_cursor() {
	CURSORINFO info;
	info.cbSize = sizeof(info);
	if (GetCursorInfo(&info) && info.flags == 0) {
		ShowCursor(TRUE);
	}
}

void system_hide_cursor() {
	CURSORINFO info;
	info.cbSize = sizeof(info);
	if (GetCursorInfo(&info) && (info.flags == CURSOR_SHOWING || info.flags == CURSOR_SUPPRESSED)) {
		ShowCursor(FALSE);
	} else {
		win32_check_for_error();
	}
}

void system_set_cursor_kind(Cursor_Kind kind) {
	HANDLE handle = INVALID_HANDLE_VALUE;
	switch (kind) {
	case Cursor_Kind_ARROW:
		handle = LoadImageW(0, MAKEINTRESOURCEW(OCR_NORMAL), IMAGE_CURSOR, 0, 0, LR_SHARED);
		break;

	case Cursor_Kind_IBEAM:
		handle = LoadImageW(0, MAKEINTRESOURCEW(OCR_IBEAM), IMAGE_CURSOR, 0, 0, LR_SHARED);
		break;

	case Cursor_Kind_SIZE_ALL:
		handle = LoadImageW(0, MAKEINTRESOURCEW(OCR_SIZEALL), IMAGE_CURSOR, 0, 0, LR_SHARED);
		break;

	case Cursor_Kind_SIZE_NS:
		handle = LoadImageW(0, MAKEINTRESOURCEW(OCR_SIZENS), IMAGE_CURSOR, 0, 0, LR_SHARED);
		break;

	case Cursor_Kind_SIZE_EW:
		handle = LoadImageW(0, MAKEINTRESOURCEW(OCR_SIZEWE), IMAGE_CURSOR, 0, 0, LR_SHARED);
		break;

	case Cursor_Kind_SIZE_NESW:
		handle = LoadImageW(0, MAKEINTRESOURCEW(OCR_SIZENESW), IMAGE_CURSOR, 0, 0, LR_SHARED);
		break;

	case Cursor_Kind_SIZE_NWSE:
		handle = LoadImageW(0, MAKEINTRESOURCEW(OCR_SIZENWSE), IMAGE_CURSOR, 0, 0, LR_SHARED);
		break;

	case Cursor_Kind_HAND:
		handle = LoadImageW(0, MAKEINTRESOURCEW(OCR_HAND), IMAGE_CURSOR, 0, 0, LR_SHARED);
		break;

		invalid_default_case();
	}
	SetCursor((HCURSOR)handle);
}

bool system_is_cursor_visible() {
	CURSORINFO info;
	info.cbSize = sizeof(info);
	return (GetCursorInfo(&info) && info.flags != 0);
}

String system_get_clipboard_text() {
	if (IsClipboardFormatAvailable(CF_UNICODETEXT)) {
		if (OpenClipboard(window_handle)) {
			HANDLE   buffer = GetClipboardData(CF_UNICODETEXT);
			wchar_t *wstring = (wchar_t *)GlobalLock(buffer);
			char *string = 0;
			int      length = 0;
			if (wstring) {
				length = WideCharToMultiByte(CP_UTF8, 0, wstring, -1, 0, 0, 0, 0);
				string = (char *)memory_allocate(length);
				length = WideCharToMultiByte(CP_UTF8, 0, wstring, -1, string, length, 0, 0);
				GlobalUnlock(buffer);
			}
			CloseClipboard();
			return String(string, length);
		}
	}
	return String{};
}

void system_set_clipboard_text(const String string) {
	if (OpenClipboard(window_handle)) {
		EmptyClipboard();
		auto handle = GlobalAlloc(GMEM_MOVEABLE, (string.count + 1) * sizeof(wchar_t));
		if (handle) {
			wchar_t *buffer = (wchar_t *)GlobalLock(handle);
			MultiByteToWideChar(CP_UTF8, 0, (const char *)string.data, (int)string.count, buffer, (int)string.count + 1);
			buffer[string.count] = 0;
			GlobalUnlock(handle);
			SetClipboardData(CF_UNICODETEXT, handle);
		}
		CloseClipboard();
	}
}

//
//
//

void win32_map_keys() {
	windows_key_map[(u32)'A'] = Key_A;
	windows_key_map[(u32)'B'] = Key_B;
	windows_key_map[(u32)'C'] = Key_C;
	windows_key_map[(u32)'D'] = Key_D;
	windows_key_map[(u32)'E'] = Key_E;
	windows_key_map[(u32)'F'] = Key_F;
	windows_key_map[(u32)'G'] = Key_G;
	windows_key_map[(u32)'H'] = Key_H;
	windows_key_map[(u32)'I'] = Key_I;
	windows_key_map[(u32)'J'] = Key_J;
	windows_key_map[(u32)'K'] = Key_K;
	windows_key_map[(u32)'L'] = Key_L;
	windows_key_map[(u32)'M'] = Key_M;
	windows_key_map[(u32)'N'] = Key_N;
	windows_key_map[(u32)'O'] = Key_O;
	windows_key_map[(u32)'P'] = Key_P;
	windows_key_map[(u32)'Q'] = Key_Q;
	windows_key_map[(u32)'R'] = Key_R;
	windows_key_map[(u32)'S'] = Key_S;
	windows_key_map[(u32)'T'] = Key_T;
	windows_key_map[(u32)'U'] = Key_U;
	windows_key_map[(u32)'V'] = Key_V;
	windows_key_map[(u32)'W'] = Key_W;
	windows_key_map[(u32)'X'] = Key_X;
	windows_key_map[(u32)'Y'] = Key_Y;
	windows_key_map[(u32)'Z'] = Key_Z;

	windows_key_map[(u32)'0'] = Key_0;
	windows_key_map[(u32)'1'] = Key_1;
	windows_key_map[(u32)'2'] = Key_2;
	windows_key_map[(u32)'3'] = Key_3;
	windows_key_map[(u32)'4'] = Key_4;
	windows_key_map[(u32)'5'] = Key_5;
	windows_key_map[(u32)'6'] = Key_6;
	windows_key_map[(u32)'7'] = Key_7;
	windows_key_map[(u32)'8'] = Key_8;
	windows_key_map[(u32)'9'] = Key_9;

	windows_key_map[VK_RETURN] = Key_RETURN;
	windows_key_map[VK_ESCAPE] = Key_ESCAPE;
	windows_key_map[VK_BACK] = Key_BACKSPACE;
	windows_key_map[VK_TAB] = Key_TAB;
	windows_key_map[VK_SPACE] = Key_SPACE;
	windows_key_map[VK_SHIFT] = Key_SHIFT;
	windows_key_map[VK_CONTROL] = Key_CTRL;

	windows_key_map[VK_F1] = Key_F1;
	windows_key_map[VK_F2] = Key_F2;
	windows_key_map[VK_F3] = Key_F3;
	windows_key_map[VK_F4] = Key_F4;
	windows_key_map[VK_F5] = Key_F5;
	windows_key_map[VK_F6] = Key_F6;
	windows_key_map[VK_F7] = Key_F7;
	windows_key_map[VK_F8] = Key_F8;
	windows_key_map[VK_F9] = Key_F9;
	windows_key_map[VK_F10] = Key_F10;
	windows_key_map[VK_F11] = Key_F11;
	windows_key_map[VK_F12] = Key_F12;

	windows_key_map[VK_SNAPSHOT] = Key_PRINT_SCREEN;
	windows_key_map[VK_INSERT] = Key_INSERT;
	windows_key_map[VK_HOME] = Key_HOME;
	windows_key_map[VK_PRIOR] = Key_PAGE_UP;
	windows_key_map[VK_NEXT] = Key_PAGE_DOWN;
	windows_key_map[VK_DELETE] = Key_DELETE;
	windows_key_map[VK_END] = Key_END;
	windows_key_map[VK_RIGHT] = Key_RIGHT;
	windows_key_map[VK_LEFT] = Key_LEFT;
	windows_key_map[VK_DOWN] = Key_DOWN;
	windows_key_map[VK_UP] = Key_UP;
	windows_key_map[VK_DIVIDE] = Key_DIVIDE;
	windows_key_map[VK_MULTIPLY] = Key_MULTIPLY;
	windows_key_map[VK_ADD] = Key_PLUS;
	windows_key_map[VK_SUBTRACT] = Key_MINUS;
	windows_key_map[VK_DECIMAL] = Key_PERIOD;
	windows_key_map[VK_OEM_3] = Key_BACK_TICK;

	windows_key_map[VK_NUMPAD0] = Key_PAD_0;
	windows_key_map[VK_NUMPAD1] = Key_PAD_1;
	windows_key_map[VK_NUMPAD2] = Key_PAD_2;
	windows_key_map[VK_NUMPAD3] = Key_PAD_3;
	windows_key_map[VK_NUMPAD4] = Key_PAD_4;
	windows_key_map[VK_NUMPAD5] = Key_PAD_5;
	windows_key_map[VK_NUMPAD6] = Key_PAD_6;
	windows_key_map[VK_NUMPAD7] = Key_PAD_7;
	windows_key_map[VK_NUMPAD8] = Key_PAD_8;
	windows_key_map[VK_NUMPAD9] = Key_PAD_9;

	windows_vk_key_map[Key_A] = (u32)'A';
	windows_vk_key_map[Key_B] = (u32)'B';
	windows_vk_key_map[Key_C] = (u32)'C';
	windows_vk_key_map[Key_D] = (u32)'D';
	windows_vk_key_map[Key_E] = (u32)'E';
	windows_vk_key_map[Key_F] = (u32)'F';
	windows_vk_key_map[Key_G] = (u32)'G';
	windows_vk_key_map[Key_H] = (u32)'H';
	windows_vk_key_map[Key_I] = (u32)'I';
	windows_vk_key_map[Key_J] = (u32)'J';
	windows_vk_key_map[Key_K] = (u32)'K';
	windows_vk_key_map[Key_L] = (u32)'L';
	windows_vk_key_map[Key_M] = (u32)'M';
	windows_vk_key_map[Key_N] = (u32)'N';
	windows_vk_key_map[Key_O] = (u32)'O';
	windows_vk_key_map[Key_P] = (u32)'P';
	windows_vk_key_map[Key_Q] = (u32)'Q';
	windows_vk_key_map[Key_R] = (u32)'R';
	windows_vk_key_map[Key_S] = (u32)'S';
	windows_vk_key_map[Key_T] = (u32)'T';
	windows_vk_key_map[Key_U] = (u32)'U';
	windows_vk_key_map[Key_V] = (u32)'V';
	windows_vk_key_map[Key_W] = (u32)'W';
	windows_vk_key_map[Key_X] = (u32)'X';
	windows_vk_key_map[Key_Y] = (u32)'Y';
	windows_vk_key_map[Key_Z] = (u32)'Z';

	windows_vk_key_map[Key_0] = (u32)'0';
	windows_vk_key_map[Key_1] = (u32)'1';
	windows_vk_key_map[Key_2] = (u32)'2';
	windows_vk_key_map[Key_3] = (u32)'3';
	windows_vk_key_map[Key_4] = (u32)'4';
	windows_vk_key_map[Key_5] = (u32)'5';
	windows_vk_key_map[Key_6] = (u32)'6';
	windows_vk_key_map[Key_7] = (u32)'7';
	windows_vk_key_map[Key_8] = (u32)'8';
	windows_vk_key_map[Key_9] = (u32)'9';

	windows_vk_key_map[Key_RETURN] = VK_RETURN;
	windows_vk_key_map[Key_ESCAPE] = VK_ESCAPE;
	windows_vk_key_map[Key_BACKSPACE] = VK_BACK;
	windows_vk_key_map[Key_TAB] = VK_TAB;
	windows_vk_key_map[Key_SPACE] = VK_SPACE;
	windows_vk_key_map[Key_SHIFT] = VK_SHIFT;
	windows_vk_key_map[Key_CTRL] = VK_CONTROL;

	windows_vk_key_map[Key_F1] = VK_F1;
	windows_vk_key_map[Key_F2] = VK_F2;
	windows_vk_key_map[Key_F3] = VK_F3;
	windows_vk_key_map[Key_F4] = VK_F4;
	windows_vk_key_map[Key_F5] = VK_F5;
	windows_vk_key_map[Key_F6] = VK_F6;
	windows_vk_key_map[Key_F7] = VK_F7;
	windows_vk_key_map[Key_F8] = VK_F8;
	windows_vk_key_map[Key_F9] = VK_F9;
	windows_vk_key_map[Key_F10] = VK_F10;
	windows_vk_key_map[Key_F11] = VK_F11;
	windows_vk_key_map[Key_F12] = VK_F12;

	windows_vk_key_map[Key_PRINT_SCREEN] = VK_SNAPSHOT;
	windows_vk_key_map[Key_INSERT] = VK_INSERT;
	windows_vk_key_map[Key_HOME] = VK_HOME;
	windows_vk_key_map[Key_PAGE_UP] = VK_PRIOR;
	windows_vk_key_map[Key_PAGE_DOWN] = VK_NEXT;
	windows_vk_key_map[Key_DELETE] = VK_DELETE;
	windows_vk_key_map[Key_END] = VK_END;
	windows_vk_key_map[Key_RIGHT] = VK_RIGHT;
	windows_vk_key_map[Key_LEFT] = VK_LEFT;
	windows_vk_key_map[Key_DOWN] = VK_DOWN;
	windows_vk_key_map[Key_UP] = VK_UP;
	windows_vk_key_map[Key_DIVIDE] = VK_DIVIDE;
	windows_vk_key_map[Key_MULTIPLY] = VK_MULTIPLY;
	windows_vk_key_map[Key_PLUS] = VK_ADD;
	windows_vk_key_map[Key_MINUS] = VK_SUBTRACT;
	windows_vk_key_map[Key_PERIOD] = VK_DECIMAL;
	windows_vk_key_map[Key_BACK_TICK] = VK_OEM_3;

	windows_vk_key_map[Key_PAD_0] = VK_NUMPAD0;
	windows_vk_key_map[Key_PAD_1] = VK_NUMPAD1;
	windows_vk_key_map[Key_PAD_2] = VK_NUMPAD2;
	windows_vk_key_map[Key_PAD_3] = VK_NUMPAD3;
	windows_vk_key_map[Key_PAD_4] = VK_NUMPAD4;
	windows_vk_key_map[Key_PAD_5] = VK_NUMPAD5;
	windows_vk_key_map[Key_PAD_6] = VK_NUMPAD6;
	windows_vk_key_map[Key_PAD_7] = VK_NUMPAD7;
	windows_vk_key_map[Key_PAD_8] = VK_NUMPAD8;
	windows_vk_key_map[Key_PAD_9] = VK_NUMPAD9;
}

Key win32_get_mapped_key(WPARAM wparam) {
	if (wparam >= WINDOWS_MAX_KEYS) return Key_UNKNOWN;
	return windows_key_map[wparam];
}

WPARAM win32_get_mapped_vk_key(Key key) {
	return windows_vk_key_map[key];
}

void win32_mouse_button_event(Mouse_Button_Event *event, WPARAM wparam, LPARAM lparam) {
	int   x = GET_X_LPARAM(lparam);
	int   y = GET_Y_LPARAM(lparam);
	event->position = vec2s(x, window_height - y); // inverting y
}

void win32_add_event(Event event) {
	windows_event_buffer[windows_event_count] = event;
	// HACK: We just loop back to 0, if the event becomes larger than WINDOWS_MAX_EVENTS
	windows_event_count = (windows_event_count + 1) % WINDOWS_MAX_EVENTS;
}

static LRESULT CALLBACK win32_wnd_proc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	Event   event = {};
	LRESULT result = 0;

	switch (msg) {
	case WM_ACTIVATE:
	{
		int low = LOWORD(wparam);

		if (low == WA_ACTIVE || low == WA_CLICKACTIVE)
			event.type = Event_Type_WINDOW_ACTIVE;
		else
			event.type = Event_Type_WINDOW_INACTIVE;

		result = DefWindowProcW(wnd, msg, wparam, lparam);
	} break;

	case WM_SIZE:
	{
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);

		window_width = x;
		window_height = y;

		// need to re-set clip rectangle after resize
		RECT rc;
		GetClientRect(wnd, &rc);

		CURSORINFO info;
		info.cbSize = sizeof(info);
		if (GetCursorInfo(&info) && (info.flags == 0)) {
			POINT pt = { rc.left, rc.top };
			POINT pt2 = { rc.right, rc.bottom };
			ClientToScreen(wnd, &pt);
			ClientToScreen(wnd, &pt2);
			SetRect(&rc, pt.x, pt.y, pt2.x, pt2.y);
			ClipCursor(&rc);
			int c_x = rc.left + (rc.right - rc.left) / 2;
			int c_y = rc.top + (rc.bottom - rc.top) / 2;
			SetCursorPos(c_x, c_y);
		}

		if (x > 0 && y > 0) {
			HDC dc = GetDC(wnd);
			SwapBuffers(dc);
			ReleaseDC(wnd, dc);
		}

		event.type = Event_Type_WINDOW_RESIZE;
		event.window.dimension = vec2s(x, y);
	} break;

	case WM_ENTERSIZEMOVE:
	{
		windows_audio_client.client->Stop();
		result = DefWindowProcW(wnd, msg, wparam, lparam);
	} break;

	case WM_EXITSIZEMOVE:
	{
		windows_audio_client.client->Start();
		result = DefWindowProcW(wnd, msg, wparam, lparam);
	} break;

	case WM_DPICHANGED:
	{
		RECT *const suggested_rect = (RECT *)lparam;
		auto        left = suggested_rect->left;
		auto        top = suggested_rect->top;
		auto        width = suggested_rect->right - suggested_rect->left;
		auto        height = suggested_rect->bottom - suggested_rect->top;
		SetWindowPos(wnd, 0, left, top, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
	} break;

	case WM_CLOSE:
	{
		PostQuitMessage(0);
	} break;

	case WM_DESTROY:
	{
	} break;

	case WM_LBUTTONDOWN:
	{
		event.type = Event_Type_MOUSE_BUTTON_DOWN;
		event.mouse_button.symbol = Button_LEFT;
		event.mouse_button.state = Key_State_DOWN;
		win32_mouse_button_event(&event.mouse_button, wparam, lparam);
	} break;

	case WM_LBUTTONUP:
	{
		event.type = Event_Type_MOUSE_BUTTON_UP;
		event.mouse_button.symbol = Button_LEFT;
		event.mouse_button.state = Key_State_UP;
		win32_mouse_button_event(&event.mouse_button, wparam, lparam);
	} break;

	case WM_MBUTTONDOWN:
	{
		event.type = Event_Type_MOUSE_BUTTON_DOWN;
		event.mouse_button.symbol = Button_MIDDLE;
		event.mouse_button.state = Key_State_DOWN;
		win32_mouse_button_event(&event.mouse_button, wparam, lparam);
	} break;

	case WM_MBUTTONUP:
	{
		event.type = Event_Type_MOUSE_BUTTON_UP;
		event.mouse_button.symbol = Button_MIDDLE;
		event.mouse_button.state = Key_State_UP;
		win32_mouse_button_event(&event.mouse_button, wparam, lparam);
	} break;

	case WM_RBUTTONDOWN:
	{
		event.type = Event_Type_MOUSE_BUTTON_DOWN;
		event.mouse_button.symbol = Button_RIGHT;
		event.mouse_button.state = Key_State_DOWN;
		win32_mouse_button_event(&event.mouse_button, wparam, lparam);
	} break;

	case WM_RBUTTONUP:
	{
		event.type = Event_Type_MOUSE_BUTTON_UP;
		event.mouse_button.symbol = Button_RIGHT;
		event.mouse_button.state = Key_State_UP;
		win32_mouse_button_event(&event.mouse_button, wparam, lparam);
	} break;

	case WM_XBUTTONDOWN:
	{
		event.type = Event_Type_MOUSE_BUTTON_DOWN;
		event.mouse_button.symbol = ((wparam & MK_XBUTTON1) == MK_XBUTTON1) ? Button_X1 : Button_X2;
		event.mouse_button.state = Key_State_DOWN;
		win32_mouse_button_event(&event.mouse_button, wparam, lparam);
	} break;

	case WM_XBUTTONUP:
	{
		event.type = Event_Type_MOUSE_BUTTON_UP;
		event.mouse_button.symbol = ((wparam & MK_XBUTTON1) == MK_XBUTTON1) ? Button_X1 : Button_X2;
		event.mouse_button.state = Key_State_UP;
		win32_mouse_button_event(&event.mouse_button, wparam, lparam);
	} break;

	case WM_MOUSEMOVE:
	{
		RECT rc;
		GetClientRect(wnd, &rc);
		Vec2s wr = vec2s(rc.right - rc.left, rc.bottom - rc.top);

		event.type = Event_Type_MOUSE_CURSOR;
		int x = GET_X_LPARAM(lparam);
		int ny = GET_Y_LPARAM(lparam);
		int y = wr.y - ny; // inverting y

		CURSORINFO info;
		info.cbSize = sizeof(info);
		if (GetCursorInfo(&info) && (info.flags == 0)) {
			// Cursor is hidden, place cursor always at center
			POINT pt = { rc.left, rc.top };
			POINT pt2 = { rc.right, rc.bottom };
			ClientToScreen(wnd, &pt);
			ClientToScreen(wnd, &pt2);
			SetRect(&rc, pt.x, pt.y, pt2.x, pt2.y);
			ClipCursor(&rc);
			int c_x = rc.left + (rc.right - rc.left) / 2;
			int c_y = rc.top + (rc.bottom - rc.top) / 2;
			SetCursorPos(c_x, c_y);
			event.mouse_cursor.x = c_x;
			event.mouse_cursor.y = c_y;
		} else {
			event.mouse_cursor.x = x;
			event.mouse_cursor.y = y;
		}
	} break;

	case WM_MOUSEWHEEL:
	{
		event.type = Event_Type_MOUSE_WHEEL;
		event.mouse_wheel.horizontal = 0;
		event.mouse_wheel.vertical = GET_WHEEL_DELTA_WPARAM(wparam);
	} break;

	case WM_MOUSEHWHEEL:
	{
		event.type = Event_Type_MOUSE_WHEEL;
		event.mouse_wheel.vertical = 0;
		event.mouse_wheel.horizontal = GET_WHEEL_DELTA_WPARAM(wparam);
	} break;

	case WM_INPUT:
	{
		if (GET_RAWINPUT_CODE_WPARAM(wparam) == RIM_INPUT) {
			UINT size;
			GetRawInputData((HRAWINPUT)lparam, RID_INPUT, 0, &size, sizeof(RAWINPUTHEADER));

			RAWINPUT *input = (RAWINPUT *)tallocate(size);
			if (GetRawInputData((HRAWINPUT)lparam, RID_INPUT, input, &size, sizeof(RAWINPUTHEADER)) == size &&
				input->header.dwType == RIM_TYPEMOUSE) {
				MONITORINFO monitor_info;
				monitor_info.cbSize = sizeof(monitor_info);

				if (GetMonitorInfoW(MonitorFromWindow(wnd, MONITOR_DEFAULTTONEAREST), &monitor_info)) {
					LONG monitor_w = monitor_info.rcMonitor.right - monitor_info.rcMonitor.left;
					LONG monitor_h = monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top;

					LONG xrel = input->data.mouse.lLastX;
					LONG yrel = input->data.mouse.lLastY;

					event.type = Event_Type_MOUSE_AXIS;
					event.mouse_axis.x = (float)xrel / (float)monitor_w;
					event.mouse_axis.y = (float)yrel / (float)monitor_h;
					event.mouse_axis.dx = xrel;
					event.mouse_axis.dy = yrel;
				}
			}
		}
		result = DefWindowProcW(wnd, msg, wparam, lparam);
	} break;

	case WM_SYSKEYDOWN:
	{
		if (wparam == VK_F10) {
			event.type = Event_Type_KEY_DOWN;
			event.key.symbol = Key_F10;
			event.key.state = Key_State_DOWN;
			event.key.repeat = ((lparam & bit(30)) == bit(30));
		}
	} break;

	case WM_SYSKEYUP:
	{
		if (wparam == VK_F10) {
			event.type = Event_Type_KEY_UP;
			event.key.symbol = Key_F10;
			event.key.state = Key_State_UP;
			event.key.repeat = ((lparam & bit(30)) == bit(30));
		}
	} break;

	case WM_KEYDOWN:
	{
		Key key = win32_get_mapped_key(wparam);
		if (key != Key_UNKNOWN) {
			event.type = Event_Type_KEY_DOWN;
			event.key.symbol = key;
			event.key.state = Key_State_DOWN;
			event.key.repeat = ((lparam & bit(30)) == bit(30));
		}
	} break;

	case WM_KEYUP:
	{
		Key key = win32_get_mapped_key(wparam);
		if (key != Key_UNKNOWN) {
			event.type = Event_Type_KEY_UP;
			event.key.symbol = key;
			event.key.state = Key_State_UP;
			event.key.repeat = ((lparam & bit(30)) == bit(30));
		}
	} break;

	case WM_CHAR:
	{
		event.type = Event_Type_TEXT_INPUT;
		wchar_t text = (wchar_t)wparam;
		char    text_utf[4] = {};
		WideCharToMultiByte(CP_UTF8, 0, &text, 1, (char *)text_utf, sizeof(text_utf), 0, 0);
		event.text.codepoint = utf8_to_utf32((u8 *)text_utf);
	} break;

	default:
	{
		result = DefWindowProcW(wnd, msg, wparam, lparam);
	}
	}

	if (event.type != Event_Type_NONE) {
		win32_add_event(event);
	}

	return result;
}

String system_get_command_line() {
	auto cmd_line = GetCommandLineW();
	int      len = WideCharToMultiByte(CP_UTF8, 0, cmd_line, -1, 0, 0, 0, 0);
	String   cmd_line_string;
	cmd_line_string.data = new u8[len];
	cmd_line_string.count = WideCharToMultiByte(CP_UTF8, 0, cmd_line, -1, (char *)cmd_line_string.data, len, 0, 0);
	return cmd_line_string;
}

System_Info system_get_info() {
	System_Info info;
	SYSTEM_INFO winfo;
	GetSystemInfo(&winfo);

	auto arch = winfo.wProcessorArchitecture;
	switch (arch) {
	case PROCESSOR_ARCHITECTURE_AMD64: info.architecture = Processor_Architecture_AMD64; break;
	case PROCESSOR_ARCHITECTURE_ARM: info.architecture = Processor_Architecture_ARM; break;
	case PROCESSOR_ARCHITECTURE_ARM64: info.architecture = Processor_Architecture_ARM64; break;
	case PROCESSOR_ARCHITECTURE_IA64: info.architecture = Processor_Architecture_IA64; break;
	case PROCESSOR_ARCHITECTURE_INTEL: info.architecture = Processor_Architecture_INTEL; break;
	default: info.architecture = Processor_Architecture_UNKNOWN;
	}

	info.page_size = winfo.dwPageSize;
	info.min_application_address = (ptrsize)winfo.lpMinimumApplicationAddress;
	info.max_application_address = (ptrsize)winfo.lpMaximumApplicationAddress;
	info.number_of_processors = winfo.dwNumberOfProcessors;
	info.allocation_granularity = winfo.dwAllocationGranularity;

	return info;
}

String system_get_user_name() {
	DWORD   buffer_len = UNLEN + 1;
	wchar_t buffer[UNLEN + 1];

	if (GetUserNameW(buffer, &buffer_len)) {
		String name;
		name.data = (utf8 *)memory_allocate(buffer_len);
		name.count = WideCharToMultiByte(CP_UTF8, 0, buffer, -1, (char *)name.data, buffer_len, 0, 0) - 1;
		return name;
	}

	return String("", 0);
}

Handle system_create_window(const char *title, s32 width, s32 height, System_Window_Show show, System_Window_State *state) {
	assert(!window_handle); // currently only single window is supported

	Handle result = {};

	HINSTANCE instance = GetModuleHandleW(0);

	WNDCLASSEXW wnd_class = {};
	wnd_class.cbSize = sizeof(wnd_class);
	wnd_class.style = CS_HREDRAW | CS_VREDRAW;
	wnd_class.lpfnWndProc = win32_wnd_proc;
	wnd_class.hInstance = instance;
	wnd_class.lpszClassName = L"Karma";
	wnd_class.hIcon = (HICON)LoadImageW(instance, MAKEINTRESOURCEW(IDI_ICON1), IMAGE_ICON, 0, 0, LR_SHARED);
	wnd_class.hIconSm = (HICON)LoadImageW(instance, MAKEINTRESOURCEW(IDI_ICON1), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	wnd_class.hCursor = (HCURSOR)LoadImageW(0, MAKEINTRESOURCEW(OCR_NORMAL), IMAGE_CURSOR, 0, 0, LR_SHARED);

	if (!RegisterClassExW(&wnd_class)) {
		win32_check_for_error();
		return result;
	}

	DWORD wnd_styles = WS_OVERLAPPEDWINDOW;
	RECT  wrc = {};
	wrc.right = width;
	wrc.bottom = height;
	AdjustWindowRectExForDpi(&wrc, wnd_styles, FALSE, 0, DPI_AWARENESS_PER_MONITOR_AWARE);
	width = wrc.right - wrc.left;
	height = wrc.bottom - wrc.top;

	int      length = MultiByteToWideChar(CP_UTF8, 0, title, -1, 0, 0);
	wchar_t *wtitle = (wchar_t *)tallocate(length * sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8, 0, title, -1, wtitle, length);

	window_handle = CreateWindowExW(0, L"Karma", wtitle, wnd_styles,
		CW_USEDEFAULT, CW_USEDEFAULT,
		width, height, 0, 0, instance, 0);

	if (!window_handle) {
		win32_check_for_error();
		UnregisterClassW(L"Karma", instance);
		return result;
	}

	auto window_size = system_get_client_size();
	window_width = window_size.x;
	window_height = window_size.y;

	RAWINPUTDEVICE device;
	device.usUsagePage = 0x1;
	device.usUsage = 0x2;
	device.dwFlags = 0;
	device.hwndTarget = window_handle;

	if (!RegisterRawInputDevices(&device, 1, sizeof(device))) {
		win32_check_for_error();
		DestroyWindow(window_handle);
		UnregisterClassW(L"Karma", instance);
		return result;
	}

	bool used_state = false;
	if (state) {
		used_state = system_set_window_state(*state);
	}

	int show_cmd = SW_SHOWNORMAL;

	if (!used_state) {
		if (show == System_Window_Show_FULLSCREEN)
			system_fullscreen_state(SYSTEM_ENABLE);
		else if (show == System_Window_Show_MAXIMIZED)
			show_cmd = SW_SHOWMAXIMIZED;
	}

	ShowWindow(window_handle, show_cmd);
	UpdateWindow(window_handle);

	GetWindowPlacement(window_handle, &windowed_placement);

	result.hptr = window_handle;

	return result;
}

void system_request_quit() {
	PostQuitMessage(0);
}

void system_exit_process(int result) {
	ExitProcess(result);
}

bool system_audio(Audio_Callback callback, Audio_Device_Refresh refresh, void *user_data) {
	return windows_audio_client.StartThread(callback, refresh, user_data);
}

u32 system_audio_sample_rate() {
	return windows_audio_client.format.Format.nSamplesPerSec;
}

u32 system_audio_channel_count() {
	return windows_audio_client.format.Format.nChannels;
}

void system_audio_resume() {
	windows_audio_client.client->Start();
}

void system_audio_pause() {
	windows_audio_client.client->Stop();
}

bool system_net_set_socket_nonblocking(Socket sock) {
	DWORD non_blockling = 1;
	if (ioctlsocket((SOCKET)sock, FIONBIO, &non_blockling) != 0) {
		win32_check_for_error();
		return false;
	}
	return true;
}

Socket system_net_open_udp_server(const Ip_Endpoint &address) {
	SOCKET win_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (win_sock == INVALID_SOCKET) {
		win32_check_for_error();
		return SOCKET_INVALID;
	}
	Socket sock = (Socket)win_sock;

	sockaddr_in sock_adrr = {};
	sock_adrr.sin_family = AF_INET;
	sock_adrr.sin_addr.s_addr = htonl(address.address);
	sock_adrr.sin_port = htons(address.port);

	int result = bind(win_sock, (const sockaddr *)&sock_adrr, sizeof(sock_adrr));
	if (result == SOCKET_ERROR) {
		result = WSAGetLastError();
		closesocket(win_sock);
		win32_check_for_error();
		return SOCKET_INVALID;
	}

	return sock;
}

Socket system_net_open_udp_client() {
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock == INVALID_SOCKET) {
		closesocket(sock);
		win32_check_for_error();
		return SOCKET_INVALID;
	}
	Socket result = (Socket)sock;
	return result;
}

s32 system_net_send_to(Socket sock, void *buffer, s32 length, const Ip_Endpoint &address) {
	sockaddr_in sock_addr;
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_addr.s_addr = htonl(address.address);
	sock_addr.sin_port = htons(address.port);
	return sendto((SOCKET)sock, (const char *)buffer, length, 0, (sockaddr *)&sock_addr, sizeof(sockaddr_in));
}

s32 system_net_receive_from(Socket sock, void *packet_buffer, s32 max_packet_size, Ip_Endpoint *address) {
	sockaddr_in from;
	int from_length = sizeof(from);
	s32 bytes = recvfrom((SOCKET)sock, (char *)packet_buffer, max_packet_size, 0, (sockaddr *)&from, &from_length);
	address->address = ntohl(from.sin_addr.s_addr);
	address->port = ntohs(from.sin_port);
	return bytes;
}

void system_net_close_socket(Socket sock) {
	closesocket((SOCKET)sock);
}

static inline Vec2 xinput_axis_deadzone_correction(SHORT x, SHORT y, SHORT deadzone) {
	r32 mag = sqrtf(((r32)x * (r32)x + (r32)y * (r32)y));
	r32 norm_x = mag ? (r32)x / mag : 0;
	r32 norm_y = mag ? (r32)y / mag : 0;
	r32 norm_mag = 0.0f;

	if (mag > deadzone) {
		if (mag > XINPUT_GAMEPAD_THUMB_MAX) mag = XINPUT_GAMEPAD_THUMB_MAX;
		mag -= deadzone;
		norm_mag = mag / (r32)(XINPUT_GAMEPAD_THUMB_MAX - deadzone);
	}

	norm_x *= norm_mag;
	norm_y *= norm_mag;

	return vec2(norm_x, norm_y);
}

static inline r32 xinput_trigger_deadzone_correction(SHORT value, SHORT deadzone) {
	r32 norm = 0.0f;

	if (value > deadzone) {
		if (value > XINPUT_GAMEPAD_TRIGGER_MAX) value = XINPUT_GAMEPAD_TRIGGER_MAX;
		norm = (r32)(value - deadzone);
		norm = norm / (r32)(XINPUT_GAMEPAD_TRIGGER_MAX - deadzone);
	}

	return norm;
}

const Array_View<Event> system_poll_events(int poll_count) {
	MSG msg;
	Event event;

	// NOTE: windows_event_count is not cleared here but cleared during return 
	// because events are polled when WindowUpdate function is called, after window
	// has been created, while system_poll_events function is called later. So if
	// we clear windows_event_count at the start of the function all the added events
	// during windows creation and UpdateWindow call will be cleared without processing
	// That's why we clear windows_event_count during this funtion return

	for (int poll_index = 0; poll_index < poll_count; ++poll_index) {
		while (PeekMessageW(&msg, 0, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				event.type = Event_Type_EXIT;
				win32_add_event(event);
				s64 event_count = (s64)windows_event_count;
				windows_event_count = 0;
				return Array_View<Event>(windows_event_buffer, event_count);
			} else {
				TranslateMessage(&msg);
				DispatchMessageW(&msg);
			}
		}

		XINPUT_STATE new_state;
		for (Controller_Id user_index = 0; user_index < XUSER_MAX_COUNT; ++user_index) {
			auto res = XInputGetState(user_index, &new_state);
			auto old_state = windows_controllers_state + user_index;

			if (res == ERROR_SUCCESS) {
				if (old_state->state.dwPacketNumber != new_state.dwPacketNumber) {
					if (!old_state->connected) {
						Event cevent;
						cevent.type = Event_Type_CONTROLLER_JOIN;
						cevent.controller_device.id = user_index;
						win32_add_event(cevent);
						old_state->connected = true;
					}

					Event controller_event;

					Controller *controller = controllers + user_index;

#define win32_update_controller_button(xbutton, button)													\
						if ((new_state.Gamepad.wButtons & xbutton) != ((old_state->state.Gamepad.wButtons & xbutton))) {	\
							Key_State button_state = (Key_State)((new_state.Gamepad.wButtons & xbutton) == xbutton);						\
							controller_event.type = Event_Type_CONTROLLER_BUTTON;											\
							controller_event.controller_button.symbol = button;												\
							controller_event.controller_button.state = button_state;										\
							controller_event.controller_button.id = user_index;												\
							controller->buttons[button] = button_state;														\
							win32_add_event(controller_event);																\
						}

					if (new_state.Gamepad.wButtons != old_state->state.Gamepad.wButtons) {
						win32_update_controller_button(XINPUT_GAMEPAD_DPAD_UP, Controller_Button_DPAD_UP);
						win32_update_controller_button(XINPUT_GAMEPAD_DPAD_DOWN, Controller_Button_DPAD_DOWN);
						win32_update_controller_button(XINPUT_GAMEPAD_DPAD_LEFT, Controller_Button_DPAD_LEFT);
						win32_update_controller_button(XINPUT_GAMEPAD_DPAD_RIGHT, Controller_Button_DPAD_RIGHT);
						win32_update_controller_button(XINPUT_GAMEPAD_START, Controller_Button_START);
						win32_update_controller_button(XINPUT_GAMEPAD_BACK, Controller_Button_BACK);
						win32_update_controller_button(XINPUT_GAMEPAD_LEFT_THUMB, Controller_Button_LTHUMB);
						win32_update_controller_button(XINPUT_GAMEPAD_RIGHT_THUMB, Controller_Button_RTHUMB);
						win32_update_controller_button(XINPUT_GAMEPAD_LEFT_SHOULDER, Controller_Button_LSHOULDER);
						win32_update_controller_button(XINPUT_GAMEPAD_RIGHT_SHOULDER, Controller_Button_RSHOULDER);
						win32_update_controller_button(XINPUT_GAMEPAD_A, Controller_Button_A);
						win32_update_controller_button(XINPUT_GAMEPAD_B, Controller_Button_B);
						win32_update_controller_button(XINPUT_GAMEPAD_X, Controller_Button_X);
						win32_update_controller_button(XINPUT_GAMEPAD_Y, Controller_Button_Y);
					}

#undef win32_update_controller_button

#define	win32_add_controller_axis_event(in_value, in_axis)				\
								controller_event.type = Event_Type_CONTROLLER_AXIS;			\
								controller_event.controller_axis.value = in_value;			\
								controller_event.controller_axis.symbol = in_axis;			\
								controller_event.controller_axis.id = user_index;			\
								win32_add_event(controller_event)

					r32 value;

					value = xinput_trigger_deadzone_correction(new_state.Gamepad.bLeftTrigger, XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
					if (value != controller->axis[Controller_Axis_LTRIGGER]) {
						win32_add_controller_axis_event(value, Controller_Axis_LTRIGGER);
						controller->axis[Controller_Axis_LTRIGGER] = value;
					}

					value = xinput_trigger_deadzone_correction(new_state.Gamepad.bRightTrigger, XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
					if (value != controller->axis[Controller_Axis_RTRIGGER]) {
						win32_add_controller_axis_event(value, Controller_Axis_RTRIGGER);
						controller->axis[Controller_Axis_RTRIGGER] = value;
					}

					Vec2 left_thumb = xinput_axis_deadzone_correction(new_state.Gamepad.sThumbLX, new_state.Gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
					Vec2 right_thumb = xinput_axis_deadzone_correction(new_state.Gamepad.sThumbRX, new_state.Gamepad.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);

					if (left_thumb.x != controller->axis[Controller_Axis_LTHUMB_X]) {
						win32_add_controller_axis_event(left_thumb.x, Controller_Axis_LTHUMB_X);
						controller->axis[Controller_Axis_LTHUMB_X] = left_thumb.x;
					}
					if (left_thumb.y != controller->axis[Controller_Axis_LTHUMB_Y]) {
						win32_add_controller_axis_event(left_thumb.y, Controller_Axis_LTHUMB_Y);
						controller->axis[Controller_Axis_LTHUMB_Y] = left_thumb.y;
					}

					if (right_thumb.x != controller->axis[Controller_Axis_RTHUMB_X]) {
						win32_add_controller_axis_event(right_thumb.x, Controller_Axis_RTHUMB_X);
						controller->axis[Controller_Axis_RTHUMB_X] = right_thumb.x;
					}
					if (right_thumb.y != controller->axis[Controller_Axis_RTHUMB_Y]) {
						win32_add_controller_axis_event(right_thumb.y, Controller_Axis_RTHUMB_Y);
						controller->axis[Controller_Axis_RTHUMB_Y] = right_thumb.y;
					}

#undef win32_add_controller_axis_event

					old_state->state = new_state;
				}
			} else if (old_state->connected) {
				Event cevent;
				cevent.type = Event_Type_CONTROLLER_LEAVE;
				cevent.controller_device.id = user_index;
				win32_add_event(cevent);

				Controller *controller = controllers + user_index;
				memset(controller, 0, sizeof(*controllers));
				memset(old_state, 0, sizeof(*old_state));
			}
		}
	}

	s64 event_count = (s64)windows_event_count;
	windows_event_count = 0;
	return Array_View<Event>(windows_event_buffer, (s64)event_count);
}

Key_State system_button(Button button) {
	switch (button) {
	case Button_LEFT:
		return (GetAsyncKeyState(VK_LBUTTON) & 0x8000) ? Key_State_DOWN : Key_State_UP;
	case Button_MIDDLE:
		return (GetAsyncKeyState(VK_MBUTTON) & 0x8000) ? Key_State_DOWN : Key_State_UP;
	case Button_RIGHT:
		return (GetAsyncKeyState(VK_RBUTTON) & 0x8000) ? Key_State_DOWN : Key_State_UP;
	case Button_X1:
		return (GetAsyncKeyState(VK_XBUTTON1) & 0x8000) ? Key_State_DOWN : Key_State_UP;
	case Button_X2:
		return (GetAsyncKeyState(VK_XBUTTON2) & 0x8000) ? Key_State_DOWN : Key_State_UP;

		invalid_default_case();
	}
	return Key_State_UP;
}

Key_State system_key(Key key) {
	if (GetKeyState((int)win32_get_mapped_vk_key(key)) & 0x8000) {
		return Key_State_DOWN;
	} else {
		return Key_State_UP;
	}
}

Key_Mods system_get_key_mods() {
	u32 mods = 0;
	if (GetKeyState(VK_LSHIFT) & 0x8000) mods |= Key_Mod_LEFT_SHIFT;
	if (GetKeyState(VK_RSHIFT) & 0x8000) mods |= Key_Mod_RIGHT_SHIFT;
	if (GetKeyState(VK_LCONTROL) & 0x8000) mods |= Key_Mod_LEFT_CTRL;
	if (GetKeyState(VK_RCONTROL) & 0x8000) mods |= Key_Mod_RIGHT_CTRL;
	if (GetKeyState(VK_LMENU) & 0x8000) mods |= Key_Mod_LEFT_ALT;
	if (GetKeyState(VK_RMENU) & 0x8000) mods |= Key_Mod_RIGHT_ALT;
	return mods;
}

Vec2s system_get_cursor_position_vec2s() {
	Vec2s  position;
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(window_handle, &point);
	position.x = point.x;
	position.y = (window_height - point.y);
	return position;
}

Vec2s system_get_cursor_position_y_inverted_vec2s() {
	Vec2s  position;
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(window_handle, &point);
	position.x = point.x;
	position.y = point.y;
	return position;
}

Vec2 system_get_cursor_position() {
	auto pos = system_get_cursor_position_vec2s();
	return vec2((r32)pos.x, (r32)pos.y);
}

Vec2 system_get_cursor_position_y_inverted() {
	auto pos = system_get_cursor_position_y_inverted_vec2s();
	return vec2((r32)pos.x, (r32)pos.y);
}

u32 system_max_controllers() {
	return XUSER_MAX_COUNT;
}

bool system_controller_is_available(Controller_Id id) {
	assert(id < XUSER_MAX_COUNT);
	return windows_controllers_state[id].connected;
}

void system_controller_vibrate(Controller_Id id, r32 left_motor, r32 right_motor) {
	assert(id < XUSER_MAX_COUNT);

	XINPUT_VIBRATION in_vibration;
	in_vibration.wLeftMotorSpeed = (WORD)(left_motor * XINPUT_GAMEPAD_VIBRATION_MAX);
	in_vibration.wRightMotorSpeed = (WORD)(right_motor * XINPUT_GAMEPAD_VIBRATION_MAX);

	XInputSetState(id, &in_vibration);
}

const Controller &system_get_controller_state(Controller_Id id) {
	assert(id < XUSER_MAX_COUNT);
	return controllers[id];
}

Key_State system_controller_button(Controller_Id id, Controller_Button button) {
	assert(id < XUSER_MAX_COUNT);
	return controllers[id].buttons[button];
}

r32 system_controller_axis(Controller_Id id, Controller_Axis axis) {
	assert(id < XUSER_MAX_COUNT);
	return controllers[id].axis[axis];
}

Vec2 system_controller_left_thumb(Controller_Id id) {
	assert(id < XUSER_MAX_COUNT);
	Vec2 result;
	result.x = controllers[id].axis[Controller_Axis_LTHUMB_X];
	result.y = controllers[id].axis[Controller_Axis_LTHUMB_Y];
	return result;
}

Vec2 system_controller_right_thumb(Controller_Id id) {
	assert(id < XUSER_MAX_COUNT);
	Vec2 result;
	result.x = controllers[id].axis[Controller_Axis_RTHUMB_X];
	result.y = controllers[id].axis[Controller_Axis_RTHUMB_Y];
	return result;
}

u64 system_get_counter() {
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);
	return counter.QuadPart;
}

u64 system_get_frequency() {
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	return frequency.QuadPart;
}

void system_fatal_error(const String msg) {
	auto     write_len = msg.count;
	wchar_t *pool = (wchar_t *)memory_allocate((msg.count + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8, 0, (const char *)msg.data, (int)write_len, pool, (int)msg.count + 1);
	pool[write_len] = 0;

	MessageBoxW(window_handle, pool, L"Karma - Fatal Error", MB_TOPMOST | MB_ICONERROR | MB_OK);

	ExitProcess(0);
}

void system_display_critical_message(const String msg) {
	auto     write_len = msg.count;
	wchar_t *pool = (wchar_t *)tallocate((msg.count + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8, 0, (const char *)msg.data, (int)write_len, pool, (int)msg.count + 1);
	pool[write_len] = 0;

	MessageBoxW(window_handle, pool, L"Karma - Critical Error", MB_TOPMOST | MB_ICONWARNING | MB_OK);
}

void *system_heap_allocator_proc(Allocation_Type type, ptrsize size, const void *ptr, void *user_ptr) {
	HANDLE heap = (HANDLE)user_ptr;

	if (type == Allocation_Type_NEW) {
		void *new_ptr = HeapAlloc(heap, 0, size);
		return new_ptr;
	} else if (type == Allocation_Type_RESIZE) {
		void *new_ptr;
		if (ptr)
			new_ptr = HeapReAlloc(heap, 0, (void *)ptr, size);
		else
			new_ptr = HeapAlloc(heap, 0, size);
		return new_ptr;
	} else if (type == Allocation_Type_FREE) {
		if (ptr) {
			HRESULT hr = HeapFree(heap, 0, (void *)ptr);
			if (FAILED(hr)) {
				win32_check_for_error();
			}
		}
	} else {
		invalid_code_path();
	}

	return 0;
}

Allocator system_create_heap_allocator(Heap_Type type, ptrsize initial_size, ptrsize maximum_size) {
	Allocator allocator;
	allocator.proc = system_heap_allocator_proc;
	allocator.data = HeapCreate(type == Heap_Type_NO_SERIALIZE ? HEAP_NO_SERIALIZE : 0, initial_size, maximum_size);
	return allocator;
}

void *system_virtual_alloc(void *address, ptrsize size, Vitual_Memory_Flags flags) {
	DWORD allocation_type = 0;
	if (flags & Virtual_Memory_COMMIT) allocation_type |= MEM_COMMIT;
	if (flags & Virtual_Memory_RESERVE) allocation_type |= MEM_RESERVE;
	if (flags & Virtual_Memory_RESET) allocation_type |= MEM_RESET;
	if (flags & Virtual_Memory_UNDO_RESET) allocation_type |= MEM_RESET_UNDO;
	assert(allocation_type);
	void *ptr = VirtualAlloc(address, size, allocation_type, PAGE_READWRITE);
	win32_check_for_error();
	return ptr;
}

void system_virtual_free(void *ptr, ptrsize size, Vitual_Memory_Flags flags) {
	DWORD free_type = 0;
	if (flags & Virtual_Memory_DECOMMIT) free_type |= MEM_DECOMMIT;
	if (flags & Virtual_Memory_RELEASE) {
		assert(size == 0);
		free_type |= MEM_RELEASE;
	}
	VirtualFree(ptr, size, free_type);
}

void win32_initialize_xinput() {
	const wchar_t *xinput_libs[] = {
		L"xinput1_4.dll", L"xinput9_1_0.dll"
	};

	HMODULE xinput = NULL;
	for (int i = 0; i < static_count(xinput_libs); ++i) {
		xinput = LoadLibraryW(xinput_libs[i]);
		if (xinput) break;
	}

	if (xinput) {
		XInputGetState = (Proc_XInput_Get_State)GetProcAddress(xinput, "XInputGetState");
		XInputSetState = (Proc_XInput_Set_State)GetProcAddress(xinput, "XInputSetState");
		XInputGetCapabilities = (Proc_XInput_Get_Capabilities)GetProcAddress(xinput, "XInputGetCapabilities");
		XInputGetAudioDeviceIds = (Proc_XInput_Get_Audio_Device_Ids)GetProcAddress(xinput, "XInputGetAudioDeviceIds");
		XInputGetBatteryInformation = (Proc_XInput_Get_Battery_Information)GetProcAddress(xinput, "XInputGetBatteryInformation");
		XInputGetKeystroke = (Proc_XInput_Get_Keystroke)GetProcAddress(xinput, "XInputGetKeystroke");
	}

	if (!XInputGetState) XInputGetState = xinput_get_state_stub;
	if (!XInputSetState) XInputSetState = xinput_set_state_stub;
	if (!XInputGetCapabilities) XInputGetCapabilities = xinput_get_capacilities_stub;
	if (!XInputGetAudioDeviceIds) XInputGetAudioDeviceIds = xinput_get_audio_device_ids_stub;
	if (!XInputGetBatteryInformation) XInputGetBatteryInformation = xinput_get_battery_information_stub;
	if (!XInputGetKeystroke) XInputGetKeystroke = xinput_get_keystroke_stub;
}

DWORD WINAPI win_thread_proc(LPVOID param) {
	Thread_Context *thread = (Thread_Context *)((ptrsize)param);
	memcpy(&context, thread, sizeof(context));

	int result = thread->proc();

	CloseHandle(context.handle.hptr);

	return result;
}

bool system_thread_create(const Builder &builder, String name, Allocator temporary_storage_allocator, Thread_Context *thread) {
	thread->proc = builder.entry;
	thread->data = builder.data;
	thread->allocator = builder.allocator;

	if (builder.temporary_buffer_size) {
		void *ptr = memory_allocate(builder.temporary_buffer_size, temporary_storage_allocator);
		if (ptr == 0) {
			win32_check_for_error();
			system_fatal_error("Out of memory: Unable to allocate temporary storage memory!");
		}
		thread->temp_memory = Temporary_Memory(ptr, builder.temporary_buffer_size);
	} else {
		thread->temp_memory = Temporary_Memory(0, 0);
	}

	thread->handle.hptr = CreateThread(0, 0, win_thread_proc, thread, CREATE_SUSPENDED, 0);
	if (thread->handle.hptr != NULL) {
		if (name.count) {
			wchar_t *desc = (wchar_t *)tallocate((name.count + 1) * sizeof(wchar_t));
			MultiByteToWideChar(CP_UTF8, 0, (char *)name.data, (int)name.count, desc, (int)name.count + 1);
			desc[name.count] = 0;
			SetThreadDescription(thread->handle.hptr, desc);
		}
	} else {
		win32_check_for_error();
		return false;
	}

	thread->id = GetThreadId(thread->handle.hptr);

	return true;
}

void system_thread_run(Thread_Context &thread) {
	ResumeThread(thread.handle.hptr);
}

Thread_Wait system_thread_wait(Thread_Context &thread, u32 millisecs) {
	auto result = WaitForSingleObject(thread.handle.hptr, millisecs);
	switch (result) {
	case WAIT_ABANDONED: return Thread_Wait_ABANDONED;
	case WAIT_OBJECT_0: return Thread_Wait_OBJECT_0;
	case WAIT_TIMEOUT: return Thread_Wait_TIMEOUT;
	case WAIT_FAILED: return Thread_Wait_FAILED;
	}
	return Thread_Wait_FAILED;
}

void system_thread_terminate(Thread_Context &thread, int exit_code) {
	TerminateThread(thread.handle.hptr, exit_code);
}

void system_thread_exit(int exit_code) {
	ExitThread(exit_code);
}

void system_thread_sleep(u32 millisecs) {
	Sleep(millisecs);
}

Handle system_create_mutex() {
	HANDLE mutex = CreateMutexA(NULL, FALSE, 0);
	Handle result;
	result.hptr = mutex;
	return result;
}

void system_destory_mutex(Handle handle) {
	CloseHandle(handle.hptr);
}

Wait_Result system_lock_mutex(Handle handle, u32 millisecs) {
	auto result = WaitForSingleObject(handle.hptr, millisecs);
	switch (result) {
	case WAIT_ABANDONED: return Wait_Result_ABANDONED;
	case WAIT_OBJECT_0: return Wait_Result_SIGNALED;
	case WAIT_TIMEOUT: return Wait_Result_TIMEOUT;
	case WAIT_FAILED: return Wait_Result_FAILED;
	}
	return Wait_Result_SUCCESS;
}

void system_unlock_mutex(Handle handle) {
	ReleaseMutex(handle.hptr);
}

Handle system_create_semaphore(u32 initial_count, u32 maximum_count) {
	HANDLE semaphore = CreateSemaphoreW(nullptr, initial_count, maximum_count, nullptr);
	Handle handle;
	handle.hptr = semaphore;
	return handle;
}

void system_destroy_semaphore(Handle handle) {
	CloseHandle(handle.hptr);
}

Wait_Result system_wait_semaphore(Handle handle, u32 millisecs) {
	auto result = WaitForSingleObject(handle.hptr, millisecs);
	switch (result) {
	case WAIT_ABANDONED: return Wait_Result_ABANDONED;
	case WAIT_OBJECT_0: return Wait_Result_SIGNALED;
	case WAIT_TIMEOUT: return Wait_Result_TIMEOUT;
	case WAIT_FAILED: return Wait_Result_FAILED;
	}
	return Wait_Result_SUCCESS;
}

bool system_signal_semaphore(Handle handle, u32 count) {
	return ReleaseSemaphore(handle.hptr, count, nullptr);
}

s32 system_interlocked_increment(s32 volatile *addend) {
	return InterlockedIncrement((long volatile *)addend);
}

s32 system_interlocked_decrement(s32 volatile *addend) {
	return InterlockedDecrement((long volatile *)addend);
}

s32 system_interlocked_compare_exchange(s32 volatile *dest, s32 exchange, s32 comperand) {
	return InterlockedCompareExchange((long volatile *)dest, exchange, comperand);
}

#if defined(SYSTEM_CONSOLE)
int main() {
	LPWSTR cmd_line = GetCommandLineW();
#else
int __stdcall wWinMain(HINSTANCE instance, HINSTANCE prev_instance, LPWSTR cmd_line, int show_cmd) {
#endif

#	if defined(BUILD_DEBUG) || defined(BUILD_DEBUG_FAST)
	AllocConsole();
	SetConsoleOutputCP(CP_UTF8);
#	endif

	DWORD task_index = 0;
	HANDLE task_handle = AvSetMmThreadCharacteristicsW(L"Games", &task_index);
	SetThreadDescription(GetCurrentThread(), L"Main");

	Builder builder = system_builder();

	win32_map_keys();
	win32_initialize_xinput();

	if (builder.flags & Builder_VIDEO) {
		SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

		HRESULT res = CoInitializeEx(0, COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE);
		if (FAILED(res)) {
			win32_check_for_error();
			system_fatal_error("Failed to Initialize COM Objects");
		}
	}

	context.handle.hptr = GetCurrentThread();
	context.id = GetCurrentThreadId();
	context.allocator = builder.allocator;
	context.proc = builder.entry;
	context.data = builder.data;

	void *ptr = nullptr;
	if (builder.temporary_buffer_size) {
		ptr = memory_allocate(builder.temporary_buffer_size);
		if (ptr == 0) {
			win32_check_for_error();
			system_fatal_error("Out of memory: Unable to allocate temporary storage memory!");
		}
	}
	context.temp_memory = Temporary_Memory(ptr, builder.temporary_buffer_size);

	if (builder.flags & Builder_NETWORK) {
		WSADATA WsaData;
		if (WSAStartup(MAKEWORD(2, 2), &WsaData) != NO_ERROR) {
			system_log(LOG_WARNING, "Windows", "Network could not be initialized");
		}
	}

	if (builder.flags & Builder_AUDIO) {
		if (!windows_audio_client.Initialize()) {
			system_log(LOG_WARNING, "Windows", "Audio could not be initialized");
		}
	}

	int result = context.proc();

	if (builder.flags & Builder_NETWORK) {
		WSACleanup();
	}

	if (builder.flags & Builder_AUDIO) {
		windows_audio_client.StopThread();
	}

	if (builder.flags & Builder_VIDEO) {
		CoUninitialize();
	}

	return result;
}

void system_log(int type, const char *title, ANALYSE_PRINTF_FORMAT_STRING(const char *fmt), ...) {
	char    pool[STB_SPRINTF_MIN];
	wchar_t msg[STB_SPRINTF_MIN + 1];

	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (handle != INVALID_HANDLE_VALUE) {
		if (type == LOG_INFO) {
			SetConsoleTextAttribute(handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		} else {
			WORD color = FOREGROUND_RED;
			if (type == LOG_WARNING) color |= FOREGROUND_GREEN;
			SetConsoleTextAttribute(handle, color);
		}
		DWORD written = 0;

		int len = stbsp_snprintf(pool, STB_SPRINTF_MIN, "[%s] - ", title);
		va_list ap;
		va_start(ap, fmt);
		len += stbsp_vsnprintf(pool + len, STB_SPRINTF_MIN, fmt, ap);
		va_end(ap);
		len += stbsp_snprintf(pool + len, STB_SPRINTF_MIN, "\n");

		MultiByteToWideChar(CP_UTF8, 0, pool, -1, msg, sizeof(wchar_t) * (STB_SPRINTF_MIN));
		WriteConsoleW(handle, msg, len, &written, 0);
		OutputDebugStringW(msg);
	}
}
