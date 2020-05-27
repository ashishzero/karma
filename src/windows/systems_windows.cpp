#define OEMRESOURCE
#include "../length_string.h"
#include "../systems.h"
#include "../stb_sprintf.h"
#include "../debug_service.h"
#include "resource.h"
#include <ShlObj_core.h>
#include <Windows.h>
#include <shlwapi.h>
#include <strsafe.h>
#include <windowsx.h>
#include <winreg.h>
#include <Lmcons.h> // for UNLEN
#include <avrt.h>

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "Advapi32.lib")
#pragma comment(lib, "Ole32.lib")
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "Avrt.lib")

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

typedef DWORD (*Proc_XInput_Get_State)(DWORD dwUserIndex, XINPUT_STATE *pState);
typedef DWORD (*Proc_XInput_Set_State)(DWORD dwUserIndex, XINPUT_VIBRATION *pVibration);
typedef DWORD (*Proc_XInput_Get_Capabilities)(DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES *pCapabilities);
typedef DWORD (*Proc_XInput_Get_Audio_Device_Ids)(DWORD dwUserIndex, LPWSTR pRenderDeviceId, UINT *pRenderCount, LPWSTR pCaptureDeviceId, UINT *pCaptureCount);
typedef DWORD (*Proc_XInput_Get_Battery_Information)(DWORD dwUserIndex, BYTE devType, XINPUT_BATTERY_INFORMATION *pBatteryInformation);
typedef DWORD (*Proc_XInput_Get_Keystroke)(DWORD dwUserIndex, DWORD dwReserved, PXINPUT_KEYSTROKE pKeystroke);

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
	DWORD packet_number;
	bool  connected;
};

//
// XInput Header File
//

//
// Windows Audio Client
//


//
//
//

thread_local Thread_Context context;

static HWND            window_handle;
static WINDOWPLACEMENT windowed_placement;

static constexpr int WINDOWS_MAX_KEYS = 256;
static Key           windows_key_map[WINDOWS_MAX_KEYS]; // This number should be enough

static constexpr int WINDOWS_MAX_EVENTS = 65535; // this number should be enough
static Event         windows_event_queue[WINDOWS_MAX_EVENTS];
static u32           windows_event_queue_push_index;
static u32           windows_event_pop_index;
static u32           windows_event_count;

static int window_width;
static int window_height;

static Win32_Controller windows_controllers_state[XUSER_MAX_COUNT];
static Controller       controllers[XUSER_MAX_COUNT];

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

