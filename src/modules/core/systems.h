#pragma once
#include "karma.h"
#include "lin_maths.h"
#include "length_string.h"

//
//
//

enum Key : u8 {
	Key_UNKNOWN = 0,
	Key_A,
	Key_B,
	Key_C,
	Key_D,
	Key_E,
	Key_F,
	Key_G,
	Key_H,
	Key_I,
	Key_J,
	Key_K,
	Key_L,
	Key_M,
	Key_N,
	Key_O,
	Key_P,
	Key_Q,
	Key_R,
	Key_S,
	Key_T,
	Key_U,
	Key_V,
	Key_W,
	Key_X,
	Key_Y,
	Key_Z,
	Key_0,
	Key_1,
	Key_2,
	Key_3,
	Key_4,
	Key_5,
	Key_6,
	Key_7,
	Key_8,
	Key_9,
	Key_RETURN,
	Key_ESCAPE,
	Key_BACKSPACE,
	Key_TAB,
	Key_SPACE,
	Key_SHIFT,
	Key_CTRL,
	Key_F1,
	Key_F2,
	Key_F3,
	Key_F4,
	Key_F5,
	Key_F6,
	Key_F7,
	Key_F8,
	Key_F9,
	Key_F10,
	Key_F11,
	Key_F12,
	Key_PRINT_SCREEN,
	Key_INSERT,
	Key_HOME,
	Key_PAGE_UP,
	Key_PAGE_DOWN,
	Key_DELETE,
	Key_END,
	Key_RIGHT,
	Key_LEFT,
	Key_DOWN,
	Key_UP,
	Key_DIVIDE,
	Key_MULTIPLY,
	Key_MINUS,
	Key_PLUS,
	Key_PERIOD,
	Key_BACK_TICK,
	Key_PAD_0,
	Key_PAD_1,
	Key_PAD_2,
	Key_PAD_3,
	Key_PAD_4,
	Key_PAD_5,
	Key_PAD_6,
	Key_PAD_7,
	Key_PAD_8,
	Key_PAD_9,

	Key_COUNT
};

enum Key_Mod : u8 {
	Key_Mod_NONE        = bit(0),
	Key_Mod_LEFT_SHIFT  = bit(1),
	Key_Mod_RIGHT_SHIFT = bit(2),
	Key_Mod_LEFT_CTRL   = bit(3),
	Key_Mod_RIGHT_CTRL  = bit(4),
	Key_Mod_LEFT_ALT    = bit(5),
	Key_Mod_RIGHT_ALT   = bit(6),

	Key_Mod_CTRL  = Key_Mod_LEFT_CTRL | Key_Mod_RIGHT_CTRL,
	Key_Mod_ALT   = Key_Mod_LEFT_ALT | Key_Mod_RIGHT_ALT,
	Key_Mod_SHIFT = Key_Mod_LEFT_SHIFT | Key_Mod_RIGHT_SHIFT,
};
typedef u8 Key_Mods;

enum Button : u8 {
	Button_UNKNOWN,
	Button_LEFT,
	Button_MIDDLE,
	Button_RIGHT,
	Button_X1,
	Button_X2,

	Button_COUNT
};

typedef u8 Controller_Id;

enum Controller_Button : u8 {
	Controller_Button_DPAD_UP,
	Controller_Button_DPAD_DOWN,
	Controller_Button_DPAD_LEFT,
	Controller_Button_DPAD_RIGHT,
	Controller_Button_START,
	Controller_Button_BACK,
	Controller_Button_LTHUMB,
	Controller_Button_RTHUMB,
	Controller_Button_LSHOULDER,
	Controller_Button_RSHOULDER,
	Controller_Button_A,
	Controller_Button_B,
	Controller_Button_X,
	Controller_Button_Y,

	Controller_Button_COUNT,
};

enum Controller_Axis : u8 {
	Controller_Axis_LTHUMB_X,
	Controller_Axis_LTHUMB_Y,
	Controller_Axis_RTHUMB_X,
	Controller_Axis_RTHUMB_Y,
	Controller_Axis_LTRIGGER,
	Controller_Axis_RTRIGGER,

	Controller_Axis_COUNT
};

//
//
//

enum Event_Type : u32 {
	Event_Type_NONE = bit(0),

	Event_Type_WINDOW_ACTIVE   = bit(1),
	Event_Type_WINDOW_INACTIVE = bit(2),
	Event_Type_WINDOW_RESIZE   = bit(3),
	Event_Type_EXIT            = bit(4),
	Event_Type_APPLICATION     = Event_Type_WINDOW_ACTIVE | Event_Type_WINDOW_INACTIVE | Event_Type_WINDOW_RESIZE | Event_Type_EXIT,

