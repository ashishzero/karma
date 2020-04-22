#pragma once
#include "karma.h"
#include "lin_maths.h"
#include "length_string.h"

//
//
//

enum Key : u32 {
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

enum Key_Mod : u32 {
	Key_Mod_NONE		= bit(0),
	Key_Mod_LEFT_SHIFT  = bit(1),
	Key_Mod_RIGHT_SHIFT = bit(2),
	Key_Mod_LEFT_CTRL   = bit(3),
	Key_Mod_RIGHT_CTRL  = bit(4),
	Key_Mod_LEFT_ALT	= bit(5),
	Key_Mod_RIGHT_ALT   = bit(6),

	Key_Mod_CTRL  = Key_Mod_LEFT_CTRL | Key_Mod_RIGHT_CTRL,
	Key_Mod_ALT   = Key_Mod_LEFT_ALT | Key_Mod_RIGHT_ALT,
	Key_Mod_SHIFT = Key_Mod_LEFT_SHIFT | Key_Mod_RIGHT_SHIFT,
};
typedef u32 Key_Mods;

enum Button : u32 {
	Button_UNKNOWN,
	Button_LEFT,
	Button_MIDDLE,
	Button_RIGHT,
	Button_X1,
	Button_X2,

	Button_COUNT
};

//
//
//

enum Event_Type : u32 {
	Event_Type_NONE = bit(0),

	Event_Type_WINDOW_ACTIVE   = bit(1),
	Event_Type_WINDOW_INACTIVE = bit(2),
	Event_Type_WINDOW_RESIZE   = bit(3),
	Event_Type_EXIT			   = bit(4),
	Event_Type_APPLICATION	 = Event_Type_WINDOW_ACTIVE | Event_Type_WINDOW_INACTIVE | Event_Type_WINDOW_RESIZE | Event_Type_EXIT,

	Event_Type_KEY_DOWN = bit(5),
	Event_Type_KEY_UP   = bit(6),
	Event_Type_KEYBOARD = Event_Type_KEY_DOWN | Event_Type_KEY_UP,

	Event_Type_MOUSE_BUTTON_DOWN = bit(7),
	Event_Type_MOUSE_BUTTON_UP   = bit(8),
	Event_Type_MOUSE_WHEEL		 = bit(9),
	Event_Type_MOUSE_CURSOR		 = bit(10),
	Event_Type_MOUSE_AXIS		 = bit(11),
	Event_Type_MOUSE_BUTTON		 = Event_Type_MOUSE_BUTTON_DOWN | Event_Type_MOUSE_BUTTON_UP,
	Event_Type_MOUSE			 = Event_Type_MOUSE_BUTTON_DOWN | Event_Type_MOUSE_BUTTON_UP | Event_Type_MOUSE_WHEEL | Event_Type_MOUSE_CURSOR | Event_Type_MOUSE_AXIS,

	Event_Type_TEXT_INPUT = bit(12),

	Event_Type_CONTROLLER_JOIN  = bit(13),
	Event_Type_CONTROLLER_LEAVE = bit(14),
	Event_Type_CONTROLLER		= Event_Type_CONTROLLER_JOIN | Event_Type_CONTROLLER_LEAVE,
};
typedef u32 Event_Types;

enum State {
	State_UP   = 0,
	State_DOWN = 1
};

struct Key_Event {
	Key   symbol;
	State state;
	u32   repeat;
};

struct Mouse_Button_Event {
	Vec2s  position;
	Button symbol;
	State  state;
};

struct Mouse_Wheel_Event {
	s32 horizontal, vertical;
};

struct Mouse_Cursor_Event {
	s32 x, y;
};

struct Mouse_Axis_Event {
	r32 x, y;
};

struct Window_Resize_Event {
	Vec2s dimension;
};

struct Text_Event {
	Utf32_Codepoint codepoint;
};

struct Controller_Event {
	u32 index;
};

struct Event {
	Event_Types type;

	union {
		Key_Event			key;
		Mouse_Button_Event  mouse_button;
		Mouse_Wheel_Event   mouse_wheel;
		Mouse_Cursor_Event  mouse_cursor;
		Mouse_Axis_Event	mouse_axis;
		Window_Resize_Event window;
		Text_Event			text;
		Controller_Event	controller;
	};

	inline Event() { type = Event_Type_NONE; }
};

enum Gamepad {
	Gamepad_DPAD_UP,
	Gamepad_DPAD_DOWN,
	Gamepad_DPAD_LEFT,
	Gamepad_DPAD_RIGHT,
	Gamepad_START,
	Gamepad_BACK,
	Gamepad_LTHUMB,
	Gamepad_RTHUMB,
	Gamepad_LSHOULDER,
	Gamepad_RSHOULDER,
	Gamepad_A,
	Gamepad_B,
	Gamepad_X,
	Gamepad_Y,

	Gamepad_COUNT,
};

struct Controller {
	State buttons[Gamepad_COUNT];
	r32   left_trigger;
	r32   right_trigger;
	Vec2  left_thumb;
	Vec2  right_thumb;
};

//
// File Handling
//
struct System_Find_File_Info {
	String path;
	String name;
	String extension;
	u64	size;
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
constexpr int SYSTEM_ENABLE  = 1;
constexpr int SYSTEM_DISABLE = 0;
constexpr int SYSTEM_TOGGLE  = 2;
constexpr int SYSTEM_QUERY   = -1;

enum {
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR
};

//
//
//

Array_View<u8> system_read_entire_file(const String path);
bool		   system_write_entire_file(const String path, Array_View<u8> content);

bool system_open_file(const String path, File_Operation op, System_File *file);
void system_close_file(System_File *file);

Array_View<System_Find_File_Info> system_find_files(const String directory, bool recursive);

Vec2s system_get_primary_monitor_size();
Vec2s system_get_client_size();
int   system_fullscreen_state(int toggle);
int   system_maximize_state(int toggle);

System_Window_State system_get_window_state();
bool				system_set_window_state(System_Window_State state);

void system_set_capture();
void system_release_capture();

void system_show_cursor();
void system_hide_cursor();
void system_set_cursor_kind(Cursor_Kind kind);
bool system_is_cursor_visible();

String system_get_clipboard_text();
void   system_set_clipboard_text(const String string);

String system_get_command_line();

Handle system_create_window(const char *title, s32 width, s32 height, System_Window_Show show, System_Window_State *state = 0);
void   system_request_quit();
void   system_exit_process(int result);

bool system_poll_events(Event *event);

State	 system_button(Button button);
Key_Mods system_get_key_mods();
Vec2s	 system_get_cursor_position();

u32				  system_max_controllers();
bool			  system_controller_is_available(u32 index);
void			  system_controller_vibrate(u32 index, r32 left_motor, r32 right_motor);
const Controller &system_get_controller_state(u32 index);
State			  system_controller_button(u32 index, Gamepad button);
r32				  system_controller_left_trigger(u32 index);
r32				  system_controller_right_trigger(u32 index);
Vec2			  system_controller_left_thumb(u32 index);
Vec2			  system_controller_right_thumb(u32 index);

u64 system_get_counter();
u64 system_get_frequency();

void system_fatal_error(const String msg);
void system_display_critical_message(const String msg);

void *system_allocator(Allocation_Type type, ptrsize size, void *ptr, void *user_ptr);

int system_main();

void system_log(int type, const char *title, ANALYSE_PRINTF_FORMAT_STRING(const char *fmt), ...) ANALYSE_PRINTF_FORMAT(3, 4);

#if defined(BUILD_DEBUG) || defined(BUILD_DEVELOPER)
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