Array_View<u8> system_read_entire_file(const String path) {
	Array_View<u8> result = {};

	auto     length = path.count + 1;
	wchar_t *wpath  = (wchar_t *)tallocate(length * sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8, 0, (char *)path.data, (int)path.count, wpath, (int)length);
	wpath[path.count] = 0;

	auto handle = CreateFileW(wpath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	win32_check_for_error();

	if (handle != INVALID_HANDLE_VALUE) {
		defer {
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
		u8 *  end_ptr    = (u8 *)buffer + file_size.QuadPart;
		for (u8 *read_ptr = (u8 *)buffer;
			 read_ptr < end_ptr;
			 read_ptr += read_bytes) {
			// resetting *read_bytes* because docs says to (for next loop)
			read_bytes = 0;
			BOOL res   = ReadFile(handle, read_ptr, read_size, &read_bytes, 0);
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

		result.data  = (u8 *)buffer;
		result.count = file_size.QuadPart;
	}

	return result;
}

bool system_write_entire_file(const String path, Array_View<u8> content) {
	auto     length = path.count + 1;
	wchar_t *wpath  = (wchar_t *)tallocate(length * sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8, 0, (char *)path.data, (int)path.count, wpath, (int)length);
	wpath[path.count] = 0;

	auto handle = CreateFileW(wpath, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (handle != INVALID_HANDLE_VALUE) {
		defer {
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
		u8 *  end_ptr       = content.data + content.count;
		for (u8 *write_ptr = content.data;
			 write_ptr < end_ptr;
			 write_ptr += written_bytes) {
			DWORD left_size = (DWORD)(content.count - (s64)written_bytes);
			if (write_size > left_size) {
				write_size = left_size;
			}

			// resetting *written_bytes* because docs says to (for next loop)
			written_bytes = 0;
			BOOL res      = WriteFile(handle, write_ptr, write_size, &written_bytes, 0);
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
	move_distance.QuadPart      = position;
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
	u8 *  end_ptr    = buffer + size;
	for (u8 *read_ptr = buffer;
		 read_ptr < end_ptr;
		 read_ptr += read_bytes) {
		// resetting *read_bytes* because docs says to (for next loop)
		read_bytes = 0;
		BOOL res   = ReadFile(handle.hptr, read_ptr, read_size, &read_bytes, 0);
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
	u8 *  end_ptr       = content + size;
	for (u8 *write_ptr = content;
		 write_ptr < end_ptr;
		 write_ptr += written_bytes) {
		DWORD left_size = (DWORD)(size - (s64)written_bytes);
		if (write_size > left_size) {
			write_size = left_size;
		}

		// resetting *written_bytes* because docs says to (for next loop)
		written_bytes = 0;
		BOOL res      = WriteFile(handle.hptr, write_ptr, write_size, &written_bytes, 0);
		if (res) {
			if (written_bytes == size) break;
		} else {
			return false;
		}
	}

	return true;
}

bool system_open_file(const String path, File_Operation options, System_File *file) {
	DWORD access   = 0;
	DWORD creation = 0;

	*file = {};
	if (options == File_Operation_READ) {
		access     = GENERIC_READ;
		creation   = OPEN_EXISTING;
		file->read = isystem_read_file;
	} else if (options == File_Operation_APPEND) {
		access      = GENERIC_WRITE;
		creation    = OPEN_EXISTING;
		file->write = isystem_write_file;
	} else if (options == File_Operation_READ_WRITE) {
		access      = GENERIC_READ | GENERIC_WRITE;
		creation    = OPEN_EXISTING;
		file->read  = isystem_read_file;
		file->write = isystem_write_file;
	} else {
		access      = GENERIC_WRITE;
		creation    = CREATE_ALWAYS;
		file->write = isystem_write_file;
	}

	auto     length = path.count + 1;
	wchar_t *wpath  = (wchar_t *)tallocate(length * sizeof(wchar_t));
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
	file->get_cursor  = isystem_get_file_pointer;
	file->set_cursor  = isystem_set_file_pointer;

	return true;
}

void system_close_file(System_File *file) {
	CloseHandle(file->handle.hptr);
	*file = {};
}

Array_View<System_Find_File_Info> system_find_files(const String search, bool recursive) {
	Array<System_Find_File_Info> items;

	Array<String> find_directories;
	defer {
		for (s64 index = 0; index < find_directories.count; ++index) {
			auto &it = find_directories[index];
			if (index) memory_free(it.data);
		}
		array_free(&find_directories);
	};

	array_add(&find_directories, search);

	String               search_param  = {};
	String_Search_Result search_result = string_isearch_reverse(search, "/");
	if (search_result.found) {
		search_param = string_substring(search, search_result.start_index, search.count - search_result.start_index);
	}

	for (s64 index = 0; index < find_directories.count; ++index) {
		u8 *tp = get_temporary_allocator_point();

		int      length         = (int)find_directories[index].count + 1;
		wchar_t *find_directory = (wchar_t *)tallocate(length * sizeof(wchar_t));
		MultiByteToWideChar(CP_UTF8, 0, (const char *)find_directories[index].data, (int)find_directories[index].count, find_directory, length);
		find_directory[length] = 0;

		WIN32_FIND_DATAW data;
		HANDLE           handle = FindFirstFileW(find_directory, &data);

		set_temporary_allocator_point(tp);

		if (handle != INVALID_HANDLE_VALUE) {
			String               directory     = {};
			String_Search_Result search_result = string_isearch_reverse(find_directories[index], "/");
			if (search_result.found) {
				directory = string_substring(find_directories[index], 0, search_result.start_index + 1);
			}

			System_Find_File_Info file = {};
			do {
				if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					if (wcscmp(data.cFileName, L".") != 0 && wcscmp(data.cFileName, L"..") != 0 && recursive) {
						String new_dir;

						size_t file_len = wcslen(data.cFileName);
						new_dir.count   = file_len + directory.count + search_param.count;
						new_dir.data    = (u8 *)memory_allocate(new_dir.count);

						memcpy(new_dir.data, directory.data, directory.count);
						WideCharToMultiByte(CP_UTF8, 0, data.cFileName, -1, (char *)new_dir.data + directory.count, (int)(new_dir.count - directory.count), 0, 0);
						memcpy(new_dir.data + directory.count + file_len, search_param.data, search_param.count);

						array_add(&find_directories, new_dir);
					}
				} else {
					file.size = ((u64)data.nFileSizeHigh * ((u64)MAXDWORD + 1)) + (u64)data.nFileSizeLow;

					file.path.count = (ptrsize)WideCharToMultiByte(CP_UTF8, 0, data.cFileName, -1, 0, 0, 0, 0) + directory.count - 1;
					file.path.data  = (u8 *)memory_allocate(file.path.count);
					memcpy(file.path.data, directory.data, directory.count);
					WideCharToMultiByte(CP_UTF8, 0, data.cFileName, -1, (char *)file.path.data + directory.count, (int)(file.path.count - directory.count), 0, 0);

					file.name = String(file.path.data + directory.count, file.path.count - directory.count);

					String_Search_Result result = string_isearch(file.name, ".");
					if (result.found) {
						file.extension = string_substring(file.name, result.start_index + 1, file.name.count - result.start_index - 1);
						file.name.count -= file.extension.count + 1;
					} else {
						file.extension = {};
					}

					array_add(&items, file);
				}

			} while (FindNextFileW(handle, &data));

			FindClose(handle);
		} else {
			win32_check_for_error();
		}
	}

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
	LONG_PTR style   = GetWindowLongPtrW(window_handle, GWL_STYLE);
	bool     is_full = !(style & WS_OVERLAPPEDWINDOW);

	if (toggle == SYSTEM_TOGGLE) {
		if (is_full)
			toggle = SYSTEM_DISABLE;
		else
			toggle = SYSTEM_ENABLE;
	}

	windowed_placement.length = sizeof(WINDOWPLACEMENT);

	switch (toggle) {
		case SYSTEM_ENABLE: {
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

		case SYSTEM_DISABLE: {
			if (is_full) {
				SetWindowLongPtrW(window_handle, GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
				SetWindowPlacement(window_handle, &windowed_placement);
				SetWindowPos(window_handle, 0, 0, 0, 0, 0,
							 SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
								 SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
			}
			return SYSTEM_DISABLE;
		} break;
		case SYSTEM_QUERY: {
			return (is_full ? SYSTEM_ENABLE : SYSTEM_DISABLE);
		} break;

			invalid_default_case();
	}

	return SYSTEM_QUERY; // i don't know what to return here so...
}

int system_maximize_state(int toggle) {
	WINDOWPLACEMENT wnd_placement = {};
	wnd_placement.length          = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(window_handle, &wnd_placement);
	bool is_maximized = (wnd_placement.showCmd == SW_SHOWMAXIMIZED);

	if (toggle == SYSTEM_TOGGLE) {
		if (is_maximized)
			toggle = SYSTEM_DISABLE;
		else
			toggle = SYSTEM_ENABLE;
	}

	switch (toggle) {
		case SYSTEM_ENABLE: {
			if (!is_maximized) {
				wnd_placement.showCmd = SW_SHOWMAXIMIZED;
				SetWindowPlacement(window_handle, &wnd_placement);
			}
			return SYSTEM_ENABLE;
		} break;

		case SYSTEM_DISABLE: {
			if (is_maximized) {
				wnd_placement.showCmd = SW_SHOWNORMAL;
				SetWindowPlacement(window_handle, &wnd_placement);
			}
			return SYSTEM_DISABLE;
		} break;
		case SYSTEM_QUERY: {
			return (is_maximized ? SYSTEM_ENABLE : SYSTEM_DISABLE);
		} break;

			invalid_default_case();
	}

	return SYSTEM_QUERY; // i don't know what to return here so...
}

System_Window_State system_get_window_state() {
	System_Window_State state = {};

	WINDOWPLACEMENT *placement = (WINDOWPLACEMENT *)memory_allocate(sizeof(*placement));
	placement->length          = sizeof(*placement);
	if (GetWindowPlacement(window_handle, placement)) {
		state.handle = placement;
		state.size   = sizeof(*placement);
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
			HANDLE   buffer  = GetClipboardData(CF_UNICODETEXT);
			wchar_t *wstring = (wchar_t *)GlobalLock(buffer);
			char *   string  = 0;
			int      length  = 0;
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
	return String {};
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

	windows_key_map[VK_RETURN]  = Key_RETURN;
	windows_key_map[VK_ESCAPE]  = Key_ESCAPE;
	windows_key_map[VK_BACK]    = Key_BACKSPACE;
	windows_key_map[VK_TAB]     = Key_TAB;
	windows_key_map[VK_SPACE]   = Key_SPACE;
	windows_key_map[VK_SHIFT]   = Key_SHIFT;
	windows_key_map[VK_CONTROL] = Key_CTRL;

	windows_key_map[VK_F1]  = Key_F1;
	windows_key_map[VK_F2]  = Key_F2;
	windows_key_map[VK_F3]  = Key_F3;
	windows_key_map[VK_F4]  = Key_F4;
	windows_key_map[VK_F5]  = Key_F5;
	windows_key_map[VK_F6]  = Key_F6;
	windows_key_map[VK_F7]  = Key_F7;
	windows_key_map[VK_F8]  = Key_F8;
	windows_key_map[VK_F9]  = Key_F9;
	windows_key_map[VK_F10] = Key_F10;
	windows_key_map[VK_F11] = Key_F11;
	windows_key_map[VK_F12] = Key_F12;

	windows_key_map[VK_SNAPSHOT] = Key_PRINT_SCREEN;
	windows_key_map[VK_INSERT]   = Key_INSERT;
	windows_key_map[VK_HOME]     = Key_HOME;
	windows_key_map[VK_PRIOR]    = Key_PAGE_UP;
	windows_key_map[VK_NEXT]     = Key_PAGE_DOWN;
	windows_key_map[VK_DELETE]   = Key_DELETE;
	windows_key_map[VK_END]      = Key_END;
	windows_key_map[VK_RIGHT]    = Key_RIGHT;
	windows_key_map[VK_LEFT]     = Key_LEFT;
	windows_key_map[VK_DOWN]     = Key_DOWN;
	windows_key_map[VK_UP]       = Key_UP;
	windows_key_map[VK_DIVIDE]   = Key_DIVIDE;
	windows_key_map[VK_MULTIPLY] = Key_MULTIPLY;
	windows_key_map[VK_ADD]      = Key_PLUS;
	windows_key_map[VK_SUBTRACT] = Key_MINUS;
	windows_key_map[VK_DECIMAL]  = Key_PERIOD;
	windows_key_map[VK_OEM_3]    = Key_BACK_TICK;

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
}

Key win32_get_mapped_key(WPARAM wparam) {
	if (wparam >= WINDOWS_MAX_KEYS) return Key_UNKNOWN;
	return windows_key_map[wparam];
}

void win32_mouse_button_event(Mouse_Button_Event *event, WPARAM wparam, LPARAM lparam) {
	Vec2s wr        = system_get_client_size();
	int   x         = GET_X_LPARAM(lparam);
	int   y         = GET_Y_LPARAM(lparam);
	event->position = vec2s(x, wr.y - y); // inverting y
}

void win32_push_event(Event event) {
	windows_event_queue[windows_event_queue_push_index] = event;
	windows_event_queue_push_index                      = (windows_event_queue_push_index + 1) % WINDOWS_MAX_EVENTS;
	//InterlockedExchangeAdd(&windows_event_count, 1);
	windows_event_count += 1;
}

Event win32_pop_event() {
	//InterlockedExchangeSubtract(&windows_event_count, 1);
	windows_event_count -= 1;
	Event event             = windows_event_queue[windows_event_pop_index];
	windows_event_pop_index = (windows_event_pop_index + 1) % WINDOWS_MAX_EVENTS;
	return event;
}

static LRESULT CALLBACK win32_wnd_proc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	Event   event  = {};
	LRESULT result = 0;

	switch (msg) {
		case WM_ACTIVATE: {
			int low = LOWORD(wparam);

			if (low == WA_ACTIVE || low == WA_CLICKACTIVE)
				event.type = Event_Type_WINDOW_ACTIVE;
			else
				event.type = Event_Type_WINDOW_INACTIVE;

			result = DefWindowProcW(wnd, msg, wparam, lparam);
		} break;

		case WM_SIZE: {
			int x = LOWORD(lparam);
			int y = HIWORD(lparam);

			window_width  = x;
			window_height = y;

			// need to re-set clip rectangle after resize
			RECT rc;
			GetClientRect(window_handle, &rc);

			CURSORINFO info;
			info.cbSize = sizeof(info);
			if (GetCursorInfo(&info) && (info.flags == 0)) {
				POINT pt  = { rc.left, rc.top };
				POINT pt2 = { rc.right, rc.bottom };
				ClientToScreen(window_handle, &pt);
				ClientToScreen(window_handle, &pt2);
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

			event.type             = Event_Type_WINDOW_RESIZE;
			event.window.dimension = vec2s(x, y);
		} break;

		case WM_DPICHANGED: {
			RECT *const suggested_rect = (RECT *)lparam;
			auto        left           = suggested_rect->left;
			auto        top            = suggested_rect->top;
			auto        width          = suggested_rect->right - suggested_rect->left;
			auto        height         = suggested_rect->bottom - suggested_rect->top;
			SetWindowPos(wnd, 0, left, top, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
		} break;

		case WM_CLOSE: {
			PostQuitMessage(0);
		} break;

		case WM_DESTROY: {
		} break;

		case WM_LBUTTONDOWN: {
			event.type                = Event_Type_MOUSE_BUTTON_DOWN;
			event.mouse_button.symbol = Button_LEFT;
			event.mouse_button.state  = State_DOWN;
			win32_mouse_button_event(&event.mouse_button, wparam, lparam);
		} break;

		case WM_LBUTTONUP: {
			event.type                = Event_Type_MOUSE_BUTTON_UP;
			event.mouse_button.symbol = Button_LEFT;
			event.mouse_button.state  = State_UP;
			win32_mouse_button_event(&event.mouse_button, wparam, lparam);
		} break;

		case WM_MBUTTONDOWN: {
			event.type                = Event_Type_MOUSE_BUTTON_DOWN;
			event.mouse_button.symbol = Button_MIDDLE;
			event.mouse_button.state  = State_DOWN;
			win32_mouse_button_event(&event.mouse_button, wparam, lparam);
		} break;

		case WM_MBUTTONUP: {
			event.type                = Event_Type_MOUSE_BUTTON_UP;
			event.mouse_button.symbol = Button_MIDDLE;
			event.mouse_button.state  = State_UP;
			win32_mouse_button_event(&event.mouse_button, wparam, lparam);
		} break;

		case WM_RBUTTONDOWN: {
			event.type                = Event_Type_MOUSE_BUTTON_DOWN;
			event.mouse_button.symbol = Button_RIGHT;
			event.mouse_button.state  = State_DOWN;
			win32_mouse_button_event(&event.mouse_button, wparam, lparam);
		} break;

		case WM_RBUTTONUP: {
			event.type                = Event_Type_MOUSE_BUTTON_UP;
			event.mouse_button.symbol = Button_RIGHT;
			event.mouse_button.state  = State_UP;
			win32_mouse_button_event(&event.mouse_button, wparam, lparam);
		} break;

		case WM_XBUTTONDOWN: {
			event.type                = Event_Type_MOUSE_BUTTON_DOWN;
			event.mouse_button.symbol = ((wparam & MK_XBUTTON1) == MK_XBUTTON1) ? Button_X1 : Button_X2;
			event.mouse_button.state  = State_DOWN;
			win32_mouse_button_event(&event.mouse_button, wparam, lparam);
		} break;

		case WM_XBUTTONUP: {
			event.type                = Event_Type_MOUSE_BUTTON_UP;
			event.mouse_button.symbol = ((wparam & MK_XBUTTON1) == MK_XBUTTON1) ? Button_X1 : Button_X2;
			event.mouse_button.state  = State_UP;
			win32_mouse_button_event(&event.mouse_button, wparam, lparam);
		} break;

		case WM_MOUSEMOVE: {
			RECT rc;
			GetClientRect(window_handle, &rc);
			Vec2s wr = vec2s(rc.right - rc.left, rc.bottom - rc.top);

			event.type = Event_Type_MOUSE_CURSOR;
			int x      = GET_X_LPARAM(lparam);
			int ny     = GET_Y_LPARAM(lparam);
			int y      = wr.y - ny; // inverting y

			CURSORINFO info;
			info.cbSize = sizeof(info);
			if (GetCursorInfo(&info) && (info.flags == 0)) {
				POINT pt  = { rc.left, rc.top };
				POINT pt2 = { rc.right, rc.bottom };
				ClientToScreen(window_handle, &pt);
				ClientToScreen(window_handle, &pt2);
				SetRect(&rc, pt.x, pt.y, pt2.x, pt2.y);
				ClipCursor(&rc);
				int c_x = rc.left + (rc.right - rc.left) / 2;
				int c_y = rc.top + (rc.bottom - rc.top) / 2;
				SetCursorPos(c_x, c_y);
			}
		} break;

		case WM_MOUSEWHEEL: {
			event.type                   = Event_Type_MOUSE_WHEEL;
			event.mouse_wheel.horizontal = 0;
			event.mouse_wheel.vertical   = GET_WHEEL_DELTA_WPARAM(wparam);
		} break;

		case WM_MOUSEHWHEEL: {
			event.type                   = Event_Type_MOUSE_WHEEL;
			event.mouse_wheel.vertical   = 0;
			event.mouse_wheel.horizontal = GET_WHEEL_DELTA_WPARAM(wparam);
		} break;

		case WM_INPUT: {
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
						LONG yrel = -input->data.mouse.lLastY;

						event.type         = Event_Type_MOUSE_AXIS;
						event.mouse_axis.x = (float)xrel / (float)monitor_w;
						event.mouse_axis.y = (float)yrel / (float)monitor_h;
					}
				}
			}
			result = DefWindowProcW(wnd, msg, wparam, lparam);
		} break;

		case WM_SYSKEYDOWN: {
			if (wparam == VK_F10) {
				event.type       = Event_Type_KEY_DOWN;
				event.key.symbol = Key_F10;
				event.key.state  = State_DOWN;
				event.key.repeat = ((lparam & bit(30)) == bit(30));
			}
		} break;

		case WM_SYSKEYUP: {
			if (wparam == VK_F10) {
				event.type       = Event_Type_KEY_UP;
				event.key.symbol = Key_F10;
				event.key.state  = State_UP;
				event.key.repeat = ((lparam & bit(30)) == bit(30));
			}
		} break;

		case WM_KEYDOWN: {
			Key key = win32_get_mapped_key(wparam);
			if (key != Key_UNKNOWN) {
				event.type       = Event_Type_KEY_DOWN;
				event.key.symbol = key;
				event.key.state  = State_DOWN;
				event.key.repeat = ((lparam & bit(30)) == bit(30));
			}
		} break;

		case WM_KEYUP: {
			Key key = win32_get_mapped_key(wparam);
			if (key != Key_UNKNOWN) {
				event.type       = Event_Type_KEY_UP;
				event.key.symbol = key;
				event.key.state  = State_UP;
				event.key.repeat = ((lparam & bit(30)) == bit(30));
			}
		} break;

		case WM_CHAR: {
			event.type          = Event_Type_TEXT_INPUT;
			wchar_t text        = (wchar_t)wparam;
			char    text_utf[4] = {};
			WideCharToMultiByte(CP_UTF8, 0, &text, 1, (char *)text_utf, sizeof(text_utf), 0, 0);
			event.text.codepoint = utf8_to_utf32((u8 *)text_utf);
		} break;

		default: {
			result = DefWindowProcW(wnd, msg, wparam, lparam);
		}
	}

	if (event.type != Event_Type_NONE) {
		win32_push_event(event);
	}

	return result;
}

String system_get_command_line() {
	assert(context.proc == system_main);
	return *((String *)context.data);
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

	info.page_size               = winfo.dwPageSize;
	info.min_application_address = (ptrsize)winfo.lpMinimumApplicationAddress;
	info.max_application_address = (ptrsize)winfo.lpMaximumApplicationAddress;
	info.number_of_processors    = winfo.dwNumberOfProcessors;
	info.allocation_granularity  = winfo.dwAllocationGranularity;

	return info;
}

String system_get_user_name() {
	DWORD   buffer_len = UNLEN + 1;
	wchar_t buffer[UNLEN + 1];

	if (GetUserNameW(buffer, &buffer_len)) {
		String name;
		name.data  = (utf8 *)memory_allocate(buffer_len);
		name.count = WideCharToMultiByte(CP_UTF8, 0, buffer, -1, (char *)name.data, buffer_len, 0, 0);
		return name;
	}

	return String("", 0);
}

Handle system_create_window(const char *title, s32 width, s32 height, System_Window_Show show, System_Window_State *state) {
	assert(!window_handle); // currently only single window is supported

	Handle result = {};

	HINSTANCE instance = GetModuleHandleW(0);

	WNDCLASSEXW wnd_class   = {};
	wnd_class.cbSize        = sizeof(wnd_class);
	wnd_class.style         = CS_HREDRAW | CS_VREDRAW;
	wnd_class.lpfnWndProc   = win32_wnd_proc;
	wnd_class.hInstance     = instance;
	wnd_class.lpszClassName = L"Karma";
	wnd_class.hIcon         = (HICON)LoadImageW(instance, MAKEINTRESOURCEW(IDI_ICON1), IMAGE_ICON, 0, 0, LR_SHARED);
	wnd_class.hIconSm       = (HICON)LoadImageW(instance, MAKEINTRESOURCEW(IDI_ICON1), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	wnd_class.hCursor       = (HCURSOR)LoadImageW(0, MAKEINTRESOURCEW(OCR_NORMAL), IMAGE_CURSOR, 0, 0, LR_SHARED);
	RegisterClassExW(&wnd_class);

	DWORD wnd_styles = WS_OVERLAPPEDWINDOW;
	RECT  wrc        = {};
	wrc.right        = width;
	wrc.bottom       = height;
	AdjustWindowRectExForDpi(&wrc, wnd_styles, FALSE, 0, DPI_AWARENESS_PER_MONITOR_AWARE);
	width  = wrc.right - wrc.left;
	height = wrc.bottom - wrc.top;

	int      length = MultiByteToWideChar(CP_UTF8, 0, title, -1, 0, 0);
	wchar_t *wtitle = (wchar_t *)tallocate(length);
	MultiByteToWideChar(CP_UTF8, 0, title, -1, wtitle, length);

	window_handle = CreateWindowExW(0, L"Karma", wtitle, wnd_styles,
									CW_USEDEFAULT, CW_USEDEFAULT,
									width, height, 0, 0, instance, 0);

	if (!window_handle) {
		win32_check_for_error();
		UnregisterClassW(L"Karma", instance);
		return result;
	}

	RAWINPUTDEVICE device;
	device.usUsagePage = 0x1;
	device.usUsage     = 0x2;
	device.dwFlags     = 0;
	device.hwndTarget  = window_handle;

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

static inline Vec2 xinput_axis_deadzone_correction(SHORT x, SHORT y, SHORT deadzone) {
	r32 mag      = sqrtf((r32)(x * x + y * y));
	r32 norm_x   = mag ? (r32)x / mag : 0;
	r32 norm_y   = mag ? (r32)y / mag : 0;
	r32 norm_mag = 0.0f;

	if (mag > deadzone) {
		if (mag > XINPUT_GAMEPAD_THUMB_MAX) mag = XINPUT_GAMEPAD_THUMB_MAX;
		mag -= deadzone;
		norm_mag = mag / (r32)(XINPUT_GAMEPAD_THUMB_MAX - deadzone);
	}

	return vec2(norm_x * norm_mag, norm_y * norm_mag);
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

bool system_poll_events(Event *event) {
	MSG msg;

	karma_timed_block_begin(WindowEvents);
	while (PeekMessageW(&msg, 0, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			event->type = Event_Type_EXIT;
			return true;
		} else {
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}
	karma_timed_block_end(WindowEvents);

	karma_timed_block_begin(XInputEvents);
	XINPUT_STATE state;
	for (int user_index = 0; user_index < XUSER_MAX_COUNT; ++user_index) {
		auto res = XInputGetState(user_index, &state);

		if (res == ERROR_SUCCESS) {
			if (windows_controllers_state[user_index].packet_number != state.dwPacketNumber) {
				if (!windows_controllers_state[user_index].connected) {
					Event cevent;
					cevent.type             = Event_Type_CONTROLLER_JOIN;
					cevent.controller.index = user_index;
					win32_push_event(cevent);
					windows_controllers_state[user_index].connected = true;
				}

				Controller *controller = controllers + user_index;
				memset(controller->buttons, State_UP, sizeof(controllers->buttons));

				if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) controller->buttons[Gamepad_DPAD_UP] = State_DOWN;
				if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) controller->buttons[Gamepad_DPAD_DOWN] = State_DOWN;
				if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) controller->buttons[Gamepad_DPAD_LEFT] = State_DOWN;
				if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) controller->buttons[Gamepad_DPAD_RIGHT] = State_DOWN;
				if (state.Gamepad.wButtons & XINPUT_GAMEPAD_START) controller->buttons[Gamepad_START] = State_DOWN;
				if (state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) controller->buttons[Gamepad_BACK] = State_DOWN;
				if (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) controller->buttons[Gamepad_LTHUMB] = State_DOWN;
				if (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) controller->buttons[Gamepad_RTHUMB] = State_DOWN;
				if (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) controller->buttons[Gamepad_LSHOULDER] = State_DOWN;
				if (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) controller->buttons[Gamepad_RSHOULDER] = State_DOWN;
				if (state.Gamepad.wButtons & XINPUT_GAMEPAD_A) controller->buttons[Gamepad_A] = State_DOWN;
				if (state.Gamepad.wButtons & XINPUT_GAMEPAD_B) controller->buttons[Gamepad_B] = State_DOWN;
				if (state.Gamepad.wButtons & XINPUT_GAMEPAD_X) controller->buttons[Gamepad_X] = State_DOWN;
				if (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) controller->buttons[Gamepad_Y] = State_DOWN;

				controller->left_trigger  = xinput_trigger_deadzone_correction(state.Gamepad.bLeftTrigger, XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
				controller->right_trigger = xinput_trigger_deadzone_correction(state.Gamepad.bRightTrigger, XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
				controller->left_thumb    = xinput_axis_deadzone_correction(state.Gamepad.sThumbLX, state.Gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
				controller->right_thumb   = xinput_axis_deadzone_correction(state.Gamepad.sThumbRX, state.Gamepad.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
			}
		} else if (windows_controllers_state[user_index].connected) {
			Event cevent;
			cevent.type             = Event_Type_CONTROLLER_LEAVE;
			cevent.controller.index = user_index;
			win32_push_event(cevent);
			windows_controllers_state[user_index].connected = false;

			Controller *controller = controllers + user_index;
			memset(controller, 0, sizeof(*controllers));
		}
	}
	karma_timed_block_end(XInputEvents);

	if (windows_event_count) {
		*event = win32_pop_event();
		return true;
	}

	return false;
}

State system_button(Button button) {
	switch (button) {
		case Button_LEFT:
			return (GetAsyncKeyState(VK_LBUTTON) & 0x8000) ? State_DOWN : State_UP;
		case Button_MIDDLE:
			return (GetAsyncKeyState(VK_MBUTTON) & 0x8000) ? State_DOWN : State_UP;
		case Button_RIGHT:
			return (GetAsyncKeyState(VK_RBUTTON) & 0x8000) ? State_DOWN : State_UP;
		case Button_X1:
			return (GetAsyncKeyState(VK_XBUTTON1) & 0x8000) ? State_DOWN : State_UP;
		case Button_X2:
			return (GetAsyncKeyState(VK_XBUTTON2) & 0x8000) ? State_DOWN : State_UP;

			invalid_default_case();
	}
	return State_UP;
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

Vec2 system_get_cursor_position() {
	Vec2  position;
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(window_handle, &point);
	position.x = (r32)point.x;
	position.y = (r32)(window_height - point.y);
	return position;
}

Vec2 system_get_cursor_position_y_inverted() {
	Vec2  position;
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(window_handle, &point);
	position.x = (r32)point.x;
	position.y = (r32)point.y;
	return position;
}

u32 system_max_controllers() {
	return XUSER_MAX_COUNT;
}

bool system_controller_is_available(u32 index) {
	assert(index < XUSER_MAX_COUNT);
	return windows_controllers_state[index].connected;
}

void system_controller_vibrate(u32 index, r32 left_motor, r32 right_motor) {
	assert(index < XUSER_MAX_COUNT);

	XINPUT_VIBRATION in_vibration;
	in_vibration.wLeftMotorSpeed  = (WORD)(left_motor * XINPUT_GAMEPAD_VIBRATION_MAX);
	in_vibration.wRightMotorSpeed = (WORD)(right_motor * XINPUT_GAMEPAD_VIBRATION_MAX);

	XInputSetState(index, &in_vibration);
}

const Controller &system_get_controller_state(u32 index) {
	assert(index < XUSER_MAX_COUNT);
	return controllers[index];
}

State system_controller_button(u32 index, Gamepad button) {
	assert(index < XUSER_MAX_COUNT);
	return controllers[index].buttons[button];
}

r32 system_controller_left_trigger(u32 index) {
	assert(index < XUSER_MAX_COUNT);
	return controllers[index].left_trigger;
}

r32 system_controller_right_trigger(u32 index) {
	assert(index < XUSER_MAX_COUNT);
	return controllers[index].right_trigger;
}

Vec2 system_controller_left_thumb(u32 index) {
	assert(index < XUSER_MAX_COUNT);
	return controllers[index].left_thumb;
}

Vec2 system_controller_right_thumb(u32 index) {
	assert(index < XUSER_MAX_COUNT);
	return controllers[index].right_thumb;
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
	wchar_t *pool      = (wchar_t *)memory_allocate(msg.count + 1);
	MultiByteToWideChar(CP_UTF8, 0, (const char *)msg.data, (int)write_len, pool, (int)msg.count + 1);
	pool[write_len] = 0;

	MessageBoxW(window_handle, pool, L"Karma - Fatal Error", MB_TOPMOST | MB_ICONERROR | MB_OK);

	ExitProcess(0);
}

void system_display_critical_message(const String msg) {
	auto     write_len = msg.count;
	wchar_t *pool      = (wchar_t *)memory_allocate(msg.count + 1);
	MultiByteToWideChar(CP_UTF8, 0, (const char *)msg.data, (int)write_len, pool, (int)msg.count + 1);
	pool[write_len] = 0;

	MessageBoxW(window_handle, pool, L"Karma - Critical Error", MB_TOPMOST | MB_ICONWARNING | MB_OK);
	memory_free(pool);
}

void *system_allocator(Allocation_Type type, ptrsize size, const void *ptr, void *user_ptr) {
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
		XInputGetState              = (Proc_XInput_Get_State)GetProcAddress(xinput, "XInputGetState");
		XInputSetState              = (Proc_XInput_Set_State)GetProcAddress(xinput, "XInputSetState");
		XInputGetCapabilities       = (Proc_XInput_Get_Capabilities)GetProcAddress(xinput, "XInputGetCapabilities");
		XInputGetAudioDeviceIds     = (Proc_XInput_Get_Audio_Device_Ids)GetProcAddress(xinput, "XInputGetAudioDeviceIds");
		XInputGetBatteryInformation = (Proc_XInput_Get_Battery_Information)GetProcAddress(xinput, "XInputGetBatteryInformation");
		XInputGetKeystroke          = (Proc_XInput_Get_Keystroke)GetProcAddress(xinput, "XInputGetKeystroke");
	}

	if (!XInputGetState) XInputGetState = xinput_get_state_stub;
	if (!XInputSetState) XInputSetState = xinput_set_state_stub;
	if (!XInputGetCapabilities) XInputGetCapabilities = xinput_get_capacilities_stub;
	if (!XInputGetAudioDeviceIds) XInputGetAudioDeviceIds = xinput_get_audio_device_ids_stub;
	if (!XInputGetBatteryInformation) XInputGetBatteryInformation = xinput_get_battery_information_stub;
	if (!XInputGetKeystroke) XInputGetKeystroke = xinput_get_keystroke_stub;
}

DWORD WINAPI win_thread_proc(LPVOID param) {
	Thread_Context *thread = (Thread_Context *)param;
	context = *thread;

	int result = thread->proc();

	return result;
}

bool system_thread_create(Thread_Proc proc, void *arg, Allocator allocator, ptrsize temporary_memory_size, String name, Thread_Context *thread) {
	thread->proc      = proc;
	thread->data      = arg;
	thread->allocator = allocator;

	if (temporary_memory_size) {
		void *ptr = memory_allocate(temporary_memory_size, thread->allocator);
		if (ptr == 0) {
			win32_check_for_error();
			system_fatal_error("Out of memory: Unable to allocate temporary storage memory!");
		}
		thread->temp_memory = Temporary_Memory(ptr, temporary_memory_size);
	} else {
		thread->temp_memory = Temporary_Memory(0, 0);
	}

	thread->handle.hptr = CreateThread(0, 0, win_thread_proc, &thread, CREATE_SUSPENDED, 0);
	if (thread->handle.hptr != NULL) {
		if (name) {
			wchar_t *desc = (wchar_t *)tallocate(name.count + 1);
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

int __stdcall wWinMain(HINSTANCE instance, HINSTANCE prev_instance, LPWSTR cmd_line, int show_cmd) {
#	if defined(BUILD_DEBUG) || defined(BUILD_DEBUG_FAST)
	AllocConsole();
	SetConsoleOutputCP(CP_UTF8);
#	endif

	DWORD task_index = 0;
	HANDLE task_handle = AvSetMmThreadCharacteristicsW(L"Games", &task_index);

	win32_map_keys();

	SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	HRESULT res = CoInitializeEx(0, COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE);
	if (FAILED(res)) {
		win32_check_for_error();
		system_fatal_error("Failed to Initialize COM Objects");
	}

	SetThreadDescription(GetCurrentThread(), L"Main");

	context.handle.hptr    = GetCurrentThread();
	context.id             = GetCurrentThreadId();
	context.allocator.proc = system_allocator;
	context.allocator.data = HeapCreate(HEAP_NO_SERIALIZE, 0, 0);
	context.proc		   = system_main;

	int      len = WideCharToMultiByte(CP_UTF8, 0, cmd_line, -1, 0, 0, 0, 0);
	String   cmd_line_string;
	cmd_line_string.data	  = new u8[len];
	cmd_line_string.count	  = WideCharToMultiByte(CP_UTF8, 0, cmd_line, -1, (char *)cmd_line_string.data, len, 0, 0);
	context.data			  = &cmd_line_string;

	void *ptr = memory_allocate(static_cast<SIZE_T>(TEMPORARY_MEMORY_SIZE));
	if (ptr == 0) {
		win32_check_for_error();
		system_fatal_error("Out of memory: Unable to allocate temporary storage memory!");
	}
	context.temp_memory = Temporary_Memory(ptr, static_cast<ptrsize>(TEMPORARY_MEMORY_SIZE));

	win32_initialize_xinput();

	int result = system_main();
	
	HeapDestroy(context.allocator.data);

	CoUninitialize();

	return result;
}

void system_log(int type, const char *title, ANALYSE_PRINTF_FORMAT_STRING(const char *fmt), ...) {
	char    pool[STB_SPRINTF_MIN];
	wchar_t msg[STB_SPRINTF_MIN + 1];

	struct Ctx {
		wchar_t *wbuf;
		HANDLE   handle;
	};
	Ctx ctx;

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
		MultiByteToWideChar(CP_UTF8, 0, pool, -1, msg, sizeof(wchar_t) * (STB_SPRINTF_MIN));
		WriteConsoleW(handle, msg, len, &written, 0);
		OutputDebugStringW(msg);

		ctx.wbuf   = msg;
		ctx.handle = handle;

		va_list ap;
		va_start(ap, fmt);
		stbsp_vsprintfcb([](char const *buf, void *user, int len) -> char * {
			auto *ctx = (Ctx *)user;
			MultiByteToWideChar(CP_UTF8, 0, buf, len, ctx->wbuf, sizeof(wchar_t) * (STB_SPRINTF_MIN));
			ctx->wbuf[len] = 0;
			DWORD written  = 0;
			WriteConsoleW(ctx->handle, ctx->wbuf, len, &written, 0);
			OutputDebugStringW(ctx->wbuf);
			return (char *)buf;
		},
						 &ctx, pool, fmt, ap);
		va_end(ap);

		len = stbsp_snprintf(pool, STB_SPRINTF_MIN, "\n");
		MultiByteToWideChar(CP_UTF8, 0, pool, -1, msg, sizeof(wchar_t) * (STB_SPRINTF_MIN));
		WriteConsoleW(handle, msg, len, &written, 0);
		OutputDebugStringW(msg);
	}
}