	Event_Type_KEY_DOWN = bit(5),
	Event_Type_KEY_UP   = bit(6),
	Event_Type_KEYBOARD = Event_Type_KEY_DOWN | Event_Type_KEY_UP,

	Event_Type_MOUSE_BUTTON_DOWN = bit(7),
	Event_Type_MOUSE_BUTTON_UP   = bit(8),
	Event_Type_MOUSE_WHEEL       = bit(9),
	Event_Type_MOUSE_CURSOR      = bit(10),
	Event_Type_MOUSE_AXIS        = bit(11),
	Event_Type_MOUSE_BUTTON      = Event_Type_MOUSE_BUTTON_DOWN | Event_Type_MOUSE_BUTTON_UP,
	Event_Type_MOUSE             = Event_Type_MOUSE_BUTTON_DOWN | Event_Type_MOUSE_BUTTON_UP | Event_Type_MOUSE_WHEEL | Event_Type_MOUSE_CURSOR | Event_Type_MOUSE_AXIS,

	Event_Type_TEXT_INPUT = bit(12),

	Event_Type_CONTROLLER_JOIN		= bit(13),
	Event_Type_CONTROLLER_LEAVE		= bit(14),
	Event_Type_CONTROLLER_BUTTON	= bit(15),
	Event_Type_CONTROLLER_AXIS		= bit(16),
	Event_Type_CONTROLLER       = Event_Type_CONTROLLER_JOIN | Event_Type_CONTROLLER_LEAVE | Event_Type_CONTROLLER_BUTTON | Event_Type_CONTROLLER_AXIS,
};
typedef u32 Event_Types;

enum Key_State : u8 {
	Key_State_UP   = 0,
	Key_State_DOWN = 1
};

struct Key_Event {
	Key			symbol;
	Key_State	state;
	u8			repeat;
};

struct Mouse_Button_Event {
	Vec2s		position;
	Button		symbol;
	Key_State	state;
};

struct Mouse_Wheel_Event {
	s32 horizontal, vertical;
};

struct Mouse_Cursor_Event {
	s32 x, y;
};

struct Mouse_Axis_Event {
	r32 x, y;
	s32 dx, dy;
};

struct Window_Resize_Event {
	Vec2s dimension;
};

struct Text_Event {
	Utf32_Codepoint codepoint;
};

struct Controller_Device_Event {
	Controller_Id id;
};

struct Controller_Button_Event {
	Controller_Button	symbol;
	Key_State			state;
	Controller_Id		id;
};

struct Controller_Axis_Event {
	r32				value;
	Controller_Axis	symbol;
	Controller_Id	id;
};

struct Event {
	Event_Types type;

	union {
		Key_Event					key;
		Mouse_Button_Event			mouse_button;
		Mouse_Wheel_Event			mouse_wheel;
		Mouse_Cursor_Event			mouse_cursor;
		Mouse_Axis_Event			mouse_axis;
		Window_Resize_Event			window;
		Text_Event					text;
		Controller_Device_Event		controller_device;
		Controller_Button_Event		controller_button;
		Controller_Axis_Event		controller_axis;
	};

	inline Event() {
		type = Event_Type_NONE;
	}
};

struct Controller {
	Key_State	buttons[Controller_Button_COUNT];
	r32			axis[Controller_Axis_COUNT];
};

//
// File Handling
//
struct System_Find_File_Info {
	String path;
	String name;
	String extension;
	u64    size;
	u64	   modified;
};

enum File_Operation {
	File_Operation_READ,
	File_Operation_NEW,
	File_Operation_APPEND,
	File_Operation_READ_WRITE
};

enum File_Position {
	File_Position_BEG,
	File_Position_END,
	File_Position_CUR
};

typedef ptrsize (*System_Get_File_Pointer)(Handle handle);
typedef bool (*System_Set_File_Pointer)(Handle handle, ptrsize position, File_Position start_position);
typedef bool (*System_Read_File)(Handle handle, ptrsize size, u8 *buffer);
typedef bool (*System_Write_File)(Handle handle, void *buffer, ptrsize size);

struct System_File {
	Handle handle = {};

	System_Get_File_Pointer get_cursor = 0;
	System_Set_File_Pointer set_cursor = 0;

	System_Read_File  read  = 0;
	System_Write_File write = 0;
};

enum System_Window_Show {
	System_Window_Show_NORMAL,
	System_Window_Show_FULLSCREEN,
	System_Window_Show_MAXIMIZED
};

struct System_Window_State {
	void *  handle;
	ptrsize size;
};

enum Cursor_Kind {
	Cursor_Kind_ARROW,
	Cursor_Kind_IBEAM,
	Cursor_Kind_SIZE_ALL,
	Cursor_Kind_SIZE_NS,
	Cursor_Kind_SIZE_EW,
	Cursor_Kind_SIZE_NESW,
	Cursor_Kind_SIZE_NWSE,
	Cursor_Kind_HAND,
};

//
//
//

enum Thread_Wait {
	Thread_Wait_ABANDONED,
	Thread_Wait_OBJECT_0,
	Thread_Wait_TIMEOUT,
	Thread_Wait_FAILED
};

//
//
//

enum Processor_Architecture {
	Processor_Architecture_UNKNOWN,
	Processor_Architecture_AMD64,
	Processor_Architecture_ARM,
	Processor_Architecture_ARM64,
	Processor_Architecture_IA64,
	Processor_Architecture_INTEL,
};

struct System_Info {
	Processor_Architecture architecture;
	u32                    page_size;
	ptrsize                min_application_address;
	ptrsize                max_application_address;
	u32                    number_of_processors;
	u32                    allocation_granularity;
};

enum Virtual_Memory_Flag_Bit {
	Virtual_Memory_COMMIT     = 0x1,
	Virtual_Memory_RESERVE    = 0x2,
	Virtual_Memory_RESET      = 0x4,
	Virtual_Memory_UNDO_RESET = 0x8,
	Virtual_Memory_DECOMMIT   = 0x10,
	Virtual_Memory_RELEASE    = 0x20,
};
typedef u32 Vitual_Memory_Flags;

constexpr u32 WAIT_INFINITE = 0xFFFFFFFF;

enum Wait_Result {
	Wait_Result_SUCCESS,
	Wait_Result_ABANDONED,
	Wait_Result_SIGNALED,
	Wait_Result_TIMEOUT,
	Wait_Result_FAILED
};

//
//
//

typedef void * System_Audio_Handle;

typedef u8 *(*System_Lock_Audio)(System_Audio_Handle audio, u32 *sample_count);
typedef void(*System_Unlock_Audio)(System_Audio_Handle audio, u32 samples_written);

struct System_Audio {
	System_Audio_Handle		handle;
	System_Lock_Audio		lock;
	System_Unlock_Audio		unlock;
};

typedef void(*Audio_Callback)(const System_Audio &sys_audio, void *user_data);
typedef void(*Audio_Device_Refresh)(u32 sample_rate, u32 channel_count, void *user_data);

//
//
//
constexpr int SYSTEM_ENABLE  = 1;
constexpr int SYSTEM_DISABLE = 0;
constexpr int SYSTEM_TOGGLE  = 2;
constexpr int SYSTEM_QUERY   = -1;

constexpr u32 SYSTEM_AUDIO_BUFFER_SIZE_IN_MILLISECS = 1000;

enum {
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR
};

//
//
//

Array_View<u8> system_read_entire_file(const String path);
bool           system_write_entire_file(const String path, Array_View<u8> content);

bool system_open_file(const String path, File_Operation op, System_File *file);
void system_close_file(System_File *file);

Array_View<System_Find_File_Info> system_find_files(const String directory, const String extension, bool recursive);

Vec2s system_get_primary_monitor_size();
Vec2s system_get_client_size();
int   system_fullscreen_state(int toggle);
int   system_maximize_state(int toggle);

System_Window_State system_get_window_state();
bool                system_set_window_state(System_Window_State state);

void system_set_capture();
void system_release_capture();

void system_show_cursor();
void system_hide_cursor();
void system_set_cursor_kind(Cursor_Kind kind);
bool system_is_cursor_visible();

String system_get_clipboard_text();
void   system_set_clipboard_text(const String string);

String      system_get_command_line();
System_Info system_get_info();
String      system_get_user_name();

Handle system_create_window(const char *title, s32 width, s32 height, System_Window_Show show, System_Window_State *state = 0);
void   system_request_quit();
void   system_exit_process(int result);

enum Audio_Channel {
	Audio_Channel_LEFT,
	Audio_Channel_RIGHT,

	Audio_Channel_COUNT
};

bool system_audio(Audio_Callback callback, Audio_Device_Refresh refresh, void *user_data);
u32 system_audio_sample_rate();
u32 system_audio_channel_count();
void system_audio_resume();
void system_audio_pause();

typedef void * Socket;
const Socket SOCKET_INVALID = (Socket)MAX_UINT64;

struct Socket_Address {
	u32 address;
	u16 port;
};

inline Socket_Address socket_address(u8 a, u8 b, u8 c, u8 d, u16 port) {
	Socket_Address sock_addr;
	sock_addr.address = (( a << 24 ) | ( b << 16 ) | ( c << 8  ) | d);
	sock_addr.port = port;
	return sock_addr;
}

inline Socket_Address socket_address_local(u16 port) {
	return socket_address(127, 0, 0, 1, port);
}

bool system_net_set_socket_nonblocking(Socket sock);
Socket system_net_open_udp_server(Socket_Address address);
Socket system_net_open_udp_client();
s32 system_net_send_to(Socket sock, void * buffer, s32 length, Socket_Address address);
s32 system_net_receive_from(Socket sock, void * packet_buffer, s32 max_packet_size, Socket_Address *address);
void system_net_close_socket(Socket sock);

const Array_View<Event> system_poll_events(int poll_count = 1);

Key_State system_button(Button button);
Key_State system_key(Key key);
Key_Mods  system_get_key_mods();
Vec2s	  system_get_cursor_position_vec2s();
Vec2s	  system_get_cursor_position_y_inverted_vec2s();
Vec2      system_get_cursor_position();
Vec2      system_get_cursor_position_y_inverted();

u32               system_max_controllers();
bool              system_controller_is_available(Controller_Id id);
void              system_controller_vibrate(Controller_Id id, r32 left_motor, r32 right_motor);
const Controller &system_get_controller_state(Controller_Id id);
Key_State         system_controller_button(Controller_Id id, Controller_Button button);
r32				  system_controller_axis(Controller_Id id, Controller_Axis axis);
Vec2              system_controller_left_thumb(Controller_Id id);
Vec2              system_controller_right_thumb(Controller_Id id);

u64 system_get_counter();
u64 system_get_frequency();

void system_fatal_error(const String msg);
void system_display_critical_message(const String msg);

enum Heap_Type {
	Heap_Type_NO_SERIALIZE,
	Heap_Type_SERIALIZE,
};

typedef u32 Builder_Flags;
enum Builder_Flag_Bit : Builder_Flags {
	Builder_NONE	= 0,
	Builder_VIDEO	= bit(0),
	Builder_AUDIO	= bit(1),
	Builder_NETWORK	= bit(1),
	Builder_ALL		= Builder_VIDEO | Builder_AUDIO | Builder_NETWORK
};

struct Builder {
	Allocator allocator;
	Thread_Proc entry;
	ptrsize temporary_buffer_size;
	void *data;
	Builder_Flags flags; // TODO: Use this in system_windows.cpp
};

Allocator system_create_heap_allocator(Heap_Type type = Heap_Type_NO_SERIALIZE, ptrsize initial_size = 0, ptrsize maximum_size = 0);
void *system_virtual_alloc(void *address, ptrsize size, Vitual_Memory_Flags flags);
void  system_virtual_free(void *ptr, ptrsize size, Vitual_Memory_Flags flags);

bool		system_thread_create(const Builder &builder, String name, Thread_Context *thread);
void        system_thread_run(Thread_Context &thread);
Thread_Wait system_thread_wait(Thread_Context &thread, u32 millisecs);
void        system_thread_terminate(Thread_Context &thread, int exit_code);
void        system_thread_exit(int exit_code);

Handle      system_create_mutex();
void        system_destory_mutex(Handle handle);
Wait_Result system_lock_mutex(Handle handle, u32 millisecs);
void        system_unlock_mutex(Handle handle);

Builder system_builder();

void system_log(int type, const char *title, ANALYSE_PRINTF_FORMAT_STRING(const char *fmt), ...) ANALYSE_PRINTF_FORMAT(3, 4);

#if defined(BUILD_DEBUG) || defined(BUILD_DEBUG_FAST)
#	define system_trace(fmt, ...) system_log(LOG_INFO, "Trace", fmt, ##__VA_ARGS__)
#	define system_assert(exp, fmt, ...)                                                                                                \
		if (!exp) {                                                                                                                     \
			system_log(LOG_ERROR, "Assertion", "File: %s, Line: %d, Function: %s", current_file(), current_line(), current_function()); \
			system_log(LOG_ERROR, "Message", fmt, ##__VA_ARGS__);                                                                       \
			trigger_breakpoint();                                                                                                       \
		}
#else
#	define system_trace(...)
#	define system_assert(...)
#endif
