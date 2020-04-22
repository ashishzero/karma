#pragma once
#include "reflection.h"


enum Allocation_Type : int;
template <> struct Enum_Info<Allocation_Type> {
	static constexpr size_t get_count() { return 3; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 2; }
	static const Allocation_Type index_value(s64 index) { return (Allocation_Type)(index + 0); }
	static const String string(Allocation_Type value) {
		static const String strings[] = {
			"Allocation_Type_NEW",
			"Allocation_Type_RESIZE",
			"Allocation_Type_FREE",
		};
		return strings[value - 0];
	}
};

template <> struct Reflect<Allocation_Type> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(Allocation_Type), "Allocation_Type", enum_count<Allocation_Type>());
		return &i;
	}
};

enum Render_Backend : int;
template <> struct Enum_Info<Render_Backend> {
	static constexpr size_t get_count() { return 3; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 2; }
	static const Render_Backend index_value(s64 index) { return (Render_Backend)(index + 0); }
	static const String string(Render_Backend value) {
		static const String strings[] = {
			"Render_Backend_NONE",
			"Render_Backend_OPENGL",
			"Render_Backend_COUNT",
		};
		return strings[value - 0];
	}
};

template <> struct Reflect<Render_Backend> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(Render_Backend), "Render_Backend", enum_count<Render_Backend>());
		return &i;
	}
};

enum Texture_Wrap : int;
template <> struct Enum_Info<Texture_Wrap> {
	static constexpr size_t get_count() { return 2; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 1; }
	static const Texture_Wrap index_value(s64 index) { return (Texture_Wrap)(index + 0); }
	static const String string(Texture_Wrap value) {
		static const String strings[] = {
			"Texture_Wrap_REPEAT",
			"Texture_Wrap_CLAMP",
		};
		return strings[value - 0];
	}
};

template <> struct Reflect<Texture_Wrap> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(Texture_Wrap), "Texture_Wrap", enum_count<Texture_Wrap>());
		return &i;
	}
};

enum Texture_Filter : int;
template <> struct Enum_Info<Texture_Filter> {
	static constexpr size_t get_count() { return 2; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 1; }
	static const Texture_Filter index_value(s64 index) { return (Texture_Filter)(index + 0); }
	static const String string(Texture_Filter value) {
		static const String strings[] = {
			"Texture_Filter_LINEAR",
			"Texture_Filter_NEAREST",
		};
		return strings[value - 0];
	}
};

template <> struct Reflect<Texture_Filter> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(Texture_Filter), "Texture_Filter", enum_count<Texture_Filter>());
		return &i;
	}
};

enum Buffer_Type : int;
template <> struct Enum_Info<Buffer_Type> {
	static constexpr size_t get_count() { return 2; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 1; }
	static const Buffer_Type index_value(s64 index) { return (Buffer_Type)(index + 0); }
	static const String string(Buffer_Type value) {
		static const String strings[] = {
			"Buffer_Type_DYNAMIC",
			"Buffer_Type_STATIC",
		};
		return strings[value - 0];
	}
};

template <> struct Reflect<Buffer_Type> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(Buffer_Type), "Buffer_Type", enum_count<Buffer_Type>());
		return &i;
	}
};

enum Color_Format : int;
template <> struct Enum_Info<Color_Format> {
	static constexpr size_t get_count() { return 5; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 4; }
	static const Color_Format index_value(s64 index) { return (Color_Format)(index + 0); }
	static const String string(Color_Format value) {
		static const String strings[] = {
			"Color_Format_NONE",
			"Color_Format_R",
			"Color_Format_RG",
			"Color_Format_RGB",
			"Color_Format_RGBA",
		};
		return strings[value - 0];
	}
};

template <> struct Reflect<Color_Format> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(Color_Format), "Color_Format", enum_count<Color_Format>());
		return &i;
	}
};

enum Depth_Format : int;
template <> struct Enum_Info<Depth_Format> {
	static constexpr size_t get_count() { return 3; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 2; }
	static const Depth_Format index_value(s64 index) { return (Depth_Format)(index + 0); }
	static const String string(Depth_Format value) {
		static const String strings[] = {
			"Depth_Format_NONE",
			"Depth_Format_STORAGE",
			"Depth_Format_TEXTURE",
		};
		return strings[value - 0];
	}
};

template <> struct Reflect<Depth_Format> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(Depth_Format), "Depth_Format", enum_count<Depth_Format>());
		return &i;
	}
};

enum Clear_Flag_Bit : u32;
template <> struct Enum_Info<Clear_Flag_Bit> {
	static constexpr size_t get_count() { return 5; }
	static constexpr s64 get_min_value() { return 1; }
	static constexpr s64 get_max_value() { return 14; }
	static const Clear_Flag_Bit index_value(s64 index) {
		static const Clear_Flag_Bit enum_values[] = {
			(Clear_Flag_Bit)1,
			(Clear_Flag_Bit)2,
			(Clear_Flag_Bit)4,
			(Clear_Flag_Bit)8,
			(Clear_Flag_Bit)14,
		};
		assert(index >= 0 && index < static_count(enum_values));
		return enum_values[index];
	}
	static const String string(Clear_Flag_Bit value) {
		if (1 == value) return "Clear_Flag_NONE";
		if (2 == value) return "Clear_Flag_COLOR";
		if (4 == value) return "Clear_Flag_DEPTH";
		if (8 == value) return "Clear_Flag_STENCIL";
		if (14 == value) return "Clear_Flag_ALL";

		invalid_code_path();
		return "";
	}
};

template <> struct Reflect<Clear_Flag_Bit> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(Clear_Flag_Bit), "Clear_Flag_Bit", enum_count<Clear_Flag_Bit>());
		return &i;
	}
};

enum Camera_View_Kind : int;
template <> struct Enum_Info<Camera_View_Kind> {
	static constexpr size_t get_count() { return 2; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 1; }
	static const Camera_View_Kind index_value(s64 index) { return (Camera_View_Kind)(index + 0); }
	static const String string(Camera_View_Kind value) {
		static const String strings[] = {
			"PERSPECTIVE",
			"ORTHOGRAPHIC",
		};
		return strings[value - 0];
	}
};

template <> struct Reflect<Camera_View_Kind> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(Camera_View_Kind), "Camera_View_Kind", enum_count<Camera_View_Kind>());
		return &i;
	}
};

enum Key : u32;
template <> struct Enum_Info<Key> {
	static constexpr size_t get_count() { return 84; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 83; }
	static const Key index_value(s64 index) { return (Key)(index + 0); }
	static const String string(Key value) {
		static const String strings[] = {
			"Key_UNKNOWN",
			"Key_A",
			"Key_B",
			"Key_C",
			"Key_D",
			"Key_E",
			"Key_F",
			"Key_G",
			"Key_H",
			"Key_I",
			"Key_J",
			"Key_K",
			"Key_L",
			"Key_M",
			"Key_N",
			"Key_O",
			"Key_P",
			"Key_Q",
			"Key_R",
			"Key_S",
			"Key_T",
			"Key_U",
			"Key_V",
			"Key_W",
			"Key_X",
			"Key_Y",
			"Key_Z",
			"Key_0",
			"Key_1",
			"Key_2",
			"Key_3",
			"Key_4",
			"Key_5",
			"Key_6",
			"Key_7",
			"Key_8",
			"Key_9",
			"Key_RETURN",
			"Key_ESCAPE",
			"Key_BACKSPACE",
			"Key_TAB",
			"Key_SPACE",
			"Key_SHIFT",
			"Key_CTRL",
			"Key_F1",
			"Key_F2",
			"Key_F3",
			"Key_F4",
			"Key_F5",
			"Key_F6",
			"Key_F7",
			"Key_F8",
			"Key_F9",
			"Key_F10",
			"Key_F11",
			"Key_F12",
			"Key_PRINT_SCREEN",
			"Key_INSERT",
			"Key_HOME",
			"Key_PAGE_UP",
			"Key_PAGE_DOWN",
			"Key_DELETE",
			"Key_END",
			"Key_RIGHT",
			"Key_LEFT",
			"Key_DOWN",
			"Key_UP",
			"Key_DIVIDE",
			"Key_MULTIPLY",
			"Key_MINUS",
			"Key_PLUS",
			"Key_PERIOD",
			"Key_BACK_TICK",
			"Key_PAD_0",
			"Key_PAD_1",
			"Key_PAD_2",
			"Key_PAD_3",
			"Key_PAD_4",
			"Key_PAD_5",
			"Key_PAD_6",
			"Key_PAD_7",
			"Key_PAD_8",
			"Key_PAD_9",
			"Key_COUNT",
		};
		return strings[value - 0];
	}
};

template <> struct Reflect<Key> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(Key), "Key", enum_count<Key>());
		return &i;
	}
};

enum Key_Mod : u32;
template <> struct Enum_Info<Key_Mod> {
	static constexpr size_t get_count() { return 10; }
	static constexpr s64 get_min_value() { return 1; }
	static constexpr s64 get_max_value() { return 96; }
	static const Key_Mod index_value(s64 index) {
		static const Key_Mod enum_values[] = {
			(Key_Mod)1,
			(Key_Mod)2,
			(Key_Mod)4,
			(Key_Mod)8,
			(Key_Mod)16,
			(Key_Mod)32,
			(Key_Mod)64,
			(Key_Mod)24,
			(Key_Mod)96,
			(Key_Mod)6,
		};
		assert(index >= 0 && index < static_count(enum_values));
		return enum_values[index];
	}
	static const String string(Key_Mod value) {
		if (1 == value) return "Key_Mod_NONE";
		if (2 == value) return "Key_Mod_LEFT_SHIFT";
		if (4 == value) return "Key_Mod_RIGHT_SHIFT";
		if (8 == value) return "Key_Mod_LEFT_CTRL";
		if (16 == value) return "Key_Mod_RIGHT_CTRL";
		if (32 == value) return "Key_Mod_LEFT_ALT";
		if (64 == value) return "Key_Mod_RIGHT_ALT";
		if (24 == value) return "Key_Mod_CTRL";
		if (96 == value) return "Key_Mod_ALT";
		if (6 == value) return "Key_Mod_SHIFT";

		invalid_code_path();
		return "";
	}
};

template <> struct Reflect<Key_Mod> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(Key_Mod), "Key_Mod", enum_count<Key_Mod>());
		return &i;
	}
};

enum Button : u32;
template <> struct Enum_Info<Button> {
	static constexpr size_t get_count() { return 7; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 6; }
	static const Button index_value(s64 index) { return (Button)(index + 0); }
	static const String string(Button value) {
		static const String strings[] = {
			"Button_UNKNOWN",
			"Button_LEFT",
			"Button_MIDDLE",
			"Button_RIGHT",
			"Button_X1",
			"Button_X2",
			"Button_COUNT",
		};
		return strings[value - 0];
	}
};

template <> struct Reflect<Button> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(Button), "Button", enum_count<Button>());
		return &i;
	}
};

enum Event_Type : u32;
template <> struct Enum_Info<Event_Type> {
	static constexpr size_t get_count() { return 20; }
	static constexpr s64 get_min_value() { return 1; }
	static constexpr s64 get_max_value() { return 24576; }
	static const Event_Type index_value(s64 index) {
		static const Event_Type enum_values[] = {
			(Event_Type)1,
			(Event_Type)2,
			(Event_Type)4,
			(Event_Type)8,
			(Event_Type)16,
			(Event_Type)30,
			(Event_Type)32,
			(Event_Type)64,
			(Event_Type)96,
			(Event_Type)128,
			(Event_Type)256,
			(Event_Type)512,
			(Event_Type)1024,
			(Event_Type)2048,
			(Event_Type)384,
			(Event_Type)3968,
			(Event_Type)4096,
			(Event_Type)8192,
			(Event_Type)16384,
			(Event_Type)24576,
		};
		assert(index >= 0 && index < static_count(enum_values));
		return enum_values[index];
	}
	static const String string(Event_Type value) {
		if (1 == value) return "Event_Type_NONE";
		if (2 == value) return "Event_Type_WINDOW_ACTIVE";
		if (4 == value) return "Event_Type_WINDOW_INACTIVE";
		if (8 == value) return "Event_Type_WINDOW_RESIZE";
		if (16 == value) return "Event_Type_EXIT";
		if (30 == value) return "Event_Type_APPLICATION";
		if (32 == value) return "Event_Type_KEY_DOWN";
		if (64 == value) return "Event_Type_KEY_UP";
		if (96 == value) return "Event_Type_KEYBOARD";
		if (128 == value) return "Event_Type_MOUSE_BUTTON_DOWN";
		if (256 == value) return "Event_Type_MOUSE_BUTTON_UP";
		if (512 == value) return "Event_Type_MOUSE_WHEEL";
		if (1024 == value) return "Event_Type_MOUSE_CURSOR";
		if (2048 == value) return "Event_Type_MOUSE_AXIS";
		if (384 == value) return "Event_Type_MOUSE_BUTTON";
		if (3968 == value) return "Event_Type_MOUSE";
		if (4096 == value) return "Event_Type_TEXT_INPUT";
		if (8192 == value) return "Event_Type_CONTROLLER_JOIN";
		if (16384 == value) return "Event_Type_CONTROLLER_LEAVE";
		if (24576 == value) return "Event_Type_CONTROLLER";

		invalid_code_path();
		return "";
	}
};

template <> struct Reflect<Event_Type> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(Event_Type), "Event_Type", enum_count<Event_Type>());
		return &i;
	}
};

enum State : int;
template <> struct Enum_Info<State> {
	static constexpr size_t get_count() { return 2; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 1; }
	static const State index_value(s64 index) { return (State)(index + 0); }
	static const String string(State value) {
		static const String strings[] = {
			"State_UP",
			"State_DOWN",
		};
		return strings[value - 0];
	}
};

template <> struct Reflect<State> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(State), "State", enum_count<State>());
		return &i;
	}
};

enum Gamepad : int;
template <> struct Enum_Info<Gamepad> {
	static constexpr size_t get_count() { return 15; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 14; }
	static const Gamepad index_value(s64 index) { return (Gamepad)(index + 0); }
	static const String string(Gamepad value) {
		static const String strings[] = {
			"Gamepad_DPAD_UP",
			"Gamepad_DPAD_DOWN",
			"Gamepad_DPAD_LEFT",
			"Gamepad_DPAD_RIGHT",
			"Gamepad_START",
			"Gamepad_BACK",
			"Gamepad_LTHUMB",
			"Gamepad_RTHUMB",
			"Gamepad_LSHOULDER",
			"Gamepad_RSHOULDER",
			"Gamepad_A",
			"Gamepad_B",
			"Gamepad_X",
			"Gamepad_Y",
			"Gamepad_COUNT",
		};
		return strings[value - 0];
	}
};

template <> struct Reflect<Gamepad> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(Gamepad), "Gamepad", enum_count<Gamepad>());
		return &i;
	}
};

enum File_Operation : int;
template <> struct Enum_Info<File_Operation> {
	static constexpr size_t get_count() { return 4; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 3; }
	static const File_Operation index_value(s64 index) { return (File_Operation)(index + 0); }
	static const String string(File_Operation value) {
		static const String strings[] = {
			"File_Operation_READ",
			"File_Operation_NEW",
			"File_Operation_APPEND",
			"File_Operation_READ_WRITE",
		};
		return strings[value - 0];
	}
};

template <> struct Reflect<File_Operation> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(File_Operation), "File_Operation", enum_count<File_Operation>());
		return &i;
	}
};

enum File_Position : int;
template <> struct Enum_Info<File_Position> {
	static constexpr size_t get_count() { return 3; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 2; }
	static const File_Position index_value(s64 index) { return (File_Position)(index + 0); }
	static const String string(File_Position value) {
		static const String strings[] = {
			"File_Position_BEG",
			"File_Position_END",
			"File_Position_CUR",
		};
		return strings[value - 0];
	}
};

template <> struct Reflect<File_Position> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(File_Position), "File_Position", enum_count<File_Position>());
		return &i;
	}
};

enum System_Window_Show : int;
template <> struct Enum_Info<System_Window_Show> {
	static constexpr size_t get_count() { return 3; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 2; }
	static const System_Window_Show index_value(s64 index) { return (System_Window_Show)(index + 0); }
	static const String string(System_Window_Show value) {
		static const String strings[] = {
			"System_Window_Show_NORMAL",
			"System_Window_Show_FULLSCREEN",
			"System_Window_Show_MAXIMIZED",
		};
		return strings[value - 0];
	}
};

template <> struct Reflect<System_Window_Show> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(System_Window_Show), "System_Window_Show", enum_count<System_Window_Show>());
		return &i;
	}
};

enum Cursor_Kind : int;
template <> struct Enum_Info<Cursor_Kind> {
	static constexpr size_t get_count() { return 8; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 7; }
	static const Cursor_Kind index_value(s64 index) { return (Cursor_Kind)(index + 0); }
	static const String string(Cursor_Kind value) {
		static const String strings[] = {
			"Cursor_Kind_ARROW",
			"Cursor_Kind_IBEAM",
			"Cursor_Kind_SIZE_ALL",
			"Cursor_Kind_SIZE_NS",
			"Cursor_Kind_SIZE_EW",
			"Cursor_Kind_SIZE_NESW",
			"Cursor_Kind_SIZE_NWSE",
			"Cursor_Kind_HAND",
		};
		return strings[value - 0];
	}
};

template <> struct Reflect<Cursor_Kind> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(Cursor_Kind), "Cursor_Kind", enum_count<Cursor_Kind>());
		return &i;
	}
};

enum ImGuiWindowFlags_ : int;
template <> struct Enum_Info<ImGuiWindowFlags_> {
	static constexpr size_t get_count() { return 32; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 536870912; }
	static const ImGuiWindowFlags_ index_value(s64 index) {
		static const ImGuiWindowFlags_ enum_values[] = {
			(ImGuiWindowFlags_)0,
			(ImGuiWindowFlags_)1,
			(ImGuiWindowFlags_)2,
			(ImGuiWindowFlags_)4,
			(ImGuiWindowFlags_)8,
			(ImGuiWindowFlags_)16,
			(ImGuiWindowFlags_)32,
			(ImGuiWindowFlags_)64,
			(ImGuiWindowFlags_)128,
			(ImGuiWindowFlags_)256,
			(ImGuiWindowFlags_)512,
			(ImGuiWindowFlags_)1024,
			(ImGuiWindowFlags_)2048,
			(ImGuiWindowFlags_)4096,
			(ImGuiWindowFlags_)8192,
			(ImGuiWindowFlags_)16384,
			(ImGuiWindowFlags_)32768,
			(ImGuiWindowFlags_)65536,
			(ImGuiWindowFlags_)262144,
			(ImGuiWindowFlags_)524288,
			(ImGuiWindowFlags_)1048576,
			(ImGuiWindowFlags_)2097152,
			(ImGuiWindowFlags_)786432,
			(ImGuiWindowFlags_)43,
			(ImGuiWindowFlags_)786944,
			(ImGuiWindowFlags_)8388608,
			(ImGuiWindowFlags_)16777216,
			(ImGuiWindowFlags_)33554432,
			(ImGuiWindowFlags_)67108864,
			(ImGuiWindowFlags_)134217728,
			(ImGuiWindowFlags_)268435456,
			(ImGuiWindowFlags_)536870912,
		};
		assert(index >= 0 && index < static_count(enum_values));
		return enum_values[index];
	}
	static const String string(ImGuiWindowFlags_ value) {
		if (0 == value) return "ImGuiWindowFlags_None";
		if (1 == value) return "ImGuiWindowFlags_NoTitleBar";
		if (2 == value) return "ImGuiWindowFlags_NoResize";
		if (4 == value) return "ImGuiWindowFlags_NoMove";
		if (8 == value) return "ImGuiWindowFlags_NoScrollbar";
		if (16 == value) return "ImGuiWindowFlags_NoScrollWithMouse";
		if (32 == value) return "ImGuiWindowFlags_NoCollapse";
		if (64 == value) return "ImGuiWindowFlags_AlwaysAutoResize";
		if (128 == value) return "ImGuiWindowFlags_NoBackground";
		if (256 == value) return "ImGuiWindowFlags_NoSavedSettings";
		if (512 == value) return "ImGuiWindowFlags_NoMouseInputs";
		if (1024 == value) return "ImGuiWindowFlags_MenuBar";
		if (2048 == value) return "ImGuiWindowFlags_HorizontalScrollbar";
		if (4096 == value) return "ImGuiWindowFlags_NoFocusOnAppearing";
		if (8192 == value) return "ImGuiWindowFlags_NoBringToFrontOnFocus";
		if (16384 == value) return "ImGuiWindowFlags_AlwaysVerticalScrollbar";
		if (32768 == value) return "ImGuiWindowFlags_AlwaysHorizontalScrollbar";
		if (65536 == value) return "ImGuiWindowFlags_AlwaysUseWindowPadding";
		if (262144 == value) return "ImGuiWindowFlags_NoNavInputs";
		if (524288 == value) return "ImGuiWindowFlags_NoNavFocus";
		if (1048576 == value) return "ImGuiWindowFlags_UnsavedDocument";
		if (2097152 == value) return "ImGuiWindowFlags_NoDocking";
		if (786432 == value) return "ImGuiWindowFlags_NoNav";
		if (43 == value) return "ImGuiWindowFlags_NoDecoration";
		if (786944 == value) return "ImGuiWindowFlags_NoInputs";
		if (8388608 == value) return "ImGuiWindowFlags_NavFlattened";
		if (16777216 == value) return "ImGuiWindowFlags_ChildWindow";
		if (33554432 == value) return "ImGuiWindowFlags_Tooltip";
		if (67108864 == value) return "ImGuiWindowFlags_Popup";
		if (134217728 == value) return "ImGuiWindowFlags_Modal";
		if (268435456 == value) return "ImGuiWindowFlags_ChildMenu";
		if (536870912 == value) return "ImGuiWindowFlags_DockNodeHost";

		invalid_code_path();
		return "";
	}
};

template <> struct Reflect<ImGuiWindowFlags_> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(ImGuiWindowFlags_), "ImGuiWindowFlags_", enum_count<ImGuiWindowFlags_>());
		return &i;
	}
};

enum ImGuiInputTextFlags_ : int;
template <> struct Enum_Info<ImGuiInputTextFlags_> {
	static constexpr size_t get_count() { return 22; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 2097152; }
	static const ImGuiInputTextFlags_ index_value(s64 index) {
		static const ImGuiInputTextFlags_ enum_values[] = {
			(ImGuiInputTextFlags_)0,
			(ImGuiInputTextFlags_)1,
			(ImGuiInputTextFlags_)2,
			(ImGuiInputTextFlags_)4,
			(ImGuiInputTextFlags_)8,
			(ImGuiInputTextFlags_)16,
			(ImGuiInputTextFlags_)32,
			(ImGuiInputTextFlags_)64,
			(ImGuiInputTextFlags_)128,
			(ImGuiInputTextFlags_)256,
			(ImGuiInputTextFlags_)512,
			(ImGuiInputTextFlags_)1024,
			(ImGuiInputTextFlags_)2048,
			(ImGuiInputTextFlags_)4096,
			(ImGuiInputTextFlags_)8192,
			(ImGuiInputTextFlags_)16384,
			(ImGuiInputTextFlags_)32768,
			(ImGuiInputTextFlags_)65536,
			(ImGuiInputTextFlags_)131072,
			(ImGuiInputTextFlags_)262144,
			(ImGuiInputTextFlags_)1048576,
			(ImGuiInputTextFlags_)2097152,
		};
		assert(index >= 0 && index < static_count(enum_values));
		return enum_values[index];
	}
	static const String string(ImGuiInputTextFlags_ value) {
		if (0 == value) return "ImGuiInputTextFlags_None";
		if (1 == value) return "ImGuiInputTextFlags_CharsDecimal";
		if (2 == value) return "ImGuiInputTextFlags_CharsHexadecimal";
		if (4 == value) return "ImGuiInputTextFlags_CharsUppercase";
		if (8 == value) return "ImGuiInputTextFlags_CharsNoBlank";
		if (16 == value) return "ImGuiInputTextFlags_AutoSelectAll";
		if (32 == value) return "ImGuiInputTextFlags_EnterReturnsTrue";
		if (64 == value) return "ImGuiInputTextFlags_CallbackCompletion";
		if (128 == value) return "ImGuiInputTextFlags_CallbackHistory";
		if (256 == value) return "ImGuiInputTextFlags_CallbackAlways";
		if (512 == value) return "ImGuiInputTextFlags_CallbackCharFilter";
		if (1024 == value) return "ImGuiInputTextFlags_AllowTabInput";
		if (2048 == value) return "ImGuiInputTextFlags_CtrlEnterForNewLine";
		if (4096 == value) return "ImGuiInputTextFlags_NoHorizontalScroll";
		if (8192 == value) return "ImGuiInputTextFlags_AlwaysInsertMode";
		if (16384 == value) return "ImGuiInputTextFlags_ReadOnly";
		if (32768 == value) return "ImGuiInputTextFlags_Password";
		if (65536 == value) return "ImGuiInputTextFlags_NoUndoRedo";
		if (131072 == value) return "ImGuiInputTextFlags_CharsScientific";
		if (262144 == value) return "ImGuiInputTextFlags_CallbackResize";
		if (1048576 == value) return "ImGuiInputTextFlags_Multiline";
		if (2097152 == value) return "ImGuiInputTextFlags_NoMarkEdited";

		invalid_code_path();
		return "";
	}
};

template <> struct Reflect<ImGuiInputTextFlags_> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(ImGuiInputTextFlags_), "ImGuiInputTextFlags_", enum_count<ImGuiInputTextFlags_>());
		return &i;
	}
};

enum ImGuiTreeNodeFlags_ : int;
template <> struct Enum_Info<ImGuiTreeNodeFlags_> {
	static constexpr size_t get_count() { return 16; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 8192; }
	static const ImGuiTreeNodeFlags_ index_value(s64 index) {
		static const ImGuiTreeNodeFlags_ enum_values[] = {
			(ImGuiTreeNodeFlags_)0,
			(ImGuiTreeNodeFlags_)1,
			(ImGuiTreeNodeFlags_)2,
			(ImGuiTreeNodeFlags_)4,
			(ImGuiTreeNodeFlags_)8,
			(ImGuiTreeNodeFlags_)16,
			(ImGuiTreeNodeFlags_)32,
			(ImGuiTreeNodeFlags_)64,
			(ImGuiTreeNodeFlags_)128,
			(ImGuiTreeNodeFlags_)256,
			(ImGuiTreeNodeFlags_)512,
			(ImGuiTreeNodeFlags_)1024,
			(ImGuiTreeNodeFlags_)2048,
			(ImGuiTreeNodeFlags_)4096,
			(ImGuiTreeNodeFlags_)8192,
			(ImGuiTreeNodeFlags_)26,
		};
		assert(index >= 0 && index < static_count(enum_values));
		return enum_values[index];
	}
	static const String string(ImGuiTreeNodeFlags_ value) {
		if (0 == value) return "ImGuiTreeNodeFlags_None";
		if (1 == value) return "ImGuiTreeNodeFlags_Selected";
		if (2 == value) return "ImGuiTreeNodeFlags_Framed";
		if (4 == value) return "ImGuiTreeNodeFlags_AllowItemOverlap";
		if (8 == value) return "ImGuiTreeNodeFlags_NoTreePushOnOpen";
		if (16 == value) return "ImGuiTreeNodeFlags_NoAutoOpenOnLog";
		if (32 == value) return "ImGuiTreeNodeFlags_DefaultOpen";
		if (64 == value) return "ImGuiTreeNodeFlags_OpenOnDoubleClick";
		if (128 == value) return "ImGuiTreeNodeFlags_OpenOnArrow";
		if (256 == value) return "ImGuiTreeNodeFlags_Leaf";
		if (512 == value) return "ImGuiTreeNodeFlags_Bullet";
		if (1024 == value) return "ImGuiTreeNodeFlags_FramePadding";
		if (2048 == value) return "ImGuiTreeNodeFlags_SpanAvailWidth";
		if (4096 == value) return "ImGuiTreeNodeFlags_SpanFullWidth";
		if (8192 == value) return "ImGuiTreeNodeFlags_NavLeftJumpsBackHere";
		if (26 == value) return "ImGuiTreeNodeFlags_CollapsingHeader";

		invalid_code_path();
		return "";
	}
};

template <> struct Reflect<ImGuiTreeNodeFlags_> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(ImGuiTreeNodeFlags_), "ImGuiTreeNodeFlags_", enum_count<ImGuiTreeNodeFlags_>());
		return &i;
	}
};

enum ImGuiSelectableFlags_ : int;
template <> struct Enum_Info<ImGuiSelectableFlags_> {
	static constexpr size_t get_count() { return 6; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 16; }
	static const ImGuiSelectableFlags_ index_value(s64 index) {
		static const ImGuiSelectableFlags_ enum_values[] = {
			(ImGuiSelectableFlags_)0,
			(ImGuiSelectableFlags_)1,
			(ImGuiSelectableFlags_)2,
			(ImGuiSelectableFlags_)4,
			(ImGuiSelectableFlags_)8,
			(ImGuiSelectableFlags_)16,
		};
		assert(index >= 0 && index < static_count(enum_values));
		return enum_values[index];
	}
	static const String string(ImGuiSelectableFlags_ value) {
		if (0 == value) return "ImGuiSelectableFlags_None";
		if (1 == value) return "ImGuiSelectableFlags_DontClosePopups";
		if (2 == value) return "ImGuiSelectableFlags_SpanAllColumns";
		if (4 == value) return "ImGuiSelectableFlags_AllowDoubleClick";
		if (8 == value) return "ImGuiSelectableFlags_Disabled";
		if (16 == value) return "ImGuiSelectableFlags_AllowItemOverlap";

		invalid_code_path();
		return "";
	}
};

template <> struct Reflect<ImGuiSelectableFlags_> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(ImGuiSelectableFlags_), "ImGuiSelectableFlags_", enum_count<ImGuiSelectableFlags_>());
		return &i;
	}
};

enum ImGuiComboFlags_ : int;
template <> struct Enum_Info<ImGuiComboFlags_> {
	static constexpr size_t get_count() { return 9; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 64; }
	static const ImGuiComboFlags_ index_value(s64 index) {
		static const ImGuiComboFlags_ enum_values[] = {
			(ImGuiComboFlags_)0,
			(ImGuiComboFlags_)1,
			(ImGuiComboFlags_)2,
			(ImGuiComboFlags_)4,
			(ImGuiComboFlags_)8,
			(ImGuiComboFlags_)16,
			(ImGuiComboFlags_)32,
			(ImGuiComboFlags_)64,
			(ImGuiComboFlags_)30,
		};
		assert(index >= 0 && index < static_count(enum_values));
		return enum_values[index];
	}
	static const String string(ImGuiComboFlags_ value) {
		if (0 == value) return "ImGuiComboFlags_None";
		if (1 == value) return "ImGuiComboFlags_PopupAlignLeft";
		if (2 == value) return "ImGuiComboFlags_HeightSmall";
		if (4 == value) return "ImGuiComboFlags_HeightRegular";
		if (8 == value) return "ImGuiComboFlags_HeightLarge";
		if (16 == value) return "ImGuiComboFlags_HeightLargest";
		if (32 == value) return "ImGuiComboFlags_NoArrowButton";
		if (64 == value) return "ImGuiComboFlags_NoPreview";
		if (30 == value) return "ImGuiComboFlags_HeightMask_";

		invalid_code_path();
		return "";
	}
};

template <> struct Reflect<ImGuiComboFlags_> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(ImGuiComboFlags_), "ImGuiComboFlags_", enum_count<ImGuiComboFlags_>());
		return &i;
	}
};

enum ImGuiTabBarFlags_ : int;
template <> struct Enum_Info<ImGuiTabBarFlags_> {
	static constexpr size_t get_count() { return 11; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 192; }
	static const ImGuiTabBarFlags_ index_value(s64 index) {
		static const ImGuiTabBarFlags_ enum_values[] = {
			(ImGuiTabBarFlags_)0,
			(ImGuiTabBarFlags_)1,
			(ImGuiTabBarFlags_)2,
			(ImGuiTabBarFlags_)4,
			(ImGuiTabBarFlags_)8,
			(ImGuiTabBarFlags_)16,
			(ImGuiTabBarFlags_)32,
			(ImGuiTabBarFlags_)64,
			(ImGuiTabBarFlags_)128,
			(ImGuiTabBarFlags_)192,
			(ImGuiTabBarFlags_)64,
		};
		assert(index >= 0 && index < static_count(enum_values));
		return enum_values[index];
	}
	static const String string(ImGuiTabBarFlags_ value) {
		if (0 == value) return "ImGuiTabBarFlags_None";
		if (1 == value) return "ImGuiTabBarFlags_Reorderable";
		if (2 == value) return "ImGuiTabBarFlags_AutoSelectNewTabs";
		if (4 == value) return "ImGuiTabBarFlags_TabListPopupButton";
		if (8 == value) return "ImGuiTabBarFlags_NoCloseWithMiddleMouseButton";
		if (16 == value) return "ImGuiTabBarFlags_NoTabListScrollingButtons";
		if (32 == value) return "ImGuiTabBarFlags_NoTooltip";
		if (64 == value) return "ImGuiTabBarFlags_FittingPolicyResizeDown";
		if (128 == value) return "ImGuiTabBarFlags_FittingPolicyScroll";
		if (192 == value) return "ImGuiTabBarFlags_FittingPolicyMask_";
		if (64 == value) return "ImGuiTabBarFlags_FittingPolicyDefault_";

		invalid_code_path();
		return "";
	}
};

template <> struct Reflect<ImGuiTabBarFlags_> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(ImGuiTabBarFlags_), "ImGuiTabBarFlags_", enum_count<ImGuiTabBarFlags_>());
		return &i;
	}
};

enum ImGuiTabItemFlags_ : int;
template <> struct Enum_Info<ImGuiTabItemFlags_> {
	static constexpr size_t get_count() { return 5; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 8; }
	static const ImGuiTabItemFlags_ index_value(s64 index) {
		static const ImGuiTabItemFlags_ enum_values[] = {
			(ImGuiTabItemFlags_)0,
			(ImGuiTabItemFlags_)1,
			(ImGuiTabItemFlags_)2,
			(ImGuiTabItemFlags_)4,
			(ImGuiTabItemFlags_)8,
		};
		assert(index >= 0 && index < static_count(enum_values));
		return enum_values[index];
	}
	static const String string(ImGuiTabItemFlags_ value) {
		if (0 == value) return "ImGuiTabItemFlags_None";
		if (1 == value) return "ImGuiTabItemFlags_UnsavedDocument";
		if (2 == value) return "ImGuiTabItemFlags_SetSelected";
		if (4 == value) return "ImGuiTabItemFlags_NoCloseWithMiddleMouseButton";
		if (8 == value) return "ImGuiTabItemFlags_NoPushId";

		invalid_code_path();
		return "";
	}
};

template <> struct Reflect<ImGuiTabItemFlags_> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(ImGuiTabItemFlags_), "ImGuiTabItemFlags_", enum_count<ImGuiTabItemFlags_>());
		return &i;
	}
};

enum ImGuiFocusedFlags_ : int;
template <> struct Enum_Info<ImGuiFocusedFlags_> {
	static constexpr size_t get_count() { return 5; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 4; }
	static const ImGuiFocusedFlags_ index_value(s64 index) {
		static const ImGuiFocusedFlags_ enum_values[] = {
			(ImGuiFocusedFlags_)0,
			(ImGuiFocusedFlags_)1,
			(ImGuiFocusedFlags_)2,
			(ImGuiFocusedFlags_)4,
			(ImGuiFocusedFlags_)3,
		};
		assert(index >= 0 && index < static_count(enum_values));
		return enum_values[index];
	}
	static const String string(ImGuiFocusedFlags_ value) {
		if (0 == value) return "ImGuiFocusedFlags_None";
		if (1 == value) return "ImGuiFocusedFlags_ChildWindows";
		if (2 == value) return "ImGuiFocusedFlags_RootWindow";
		if (4 == value) return "ImGuiFocusedFlags_AnyWindow";
		if (3 == value) return "ImGuiFocusedFlags_RootAndChildWindows";

		invalid_code_path();
		return "";
	}
};

template <> struct Reflect<ImGuiFocusedFlags_> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(ImGuiFocusedFlags_), "ImGuiFocusedFlags_", enum_count<ImGuiFocusedFlags_>());
		return &i;
	}
};

enum ImGuiHoveredFlags_ : int;
template <> struct Enum_Info<ImGuiHoveredFlags_> {
	static constexpr size_t get_count() { return 10; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 128; }
	static const ImGuiHoveredFlags_ index_value(s64 index) {
		static const ImGuiHoveredFlags_ enum_values[] = {
			(ImGuiHoveredFlags_)0,
			(ImGuiHoveredFlags_)1,
			(ImGuiHoveredFlags_)2,
			(ImGuiHoveredFlags_)4,
			(ImGuiHoveredFlags_)8,
			(ImGuiHoveredFlags_)32,
			(ImGuiHoveredFlags_)64,
			(ImGuiHoveredFlags_)128,
			(ImGuiHoveredFlags_)104,
			(ImGuiHoveredFlags_)3,
		};
		assert(index >= 0 && index < static_count(enum_values));
		return enum_values[index];
	}
	static const String string(ImGuiHoveredFlags_ value) {
		if (0 == value) return "ImGuiHoveredFlags_None";
		if (1 == value) return "ImGuiHoveredFlags_ChildWindows";
		if (2 == value) return "ImGuiHoveredFlags_RootWindow";
		if (4 == value) return "ImGuiHoveredFlags_AnyWindow";
		if (8 == value) return "ImGuiHoveredFlags_AllowWhenBlockedByPopup";
		if (32 == value) return "ImGuiHoveredFlags_AllowWhenBlockedByActiveItem";
		if (64 == value) return "ImGuiHoveredFlags_AllowWhenOverlapped";
		if (128 == value) return "ImGuiHoveredFlags_AllowWhenDisabled";
		if (104 == value) return "ImGuiHoveredFlags_RectOnly";
		if (3 == value) return "ImGuiHoveredFlags_RootAndChildWindows";

		invalid_code_path();
		return "";
	}
};

template <> struct Reflect<ImGuiHoveredFlags_> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(ImGuiHoveredFlags_), "ImGuiHoveredFlags_", enum_count<ImGuiHoveredFlags_>());
		return &i;
	}
};

enum ImGuiDockNodeFlags_ : int;
template <> struct Enum_Info<ImGuiDockNodeFlags_> {
	static constexpr size_t get_count() { return 7; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 64; }
	static const ImGuiDockNodeFlags_ index_value(s64 index) {
		static const ImGuiDockNodeFlags_ enum_values[] = {
			(ImGuiDockNodeFlags_)0,
			(ImGuiDockNodeFlags_)1,
			(ImGuiDockNodeFlags_)4,
			(ImGuiDockNodeFlags_)8,
			(ImGuiDockNodeFlags_)16,
			(ImGuiDockNodeFlags_)32,
			(ImGuiDockNodeFlags_)64,
		};
		assert(index >= 0 && index < static_count(enum_values));
		return enum_values[index];
	}
	static const String string(ImGuiDockNodeFlags_ value) {
		if (0 == value) return "ImGuiDockNodeFlags_None";
		if (1 == value) return "ImGuiDockNodeFlags_KeepAliveOnly";
		if (4 == value) return "ImGuiDockNodeFlags_NoDockingInCentralNode";
		if (8 == value) return "ImGuiDockNodeFlags_PassthruCentralNode";
		if (16 == value) return "ImGuiDockNodeFlags_NoSplit";
		if (32 == value) return "ImGuiDockNodeFlags_NoResize";
		if (64 == value) return "ImGuiDockNodeFlags_AutoHideTabBar";

		invalid_code_path();
		return "";
	}
};

template <> struct Reflect<ImGuiDockNodeFlags_> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(ImGuiDockNodeFlags_), "ImGuiDockNodeFlags_", enum_count<ImGuiDockNodeFlags_>());
		return &i;
	}
};

enum ImGuiDragDropFlags_ : int;
template <> struct Enum_Info<ImGuiDragDropFlags_> {
	static constexpr size_t get_count() { return 11; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 4096; }
	static const ImGuiDragDropFlags_ index_value(s64 index) {
		static const ImGuiDragDropFlags_ enum_values[] = {
			(ImGuiDragDropFlags_)0,
			(ImGuiDragDropFlags_)1,
			(ImGuiDragDropFlags_)2,
			(ImGuiDragDropFlags_)4,
			(ImGuiDragDropFlags_)8,
			(ImGuiDragDropFlags_)16,
			(ImGuiDragDropFlags_)32,
			(ImGuiDragDropFlags_)1024,
			(ImGuiDragDropFlags_)2048,
			(ImGuiDragDropFlags_)4096,
			(ImGuiDragDropFlags_)3072,
		};
		assert(index >= 0 && index < static_count(enum_values));
		return enum_values[index];
	}
	static const String string(ImGuiDragDropFlags_ value) {
		if (0 == value) return "ImGuiDragDropFlags_None";
		if (1 == value) return "ImGuiDragDropFlags_SourceNoPreviewTooltip";
		if (2 == value) return "ImGuiDragDropFlags_SourceNoDisableHover";
		if (4 == value) return "ImGuiDragDropFlags_SourceNoHoldToOpenOthers";
		if (8 == value) return "ImGuiDragDropFlags_SourceAllowNullID";
		if (16 == value) return "ImGuiDragDropFlags_SourceExtern";
		if (32 == value) return "ImGuiDragDropFlags_SourceAutoExpirePayload";
		if (1024 == value) return "ImGuiDragDropFlags_AcceptBeforeDelivery";
		if (2048 == value) return "ImGuiDragDropFlags_AcceptNoDrawDefaultRect";
		if (4096 == value) return "ImGuiDragDropFlags_AcceptNoPreviewTooltip";
		if (3072 == value) return "ImGuiDragDropFlags_AcceptPeekOnly";

		invalid_code_path();
		return "";
	}
};

template <> struct Reflect<ImGuiDragDropFlags_> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(ImGuiDragDropFlags_), "ImGuiDragDropFlags_", enum_count<ImGuiDragDropFlags_>());
		return &i;
	}
};

enum ImGuiDataType_ : int;
template <> struct Enum_Info<ImGuiDataType_> {
	static constexpr size_t get_count() { return 11; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 10; }
	static const ImGuiDataType_ index_value(s64 index) { return (ImGuiDataType_)(index + 0); }
	static const String string(ImGuiDataType_ value) {
		static const String strings[] = {
			"ImGuiDataType_S8",
			"ImGuiDataType_U8",
			"ImGuiDataType_S16",
			"ImGuiDataType_U16",
			"ImGuiDataType_S32",
			"ImGuiDataType_U32",
			"ImGuiDataType_S64",
			"ImGuiDataType_U64",
			"ImGuiDataType_Float",
			"ImGuiDataType_Double",
			"ImGuiDataType_COUNT",
		};
		return strings[value - 0];
	}
};

template <> struct Reflect<ImGuiDataType_> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(ImGuiDataType_), "ImGuiDataType_", enum_count<ImGuiDataType_>());
		return &i;
	}
};

enum ImGuiDir_ : int;
template <> struct Enum_Info<ImGuiDir_> {
	static constexpr size_t get_count() { return 6; }
	static constexpr s64 get_min_value() { return -1; }
	static constexpr s64 get_max_value() { return 4; }
	static const ImGuiDir_ index_value(s64 index) { return (ImGuiDir_)(index + -1); }
	static const String string(ImGuiDir_ value) {
		static const String strings[] = {
			"ImGuiDir_None",
			"ImGuiDir_Left",
			"ImGuiDir_Right",
			"ImGuiDir_Up",
			"ImGuiDir_Down",
			"ImGuiDir_COUNT",
		};
		return strings[value - -1];
	}
};

template <> struct Reflect<ImGuiDir_> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(ImGuiDir_), "ImGuiDir_", enum_count<ImGuiDir_>());
		return &i;
	}
};

enum ImGuiKey_ : int;
template <> struct Enum_Info<ImGuiKey_> {
	static constexpr size_t get_count() { return 23; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 22; }
	static const ImGuiKey_ index_value(s64 index) { return (ImGuiKey_)(index + 0); }
	static const String string(ImGuiKey_ value) {
		static const String strings[] = {
			"ImGuiKey_Tab",
			"ImGuiKey_LeftArrow",
			"ImGuiKey_RightArrow",
			"ImGuiKey_UpArrow",
			"ImGuiKey_DownArrow",
			"ImGuiKey_PageUp",
			"ImGuiKey_PageDown",
			"ImGuiKey_Home",
			"ImGuiKey_End",
			"ImGuiKey_Insert",
			"ImGuiKey_Delete",
			"ImGuiKey_Backspace",
			"ImGuiKey_Space",
			"ImGuiKey_Enter",
			"ImGuiKey_Escape",
			"ImGuiKey_KeyPadEnter",
			"ImGuiKey_A",
			"ImGuiKey_C",
			"ImGuiKey_V",
			"ImGuiKey_X",
			"ImGuiKey_Y",
			"ImGuiKey_Z",
			"ImGuiKey_COUNT",
		};
		return strings[value - 0];
	}
};

template <> struct Reflect<ImGuiKey_> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(ImGuiKey_), "ImGuiKey_", enum_count<ImGuiKey_>());
		return &i;
	}
};

enum ImGuiKeyModFlags_ : int;
template <> struct Enum_Info<ImGuiKeyModFlags_> {
	static constexpr size_t get_count() { return 5; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 8; }
	static const ImGuiKeyModFlags_ index_value(s64 index) {
		static const ImGuiKeyModFlags_ enum_values[] = {
			(ImGuiKeyModFlags_)0,
			(ImGuiKeyModFlags_)1,
			(ImGuiKeyModFlags_)2,
			(ImGuiKeyModFlags_)4,
			(ImGuiKeyModFlags_)8,
		};
		assert(index >= 0 && index < static_count(enum_values));
		return enum_values[index];
	}
	static const String string(ImGuiKeyModFlags_ value) {
		if (0 == value) return "ImGuiKeyModFlags_None";
		if (1 == value) return "ImGuiKeyModFlags_Ctrl";
		if (2 == value) return "ImGuiKeyModFlags_Shift";
		if (4 == value) return "ImGuiKeyModFlags_Alt";
		if (8 == value) return "ImGuiKeyModFlags_Super";

		invalid_code_path();
		return "";
	}
};

template <> struct Reflect<ImGuiKeyModFlags_> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(ImGuiKeyModFlags_), "ImGuiKeyModFlags_", enum_count<ImGuiKeyModFlags_>());
		return &i;
	}
};

enum ImGuiNavInput_ : int;
template <> struct Enum_Info<ImGuiNavInput_> {
	static constexpr size_t get_count() { return 23; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 21; }
	static const ImGuiNavInput_ index_value(s64 index) {
		static const ImGuiNavInput_ enum_values[] = {
			(ImGuiNavInput_)0,
			(ImGuiNavInput_)1,
			(ImGuiNavInput_)2,
			(ImGuiNavInput_)3,
			(ImGuiNavInput_)4,
			(ImGuiNavInput_)5,
			(ImGuiNavInput_)6,
			(ImGuiNavInput_)7,
			(ImGuiNavInput_)8,
			(ImGuiNavInput_)9,
			(ImGuiNavInput_)10,
			(ImGuiNavInput_)11,
			(ImGuiNavInput_)12,
			(ImGuiNavInput_)13,
			(ImGuiNavInput_)14,
			(ImGuiNavInput_)15,
			(ImGuiNavInput_)16,
			(ImGuiNavInput_)17,
			(ImGuiNavInput_)18,
			(ImGuiNavInput_)19,
			(ImGuiNavInput_)20,
			(ImGuiNavInput_)21,
			(ImGuiNavInput_)16,
		};
		assert(index >= 0 && index < static_count(enum_values));
		return enum_values[index];
	}
	static const String string(ImGuiNavInput_ value) {
		if (0 == value) return "ImGuiNavInput_Activate";
		if (1 == value) return "ImGuiNavInput_Cancel";
		if (2 == value) return "ImGuiNavInput_Input";
		if (3 == value) return "ImGuiNavInput_Menu";
		if (4 == value) return "ImGuiNavInput_DpadLeft";
		if (5 == value) return "ImGuiNavInput_DpadRight";
		if (6 == value) return "ImGuiNavInput_DpadUp";
		if (7 == value) return "ImGuiNavInput_DpadDown";
		if (8 == value) return "ImGuiNavInput_LStickLeft";
		if (9 == value) return "ImGuiNavInput_LStickRight";
		if (10 == value) return "ImGuiNavInput_LStickUp";
		if (11 == value) return "ImGuiNavInput_LStickDown";
		if (12 == value) return "ImGuiNavInput_FocusPrev";
		if (13 == value) return "ImGuiNavInput_FocusNext";
		if (14 == value) return "ImGuiNavInput_TweakSlow";
		if (15 == value) return "ImGuiNavInput_TweakFast";
		if (16 == value) return "ImGuiNavInput_KeyMenu_";
		if (17 == value) return "ImGuiNavInput_KeyLeft_";
		if (18 == value) return "ImGuiNavInput_KeyRight_";
		if (19 == value) return "ImGuiNavInput_KeyUp_";
		if (20 == value) return "ImGuiNavInput_KeyDown_";
		if (21 == value) return "ImGuiNavInput_COUNT";
		if (16 == value) return "ImGuiNavInput_InternalStart_";

		invalid_code_path();
		return "";
	}
};

template <> struct Reflect<ImGuiNavInput_> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(ImGuiNavInput_), "ImGuiNavInput_", enum_count<ImGuiNavInput_>());
		return &i;
	}
};

enum ImGuiConfigFlags_ : int;
template <> struct Enum_Info<ImGuiConfigFlags_> {
	static constexpr size_t get_count() { return 13; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 2097152; }
	static const ImGuiConfigFlags_ index_value(s64 index) {
		static const ImGuiConfigFlags_ enum_values[] = {
			(ImGuiConfigFlags_)0,
			(ImGuiConfigFlags_)1,
			(ImGuiConfigFlags_)2,
			(ImGuiConfigFlags_)4,
			(ImGuiConfigFlags_)8,
			(ImGuiConfigFlags_)16,
			(ImGuiConfigFlags_)32,
			(ImGuiConfigFlags_)64,
			(ImGuiConfigFlags_)1024,
			(ImGuiConfigFlags_)16384,
			(ImGuiConfigFlags_)32768,
			(ImGuiConfigFlags_)1048576,
			(ImGuiConfigFlags_)2097152,
		};
		assert(index >= 0 && index < static_count(enum_values));
		return enum_values[index];
	}
	static const String string(ImGuiConfigFlags_ value) {
		if (0 == value) return "ImGuiConfigFlags_None";
		if (1 == value) return "ImGuiConfigFlags_NavEnableKeyboard";
		if (2 == value) return "ImGuiConfigFlags_NavEnableGamepad";
		if (4 == value) return "ImGuiConfigFlags_NavEnableSetMousePos";
		if (8 == value) return "ImGuiConfigFlags_NavNoCaptureKeyboard";
		if (16 == value) return "ImGuiConfigFlags_NoMouse";
		if (32 == value) return "ImGuiConfigFlags_NoMouseCursorChange";
		if (64 == value) return "ImGuiConfigFlags_DockingEnable";
		if (1024 == value) return "ImGuiConfigFlags_ViewportsEnable";
		if (16384 == value) return "ImGuiConfigFlags_DpiEnableScaleViewports";
		if (32768 == value) return "ImGuiConfigFlags_DpiEnableScaleFonts";
		if (1048576 == value) return "ImGuiConfigFlags_IsSRGB";
		if (2097152 == value) return "ImGuiConfigFlags_IsTouchScreen";

		invalid_code_path();
		return "";
	}
};

template <> struct Reflect<ImGuiConfigFlags_> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(ImGuiConfigFlags_), "ImGuiConfigFlags_", enum_count<ImGuiConfigFlags_>());
		return &i;
	}
};

enum ImGuiBackendFlags_ : int;
template <> struct Enum_Info<ImGuiBackendFlags_> {
	static constexpr size_t get_count() { return 8; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 4096; }
	static const ImGuiBackendFlags_ index_value(s64 index) {
		static const ImGuiBackendFlags_ enum_values[] = {
			(ImGuiBackendFlags_)0,
			(ImGuiBackendFlags_)1,
			(ImGuiBackendFlags_)2,
			(ImGuiBackendFlags_)4,
			(ImGuiBackendFlags_)8,
			(ImGuiBackendFlags_)1024,
			(ImGuiBackendFlags_)2048,
			(ImGuiBackendFlags_)4096,
		};
		assert(index >= 0 && index < static_count(enum_values));
		return enum_values[index];
	}
	static const String string(ImGuiBackendFlags_ value) {
		if (0 == value) return "ImGuiBackendFlags_None";
		if (1 == value) return "ImGuiBackendFlags_HasGamepad";
		if (2 == value) return "ImGuiBackendFlags_HasMouseCursors";
		if (4 == value) return "ImGuiBackendFlags_HasSetMousePos";
		if (8 == value) return "ImGuiBackendFlags_RendererHasVtxOffset";
		if (1024 == value) return "ImGuiBackendFlags_PlatformHasViewports";
		if (2048 == value) return "ImGuiBackendFlags_HasMouseHoveredViewport";
		if (4096 == value) return "ImGuiBackendFlags_RendererHasViewports";

		invalid_code_path();
		return "";
	}
};

template <> struct Reflect<ImGuiBackendFlags_> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(ImGuiBackendFlags_), "ImGuiBackendFlags_", enum_count<ImGuiBackendFlags_>());
		return &i;
	}
};

enum ImGuiCol_ : int;
template <> struct Enum_Info<ImGuiCol_> {
	static constexpr size_t get_count() { return 51; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 50; }
	static const ImGuiCol_ index_value(s64 index) { return (ImGuiCol_)(index + 0); }
	static const String string(ImGuiCol_ value) {
		static const String strings[] = {
			"ImGuiCol_Text",
			"ImGuiCol_TextDisabled",
			"ImGuiCol_WindowBg",
			"ImGuiCol_ChildBg",
			"ImGuiCol_PopupBg",
			"ImGuiCol_Border",
			"ImGuiCol_BorderShadow",
			"ImGuiCol_FrameBg",
			"ImGuiCol_FrameBgHovered",
			"ImGuiCol_FrameBgActive",
			"ImGuiCol_TitleBg",
			"ImGuiCol_TitleBgActive",
			"ImGuiCol_TitleBgCollapsed",
			"ImGuiCol_MenuBarBg",
			"ImGuiCol_ScrollbarBg",
			"ImGuiCol_ScrollbarGrab",
			"ImGuiCol_ScrollbarGrabHovered",
			"ImGuiCol_ScrollbarGrabActive",
			"ImGuiCol_CheckMark",
			"ImGuiCol_SliderGrab",
			"ImGuiCol_SliderGrabActive",
			"ImGuiCol_Button",
			"ImGuiCol_ButtonHovered",
			"ImGuiCol_ButtonActive",
			"ImGuiCol_Header",
			"ImGuiCol_HeaderHovered",
			"ImGuiCol_HeaderActive",
			"ImGuiCol_Separator",
			"ImGuiCol_SeparatorHovered",
			"ImGuiCol_SeparatorActive",
			"ImGuiCol_ResizeGrip",
			"ImGuiCol_ResizeGripHovered",
			"ImGuiCol_ResizeGripActive",
			"ImGuiCol_Tab",
			"ImGuiCol_TabHovered",
			"ImGuiCol_TabActive",
			"ImGuiCol_TabUnfocused",
			"ImGuiCol_TabUnfocusedActive",
			"ImGuiCol_DockingPreview",
			"ImGuiCol_DockingEmptyBg",
			"ImGuiCol_PlotLines",
			"ImGuiCol_PlotLinesHovered",
			"ImGuiCol_PlotHistogram",
			"ImGuiCol_PlotHistogramHovered",
			"ImGuiCol_TextSelectedBg",
			"ImGuiCol_DragDropTarget",
			"ImGuiCol_NavHighlight",
			"ImGuiCol_NavWindowingHighlight",
			"ImGuiCol_NavWindowingDimBg",
			"ImGuiCol_ModalWindowDimBg",
			"ImGuiCol_COUNT",
		};
		return strings[value - 0];
	}
};

template <> struct Reflect<ImGuiCol_> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(ImGuiCol_), "ImGuiCol_", enum_count<ImGuiCol_>());
		return &i;
	}
};

enum ImGuiStyleVar_ : int;
template <> struct Enum_Info<ImGuiStyleVar_> {
	static constexpr size_t get_count() { return 24; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 23; }
	static const ImGuiStyleVar_ index_value(s64 index) { return (ImGuiStyleVar_)(index + 0); }
	static const String string(ImGuiStyleVar_ value) {
		static const String strings[] = {
			"ImGuiStyleVar_Alpha",
			"ImGuiStyleVar_WindowPadding",
			"ImGuiStyleVar_WindowRounding",
			"ImGuiStyleVar_WindowBorderSize",
			"ImGuiStyleVar_WindowMinSize",
			"ImGuiStyleVar_WindowTitleAlign",
			"ImGuiStyleVar_ChildRounding",
			"ImGuiStyleVar_ChildBorderSize",
			"ImGuiStyleVar_PopupRounding",
			"ImGuiStyleVar_PopupBorderSize",
			"ImGuiStyleVar_FramePadding",
			"ImGuiStyleVar_FrameRounding",
			"ImGuiStyleVar_FrameBorderSize",
			"ImGuiStyleVar_ItemSpacing",
			"ImGuiStyleVar_ItemInnerSpacing",
			"ImGuiStyleVar_IndentSpacing",
			"ImGuiStyleVar_ScrollbarSize",
			"ImGuiStyleVar_ScrollbarRounding",
			"ImGuiStyleVar_GrabMinSize",
			"ImGuiStyleVar_GrabRounding",
			"ImGuiStyleVar_TabRounding",
			"ImGuiStyleVar_ButtonTextAlign",
			"ImGuiStyleVar_SelectableTextAlign",
			"ImGuiStyleVar_COUNT",
		};
		return strings[value - 0];
	}
};

template <> struct Reflect<ImGuiStyleVar_> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(ImGuiStyleVar_), "ImGuiStyleVar_", enum_count<ImGuiStyleVar_>());
		return &i;
	}
};

enum ImGuiColorEditFlags_ : int;
template <> struct Enum_Info<ImGuiColorEditFlags_> {
	static constexpr size_t get_count() { return 29; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 402653184; }
	static const ImGuiColorEditFlags_ index_value(s64 index) {
		static const ImGuiColorEditFlags_ enum_values[] = {
			(ImGuiColorEditFlags_)0,
			(ImGuiColorEditFlags_)2,
			(ImGuiColorEditFlags_)4,
			(ImGuiColorEditFlags_)8,
			(ImGuiColorEditFlags_)16,
			(ImGuiColorEditFlags_)32,
			(ImGuiColorEditFlags_)64,
			(ImGuiColorEditFlags_)128,
			(ImGuiColorEditFlags_)256,
			(ImGuiColorEditFlags_)512,
			(ImGuiColorEditFlags_)1024,
			(ImGuiColorEditFlags_)65536,
			(ImGuiColorEditFlags_)131072,
			(ImGuiColorEditFlags_)262144,
			(ImGuiColorEditFlags_)524288,
			(ImGuiColorEditFlags_)1048576,
			(ImGuiColorEditFlags_)2097152,
			(ImGuiColorEditFlags_)4194304,
			(ImGuiColorEditFlags_)8388608,
			(ImGuiColorEditFlags_)16777216,
			(ImGuiColorEditFlags_)33554432,
			(ImGuiColorEditFlags_)67108864,
			(ImGuiColorEditFlags_)134217728,
			(ImGuiColorEditFlags_)268435456,
			(ImGuiColorEditFlags_)177209344,
			(ImGuiColorEditFlags_)7340032,
			(ImGuiColorEditFlags_)25165824,
			(ImGuiColorEditFlags_)100663296,
			(ImGuiColorEditFlags_)402653184,
		};
		assert(index >= 0 && index < static_count(enum_values));
		return enum_values[index];
	}
	static const String string(ImGuiColorEditFlags_ value) {
		if (0 == value) return "ImGuiColorEditFlags_None";
		if (2 == value) return "ImGuiColorEditFlags_NoAlpha";
		if (4 == value) return "ImGuiColorEditFlags_NoPicker";
		if (8 == value) return "ImGuiColorEditFlags_NoOptions";
		if (16 == value) return "ImGuiColorEditFlags_NoSmallPreview";
		if (32 == value) return "ImGuiColorEditFlags_NoInputs";
		if (64 == value) return "ImGuiColorEditFlags_NoTooltip";
		if (128 == value) return "ImGuiColorEditFlags_NoLabel";
		if (256 == value) return "ImGuiColorEditFlags_NoSidePreview";
		if (512 == value) return "ImGuiColorEditFlags_NoDragDrop";
		if (1024 == value) return "ImGuiColorEditFlags_NoBorder";
		if (65536 == value) return "ImGuiColorEditFlags_AlphaBar";
		if (131072 == value) return "ImGuiColorEditFlags_AlphaPreview";
		if (262144 == value) return "ImGuiColorEditFlags_AlphaPreviewHalf";
		if (524288 == value) return "ImGuiColorEditFlags_HDR";
		if (1048576 == value) return "ImGuiColorEditFlags_DisplayRGB";
		if (2097152 == value) return "ImGuiColorEditFlags_DisplayHSV";
		if (4194304 == value) return "ImGuiColorEditFlags_DisplayHex";
		if (8388608 == value) return "ImGuiColorEditFlags_Uint8";
		if (16777216 == value) return "ImGuiColorEditFlags_Float";
		if (33554432 == value) return "ImGuiColorEditFlags_PickerHueBar";
		if (67108864 == value) return "ImGuiColorEditFlags_PickerHueWheel";
		if (134217728 == value) return "ImGuiColorEditFlags_InputRGB";
		if (268435456 == value) return "ImGuiColorEditFlags_InputHSV";
		if (177209344 == value) return "ImGuiColorEditFlags__OptionsDefault";
		if (7340032 == value) return "ImGuiColorEditFlags__DisplayMask";
		if (25165824 == value) return "ImGuiColorEditFlags__DataTypeMask";
		if (100663296 == value) return "ImGuiColorEditFlags__PickerMask";
		if (402653184 == value) return "ImGuiColorEditFlags__InputMask";

		invalid_code_path();
		return "";
	}
};

template <> struct Reflect<ImGuiColorEditFlags_> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(ImGuiColorEditFlags_), "ImGuiColorEditFlags_", enum_count<ImGuiColorEditFlags_>());
		return &i;
	}
};

enum ImGuiMouseButton_ : int;
template <> struct Enum_Info<ImGuiMouseButton_> {
	static constexpr size_t get_count() { return 4; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 5; }
	static const ImGuiMouseButton_ index_value(s64 index) {
		static const ImGuiMouseButton_ enum_values[] = {
			(ImGuiMouseButton_)0,
			(ImGuiMouseButton_)1,
			(ImGuiMouseButton_)2,
			(ImGuiMouseButton_)5,
		};
		assert(index >= 0 && index < static_count(enum_values));
		return enum_values[index];
	}
	static const String string(ImGuiMouseButton_ value) {
		if (0 == value) return "ImGuiMouseButton_Left";
		if (1 == value) return "ImGuiMouseButton_Right";
		if (2 == value) return "ImGuiMouseButton_Middle";
		if (5 == value) return "ImGuiMouseButton_COUNT";

		invalid_code_path();
		return "";
	}
};

template <> struct Reflect<ImGuiMouseButton_> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(ImGuiMouseButton_), "ImGuiMouseButton_", enum_count<ImGuiMouseButton_>());
		return &i;
	}
};

enum ImGuiMouseCursor_ : int;
template <> struct Enum_Info<ImGuiMouseCursor_> {
	static constexpr size_t get_count() { return 11; }
	static constexpr s64 get_min_value() { return -1; }
	static constexpr s64 get_max_value() { return 9; }
	static const ImGuiMouseCursor_ index_value(s64 index) { return (ImGuiMouseCursor_)(index + -1); }
	static const String string(ImGuiMouseCursor_ value) {
		static const String strings[] = {
			"ImGuiMouseCursor_None",
			"ImGuiMouseCursor_Arrow",
			"ImGuiMouseCursor_TextInput",
			"ImGuiMouseCursor_ResizeAll",
			"ImGuiMouseCursor_ResizeNS",
			"ImGuiMouseCursor_ResizeEW",
			"ImGuiMouseCursor_ResizeNESW",
			"ImGuiMouseCursor_ResizeNWSE",
			"ImGuiMouseCursor_Hand",
			"ImGuiMouseCursor_NotAllowed",
			"ImGuiMouseCursor_COUNT",
		};
		return strings[value - -1];
	}
};

template <> struct Reflect<ImGuiMouseCursor_> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(ImGuiMouseCursor_), "ImGuiMouseCursor_", enum_count<ImGuiMouseCursor_>());
		return &i;
	}
};

enum ImGuiCond_ : int;
template <> struct Enum_Info<ImGuiCond_> {
	static constexpr size_t get_count() { return 4; }
	static constexpr s64 get_min_value() { return 1; }
	static constexpr s64 get_max_value() { return 8; }
	static const ImGuiCond_ index_value(s64 index) {
		static const ImGuiCond_ enum_values[] = {
			(ImGuiCond_)1,
			(ImGuiCond_)2,
			(ImGuiCond_)4,
			(ImGuiCond_)8,
		};
		assert(index >= 0 && index < static_count(enum_values));
		return enum_values[index];
	}
	static const String string(ImGuiCond_ value) {
		if (1 == value) return "ImGuiCond_Always";
		if (2 == value) return "ImGuiCond_Once";
		if (4 == value) return "ImGuiCond_FirstUseEver";
		if (8 == value) return "ImGuiCond_Appearing";

		invalid_code_path();
		return "";
	}
};

template <> struct Reflect<ImGuiCond_> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(ImGuiCond_), "ImGuiCond_", enum_count<ImGuiCond_>());
		return &i;
	}
};

enum ImDrawCornerFlags_ : int;
template <> struct Enum_Info<ImDrawCornerFlags_> {
	static constexpr size_t get_count() { return 10; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 15; }
	static const ImDrawCornerFlags_ index_value(s64 index) {
		static const ImDrawCornerFlags_ enum_values[] = {
			(ImDrawCornerFlags_)0,
			(ImDrawCornerFlags_)1,
			(ImDrawCornerFlags_)2,
			(ImDrawCornerFlags_)4,
			(ImDrawCornerFlags_)8,
			(ImDrawCornerFlags_)3,
			(ImDrawCornerFlags_)12,
			(ImDrawCornerFlags_)5,
			(ImDrawCornerFlags_)10,
			(ImDrawCornerFlags_)15,
		};
		assert(index >= 0 && index < static_count(enum_values));
		return enum_values[index];
	}
	static const String string(ImDrawCornerFlags_ value) {
		if (0 == value) return "ImDrawCornerFlags_None";
		if (1 == value) return "ImDrawCornerFlags_TopLeft";
		if (2 == value) return "ImDrawCornerFlags_TopRight";
		if (4 == value) return "ImDrawCornerFlags_BotLeft";
		if (8 == value) return "ImDrawCornerFlags_BotRight";
		if (3 == value) return "ImDrawCornerFlags_Top";
		if (12 == value) return "ImDrawCornerFlags_Bot";
		if (5 == value) return "ImDrawCornerFlags_Left";
		if (10 == value) return "ImDrawCornerFlags_Right";
		if (15 == value) return "ImDrawCornerFlags_All";

		invalid_code_path();
		return "";
	}
};

template <> struct Reflect<ImDrawCornerFlags_> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(ImDrawCornerFlags_), "ImDrawCornerFlags_", enum_count<ImDrawCornerFlags_>());
		return &i;
	}
};

enum ImDrawListFlags_ : int;
template <> struct Enum_Info<ImDrawListFlags_> {
	static constexpr size_t get_count() { return 4; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 4; }
	static const ImDrawListFlags_ index_value(s64 index) {
		static const ImDrawListFlags_ enum_values[] = {
			(ImDrawListFlags_)0,
			(ImDrawListFlags_)1,
			(ImDrawListFlags_)2,
			(ImDrawListFlags_)4,
		};
		assert(index >= 0 && index < static_count(enum_values));
		return enum_values[index];
	}
	static const String string(ImDrawListFlags_ value) {
		if (0 == value) return "ImDrawListFlags_None";
		if (1 == value) return "ImDrawListFlags_AntiAliasedLines";
		if (2 == value) return "ImDrawListFlags_AntiAliasedFill";
		if (4 == value) return "ImDrawListFlags_AllowVtxOffset";

		invalid_code_path();
		return "";
	}
};

template <> struct Reflect<ImDrawListFlags_> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(ImDrawListFlags_), "ImDrawListFlags_", enum_count<ImDrawListFlags_>());
		return &i;
	}
};

enum ImFontAtlasFlags_ : int;
template <> struct Enum_Info<ImFontAtlasFlags_> {
	static constexpr size_t get_count() { return 3; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 2; }
	static const ImFontAtlasFlags_ index_value(s64 index) { return (ImFontAtlasFlags_)(index + 0); }
	static const String string(ImFontAtlasFlags_ value) {
		static const String strings[] = {
			"ImFontAtlasFlags_None",
			"ImFontAtlasFlags_NoPowerOfTwoHeight",
			"ImFontAtlasFlags_NoMouseCursors",
		};
		return strings[value - 0];
	}
};

template <> struct Reflect<ImFontAtlasFlags_> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(ImFontAtlasFlags_), "ImFontAtlasFlags_", enum_count<ImFontAtlasFlags_>());
		return &i;
	}
};

enum ImGuiViewportFlags_ : int;
template <> struct Enum_Info<ImGuiViewportFlags_> {
	static constexpr size_t get_count() { return 11; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 512; }
	static const ImGuiViewportFlags_ index_value(s64 index) {
		static const ImGuiViewportFlags_ enum_values[] = {
			(ImGuiViewportFlags_)0,
			(ImGuiViewportFlags_)1,
			(ImGuiViewportFlags_)2,
			(ImGuiViewportFlags_)4,
			(ImGuiViewportFlags_)8,
			(ImGuiViewportFlags_)16,
			(ImGuiViewportFlags_)32,
			(ImGuiViewportFlags_)64,
			(ImGuiViewportFlags_)128,
			(ImGuiViewportFlags_)256,
			(ImGuiViewportFlags_)512,
		};
		assert(index >= 0 && index < static_count(enum_values));
		return enum_values[index];
	}
	static const String string(ImGuiViewportFlags_ value) {
		if (0 == value) return "ImGuiViewportFlags_None";
		if (1 == value) return "ImGuiViewportFlags_NoDecoration";
		if (2 == value) return "ImGuiViewportFlags_NoTaskBarIcon";
		if (4 == value) return "ImGuiViewportFlags_NoFocusOnAppearing";
		if (8 == value) return "ImGuiViewportFlags_NoFocusOnClick";
		if (16 == value) return "ImGuiViewportFlags_NoInputs";
		if (32 == value) return "ImGuiViewportFlags_NoRendererClear";
		if (64 == value) return "ImGuiViewportFlags_TopMost";
		if (128 == value) return "ImGuiViewportFlags_Minimized";
		if (256 == value) return "ImGuiViewportFlags_NoAutoMerge";
		if (512 == value) return "ImGuiViewportFlags_CanHostOtherWindows";

		invalid_code_path();
		return "";
	}
};

template <> struct Reflect<ImGuiViewportFlags_> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(ImGuiViewportFlags_), "ImGuiViewportFlags_", enum_count<ImGuiViewportFlags_>());
		return &i;
	}
};

enum Ucd_Script : uint32_t;
template <> struct Enum_Info<Ucd_Script> {
	static constexpr size_t get_count() { return 158; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 157; }
	static const Ucd_Script index_value(s64 index) { return (Ucd_Script)(index + 0); }
	static const String string(Ucd_Script value) {
		static const String strings[] = {
			"Ucd_Script_UNKNOWN",
			"Ucd_Script_COMMON",
			"Ucd_Script_LATIN",
			"Ucd_Script_GREEK",
			"Ucd_Script_CYRILLIC",
			"Ucd_Script_ARMENIAN",
			"Ucd_Script_HEBREW",
			"Ucd_Script_ARABIC",
			"Ucd_Script_SYRIAC",
			"Ucd_Script_THAANA",
			"Ucd_Script_DEVANAGARI",
			"Ucd_Script_BENGALI",
			"Ucd_Script_GURMUKHI",
			"Ucd_Script_GUJARATI",
			"Ucd_Script_ORIYA",
			"Ucd_Script_TAMIL",
			"Ucd_Script_TELUGU",
			"Ucd_Script_KANNADA",
			"Ucd_Script_MALAYALAM",
			"Ucd_Script_SINHALA",
			"Ucd_Script_THAI",
			"Ucd_Script_LAO",
			"Ucd_Script_TIBETAN",
			"Ucd_Script_MYANMAR",
			"Ucd_Script_GEORGIAN",
			"Ucd_Script_HANGUL",
			"Ucd_Script_ETHIOPIC",
			"Ucd_Script_CHEROKEE",
			"Ucd_Script_CANADIAN_ABORIGINAL",
			"Ucd_Script_OGHAM",
			"Ucd_Script_RUNIC",
			"Ucd_Script_KHMER",
			"Ucd_Script_MONGOLIAN",
			"Ucd_Script_HIRAGANA",
			"Ucd_Script_KATAKANA",
			"Ucd_Script_BOPOMOFO",
			"Ucd_Script_HAN",
			"Ucd_Script_YI",
			"Ucd_Script_OLD_ITALIC",
			"Ucd_Script_GOTHIC",
			"Ucd_Script_DESERET",
			"Ucd_Script_INHERITED",
			"Ucd_Script_TAGALOG",
			"Ucd_Script_HANUNOO",
			"Ucd_Script_BUHID",
			"Ucd_Script_TAGBANWA",
			"Ucd_Script_LIMBU",
			"Ucd_Script_TAI_LE",
			"Ucd_Script_LINEAR_B",
			"Ucd_Script_UGARITIC",
			"Ucd_Script_SHAVIAN",
			"Ucd_Script_OSMANYA",
			"Ucd_Script_CYPRIOT",
			"Ucd_Script_BRAILLE",
			"Ucd_Script_BUGINESE",
			"Ucd_Script_COPTIC",
			"Ucd_Script_NEW_TAI_LUE",
			"Ucd_Script_GLAGOLITIC",
			"Ucd_Script_TIFINAGH",
			"Ucd_Script_SYLOTI_NAGRI",
			"Ucd_Script_OLD_PERSIAN",
			"Ucd_Script_KHAROSHTHI",
			"Ucd_Script_BALINESE",
			"Ucd_Script_CUNEIFORM",
			"Ucd_Script_PHOENICIAN",
			"Ucd_Script_PHAGS_PA",
			"Ucd_Script_NKO",
			"Ucd_Script_SUNDANESE",
			"Ucd_Script_LEPCHA",
			"Ucd_Script_OL_CHIKI",
			"Ucd_Script_VAI",
			"Ucd_Script_SAURASHTRA",
			"Ucd_Script_KAYAH_LI",
			"Ucd_Script_REJANG",
			"Ucd_Script_LYCIAN",
			"Ucd_Script_CARIAN",
			"Ucd_Script_LYDIAN",
			"Ucd_Script_CHAM",
			"Ucd_Script_TAI_THAM",
			"Ucd_Script_TAI_VIET",
			"Ucd_Script_AVESTAN",
			"Ucd_Script_EGYPTIAN_HIEROGLYPHS",
			"Ucd_Script_SAMARITAN",
			"Ucd_Script_LISU",
			"Ucd_Script_BAMUM",
			"Ucd_Script_JAVANESE",
			"Ucd_Script_MEETEI_MAYEK",
			"Ucd_Script_IMPERIAL_ARAMAIC",
			"Ucd_Script_OLD_SOUTH_ARABIAN",
			"Ucd_Script_INSCRIPTIONAL_PARTHIAN",
			"Ucd_Script_INSCRIPTIONAL_PAHLAVI",
			"Ucd_Script_OLD_TURKIC",
			"Ucd_Script_KAITHI",
			"Ucd_Script_BATAK",
			"Ucd_Script_BRAHMI",
			"Ucd_Script_MANDAIC",
			"Ucd_Script_CHAKMA",
			"Ucd_Script_MEROITIC_CURSIVE",
			"Ucd_Script_MEROITIC_HIEROGLYPHS",
			"Ucd_Script_MIAO",
			"Ucd_Script_SHARADA",
			"Ucd_Script_SORA_SOMPENG",
			"Ucd_Script_TAKRI",
			"Ucd_Script_CAUCASIAN_ALBANIAN",
			"Ucd_Script_BASSA_VAH",
			"Ucd_Script_DUPLOYAN",
			"Ucd_Script_ELBASAN",
			"Ucd_Script_GRANTHA",
			"Ucd_Script_PAHAWH_HMONG",
			"Ucd_Script_KHOJKI",
			"Ucd_Script_LINEAR_A",
			"Ucd_Script_MAHAJANI",
			"Ucd_Script_MANICHAEAN",
			"Ucd_Script_MENDE_KIKAKUI",
			"Ucd_Script_MODI",
			"Ucd_Script_MRO",
			"Ucd_Script_OLD_NORTH_ARABIAN",
			"Ucd_Script_NABATAEAN",
			"Ucd_Script_PALMYRENE",
			"Ucd_Script_PAU_CIN_HAU",
			"Ucd_Script_OLD_PERMIC",
			"Ucd_Script_PSALTER_PAHLAVI",
			"Ucd_Script_SIDDHAM",
			"Ucd_Script_KHUDAWADI",
			"Ucd_Script_TIRHUTA",
			"Ucd_Script_WARANG_CITI",
			"Ucd_Script_AHOM",
			"Ucd_Script_ANATOLIAN_HIEROGLYPHS",
			"Ucd_Script_HATRAN",
			"Ucd_Script_MULTANI",
			"Ucd_Script_OLD_HUNGARIAN",
			"Ucd_Script_SIGNWRITING",
			"Ucd_Script_ADLAM",
			"Ucd_Script_BHAIKSUKI",
			"Ucd_Script_MARCHEN",
			"Ucd_Script_NEWA",
			"Ucd_Script_OSAGE",
			"Ucd_Script_TANGUT",
			"Ucd_Script_MASARAM_GONDI",
			"Ucd_Script_NUSHU",
			"Ucd_Script_SOYOMBO",
			"Ucd_Script_ZANABAZAR_SQUARE",
			"Ucd_Script_DOGRA",
			"Ucd_Script_GUNJALA_GONDI",
			"Ucd_Script_MAKASAR",
			"Ucd_Script_MEDEFAIDRIN",
			"Ucd_Script_HANIFI_ROHINGYA",
			"Ucd_Script_SOGDIAN",
			"Ucd_Script_OLD_SOGDIAN",
			"Ucd_Script_ELYMAIC",
			"Ucd_Script_NANDINAGARI",
			"Ucd_Script_NYIAKENG_PUACHUE_HMONG",
			"Ucd_Script_WANCHO",
			"Ucd_Script_CHORASMIAN",
			"Ucd_Script_DIVES_AKURU",
			"Ucd_Script_KHITAN_SMALL_SCRIPT",
			"Ucd_Script_YEZIDI",
			"Ucd_Script_COUNT",
		};
		return strings[value - 0];
	}
};

template <> struct Reflect<Ucd_Script> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(Ucd_Script), "Ucd_Script", enum_count<Ucd_Script>());
		return &i;
	}
};

enum Ucd_Script_Ext : uint32_t;
template <> struct Enum_Info<Ucd_Script_Ext> {
	static constexpr size_t get_count() { return 59; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 58; }
	static const Ucd_Script_Ext index_value(s64 index) { return (Ucd_Script_Ext)(index + 0); }
	static const String string(Ucd_Script_Ext value) {
		static const String strings[] = {
			"Ucd_Script_Ext_UNKNOWN",
			"Ucd_Script_Ext_BENG",
			"Ucd_Script_Ext_DEVA",
			"Ucd_Script_Ext_DUPL",
			"Ucd_Script_Ext_GREK",
			"Ucd_Script_Ext_HANI",
			"Ucd_Script_Ext_LATN",
			"Ucd_Script_Ext_NAND",
			"Ucd_Script_Ext_ARAB_COPT",
			"Ucd_Script_Ext_ARAB_ROHG",
			"Ucd_Script_Ext_ARAB_SYRC",
			"Ucd_Script_Ext_ARAB_THAA",
			"Ucd_Script_Ext_BENG_DEVA",
			"Ucd_Script_Ext_BOPO_HANI",
			"Ucd_Script_Ext_BUGI_JAVA",
			"Ucd_Script_Ext_CPRT_LINB",
			"Ucd_Script_Ext_CYRL_GLAG",
			"Ucd_Script_Ext_CYRL_LATN",
			"Ucd_Script_Ext_CYRL_PERM",
			"Ucd_Script_Ext_CYRL_SYRC",
			"Ucd_Script_Ext_DEVA_GRAN",
			"Ucd_Script_Ext_DEVA_NAND",
			"Ucd_Script_Ext_DEVA_SHRD",
			"Ucd_Script_Ext_DEVA_TAML",
			"Ucd_Script_Ext_GEOR_LATN",
			"Ucd_Script_Ext_GRAN_TAML",
			"Ucd_Script_Ext_GUJR_KHOJ",
			"Ucd_Script_Ext_GURU_MULT",
			"Ucd_Script_Ext_HANI_LATN",
			"Ucd_Script_Ext_HIRA_KANA",
			"Ucd_Script_Ext_KNDA_NAND",
			"Ucd_Script_Ext_LATN_MONG",
			"Ucd_Script_Ext_MONG_PHAG",
			"Ucd_Script_Ext_ARAB_SYRC_THAA",
			"Ucd_Script_Ext_ARAB_THAA_YEZI",
			"Ucd_Script_Ext_BENG_CAKM_SYLO",
			"Ucd_Script_Ext_CAKM_MYMR_TALE",
			"Ucd_Script_Ext_CPRT_LINA_LINB",
			"Ucd_Script_Ext_DEVA_GRAN_KNDA",
			"Ucd_Script_Ext_DEVA_GRAN_LATN",
			"Ucd_Script_Ext_HANI_HIRA_KANA",
			"Ucd_Script_Ext_KALI_LATN_MYMR",
			"Ucd_Script_Ext_BENG_DEVA_GRAN_KNDA",
			"Ucd_Script_Ext_BUHD_HANO_TAGB_TGLG",
			"Ucd_Script_Ext_DEVA_DOGR_KTHI_MAHJ",
			"Ucd_Script_Ext_ARAB_ROHG_SYRC_THAA_YEZI",
			"Ucd_Script_Ext_BOPO_HANG_HANI_HIRA_KANA",
			"Ucd_Script_Ext_BOPO_HANG_HANI_HIRA_KANA_YIII",
			"Ucd_Script_Ext_DEVA_KNDA_MLYM_ORYA_TAML_TELU",
			"Ucd_Script_Ext_ADLM_ARAB_MAND_MANI_PHLP_ROHG_SOGD_SYRC",
			"Ucd_Script_Ext_BENG_DEVA_GRAN_KNDA_NAND_ORYA_TELU_TIRH",
			"Ucd_Script_Ext_DEVA_DOGR_GUJR_GURU_KHOJ_KTHI_MAHJ_MODI_SIND_TAKR_TIRH",
			"Ucd_Script_Ext_BENG_DEVA_GRAN_GUJR_GURU_KNDA_LATN_MLYM_ORYA_TAML_TELU_TIRH",
			"Ucd_Script_Ext_BENG_DEVA_GRAN_GUJR_GURU_KNDA_LATN_MLYM_ORYA_SHRD_TAML_TELU_TIRH",
			"Ucd_Script_Ext_DEVA_DOGR_GUJR_GURU_KHOJ_KNDA_KTHI_MAHJ_MODI_NAND_SIND_TAKR_TIRH",
			"Ucd_Script_Ext_DEVA_DOGR_GUJR_GURU_KHOJ_KNDA_KTHI_MAHJ_MLYM_MODI_NAND_SIND_TAKR_TIRH",
			"Ucd_Script_Ext_BENG_DEVA_DOGR_GONG_GONM_GRAN_GUJR_GURU_KNDA_MAHJ_MLYM_NAND_ORYA_SIND_SINH_SYLO_TAKR_TAML_TELU_TIRH",
			"Ucd_Script_Ext_BENG_DEVA_DOGR_GONG_GONM_GRAN_GUJR_GURU_KNDA_LIMB_MAHJ_MLYM_NAND_ORYA_SIND_SINH_SYLO_TAKR_TAML_TELU_TIRH",
			"Ucd_Script_Ext_COUNT",
		};
		return strings[value - 0];
	}
};

template <> struct Reflect<Ucd_Script_Ext> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(Ucd_Script_Ext), "Ucd_Script_Ext", enum_count<Ucd_Script_Ext>());
		return &i;
	}
};

enum Ucd_Block : uint32_t;
template <> struct Enum_Info<Ucd_Block> {
	static constexpr size_t get_count() { return 310; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 309; }
	static const Ucd_Block index_value(s64 index) { return (Ucd_Block)(index + 0); }
	static const String string(Ucd_Block value) {
		static const String strings[] = {
			"Ucd_Block_UNKNOWN",
			"Ucd_Block_BASIC_LATIN",
			"Ucd_Block_LATIN_1_SUPPLEMENT",
			"Ucd_Block_LATIN_EXTENDED_A",
			"Ucd_Block_LATIN_EXTENDED_B",
			"Ucd_Block_IPA_EXTENSIONS",
			"Ucd_Block_SPACING_MODIFIER_LETTERS",
			"Ucd_Block_COMBINING_DIACRITICAL_MARKS",
			"Ucd_Block_GREEK_AND_COPTIC",
			"Ucd_Block_CYRILLIC",
			"Ucd_Block_CYRILLIC_SUPPLEMENT",
			"Ucd_Block_ARMENIAN",
			"Ucd_Block_HEBREW",
			"Ucd_Block_ARABIC",
			"Ucd_Block_SYRIAC",
			"Ucd_Block_ARABIC_SUPPLEMENT",
			"Ucd_Block_THAANA",
			"Ucd_Block_NKO",
			"Ucd_Block_SAMARITAN",
			"Ucd_Block_MANDAIC",
			"Ucd_Block_SYRIAC_SUPPLEMENT",
			"Ucd_Block_ARABIC_EXTENDED_A",
			"Ucd_Block_DEVANAGARI",
			"Ucd_Block_BENGALI",
			"Ucd_Block_GURMUKHI",
			"Ucd_Block_GUJARATI",
			"Ucd_Block_ORIYA",
			"Ucd_Block_TAMIL",
			"Ucd_Block_TELUGU",
			"Ucd_Block_KANNADA",
			"Ucd_Block_MALAYALAM",
			"Ucd_Block_SINHALA",
			"Ucd_Block_THAI",
			"Ucd_Block_LAO",
			"Ucd_Block_TIBETAN",
			"Ucd_Block_MYANMAR",
			"Ucd_Block_GEORGIAN",
			"Ucd_Block_HANGUL_JAMO",
			"Ucd_Block_ETHIOPIC",
			"Ucd_Block_ETHIOPIC_SUPPLEMENT",
			"Ucd_Block_CHEROKEE",
			"Ucd_Block_UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS",
			"Ucd_Block_OGHAM",
			"Ucd_Block_RUNIC",
			"Ucd_Block_TAGALOG",
			"Ucd_Block_HANUNOO",
			"Ucd_Block_BUHID",
			"Ucd_Block_TAGBANWA",
			"Ucd_Block_KHMER",
			"Ucd_Block_MONGOLIAN",
			"Ucd_Block_UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS_EXTENDED",
			"Ucd_Block_LIMBU",
			"Ucd_Block_TAI_LE",
			"Ucd_Block_NEW_TAI_LUE",
			"Ucd_Block_KHMER_SYMBOLS",
			"Ucd_Block_BUGINESE",
			"Ucd_Block_TAI_THAM",
			"Ucd_Block_COMBINING_DIACRITICAL_MARKS_EXTENDED",
			"Ucd_Block_BALINESE",
			"Ucd_Block_SUNDANESE",
			"Ucd_Block_BATAK",
			"Ucd_Block_LEPCHA",
			"Ucd_Block_OL_CHIKI",
			"Ucd_Block_CYRILLIC_EXTENDED_C",
			"Ucd_Block_GEORGIAN_EXTENDED",
			"Ucd_Block_SUNDANESE_SUPPLEMENT",
			"Ucd_Block_VEDIC_EXTENSIONS",
			"Ucd_Block_PHONETIC_EXTENSIONS",
			"Ucd_Block_PHONETIC_EXTENSIONS_SUPPLEMENT",
			"Ucd_Block_COMBINING_DIACRITICAL_MARKS_SUPPLEMENT",
			"Ucd_Block_LATIN_EXTENDED_ADDITIONAL",
			"Ucd_Block_GREEK_EXTENDED",
			"Ucd_Block_GENERAL_PUNCTUATION",
			"Ucd_Block_SUPERSCRIPTS_AND_SUBSCRIPTS",
			"Ucd_Block_CURRENCY_SYMBOLS",
			"Ucd_Block_COMBINING_DIACRITICAL_MARKS_FOR_SYMBOLS",
			"Ucd_Block_LETTERLIKE_SYMBOLS",
			"Ucd_Block_NUMBER_FORMS",
			"Ucd_Block_ARROWS",
			"Ucd_Block_MATHEMATICAL_OPERATORS",
			"Ucd_Block_MISCELLANEOUS_TECHNICAL",
			"Ucd_Block_CONTROL_PICTURES",
			"Ucd_Block_OPTICAL_CHARACTER_RECOGNITION",
			"Ucd_Block_ENCLOSED_ALPHANUMERICS",
			"Ucd_Block_BOX_DRAWING",
			"Ucd_Block_BLOCK_ELEMENTS",
			"Ucd_Block_GEOMETRIC_SHAPES",
			"Ucd_Block_MISCELLANEOUS_SYMBOLS",
			"Ucd_Block_DINGBATS",
			"Ucd_Block_MISCELLANEOUS_MATHEMATICAL_SYMBOLS_A",
			"Ucd_Block_SUPPLEMENTAL_ARROWS_A",
			"Ucd_Block_BRAILLE_PATTERNS",
			"Ucd_Block_SUPPLEMENTAL_ARROWS_B",
			"Ucd_Block_MISCELLANEOUS_MATHEMATICAL_SYMBOLS_B",
			"Ucd_Block_SUPPLEMENTAL_MATHEMATICAL_OPERATORS",
			"Ucd_Block_MISCELLANEOUS_SYMBOLS_AND_ARROWS",
			"Ucd_Block_GLAGOLITIC",
			"Ucd_Block_LATIN_EXTENDED_C",
			"Ucd_Block_COPTIC",
			"Ucd_Block_GEORGIAN_SUPPLEMENT",
			"Ucd_Block_TIFINAGH",
			"Ucd_Block_ETHIOPIC_EXTENDED",
			"Ucd_Block_CYRILLIC_EXTENDED_A",
			"Ucd_Block_SUPPLEMENTAL_PUNCTUATION",
			"Ucd_Block_CJK_RADICALS_SUPPLEMENT",
			"Ucd_Block_KANGXI_RADICALS",
			"Ucd_Block_IDEOGRAPHIC_DESCRIPTION_CHARACTERS",
			"Ucd_Block_CJK_SYMBOLS_AND_PUNCTUATION",
			"Ucd_Block_HIRAGANA",
			"Ucd_Block_KATAKANA",
			"Ucd_Block_BOPOMOFO",
			"Ucd_Block_HANGUL_COMPATIBILITY_JAMO",
			"Ucd_Block_KANBUN",
			"Ucd_Block_BOPOMOFO_EXTENDED",
			"Ucd_Block_CJK_STROKES",
			"Ucd_Block_KATAKANA_PHONETIC_EXTENSIONS",
			"Ucd_Block_ENCLOSED_CJK_LETTERS_AND_MONTHS",
			"Ucd_Block_CJK_COMPATIBILITY",
			"Ucd_Block_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A",
			"Ucd_Block_YIJING_HEXAGRAM_SYMBOLS",
			"Ucd_Block_CJK_UNIFIED_IDEOGRAPHS",
			"Ucd_Block_YI_SYLLABLES",
			"Ucd_Block_YI_RADICALS",
			"Ucd_Block_LISU",
			"Ucd_Block_VAI",
			"Ucd_Block_CYRILLIC_EXTENDED_B",
			"Ucd_Block_BAMUM",
			"Ucd_Block_MODIFIER_TONE_LETTERS",
			"Ucd_Block_LATIN_EXTENDED_D",
			"Ucd_Block_SYLOTI_NAGRI",
			"Ucd_Block_COMMON_INDIC_NUMBER_FORMS",
			"Ucd_Block_PHAGS_PA",
			"Ucd_Block_SAURASHTRA",
			"Ucd_Block_DEVANAGARI_EXTENDED",
			"Ucd_Block_KAYAH_LI",
			"Ucd_Block_REJANG",
			"Ucd_Block_HANGUL_JAMO_EXTENDED_A",
			"Ucd_Block_JAVANESE",
			"Ucd_Block_MYANMAR_EXTENDED_B",
			"Ucd_Block_CHAM",
			"Ucd_Block_MYANMAR_EXTENDED_A",
			"Ucd_Block_TAI_VIET",
			"Ucd_Block_MEETEI_MAYEK_EXTENSIONS",
			"Ucd_Block_ETHIOPIC_EXTENDED_A",
			"Ucd_Block_LATIN_EXTENDED_E",
			"Ucd_Block_CHEROKEE_SUPPLEMENT",
			"Ucd_Block_MEETEI_MAYEK",
			"Ucd_Block_HANGUL_SYLLABLES",
			"Ucd_Block_HANGUL_JAMO_EXTENDED_B",
			"Ucd_Block_HIGH_SURROGATES",
			"Ucd_Block_HIGH_PRIVATE_USE_SURROGATES",
			"Ucd_Block_LOW_SURROGATES",
			"Ucd_Block_PRIVATE_USE_AREA",
			"Ucd_Block_CJK_COMPATIBILITY_IDEOGRAPHS",
			"Ucd_Block_ALPHABETIC_PRESENTATION_FORMS",
			"Ucd_Block_ARABIC_PRESENTATION_FORMS_A",
			"Ucd_Block_VARIATION_SELECTORS",
			"Ucd_Block_VERTICAL_FORMS",
			"Ucd_Block_COMBINING_HALF_MARKS",
			"Ucd_Block_CJK_COMPATIBILITY_FORMS",
			"Ucd_Block_SMALL_FORM_VARIANTS",
			"Ucd_Block_ARABIC_PRESENTATION_FORMS_B",
			"Ucd_Block_HALFWIDTH_AND_FULLWIDTH_FORMS",
			"Ucd_Block_SPECIALS",
			"Ucd_Block_LINEAR_B_SYLLABARY",
			"Ucd_Block_LINEAR_B_IDEOGRAMS",
			"Ucd_Block_AEGEAN_NUMBERS",
			"Ucd_Block_ANCIENT_GREEK_NUMBERS",
			"Ucd_Block_ANCIENT_SYMBOLS",
			"Ucd_Block_PHAISTOS_DISC",
			"Ucd_Block_LYCIAN",
			"Ucd_Block_CARIAN",
			"Ucd_Block_COPTIC_EPACT_NUMBERS",
			"Ucd_Block_OLD_ITALIC",
			"Ucd_Block_GOTHIC",
			"Ucd_Block_OLD_PERMIC",
			"Ucd_Block_UGARITIC",
			"Ucd_Block_OLD_PERSIAN",
			"Ucd_Block_DESERET",
			"Ucd_Block_SHAVIAN",
			"Ucd_Block_OSMANYA",
			"Ucd_Block_OSAGE",
			"Ucd_Block_ELBASAN",
			"Ucd_Block_CAUCASIAN_ALBANIAN",
			"Ucd_Block_LINEAR_A",
			"Ucd_Block_CYPRIOT_SYLLABARY",
			"Ucd_Block_IMPERIAL_ARAMAIC",
			"Ucd_Block_PALMYRENE",
			"Ucd_Block_NABATAEAN",
			"Ucd_Block_HATRAN",
			"Ucd_Block_PHOENICIAN",
			"Ucd_Block_LYDIAN",
			"Ucd_Block_MEROITIC_HIEROGLYPHS",
			"Ucd_Block_MEROITIC_CURSIVE",
			"Ucd_Block_KHAROSHTHI",
			"Ucd_Block_OLD_SOUTH_ARABIAN",
			"Ucd_Block_OLD_NORTH_ARABIAN",
			"Ucd_Block_MANICHAEAN",
			"Ucd_Block_AVESTAN",
			"Ucd_Block_INSCRIPTIONAL_PARTHIAN",
			"Ucd_Block_INSCRIPTIONAL_PAHLAVI",
			"Ucd_Block_PSALTER_PAHLAVI",
			"Ucd_Block_OLD_TURKIC",
			"Ucd_Block_OLD_HUNGARIAN",
			"Ucd_Block_HANIFI_ROHINGYA",
			"Ucd_Block_RUMI_NUMERAL_SYMBOLS",
			"Ucd_Block_YEZIDI",
			"Ucd_Block_OLD_SOGDIAN",
			"Ucd_Block_SOGDIAN",
			"Ucd_Block_CHORASMIAN",
			"Ucd_Block_ELYMAIC",
			"Ucd_Block_BRAHMI",
			"Ucd_Block_KAITHI",
			"Ucd_Block_SORA_SOMPENG",
			"Ucd_Block_CHAKMA",
			"Ucd_Block_MAHAJANI",
			"Ucd_Block_SHARADA",
			"Ucd_Block_SINHALA_ARCHAIC_NUMBERS",
			"Ucd_Block_KHOJKI",
			"Ucd_Block_MULTANI",
			"Ucd_Block_KHUDAWADI",
			"Ucd_Block_GRANTHA",
			"Ucd_Block_NEWA",
			"Ucd_Block_TIRHUTA",
			"Ucd_Block_SIDDHAM",
			"Ucd_Block_MODI",
			"Ucd_Block_MONGOLIAN_SUPPLEMENT",
			"Ucd_Block_TAKRI",
			"Ucd_Block_AHOM",
			"Ucd_Block_DOGRA",
			"Ucd_Block_WARANG_CITI",
			"Ucd_Block_DIVES_AKURU",
			"Ucd_Block_NANDINAGARI",
			"Ucd_Block_ZANABAZAR_SQUARE",
			"Ucd_Block_SOYOMBO",
			"Ucd_Block_PAU_CIN_HAU",
			"Ucd_Block_BHAIKSUKI",
			"Ucd_Block_MARCHEN",
			"Ucd_Block_MASARAM_GONDI",
			"Ucd_Block_GUNJALA_GONDI",
			"Ucd_Block_MAKASAR",
			"Ucd_Block_LISU_SUPPLEMENT",
			"Ucd_Block_TAMIL_SUPPLEMENT",
			"Ucd_Block_CUNEIFORM",
			"Ucd_Block_CUNEIFORM_NUMBERS_AND_PUNCTUATION",
			"Ucd_Block_EARLY_DYNASTIC_CUNEIFORM",
			"Ucd_Block_EGYPTIAN_HIEROGLYPHS",
			"Ucd_Block_EGYPTIAN_HIEROGLYPH_FORMAT_CONTROLS",
			"Ucd_Block_ANATOLIAN_HIEROGLYPHS",
			"Ucd_Block_BAMUM_SUPPLEMENT",
			"Ucd_Block_MRO",
			"Ucd_Block_BASSA_VAH",
			"Ucd_Block_PAHAWH_HMONG",
			"Ucd_Block_MEDEFAIDRIN",
			"Ucd_Block_MIAO",
			"Ucd_Block_IDEOGRAPHIC_SYMBOLS_AND_PUNCTUATION",
			"Ucd_Block_TANGUT",
			"Ucd_Block_TANGUT_COMPONENTS",
			"Ucd_Block_KHITAN_SMALL_SCRIPT",
			"Ucd_Block_TANGUT_SUPPLEMENT",
			"Ucd_Block_KANA_SUPPLEMENT",
			"Ucd_Block_KANA_EXTENDED_A",
			"Ucd_Block_SMALL_KANA_EXTENSION",
			"Ucd_Block_NUSHU",
			"Ucd_Block_DUPLOYAN",
			"Ucd_Block_SHORTHAND_FORMAT_CONTROLS",
			"Ucd_Block_BYZANTINE_MUSICAL_SYMBOLS",
			"Ucd_Block_MUSICAL_SYMBOLS",
			"Ucd_Block_ANCIENT_GREEK_MUSICAL_NOTATION",
			"Ucd_Block_MAYAN_NUMERALS",
			"Ucd_Block_TAI_XUAN_JING_SYMBOLS",
			"Ucd_Block_COUNTING_ROD_NUMERALS",
			"Ucd_Block_MATHEMATICAL_ALPHANUMERIC_SYMBOLS",
			"Ucd_Block_SUTTON_SIGNWRITING",
			"Ucd_Block_GLAGOLITIC_SUPPLEMENT",
			"Ucd_Block_NYIAKENG_PUACHUE_HMONG",
			"Ucd_Block_WANCHO",
			"Ucd_Block_MENDE_KIKAKUI",
			"Ucd_Block_ADLAM",
			"Ucd_Block_INDIC_SIYAQ_NUMBERS",
			"Ucd_Block_OTTOMAN_SIYAQ_NUMBERS",
			"Ucd_Block_ARABIC_MATHEMATICAL_ALPHABETIC_SYMBOLS",
			"Ucd_Block_MAHJONG_TILES",
			"Ucd_Block_DOMINO_TILES",
			"Ucd_Block_PLAYING_CARDS",
			"Ucd_Block_ENCLOSED_ALPHANUMERIC_SUPPLEMENT",
			"Ucd_Block_ENCLOSED_IDEOGRAPHIC_SUPPLEMENT",
			"Ucd_Block_MISCELLANEOUS_SYMBOLS_AND_PICTOGRAPHS",
			"Ucd_Block_EMOTICONS",
			"Ucd_Block_ORNAMENTAL_DINGBATS",
			"Ucd_Block_TRANSPORT_AND_MAP_SYMBOLS",
			"Ucd_Block_ALCHEMICAL_SYMBOLS",
			"Ucd_Block_GEOMETRIC_SHAPES_EXTENDED",
			"Ucd_Block_SUPPLEMENTAL_ARROWS_C",
			"Ucd_Block_SUPPLEMENTAL_SYMBOLS_AND_PICTOGRAPHS",
			"Ucd_Block_CHESS_SYMBOLS",
			"Ucd_Block_SYMBOLS_AND_PICTOGRAPHS_EXTENDED_A",
			"Ucd_Block_SYMBOLS_FOR_LEGACY_COMPUTING",
			"Ucd_Block_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_B",
			"Ucd_Block_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_C",
			"Ucd_Block_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_D",
			"Ucd_Block_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_E",
			"Ucd_Block_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_F",
			"Ucd_Block_CJK_COMPATIBILITY_IDEOGRAPHS_SUPPLEMENT",
			"Ucd_Block_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_G",
			"Ucd_Block_TAGS",
			"Ucd_Block_VARIATION_SELECTORS_SUPPLEMENT",
			"Ucd_Block_SUPPLEMENTARY_PRIVATE_USE_AREA_A",
			"Ucd_Block_SUPPLEMENTARY_PRIVATE_USE_AREA_B",
			"Ucd_Block_COUNT",
		};
		return strings[value - 0];
	}
};

template <> struct Reflect<Ucd_Block> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(Ucd_Block), "Ucd_Block", enum_count<Ucd_Block>());
		return &i;
	}
};

enum Ucd_Indic_Syllable : uint32_t;
template <> struct Enum_Info<Ucd_Indic_Syllable> {
	static constexpr size_t get_count() { return 37; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 36; }
	static const Ucd_Indic_Syllable index_value(s64 index) { return (Ucd_Indic_Syllable)(index + 0); }
	static const String string(Ucd_Indic_Syllable value) {
		static const String strings[] = {
			"Ucd_Indic_Syllable_UNKNOWN",
			"Ucd_Indic_Syllable_BINDU",
			"Ucd_Indic_Syllable_VISARGA",
			"Ucd_Indic_Syllable_AVAGRAHA",
			"Ucd_Indic_Syllable_NUKTA",
			"Ucd_Indic_Syllable_VIRAMA",
			"Ucd_Indic_Syllable_PURE_KILLER",
			"Ucd_Indic_Syllable_INVISIBLE_STACKER",
			"Ucd_Indic_Syllable_VOWEL_INDEPENDENT",
			"Ucd_Indic_Syllable_VOWEL_DEPENDENT",
			"Ucd_Indic_Syllable_VOWEL",
			"Ucd_Indic_Syllable_CONSONANT_PLACEHOLDER",
			"Ucd_Indic_Syllable_CONSONANT",
			"Ucd_Indic_Syllable_CONSONANT_DEAD",
			"Ucd_Indic_Syllable_CONSONANT_WITH_STACKER",
			"Ucd_Indic_Syllable_CONSONANT_PREFIXED",
			"Ucd_Indic_Syllable_CONSONANT_PRECEDING_REPHA",
			"Ucd_Indic_Syllable_CONSONANT_INITIAL_POSTFIXED",
			"Ucd_Indic_Syllable_CONSONANT_SUCCEEDING_REPHA",
			"Ucd_Indic_Syllable_CONSONANT_SUBJOINED",
			"Ucd_Indic_Syllable_CONSONANT_MEDIAL",
			"Ucd_Indic_Syllable_CONSONANT_FINAL",
			"Ucd_Indic_Syllable_CONSONANT_HEAD_LETTER",
			"Ucd_Indic_Syllable_MODIFYING_LETTER",
			"Ucd_Indic_Syllable_TONE_LETTER",
			"Ucd_Indic_Syllable_TONE_MARK",
			"Ucd_Indic_Syllable_GEMINATION_MARK",
			"Ucd_Indic_Syllable_CANTILLATION_MARK",
			"Ucd_Indic_Syllable_REGISTER_SHIFTER",
			"Ucd_Indic_Syllable_SYLLABLE_MODIFIER",
			"Ucd_Indic_Syllable_CONSONANT_KILLER",
			"Ucd_Indic_Syllable_NON_JOINER",
			"Ucd_Indic_Syllable_JOINER",
			"Ucd_Indic_Syllable_NUMBER_JOINER",
			"Ucd_Indic_Syllable_NUMBER",
			"Ucd_Indic_Syllable_BRAHMI_JOINING_NUMBER",
			"Ucd_Indic_Syllable_COUNT",
		};
		return strings[value - 0];
	}
};

template <> struct Reflect<Ucd_Indic_Syllable> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(Ucd_Indic_Syllable), "Ucd_Indic_Syllable", enum_count<Ucd_Indic_Syllable>());
		return &i;
	}
};

enum Ucd_Indic_Position : uint32_t;
template <> struct Enum_Info<Ucd_Indic_Position> {
	static constexpr size_t get_count() { return 17; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 16; }
	static const Ucd_Indic_Position index_value(s64 index) { return (Ucd_Indic_Position)(index + 0); }
	static const String string(Ucd_Indic_Position value) {
		static const String strings[] = {
			"Ucd_Indic_Position_UNKNOWN",
			"Ucd_Indic_Position_RIGHT",
			"Ucd_Indic_Position_LEFT",
			"Ucd_Indic_Position_VISUAL_ORDER_LEFT",
			"Ucd_Indic_Position_LEFT_AND_RIGHT",
			"Ucd_Indic_Position_TOP",
			"Ucd_Indic_Position_BOTTOM",
			"Ucd_Indic_Position_TOP_AND_BOTTOM",
			"Ucd_Indic_Position_TOP_AND_RIGHT",
			"Ucd_Indic_Position_TOP_AND_LEFT",
			"Ucd_Indic_Position_TOP_AND_LEFT_AND_RIGHT",
			"Ucd_Indic_Position_BOTTOM_AND_RIGHT",
			"Ucd_Indic_Position_BOTTOM_AND_LEFT",
			"Ucd_Indic_Position_TOP_AND_BOTTOM_AND_RIGHT",
			"Ucd_Indic_Position_TOP_AND_BOTTOM_AND_LEFT",
			"Ucd_Indic_Position_OVERSTRUCK",
			"Ucd_Indic_Position_COUNT",
		};
		return strings[value - 0];
	}
};

template <> struct Reflect<Ucd_Indic_Position> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(Ucd_Indic_Position), "Ucd_Indic_Position", enum_count<Ucd_Indic_Position>());
		return &i;
	}
};

enum Ucd_Grapheme_Property : uint32_t;
template <> struct Enum_Info<Ucd_Grapheme_Property> {
	static constexpr size_t get_count() { return 15; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 14; }
	static const Ucd_Grapheme_Property index_value(s64 index) { return (Ucd_Grapheme_Property)(index + 0); }
	static const String string(Ucd_Grapheme_Property value) {
		static const String strings[] = {
			"Ucd_Grapheme_Property_UNKNOWN",
			"Ucd_Grapheme_Property_PREPEND",
			"Ucd_Grapheme_Property_CR",
			"Ucd_Grapheme_Property_LF",
			"Ucd_Grapheme_Property_CONTROL",
			"Ucd_Grapheme_Property_EXTEND",
			"Ucd_Grapheme_Property_REGIONAL_INDICATOR",
			"Ucd_Grapheme_Property_SPACINGMARK",
			"Ucd_Grapheme_Property_L",
			"Ucd_Grapheme_Property_V",
			"Ucd_Grapheme_Property_T",
			"Ucd_Grapheme_Property_LV",
			"Ucd_Grapheme_Property_LVT",
			"Ucd_Grapheme_Property_ZWJ",
			"Ucd_Grapheme_Property_COUNT",
		};
		return strings[value - 0];
	}
};

template <> struct Reflect<Ucd_Grapheme_Property> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(Ucd_Grapheme_Property), "Ucd_Grapheme_Property", enum_count<Ucd_Grapheme_Property>());
		return &i;
	}
};

enum Lookup_Flag_Bit : u16;
template <> struct Enum_Info<Lookup_Flag_Bit> {
	static constexpr size_t get_count() { return 7; }
	static constexpr s64 get_min_value() { return -256; }
	static constexpr s64 get_max_value() { return 224; }
	static const Lookup_Flag_Bit index_value(s64 index) {
		static const Lookup_Flag_Bit enum_values[] = {
			(Lookup_Flag_Bit)1,
			(Lookup_Flag_Bit)2,
			(Lookup_Flag_Bit)4,
			(Lookup_Flag_Bit)8,
			(Lookup_Flag_Bit)16,
			(Lookup_Flag_Bit)224,
			(Lookup_Flag_Bit)-256,
		};
		assert(index >= 0 && index < static_count(enum_values));
		return enum_values[index];
	}
	static const String string(Lookup_Flag_Bit value) {
		if (1 == value) return "Lookup_Flag_RIGHT_TO_LEFT";
		if (2 == value) return "Lookup_Flag_IGNORE_BASE_GLYPH";
		if (4 == value) return "Lookup_Flag_IGNORE_LIGATURES";
		if (8 == value) return "Lookup_Flag_IGNORE_MARKS";
		if (16 == value) return "Lookup_Flag_USE_MARK_FILTERING_SET";
		if (224 == value) return "Lookup_Flag_RESERVED";
		if (-256 == value) return "Lookup_Flag_MARK_ATTACHMENT_TYPE";

		invalid_code_path();
		return "";
	}
};

template <> struct Reflect<Lookup_Flag_Bit> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(Lookup_Flag_Bit), "Lookup_Flag_Bit", enum_count<Lookup_Flag_Bit>());
		return &i;
	}
};

enum Script_Tag : u32;
template <> struct Enum_Info<Script_Tag> {
	static constexpr size_t get_count() { return 160; }
	static constexpr s64 get_min_value() { return 538995065; }
	static constexpr s64 get_max_value() { return 2020963192; }
	static const Script_Tag index_value(s64 index) {
		static const Script_Tag enum_values[] = {
			(Script_Tag)1835820129,
			(Script_Tag)1836017761,
			(Script_Tag)2004184168,
			(Script_Tag)1650553441,
			(Script_Tag)1852666465,
			(Script_Tag)1953724001,
			(Script_Tag)1768710498,
			(Script_Tag)1970102626,
			(Script_Tag)1936941410,
			(Script_Tag)1802789218,
			(Script_Tag)1735288162,
			(Script_Tag)845639266,
			(Script_Tag)1936418914,
			(Script_Tag)1869639522,
			(Script_Tag)1751216738,
			(Script_Tag)1767993954,
			(Script_Tag)1768387938,
			(Script_Tag)1684567394,
			(Script_Tag)1836743010,
			(Script_Tag)1936613731,
			(Script_Tag)1769103715,
			(Script_Tag)1651009377,
			(Script_Tag)1835753827,
			(Script_Tag)1835100259,
			(Script_Tag)1919248483,
			(Script_Tag)1768841576,
			(Script_Tag)1953525603,
			(Script_Tag)1953656931,
			(Script_Tag)1819441507,
			(Script_Tag)1414284868,
			(Script_Tag)1953657700,
			(Script_Tag)1635149156,
			(Script_Tag)846620004,
			(Script_Tag)1919381348,
			(Script_Tag)1819309412,
			(Script_Tag)1887004517,
			(Script_Tag)1633840229,
			(Script_Tag)1768453221,
			(Script_Tag)1919903079,
			(Script_Tag)1734437991,
			(Script_Tag)1752461159,
			(Script_Tag)1851880039,
			(Script_Tag)1801810535,
			(Script_Tag)1919579495,
			(Script_Tag)846359143,
			(Script_Tag)1735290727,
			(Script_Tag)1970435431,
			(Script_Tag)846361959,
			(Script_Tag)1735287144,
			(Script_Tag)1869439338,
			(Script_Tag)1734897522,
			(Script_Tag)1869504872,
			(Script_Tag)1920229736,
			(Script_Tag)1919051112,
			(Script_Tag)1634623851,
			(Script_Tag)1768780385,
			(Script_Tag)1768712304,
			(Script_Tag)1769239152,
			(Script_Tag)1635148138,
			(Script_Tag)1768453227,
			(Script_Tag)1633971819,
			(Script_Tag)845442667,
			(Script_Tag)1768710507,
			(Script_Tag)1918986347,
			(Script_Tag)1919772779,
			(Script_Tag)1785686123,
			(Script_Tag)1684957555,
			(Script_Tag)544170348,
			(Script_Tag)1853120876,
			(Script_Tag)1668310380,
			(Script_Tag)1651337580,
			(Script_Tag)1634625900,
			(Script_Tag)1651403116,
			(Script_Tag)1970497900,
			(Script_Tag)1768126828,
			(Script_Tag)1768192364,
			(Script_Tag)1785225581,
			(Script_Tag)1634427245,
			(Script_Tag)1836674157,
			(Script_Tag)846031981,
			(Script_Tag)1684955501,
			(Script_Tag)1768841581,
			(Script_Tag)1668440429,
			(Script_Tag)1835954023,
			(Script_Tag)1752457581,
			(Script_Tag)1717855597,
			(Script_Tag)1768256621,
			(Script_Tag)1684956525,
			(Script_Tag)1668441453,
			(Script_Tag)1869768045,
			(Script_Tag)1685220464,
			(Script_Tag)1768189805,
			(Script_Tag)1735290733,
			(Script_Tag)1869574765,
			(Script_Tag)1953265005,
			(Script_Tag)1668511085,
			(Script_Tag)1919777133,
			(Script_Tag)846035309,
			(Script_Tag)1952539246,
			(Script_Tag)1635214702,
			(Script_Tag)1970037108,
			(Script_Tag)544172910,
			(Script_Tag)1969779566,
			(Script_Tag)1635349103,
			(Script_Tag)846819951,
			(Script_Tag)1835100015,
			(Script_Tag)1801677935,
			(Script_Tag)1818326121,
			(Script_Tag)1735292264,
			(Script_Tag)1651663214,
			(Script_Tag)1836213616,
			(Script_Tag)1868918904,
			(Script_Tag)1869049715,
			(Script_Tag)1651663219,
			(Script_Tag)1751872111,
			(Script_Tag)1701278575,
			(Script_Tag)1634562927,
			(Script_Tag)1735290216,
			(Script_Tag)1835819376,
			(Script_Tag)1668637040,
			(Script_Tag)1734436976,
			(Script_Tag)2020501616,
			(Script_Tag)1886152816,
			(Script_Tag)1735289458,
			(Script_Tag)1919841650,
			(Script_Tag)1919770995,
			(Script_Tag)1920295283,
			(Script_Tag)1685219443,
			(Script_Tag)2002872435,
			(Script_Tag)1684302195,
			(Script_Tag)2003724147,
			(Script_Tag)1752066419,
			(Script_Tag)1684500339,
			(Script_Tag)1634889587,
			(Script_Tag)1870229363,
			(Script_Tag)2020963192,
			(Script_Tag)1684960627,
			(Script_Tag)1869379955,
			(Script_Tag)1668446579,
			(Script_Tag)1735157620,
			(Script_Tag)1650942324,
			(Script_Tag)1701601652,
			(Script_Tag)1634623852,
			(Script_Tag)1953915252,
			(Script_Tag)1919639924,
			(Script_Tag)1819107700,
			(Script_Tag)845966708,
			(Script_Tag)1735287156,
			(Script_Tag)1970038132,
			(Script_Tag)845964660,
			(Script_Tag)1633773684,
			(Script_Tag)1767991412,
			(Script_Tag)1952606580,
			(Script_Tag)1735288436,
			(Script_Tag)1752328564,
			(Script_Tag)1918986101,
			(Script_Tag)543777142,
			(Script_Tag)1634886007,
			(Script_Tag)538995065,
			(Script_Tag)1651401082,
		};
		assert(index >= 0 && index < static_count(enum_values));
		return enum_values[index];
	}
	static const String string(Script_Tag value) {
		if (1835820129 == value) return "Script_Tag_ADLAM";
		if (1836017761 == value) return "Script_Tag_AHOM";
		if (2004184168 == value) return "Script_Tag_ANATOLIAN_HIEROGLYPHS";
		if (1650553441 == value) return "Script_Tag_ARABIC";
		if (1852666465 == value) return "Script_Tag_ARMENIAN";
		if (1953724001 == value) return "Script_Tag_AVESTAN";
		if (1768710498 == value) return "Script_Tag_BALINESE";
		if (1970102626 == value) return "Script_Tag_BAMUM";
		if (1936941410 == value) return "Script_Tag_BASSA_VAH";
		if (1802789218 == value) return "Script_Tag_BATAK";
		if (1735288162 == value) return "Script_Tag_BENGALI";
		if (845639266 == value) return "Script_Tag_BENGALI2";
		if (1936418914 == value) return "Script_Tag_BHAIKSUKI";
		if (1869639522 == value) return "Script_Tag_BOPOMOFO";
		if (1751216738 == value) return "Script_Tag_BRAHMI";
		if (1767993954 == value) return "Script_Tag_BRAILLE";
		if (1768387938 == value) return "Script_Tag_BUGINESE";
		if (1684567394 == value) return "Script_Tag_BUHID";
		if (1836743010 == value) return "Script_Tag_BYZANTINE_MUSIC";
		if (1936613731 == value) return "Script_Tag_CANADIAN_SYLLABICS";
		if (1769103715 == value) return "Script_Tag_CARIAN";
		if (1651009377 == value) return "Script_Tag_CAUCASIAN_ALBANIAN";
		if (1835753827 == value) return "Script_Tag_CHAKMA";
		if (1835100259 == value) return "Script_Tag_CHAM";
		if (1919248483 == value) return "Script_Tag_CHEROKEE";
		if (1768841576 == value) return "Script_Tag_CJK_IDEOGRAPHIC";
		if (1953525603 == value) return "Script_Tag_COPTIC";
		if (1953656931 == value) return "Script_Tag_CYPRIOT_SYLLABARY";
		if (1819441507 == value) return "Script_Tag_CYRILLIC";
		if (1414284868 == value) return "Script_Tag_DEFAULT";
		if (1953657700 == value) return "Script_Tag_DESERET";
		if (1635149156 == value) return "Script_Tag_DEVANAGARI";
		if (846620004 == value) return "Script_Tag_DEVANAGARI2";
		if (1919381348 == value) return "Script_Tag_DOGRA";
		if (1819309412 == value) return "Script_Tag_DUPLOYAN";
		if (1887004517 == value) return "Script_Tag_EGYPTIAN_HIEROGLYPHS";
		if (1633840229 == value) return "Script_Tag_ELBASAN";
		if (1768453221 == value) return "Script_Tag_ETHIOPIC";
		if (1919903079 == value) return "Script_Tag_GEORGIAN";
		if (1734437991 == value) return "Script_Tag_GLAGOLITIC";
		if (1752461159 == value) return "Script_Tag_GOTHIC";
		if (1851880039 == value) return "Script_Tag_GRANTHA";
		if (1801810535 == value) return "Script_Tag_GREEK";
		if (1919579495 == value) return "Script_Tag_GUJARATI";
		if (846359143 == value) return "Script_Tag_GUJARATI2";
		if (1735290727 == value) return "Script_Tag_GUNJALA_GONDI";
		if (1970435431 == value) return "Script_Tag_GURMUKHI";
		if (846361959 == value) return "Script_Tag_GURMUKHI2";
		if (1735287144 == value) return "Script_Tag_HANGUL";
		if (1869439338 == value) return "Script_Tag_HANGUL_JAMO";
		if (1734897522 == value) return "Script_Tag_HANIFI_ROHINGYA";
		if (1869504872 == value) return "Script_Tag_HANUNOO";
		if (1920229736 == value) return "Script_Tag_HATRAN";
		if (1919051112 == value) return "Script_Tag_HEBREW";
		if (1634623851 == value) return "Script_Tag_KANA";
		if (1768780385 == value) return "Script_Tag_IMPERIAL_ARAMAIC";
		if (1768712304 == value) return "Script_Tag_INSCRIPTIONAL_PAHLAVI";
		if (1769239152 == value) return "Script_Tag_INSCRIPTIONAL_PARTHIAN";
		if (1635148138 == value) return "Script_Tag_JAVANESE";
		if (1768453227 == value) return "Script_Tag_KAITHI";
		if (1633971819 == value) return "Script_Tag_KANNADA";
		if (845442667 == value) return "Script_Tag_KANNADA2";
		if (1768710507 == value) return "Script_Tag_KAYAH_LI";
		if (1918986347 == value) return "Script_Tag_KHAROSTHI";
		if (1919772779 == value) return "Script_Tag_KHMER";
		if (1785686123 == value) return "Script_Tag_KHOJKI";
		if (1684957555 == value) return "Script_Tag_KHUDAWADI";
		if (544170348 == value) return "Script_Tag_LAO";
		if (1853120876 == value) return "Script_Tag_LATIN";
		if (1668310380 == value) return "Script_Tag_LEPCHA";
		if (1651337580 == value) return "Script_Tag_LIMBU";
		if (1634625900 == value) return "Script_Tag_LINEAR_A";
		if (1651403116 == value) return "Script_Tag_LINEAR_B";
		if (1970497900 == value) return "Script_Tag_LISU";
		if (1768126828 == value) return "Script_Tag_LYCIAN";
		if (1768192364 == value) return "Script_Tag_LYDIAN";
		if (1785225581 == value) return "Script_Tag_MAHAJANI";
		if (1634427245 == value) return "Script_Tag_MAKASAR";
		if (1836674157 == value) return "Script_Tag_MALAYALAM";
		if (846031981 == value) return "Script_Tag_MALAYALAM2";
		if (1684955501 == value) return "Script_Tag_MANDAIC_MANDAEAN";
		if (1768841581 == value) return "Script_Tag_MANICHAEAN";
		if (1668440429 == value) return "Script_Tag_MARCHEN";
		if (1835954023 == value) return "Script_Tag_MASARAM_GONDI";
		if (1752457581 == value) return "Script_Tag_MATHEMATICAL_ALPHANUMERIC_SYMBOLS";
		if (1717855597 == value) return "Script_Tag_MEDEFAIDRIN";
		if (1768256621 == value) return "Script_Tag_MEITEI_MAYEK";
		if (1684956525 == value) return "Script_Tag_MENDE_KIKAKUI";
		if (1668441453 == value) return "Script_Tag_MEROITIC_CURSIVE";
		if (1869768045 == value) return "Script_Tag_MEROITIC_HIEROGLYPHS";
		if (1685220464 == value) return "Script_Tag_MIAO";
		if (1768189805 == value) return "Script_Tag_MODI";
		if (1735290733 == value) return "Script_Tag_MONGOLIAN";
		if (1869574765 == value) return "Script_Tag_MRO";
		if (1953265005 == value) return "Script_Tag_MULTANI";
		if (1668511085 == value) return "Script_Tag_MUSICAL_SYMBOLS";
		if (1919777133 == value) return "Script_Tag_MYANMAR";
		if (846035309 == value) return "Script_Tag_MYANMAR2";
		if (1952539246 == value) return "Script_Tag_NABATAEAN";
		if (1635214702 == value) return "Script_Tag_NEWA";
		if (1970037108 == value) return "Script_Tag_NEW_TAI_LUE";
		if (544172910 == value) return "Script_Tag_N_KO";
		if (1969779566 == value) return "Script_Tag_NUSHU";
		if (1635349103 == value) return "Script_Tag_ODIA";
		if (846819951 == value) return "Script_Tag_ODIA2";
		if (1835100015 == value) return "Script_Tag_OGHAM";
		if (1801677935 == value) return "Script_Tag_OL_CHIKI";
		if (1818326121 == value) return "Script_Tag_OLD_ITALIC";
		if (1735292264 == value) return "Script_Tag_OLD_HUNGARIAN";
		if (1651663214 == value) return "Script_Tag_OLD_NORTH_ARABIAN";
		if (1836213616 == value) return "Script_Tag_OLD_PERMIC";
		if (1868918904 == value) return "Script_Tag_OLD_PERSIAN_CUNEIFORM";
		if (1869049715 == value) return "Script_Tag_OLD_SOGDIAN";
		if (1651663219 == value) return "Script_Tag_OLD_SOUTH_ARABIAN";
		if (1751872111 == value) return "Script_Tag_OLD_TURKIC_ORKHON_RUNIC";
		if (1701278575 == value) return "Script_Tag_OSAGE";
		if (1634562927 == value) return "Script_Tag_OSMANYA";
		if (1735290216 == value) return "Script_Tag_PAHAWH_HMONG";
		if (1835819376 == value) return "Script_Tag_PALMYRENE";
		if (1668637040 == value) return "Script_Tag_PAU_CIN_HAU";
		if (1734436976 == value) return "Script_Tag_PHAGS";
		if (2020501616 == value) return "Script_Tag_PHOENICIAN";
		if (1886152816 == value) return "Script_Tag_PSALTER_PAHLAVI";
		if (1735289458 == value) return "Script_Tag_REJANG";
		if (1919841650 == value) return "Script_Tag_RUNIC";
		if (1919770995 == value) return "Script_Tag_SAMARITAN";
		if (1920295283 == value) return "Script_Tag_SAURASHTRA";
		if (1685219443 == value) return "Script_Tag_SHARADA";
		if (2002872435 == value) return "Script_Tag_SHAVIAN";
		if (1684302195 == value) return "Script_Tag_SIDDHAM";
		if (2003724147 == value) return "Script_Tag_SIGN_WRITING";
		if (1752066419 == value) return "Script_Tag_SINHALA";
		if (1684500339 == value) return "Script_Tag_SOGDIAN";
		if (1634889587 == value) return "Script_Tag_SORA_SOMPENG";
		if (1870229363 == value) return "Script_Tag_SOYOMBO";
		if (2020963192 == value) return "Script_Tag_SUMERO_AKKADIAN_CUNEIFORM";
		if (1684960627 == value) return "Script_Tag_SUNDANESE";
		if (1869379955 == value) return "Script_Tag_SYLOTI_NAGRI";
		if (1668446579 == value) return "Script_Tag_SYRIAC";
		if (1735157620 == value) return "Script_Tag_TAGALOG";
		if (1650942324 == value) return "Script_Tag_TAGBANWA";
		if (1701601652 == value) return "Script_Tag_TAI_LE";
		if (1634623852 == value) return "Script_Tag_TAI_THAM";
		if (1953915252 == value) return "Script_Tag_TAI_VIET";
		if (1919639924 == value) return "Script_Tag_TAKRI";
		if (1819107700 == value) return "Script_Tag_TAMIL";
		if (845966708 == value) return "Script_Tag_TAMIL2";
		if (1735287156 == value) return "Script_Tag_TANGUT";
		if (1970038132 == value) return "Script_Tag_TELUGU";
		if (845964660 == value) return "Script_Tag_TELUGU2";
		if (1633773684 == value) return "Script_Tag_THAANA";
		if (1767991412 == value) return "Script_Tag_THAI";
		if (1952606580 == value) return "Script_Tag_TIBETAN";
		if (1735288436 == value) return "Script_Tag_TIFINAGH";
		if (1752328564 == value) return "Script_Tag_TIRHUTA";
		if (1918986101 == value) return "Script_Tag_UGARITIC_CUNEIFORM";
		if (543777142 == value) return "Script_Tag_VAI";
		if (1634886007 == value) return "Script_Tag_WARANG_CITI";
		if (538995065 == value) return "Script_Tag_YI";
		if (1651401082 == value) return "Script_Tag_ZANABAZAR_SQUARE";

		invalid_code_path();
		return "";
	}
};

template <> struct Reflect<Script_Tag> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(Script_Tag), "Script_Tag", enum_count<Script_Tag>());
		return &i;
	}
};

enum Feature_Tag : u32;
template <> struct Enum_Info<Feature_Tag> {
	static constexpr size_t get_count() { return 141; }
	static constexpr s64 get_min_value() { return 808547187; }
	static constexpr s64 get_max_value() { return 2037674867; }
	static const Feature_Tag index_value(s64 index) {
		static const Feature_Tag enum_values[] = {
			(Feature_Tag)1953259873,
			(Feature_Tag)1719034465,
			(Feature_Tag)1836474977,
			(Feature_Tag)1937138273,
			(Feature_Tag)1668441697,
			(Feature_Tag)1852336993,
			(Feature_Tag)1719102562,
			(Feature_Tag)1836543074,
			(Feature_Tag)1937206370,
			(Feature_Tag)1953259875,
			(Feature_Tag)1702060387,
			(Feature_Tag)1886217059,
			(Feature_Tag)1918985827,
			(Feature_Tag)1952672355,
			(Feature_Tag)1734962275,
			(Feature_Tag)1952673891,
			(Feature_Tag)1886613603,
			(Feature_Tag)1752658787,
			(Feature_Tag)1936880995,
			(Feature_Tag)825259619,
			(Feature_Tag)1668297315,
			(Feature_Tag)1668493923,
			(Feature_Tag)1953720676,
			(Feature_Tag)1734962276,
			(Feature_Tag)1836019300,
			(Feature_Tag)1936487524,
			(Feature_Tag)1953527909,
			(Feature_Tag)1953259878,
			(Feature_Tag)846096742,
			(Feature_Tag)862873958,
			(Feature_Tag)1634625894,
			(Feature_Tag)1667329126,
			(Feature_Tag)1667330662,
			(Feature_Tag)1684633446,
			(Feature_Tag)1718378856,
			(Feature_Tag)1852596584,
			(Feature_Tag)1953259880,
			(Feature_Tag)1953720680,
			(Feature_Tag)1634626408,
			(Feature_Tag)1734962280,
			(Feature_Tag)1818717800,
			(Feature_Tag)1869246312,
			(Feature_Tag)1684633448,
			(Feature_Tag)1953066601,
			(Feature_Tag)1819243369,
			(Feature_Tag)1818326121,
			(Feature_Tag)1953259882,
			(Feature_Tag)943157354,
			(Feature_Tag)859336810,
			(Feature_Tag)809070698,
			(Feature_Tag)875589738,
			(Feature_Tag)1852990827,
			(Feature_Tag)1684170348,
			(Feature_Tag)1634167148,
			(Feature_Tag)1869441644,
			(Feature_Tag)1836412524,
			(Feature_Tag)1818455916,
			(Feature_Tag)1634890860,
			(Feature_Tag)1836217452,
			(Feature_Tag)1802658157,
			(Feature_Tag)845440365,
			(Feature_Tag)1768187245,
			(Feature_Tag)1802659693,
			(Feature_Tag)1802333037,
			(Feature_Tag)1952805741,
			(Feature_Tag)1953259886,
			(Feature_Tag)1801677934,
			(Feature_Tag)1953199470,
			(Feature_Tag)1919776110,
			(Feature_Tag)1836412527,
			(Feature_Tag)1684172911,
			(Feature_Tag)1852076655,
			(Feature_Tag)1835954799,
			(Feature_Tag)1953259888,
			(Feature_Tag)1885430640,
			(Feature_Tag)1634626416,
			(Feature_Tag)1836412528,
			(Feature_Tag)1717924464,
			(Feature_Tag)1936028272,
			(Feature_Tag)1718907760,
			(Feature_Tag)1937011568,
			(Feature_Tag)1684633456,
			(Feature_Tag)1684633457,
			(Feature_Tag)1684955506,
			(Feature_Tag)1953260402,
			(Feature_Tag)1718774642,
			(Feature_Tag)1734962290,
			(Feature_Tag)1718120562,
			(Feature_Tag)1684173938,
			(Feature_Tag)1634497650,
			(Feature_Tag)1835824242,
			(Feature_Tag)2036495730,
			(Feature_Tag)1852995186,
			(Feature_Tag)1953259891,
			(Feature_Tag)1718511987,
			(Feature_Tag)1702521203,
			(Feature_Tag)1885564275,
			(Feature_Tag)1819307379,
			(Feature_Tag)825258867,
			(Feature_Tag)842036083,
			(Feature_Tag)858813299,
			(Feature_Tag)875590515,
			(Feature_Tag)892367731,
			(Feature_Tag)909144947,
			(Feature_Tag)925922163,
			(Feature_Tag)942699379,
			(Feature_Tag)959476595,
			(Feature_Tag)808547187,
			(Feature_Tag)825324403,
			(Feature_Tag)842101619,
			(Feature_Tag)858878835,
			(Feature_Tag)875656051,
			(Feature_Tag)892433267,
			(Feature_Tag)909210483,
			(Feature_Tag)925987699,
			(Feature_Tag)942764915,
			(Feature_Tag)959542131,
			(Feature_Tag)808612723,
			(Feature_Tag)2037674867,
			(Feature_Tag)1751348339,
			(Feature_Tag)1935832435,
			(Feature_Tag)1936749939,
			(Feature_Tag)1752397683,
			(Feature_Tag)1819568500,
			(Feature_Tag)1869441652,
			(Feature_Tag)1835101812,
			(Feature_Tag)1836412532,
			(Feature_Tag)1684107892,
			(Feature_Tag)1684633460,
			(Feature_Tag)1667853941,
			(Feature_Tag)1953259894,
			(Feature_Tag)1970561398,
			(Feature_Tag)1953654134,
			(Feature_Tag)1818323062,
			(Feature_Tag)1869441654,
			(Feature_Tag)1634626422,
			(Feature_Tag)1852992374,
			(Feature_Tag)1818325110,
			(Feature_Tag)846492278,
			(Feature_Tag)1920234102,
			(Feature_Tag)1869768058,
		};
		assert(index >= 0 && index < static_count(enum_values));
		return enum_values[index];
	}
	static const String string(Feature_Tag value) {
		if (1953259873 == value) return "Feature_Tag_AALT";
		if (1719034465 == value) return "Feature_Tag_ABVF";
		if (1836474977 == value) return "Feature_Tag_ABVM";
		if (1937138273 == value) return "Feature_Tag_ABVS";
		if (1668441697 == value) return "Feature_Tag_AFRC";
		if (1852336993 == value) return "Feature_Tag_AKHN";
		if (1719102562 == value) return "Feature_Tag_BLWF";
		if (1836543074 == value) return "Feature_Tag_BLWM";
		if (1937206370 == value) return "Feature_Tag_BLWS";
		if (1953259875 == value) return "Feature_Tag_CALT";
		if (1702060387 == value) return "Feature_Tag_CASE";
		if (1886217059 == value) return "Feature_Tag_CCMP";
		if (1918985827 == value) return "Feature_Tag_CFAR";
		if (1952672355 == value) return "Feature_Tag_CJCT";
		if (1734962275 == value) return "Feature_Tag_CLIG";
		if (1952673891 == value) return "Feature_Tag_CPCT";
		if (1886613603 == value) return "Feature_Tag_CPSP";
		if (1752658787 == value) return "Feature_Tag_CSWH";
		if (1936880995 == value) return "Feature_Tag_CURS";
		if (825259619 == value) return "Feature_Tag_CV01";
		if (1668297315 == value) return "Feature_Tag_C2PC";
		if (1668493923 == value) return "Feature_Tag_C2SC";
		if (1953720676 == value) return "Feature_Tag_DIST";
		if (1734962276 == value) return "Feature_Tag_DLIG";
		if (1836019300 == value) return "Feature_Tag_DNOM";
		if (1936487524 == value) return "Feature_Tag_DTLS";
		if (1953527909 == value) return "Feature_Tag_EXPT";
		if (1953259878 == value) return "Feature_Tag_FALT";
		if (846096742 == value) return "Feature_Tag_FIN2";
		if (862873958 == value) return "Feature_Tag_FIN3";
		if (1634625894 == value) return "Feature_Tag_FINA";
		if (1667329126 == value) return "Feature_Tag_FLAC";
		if (1667330662 == value) return "Feature_Tag_FRAC";
		if (1684633446 == value) return "Feature_Tag_FWID";
		if (1718378856 == value) return "Feature_Tag_HALF";
		if (1852596584 == value) return "Feature_Tag_HALN";
		if (1953259880 == value) return "Feature_Tag_HALT";
		if (1953720680 == value) return "Feature_Tag_HIST";
		if (1634626408 == value) return "Feature_Tag_HKNA";
		if (1734962280 == value) return "Feature_Tag_HLIG";
		if (1818717800 == value) return "Feature_Tag_HNGL";
		if (1869246312 == value) return "Feature_Tag_HOJO";
		if (1684633448 == value) return "Feature_Tag_HWID";
		if (1953066601 == value) return "Feature_Tag_INIT";
		if (1819243369 == value) return "Feature_Tag_ISOL";
		if (1818326121 == value) return "Feature_Tag_ITAL";
		if (1953259882 == value) return "Feature_Tag_JALT";
		if (943157354 == value) return "Feature_Tag_JP78";
		if (859336810 == value) return "Feature_Tag_JP83";
		if (809070698 == value) return "Feature_Tag_JP90";
		if (875589738 == value) return "Feature_Tag_JP04";
		if (1852990827 == value) return "Feature_Tag_KERN";
		if (1684170348 == value) return "Feature_Tag_LFBD";
		if (1634167148 == value) return "Feature_Tag_LIGA";
		if (1869441644 == value) return "Feature_Tag_LJMO";
		if (1836412524 == value) return "Feature_Tag_LNUM";
		if (1818455916 == value) return "Feature_Tag_LOCL";
		if (1634890860 == value) return "Feature_Tag_LTRA";
		if (1836217452 == value) return "Feature_Tag_LTRM";
		if (1802658157 == value) return "Feature_Tag_MARK";
		if (845440365 == value) return "Feature_Tag_MED2";
		if (1768187245 == value) return "Feature_Tag_MEDI";
		if (1802659693 == value) return "Feature_Tag_MGRK";
		if (1802333037 == value) return "Feature_Tag_MKMK";
		if (1952805741 == value) return "Feature_Tag_MSET";
		if (1953259886 == value) return "Feature_Tag_NALT";
		if (1801677934 == value) return "Feature_Tag_NLCK";
		if (1953199470 == value) return "Feature_Tag_NUKT";
		if (1919776110 == value) return "Feature_Tag_NUMR";
		if (1836412527 == value) return "Feature_Tag_ONUM";
		if (1684172911 == value) return "Feature_Tag_OPBD";
		if (1852076655 == value) return "Feature_Tag_ORDN";
		if (1835954799 == value) return "Feature_Tag_ORNM";
		if (1953259888 == value) return "Feature_Tag_PALT";
		if (1885430640 == value) return "Feature_Tag_PCAP";
		if (1634626416 == value) return "Feature_Tag_PKNA";
		if (1836412528 == value) return "Feature_Tag_PNUM";
		if (1717924464 == value) return "Feature_Tag_PREF";
		if (1936028272 == value) return "Feature_Tag_PRES";
		if (1718907760 == value) return "Feature_Tag_PSTF";
		if (1937011568 == value) return "Feature_Tag_PSTS";
		if (1684633456 == value) return "Feature_Tag_PWID";
		if (1684633457 == value) return "Feature_Tag_QWID";
		if (1684955506 == value) return "Feature_Tag_RAND";
		if (1953260402 == value) return "Feature_Tag_RCLT";
		if (1718774642 == value) return "Feature_Tag_RKRF";
		if (1734962290 == value) return "Feature_Tag_RLIG";
		if (1718120562 == value) return "Feature_Tag_RPHF";
		if (1684173938 == value) return "Feature_Tag_RTBD";
		if (1634497650 == value) return "Feature_Tag_RTLA";
		if (1835824242 == value) return "Feature_Tag_RTLM";
		if (2036495730 == value) return "Feature_Tag_RUBY";
		if (1852995186 == value) return "Feature_Tag_RVRN";
		if (1953259891 == value) return "Feature_Tag_SALT";
		if (1718511987 == value) return "Feature_Tag_SINF";
		if (1702521203 == value) return "Feature_Tag_SIZE";
		if (1885564275 == value) return "Feature_Tag_SMCP";
		if (1819307379 == value) return "Feature_Tag_SMPL";
		if (825258867 == value) return "Feature_Tag_SS01";
		if (842036083 == value) return "Feature_Tag_SS02";
		if (858813299 == value) return "Feature_Tag_SS03";
		if (875590515 == value) return "Feature_Tag_SS04";
		if (892367731 == value) return "Feature_Tag_SS05";
		if (909144947 == value) return "Feature_Tag_SS06";
		if (925922163 == value) return "Feature_Tag_SS07";
		if (942699379 == value) return "Feature_Tag_SS08";
		if (959476595 == value) return "Feature_Tag_SS09";
		if (808547187 == value) return "Feature_Tag_SS10";
		if (825324403 == value) return "Feature_Tag_SS11";
		if (842101619 == value) return "Feature_Tag_SS12";
		if (858878835 == value) return "Feature_Tag_SS13";
		if (875656051 == value) return "Feature_Tag_SS14";
		if (892433267 == value) return "Feature_Tag_SS15";
		if (909210483 == value) return "Feature_Tag_SS16";
		if (925987699 == value) return "Feature_Tag_SS17";
		if (942764915 == value) return "Feature_Tag_SS18";
		if (959542131 == value) return "Feature_Tag_SS19";
		if (808612723 == value) return "Feature_Tag_SS20";
		if (2037674867 == value) return "Feature_Tag_SSTY";
		if (1751348339 == value) return "Feature_Tag_STCH";
		if (1935832435 == value) return "Feature_Tag_SUBS";
		if (1936749939 == value) return "Feature_Tag_SUPS";
		if (1752397683 == value) return "Feature_Tag_SWSH";
		if (1819568500 == value) return "Feature_Tag_TITL";
		if (1869441652 == value) return "Feature_Tag_TJMO";
		if (1835101812 == value) return "Feature_Tag_TNAM";
		if (1836412532 == value) return "Feature_Tag_TNUM";
		if (1684107892 == value) return "Feature_Tag_TRAD";
		if (1684633460 == value) return "Feature_Tag_TWID";
		if (1667853941 == value) return "Feature_Tag_UNIC";
		if (1953259894 == value) return "Feature_Tag_VALT";
		if (1970561398 == value) return "Feature_Tag_VATU";
		if (1953654134 == value) return "Feature_Tag_VERT";
		if (1818323062 == value) return "Feature_Tag_VHAL";
		if (1869441654 == value) return "Feature_Tag_VJMO";
		if (1634626422 == value) return "Feature_Tag_VKNA";
		if (1852992374 == value) return "Feature_Tag_VKRN";
		if (1818325110 == value) return "Feature_Tag_VPAL";
		if (846492278 == value) return "Feature_Tag_VRT2";
		if (1920234102 == value) return "Feature_Tag_VRTR";
		if (1869768058 == value) return "Feature_Tag_ZERO";

		invalid_code_path();
		return "";
	}
};

template <> struct Reflect<Feature_Tag> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(Feature_Tag), "Feature_Tag", enum_count<Feature_Tag>());
		return &i;
	}
};

enum Lang_Tag : u32;
template <> struct Enum_Info<Lang_Tag> {
	static constexpr size_t get_count() { return 602; }
	static constexpr s64 get_min_value() { return 537477463; }
	static constexpr s64 get_max_value() { return 1314019667; }
	static const Lang_Tag index_value(s64 index) {
		static const Lang_Tag enum_values[] = {
			(Lang_Tag)541147713,
			(Lang_Tag)541803073,
			(Lang_Tag)541606721,
			(Lang_Tag)542262081,
			(Lang_Tag)542721089,
			(Lang_Tag)541804097,
			(Lang_Tag)542262849,
			(Lang_Tag)542590785,
			(Lang_Tag)542067009,
			(Lang_Tag)541150017,
			(Lang_Tag)542329921,
			(Lang_Tag)542395457,
			(Lang_Tag)541609281,
			(Lang_Tag)541544001,
			(Lang_Tag)1213222977,
			(Lang_Tag)541151809,
			(Lang_Tag)541545025,
			(Lang_Tag)541676097,
			(Lang_Tag)541807169,
			(Lang_Tag)541938497,
			(Lang_Tag)542397249,
			(Lang_Tag)541611073,
			(Lang_Tag)542266945,
			(Lang_Tag)541153089,
			(Lang_Tag)541940033,
			(Lang_Tag)541219393,
			(Lang_Tag)541416001,
			(Lang_Tag)541344066,
			(Lang_Tag)809779522,
			(Lang_Tag)541540674,
			(Lang_Tag)541868354,
			(Lang_Tag)541999426,
			(Lang_Tag)542261570,
			(Lang_Tag)542458178,
			(Lang_Tag)541278786,
			(Lang_Tag)542261826,
			(Lang_Tag)541606722,
			(Lang_Tag)542262082,
			(Lang_Tag)542721090,
			(Lang_Tag)541869378,
			(Lang_Tag)541934914,
			(Lang_Tag)542000450,
			(Lang_Tag)541280066,
			(Lang_Tag)542197570,
			(Lang_Tag)542263106,
			(Lang_Tag)541673538,
			(Lang_Tag)542066754,
			(Lang_Tag)541804866,
			(Lang_Tag)541870402,
			(Lang_Tag)542329154,
			(Lang_Tag)541739586,
			(Lang_Tag)541477698,
			(Lang_Tag)541674562,
			(Lang_Tag)541805634,
			(Lang_Tag)542002242,
			(Lang_Tag)542395458,
			(Lang_Tag)541216066,
			(Lang_Tag)541871426,
			(Lang_Tag)542330690,
			(Lang_Tag)542724162,
			(Lang_Tag)541413954,
			(Lang_Tag)541610562,
			(Lang_Tag)541676098,
			(Lang_Tag)541938242,
			(Lang_Tag)542659138,
			(Lang_Tag)541610818,
			(Lang_Tag)541807426,
			(Lang_Tag)541676610,
			(Lang_Tag)542331970,
			(Lang_Tag)541545794,
			(Lang_Tag)542529858,
			(Lang_Tag)541802819,
			(Lang_Tag)542392643,
			(Lang_Tag)541803075,
			(Lang_Tag)1313358659,
			(Lang_Tag)541214019,
			(Lang_Tag)541411395,
			(Lang_Tag)541542467,
			(Lang_Tag)541608003,
			(Lang_Tag)541673539,
			(Lang_Tag)541804611,
			(Lang_Tag)810240067,
			(Lang_Tag)542066755,
			(Lang_Tag)542132291,
			(Lang_Tag)542263363,
			(Lang_Tag)541149251,
			(Lang_Tag)542459971,
			(Lang_Tag)542722115,
			(Lang_Tag)541542211,
			(Lang_Tag)541149763,
			(Lang_Tag)541936195,
			(Lang_Tag)542264643,
			(Lang_Tag)542134083,
			(Lang_Tag)542265155,
			(Lang_Tag)542330691,
			(Lang_Tag)542134339,
			(Lang_Tag)541413955,
			(Lang_Tag)542265923,
			(Lang_Tag)542396995,
			(Lang_Tag)541217603,
			(Lang_Tag)541872963,
			(Lang_Tag)542724931,
			(Lang_Tag)541545539,
			(Lang_Tag)541807939,
			(Lang_Tag)541999428,
			(Lang_Tag)542261572,
			(Lang_Tag)542654788,
			(Lang_Tag)542262084,
			(Lang_Tag)542459204,
			(Lang_Tag)542066500,
			(Lang_Tag)542263108,
			(Lang_Tag)541542468,
			(Lang_Tag)542198084,
			(Lang_Tag)542525764,
			(Lang_Tag)542263876,
			(Lang_Tag)810699332,
			(Lang_Tag)541544004,
			(Lang_Tag)541740612,
			(Lang_Tag)541806148,
			(Lang_Tag)541676100,
			(Lang_Tag)541742404,
			(Lang_Tag)542004548,
			(Lang_Tag)542005828,
			(Lang_Tag)541672005,
			(Lang_Tag)542262085,
			(Lang_Tag)542065733,
			(Lang_Tag)541673029,
			(Lang_Tag)541871173,
			(Lang_Tag)541805893,
			(Lang_Tag)541544005,
			(Lang_Tag)542790213,
			(Lang_Tag)542135109,
			(Lang_Tag)542462789,
			(Lang_Tag)541676613,
			(Lang_Tag)542201157,
			(Lang_Tag)541808197,
			(Lang_Tag)542004805,
			(Lang_Tag)541415237,
			(Lang_Tag)541999430,
			(Lang_Tag)810434886,
			(Lang_Tag)542261574,
			(Lang_Tag)542392646,
			(Lang_Tag)542001478,
			(Lang_Tag)541674054,
			(Lang_Tag)541412422,
			(Lang_Tag)542133574,
			(Lang_Tag)541412934,
			(Lang_Tag)542003014,
			(Lang_Tag)542330694,
			(Lang_Tag)541151814,
			(Lang_Tag)541282886,
			(Lang_Tag)541676102,
			(Lang_Tag)541872710,
			(Lang_Tag)542134854,
			(Lang_Tag)541152326,
			(Lang_Tag)541873478,
			(Lang_Tag)542528838,
			(Lang_Tag)541344071,
			(Lang_Tag)541409607,
			(Lang_Tag)541540679,
			(Lang_Tag)541868359,
			(Lang_Tag)542261575,
			(Lang_Tag)542589255,
			(Lang_Tag)542786887,
			(Lang_Tag)541608263,
			(Lang_Tag)541870407,
			(Lang_Tag)810305863,
			(Lang_Tag)542133063,
			(Lang_Tag)541805639,
			(Lang_Tag)542788935,
			(Lang_Tag)542002759,
			(Lang_Tag)541544263,
			(Lang_Tag)542003015,
			(Lang_Tag)542003783,
			(Lang_Tag)542069319,
			(Lang_Tag)541152583,
			(Lang_Tag)541283655,
			(Lang_Tag)541480263,
			(Lang_Tag)541742407,
			(Lang_Tag)542790983,
			(Lang_Tag)541671752,
			(Lang_Tag)541868360,
			(Lang_Tag)542261576,
			(Lang_Tag)542458184,
			(Lang_Tag)542589256,
			(Lang_Tag)542720328,
			(Lang_Tag)542785864,
			(Lang_Tag)541999688,
			(Lang_Tag)542262600,
			(Lang_Tag)541870408,
			(Lang_Tag)542001480,
			(Lang_Tag)541150536,
			(Lang_Tag)542002504,
			(Lang_Tag)542068040,
			(Lang_Tag)541347400,
			(Lang_Tag)537481032,
			(Lang_Tag)541676104,
			(Lang_Tag)542528072,
			(Lang_Tag)542004552,
			(Lang_Tag)541415752,
			(Lang_Tag)809851208,
			(Lang_Tag)541147721,
			(Lang_Tag)541213257,
			(Lang_Tag)542065225,
			(Lang_Tag)542067273,
			(Lang_Tag)542065737,
			(Lang_Tag)541412425,
			(Lang_Tag)542067785,
			(Lang_Tag)541150793,
			(Lang_Tag)541347401,
			(Lang_Tag)541544009,
			(Lang_Tag)542461513,
			(Lang_Tag)541806665,
			(Lang_Tag)1213222985,
			(Lang_Tag)541676105,
			(Lang_Tag)542397001,
			(Lang_Tag)541872969,
			(Lang_Tag)541938505,
			(Lang_Tag)541152329,
			(Lang_Tag)542267209,
			(Lang_Tag)541933898,
			(Lang_Tag)541999434,
			(Lang_Tag)542523722,
			(Lang_Tag)542065226,
			(Lang_Tag)542393162,
			(Lang_Tag)541673802,
			(Lang_Tag)541349194,
			(Lang_Tag)541873482,
			(Lang_Tag)541213003,
			(Lang_Tag)809648459,
			(Lang_Tag)541278539,
			(Lang_Tag)541868363,
			(Lang_Tag)541999435,
			(Lang_Tag)542261579,
			(Lang_Tag)542392651,
			(Lang_Tag)542785867,
			(Lang_Tag)541410379,
			(Lang_Tag)541148491,
			(Lang_Tag)541214027,
			(Lang_Tag)541803851,
			(Lang_Tag)541411147,
			(Lang_Tag)541149259,
			(Lang_Tag)541804619,
			(Lang_Tag)541935691,
			(Lang_Tag)542328907,
			(Lang_Tag)542394443,
			(Lang_Tag)542525515,
			(Lang_Tag)542591051,
			(Lang_Tag)541804875,
			(Lang_Tag)542263627,
			(Lang_Tag)542329163,
			(Lang_Tag)542460235,
			(Lang_Tag)541346379,
			(Lang_Tag)542132811,
			(Lang_Tag)542788171,
			(Lang_Tag)542001995,
			(Lang_Tag)541936715,
			(Lang_Tag)541216075,
			(Lang_Tag)542002507,
			(Lang_Tag)542068043,
			(Lang_Tag)542330187,
			(Lang_Tag)542788939,
			(Lang_Tag)542264907,
			(Lang_Tag)541347659,
			(Lang_Tag)541609803,
			(Lang_Tag)541806411,
			(Lang_Tag)542003019,
			(Lang_Tag)541937483,
			(Lang_Tag)810438475,
			(Lang_Tag)542134091,
			(Lang_Tag)542265163,
			(Lang_Tag)542330699,
			(Lang_Tag)542789451,
			(Lang_Tag)541872203,
			(Lang_Tag)541676107,
			(Lang_Tag)541807179,
			(Lang_Tag)541872715,
			(Lang_Tag)541938251,
			(Lang_Tag)542003787,
			(Lang_Tag)542397003,
			(Lang_Tag)541610827,
			(Lang_Tag)810046283,
			(Lang_Tag)541676363,
			(Lang_Tag)541938507,
			(Lang_Tag)542593867,
			(Lang_Tag)541152587,
			(Lang_Tag)541676875,
			(Lang_Tag)541873483,
			(Lang_Tag)541939019,
			(Lang_Tag)542266699,
			(Lang_Tag)542463307,
			(Lang_Tag)542725451,
			(Lang_Tag)541808971,
			(Lang_Tag)542464331,
			(Lang_Tag)541344076,
			(Lang_Tag)541606220,
			(Lang_Tag)541802828,
			(Lang_Tag)541933900,
			(Lang_Tag)542064972,
			(Lang_Tag)542392652,
			(Lang_Tag)542785868,
			(Lang_Tag)542262092,
			(Lang_Tag)541803596,
			(Lang_Tag)542786892,
			(Lang_Tag)541739340,
			(Lang_Tag)541935948,
			(Lang_Tag)542001484,
			(Lang_Tag)542329164,
			(Lang_Tag)542132812,
			(Lang_Tag)541674316,
			(Lang_Tag)541150540,
			(Lang_Tag)541216076,
			(Lang_Tag)542068044,
			(Lang_Tag)542592332,
			(Lang_Tag)541937484,
			(Lang_Tag)541282892,
			(Lang_Tag)541217612,
			(Lang_Tag)541938508,
			(Lang_Tag)541611084,
			(Lang_Tag)542790732,
			(Lang_Tag)541152588,
			(Lang_Tag)541218124,
			(Lang_Tag)541545804,
			(Lang_Tag)541611340,
			(Lang_Tag)542070092,
			(Lang_Tag)541677132,
			(Lang_Tag)541344077,
			(Lang_Tag)541540685,
			(Lang_Tag)541606221,
			(Lang_Tag)541737293,
			(Lang_Tag)541802829,
			(Lang_Tag)541868365,
			(Lang_Tag)541933901,
			(Lang_Tag)541999437,
			(Lang_Tag)542130509,
			(Lang_Tag)542261581,
			(Lang_Tag)542589261,
			(Lang_Tag)541999693,
			(Lang_Tag)542065229,
			(Lang_Tag)541606733,
			(Lang_Tag)542262093,
			(Lang_Tag)541410381,
			(Lang_Tag)542262349,
			(Lang_Tag)542000461,
			(Lang_Tag)542262605,
			(Lang_Tag)541148749,
			(Lang_Tag)541410893,
			(Lang_Tag)542001485,
			(Lang_Tag)542787917,
			(Lang_Tag)541346637,
			(Lang_Tag)542264141,
			(Lang_Tag)542591821,
			(Lang_Tag)541412429,
			(Lang_Tag)541543501,
			(Lang_Tag)542002253,
			(Lang_Tag)542264397,
			(Lang_Tag)542723149,
			(Lang_Tag)541347405,
			(Lang_Tag)541544013,
			(Lang_Tag)541675085,
			(Lang_Tag)541806157,
			(Lang_Tag)542658125,
			(Lang_Tag)541609805,
			(Lang_Tag)541806413,
			(Lang_Tag)541871949,
			(Lang_Tag)542003021,
			(Lang_Tag)542265165,
			(Lang_Tag)542330701,
			(Lang_Tag)541676109,
			(Lang_Tag)541611085,
			(Lang_Tag)542331981,
			(Lang_Tag)542004557,
			(Lang_Tag)542332237,
			(Lang_Tag)541873997,
			(Lang_Tag)542594893,
			(Lang_Tag)542005581,
			(Lang_Tag)542005837,
			(Lang_Tag)541540686,
			(Lang_Tag)541606222,
			(Lang_Tag)541999438,
			(Lang_Tag)542130510,
			(Lang_Tag)542327118,
			(Lang_Tag)542458190,
			(Lang_Tag)542523726,
			(Lang_Tag)542262094,
			(Lang_Tag)541213774,
			(Lang_Tag)541279310,
			(Lang_Tag)541541454,
			(Lang_Tag)542327886,
			(Lang_Tag)542131534,
			(Lang_Tag)542590286,
			(Lang_Tag)541149006,
			(Lang_Tag)542263118,
			(Lang_Tag)541280334,
			(Lang_Tag)542329166,
			(Lang_Tag)542460238,
			(Lang_Tag)541870926,
			(Lang_Tag)542067534,
			(Lang_Tag)541346894,
			(Lang_Tag)541413198,
			(Lang_Tag)541544270,
			(Lang_Tag)542265166,
			(Lang_Tag)542527310,
			(Lang_Tag)541938510,
			(Lang_Tag)542069582,
			(Lang_Tag)541152334,
			(Lang_Tag)542069838,
			(Lang_Tag)541940046,
			(Lang_Tag)542005582,
			(Lang_Tag)541153870,
			(Lang_Tag)541672271,
			(Lang_Tag)542262095,
			(Lang_Tag)541215311,
			(Lang_Tag)541676111,
			(Lang_Tag)542069327,
			(Lang_Tag)542331727,
			(Lang_Tag)541147472,
			(Lang_Tag)541540688,
			(Lang_Tag)541868368,
			(Lang_Tag)541933904,
			(Lang_Tag)541999440,
			(Lang_Tag)542130512,
			(Lang_Tag)810565968,
			(Lang_Tag)542327120,
			(Lang_Tag)542458192,
			(Lang_Tag)541279056,
			(Lang_Tag)541344592,
			(Lang_Tag)541279312,
			(Lang_Tag)542263120,
			(Lang_Tag)541804624,
			(Lang_Tag)541608272,
			(Lang_Tag)541870416,
			(Lang_Tag)541543504,
			(Lang_Tag)541805648,
			(Lang_Tag)542330192,
			(Lang_Tag)541216336,
			(Lang_Tag)541609808,
			(Lang_Tag)542003024,
			(Lang_Tag)542069328,
			(Lang_Tag)541545552,
			(Lang_Tag)542070608,
			(Lang_Tag)542001489,
			(Lang_Tag)541283665,
			(Lang_Tag)541611345,
			(Lang_Tag)542790993,
			(Lang_Tag)541677137,
			(Lang_Tag)541611857,
			(Lang_Tag)541737298,
			(Lang_Tag)542261586,
			(Lang_Tag)542458450,
			(Lang_Tag)542262098,
			(Lang_Tag)541738322,
			(Lang_Tag)541149522,
			(Lang_Tag)541477202,
			(Lang_Tag)542394706,
			(Lang_Tag)542591826,
			(Lang_Tag)542330194,
			(Lang_Tag)542723410,
			(Lang_Tag)541937490,
			(Lang_Tag)542723922,
			(Lang_Tag)542724946,
			(Lang_Tag)541938770,
			(Lang_Tag)541152594,
			(Lang_Tag)542004562,
			(Lang_Tag)542135634,
			(Lang_Tag)542332242,
			(Lang_Tag)541344083,
			(Lang_Tag)541999443,
			(Lang_Tag)542327123,
			(Lang_Tag)542392659,
			(Lang_Tag)542720339,
			(Lang_Tag)541999955,
			(Lang_Tag)542065491,
			(Lang_Tag)541803859,
			(Lang_Tag)541869395,
			(Lang_Tag)541149011,
			(Lang_Tag)542066515,
			(Lang_Tag)542328659,
			(Lang_Tag)541673555,
			(Lang_Tag)542001235,
			(Lang_Tag)541215059,
			(Lang_Tag)541346131,
			(Lang_Tag)541542739,
			(Lang_Tag)542329683,
			(Lang_Tag)542722899,
			(Lang_Tag)542327635,
			(Lang_Tag)541150291,
			(Lang_Tag)542526547,
			(Lang_Tag)541871443,
			(Lang_Tag)542068051,
			(Lang_Tag)541150803,
			(Lang_Tag)809586259,
			(Lang_Tag)541347411,
			(Lang_Tag)541609555,
			(Lang_Tag)541806163,
			(Lang_Tag)541544275,
			(Lang_Tag)542134099,
			(Lang_Tag)542396243,
			(Lang_Tag)541675859,
			(Lang_Tag)541217363,
			(Lang_Tag)541348435,
			(Lang_Tag)541807187,
			(Lang_Tag)542265939,
			(Lang_Tag)541872979,
			(Lang_Tag)541938515,
			(Lang_Tag)542200915,
			(Lang_Tag)541807955,
			(Lang_Tag)542004563,
			(Lang_Tag)542266707,
			(Lang_Tag)541152851,
			(Lang_Tag)541414995,
			(Lang_Tag)541153107,
			(Lang_Tag)541808467,
			(Lang_Tag)542791507,
			(Lang_Tag)542398547,
			(Lang_Tag)542464083,
			(Lang_Tag)541874515,
			(Lang_Tag)542267731,
			(Lang_Tag)1163024723,
			(Lang_Tag)1246910803,
			(Lang_Tag)1314019667,
			(Lang_Tag)541874771,
			(Lang_Tag)541213012,
			(Lang_Tag)541737300,
			(Lang_Tag)541933908,
			(Lang_Tag)542392660,
			(Lang_Tag)542262100,
			(Lang_Tag)541344852,
			(Lang_Tag)541869396,
			(Lang_Tag)542393684,
			(Lang_Tag)541869908,
			(Lang_Tag)542000980,
			(Lang_Tag)542263124,
			(Lang_Tag)542721876,
			(Lang_Tag)541149268,
			(Lang_Tag)542394452,
			(Lang_Tag)541215060,
			(Lang_Tag)542525780,
			(Lang_Tag)541936468,
			(Lang_Tag)541609300,
			(Lang_Tag)542002516,
			(Lang_Tag)541150804,
			(Lang_Tag)541412948,
			(Lang_Tag)541544020,
			(Lang_Tag)541347668,
			(Lang_Tag)809783124,
			(Lang_Tag)541675604,
			(Lang_Tag)541807188,
			(Lang_Tag)541545300,
			(Lang_Tag)541741908,
			(Lang_Tag)541152596,
			(Lang_Tag)541939028,
			(Lang_Tag)541873492,
			(Lang_Tag)542528852,
			(Lang_Tag)541873748,
			(Lang_Tag)541677396,
			(Lang_Tag)542792020,
			(Lang_Tag)541940308,
			(Lang_Tag)542071380,
			(Lang_Tag)541934677,
			(Lang_Tag)542264149,
			(Lang_Tag)541216085,
			(Lang_Tag)541348437,
			(Lang_Tag)541217621,
			(Lang_Tag)541546837,
			(Lang_Tag)541219413,
			(Lang_Tag)541279574,
			(Lang_Tag)542000470,
			(Lang_Tag)542394710,
			(Lang_Tag)541871958,
			(Lang_Tag)542069334,
			(Lang_Tag)537477463,
			(Lang_Tag)541540695,
			(Lang_Tag)542261591,
			(Lang_Tag)542262103,
			(Lang_Tag)541869399,
			(Lang_Tag)542002263,
			(Lang_Tag)541477975,
			(Lang_Tag)541938775,
			(Lang_Tag)541344344,
			(Lang_Tag)541477720,
			(Lang_Tag)542328920,
			(Lang_Tag)541215320,
			(Lang_Tag)541544280,
			(Lang_Tag)541413464,
			(Lang_Tag)541802841,
			(Lang_Tag)542064985,
			(Lang_Tag)542130521,
			(Lang_Tag)541147737,
			(Lang_Tag)542262105,
			(Lang_Tag)541280601,
			(Lang_Tag)541935961,
			(Lang_Tag)541148506,
			(Lang_Tag)541607770,
			(Lang_Tag)541149274,
			(Lang_Tag)541608026,
			(Lang_Tag)542132314,
			(Lang_Tag)542328922,
			(Lang_Tag)542394458,
			(Lang_Tag)541347418,
			(Lang_Tag)541873498,
			(Lang_Tag)541153882,
		};
		assert(index >= 0 && index < static_count(enum_values));
		return enum_values[index];
	}
	static const String string(Lang_Tag value) {
		if (541147713 == value) return "Lang_Tag_ABAZA";
		if (541803073 == value) return "Lang_Tag_ABKHAZIAN";
		if (541606721 == value) return "Lang_Tag_ACHOLI";
		if (542262081 == value) return "Lang_Tag_ACHI";
		if (542721089 == value) return "Lang_Tag_ADYGHE";
		if (541804097 == value) return "Lang_Tag_AFRIKAANS";
		if (542262849 == value) return "Lang_Tag_AFAR";
		if (542590785 == value) return "Lang_Tag_AGAW";
		if (542067009 == value) return "Lang_Tag_AITON";
		if (541150017 == value) return "Lang_Tag_AKAN";
		if (542329921 == value) return "Lang_Tag_ALSATIAN";
		if (542395457 == value) return "Lang_Tag_ALTAI";
		if (541609281 == value) return "Lang_Tag_AMHARIC";
		if (541544001 == value) return "Lang_Tag_ANGLO_SAXON";
		if (1213222977 == value) return "Lang_Tag_PHONETIC_TRANSCRIPTION";
		if (541151809 == value) return "Lang_Tag_ARABIC";
		if (541545025 == value) return "Lang_Tag_ARAGONESE";
		if (541676097 == value) return "Lang_Tag_AARI";
		if (541807169 == value) return "Lang_Tag_RAKHINE";
		if (541938497 == value) return "Lang_Tag_ASSAMESE";
		if (542397249 == value) return "Lang_Tag_ASTURIAN";
		if (541611073 == value) return "Lang_Tag_ATHAPASKAN";
		if (542266945 == value) return "Lang_Tag_AVAR";
		if (541153089 == value) return "Lang_Tag_AWADHI";
		if (541940033 == value) return "Lang_Tag_AYMARA";
		if (541219393 == value) return "Lang_Tag_TORKI";
		if (541416001 == value) return "Lang_Tag_AZERBAIJANI";
		if (541344066 == value) return "Lang_Tag_BADAGA";
		if (809779522 == value) return "Lang_Tag_BANDA";
		if (541540674 == value) return "Lang_Tag_BAGHELKHANDI";
		if (541868354 == value) return "Lang_Tag_BALKAR";
		if (541999426 == value) return "Lang_Tag_BALINESE";
		if (542261570 == value) return "Lang_Tag_BAVARIAN";
		if (542458178 == value) return "Lang_Tag_BAULE";
		if (541278786 == value) return "Lang_Tag_BATAK_TOBA";
		if (542261826 == value) return "Lang_Tag_BERBER";
		if (541606722 == value) return "Lang_Tag_BENCH";
		if (542262082 == value) return "Lang_Tag_BIBLE_CREE";
		if (542721090 == value) return "Lang_Tag_BANDJALANG";
		if (541869378 == value) return "Lang_Tag_BELARUSSIAN";
		if (541934914 == value) return "Lang_Tag_BEMBA";
		if (542000450 == value) return "Lang_Tag_BENGALI";
		if (541280066 == value) return "Lang_Tag_HARYANVI";
		if (542197570 == value) return "Lang_Tag_BAGRI";
		if (542263106 == value) return "Lang_Tag_BULGARIAN";
		if (541673538 == value) return "Lang_Tag_BHILI";
		if (542066754 == value) return "Lang_Tag_BHOJPURI";
		if (541804866 == value) return "Lang_Tag_BIKOL";
		if (541870402 == value) return "Lang_Tag_BILEN";
		if (542329154 == value) return "Lang_Tag_BISLAMA";
		if (541739586 == value) return "Lang_Tag_KANAUJI";
		if (541477698 == value) return "Lang_Tag_BLACKFOOT";
		if (541674562 == value) return "Lang_Tag_BALUCHI";
		if (541805634 == value) return "Lang_Tag_PAO_KAREN";
		if (542002242 == value) return "Lang_Tag_BALANTE";
		if (542395458 == value) return "Lang_Tag_BALTI";
		if (541216066 == value) return "Lang_Tag_BAMBARA_BAMANANKAN";
		if (541871426 == value) return "Lang_Tag_BAMILEKE";
		if (542330690 == value) return "Lang_Tag_BOSNIAN";
		if (542724162 == value) return "Lang_Tag_BISHNUPRIYA_MANIPURI";
		if (541413954 == value) return "Lang_Tag_BRETON";
		if (541610562 == value) return "Lang_Tag_BRAHUI";
		if (541676098 == value) return "Lang_Tag_BRAJ_BHASHA";
		if (541938242 == value) return "Lang_Tag_BURMESE";
		if (542659138 == value) return "Lang_Tag_BODO";
		if (541610818 == value) return "Lang_Tag_BASHKIR";
		if (541807426 == value) return "Lang_Tag_BURUSHASKI";
		if (541676610 == value) return "Lang_Tag_BETI";
		if (542331970 == value) return "Lang_Tag_BATAK_SIMALUNGUN";
		if (541545794 == value) return "Lang_Tag_BUGIS";
		if (542529858 == value) return "Lang_Tag_MEDUMBA";
		if (541802819 == value) return "Lang_Tag_KAQCHIKEL";
		if (542392643 == value) return "Lang_Tag_CATALAN";
		if (541803075 == value) return "Lang_Tag_ZAMBOANGA_CHAVACANO";
		if (1313358659 == value) return "Lang_Tag_CHINANTEC";
		if (541214019 == value) return "Lang_Tag_CEBUANO";
		if (541411395 == value) return "Lang_Tag_CHECHEN";
		if (541542467 == value) return "Lang_Tag_CHAHA_GURAGE";
		if (541608003 == value) return "Lang_Tag_CHATTISGARHI";
		if (541673539 == value) return "Lang_Tag_CHICHEWA_CHEWA_NYANJA";
		if (541804611 == value) return "Lang_Tag_CHUKCHI";
		if (810240067 == value) return "Lang_Tag_CHUUKESE";
		if (542066755 == value) return "Lang_Tag_CHOCTAW";
		if (542132291 == value) return "Lang_Tag_CHIPEWYAN";
		if (542263363 == value) return "Lang_Tag_CHEROKEE";
		if (541149251 == value) return "Lang_Tag_CHAMORRO";
		if (542459971 == value) return "Lang_Tag_CHUVASH";
		if (542722115 == value) return "Lang_Tag_CHEYENNE";
		if (541542211 == value) return "Lang_Tag_CHIGA";
		if (541149763 == value) return "Lang_Tag_WESTERN_CHAM";
		if (541936195 == value) return "Lang_Tag_EASTERN_CHAM";
		if (542264643 == value) return "Lang_Tag_COMORIAN";
		if (542134083 == value) return "Lang_Tag_COPTIC";
		if (542265155 == value) return "Lang_Tag_CORNISH";
		if (542330691 == value) return "Lang_Tag_CORSICAN";
		if (542134339 == value) return "Lang_Tag_CREOLES";
		if (541413955 == value) return "Lang_Tag_CREE";
		if (542265923 == value) return "Lang_Tag_CARRIER";
		if (542396995 == value) return "Lang_Tag_CRIMEAN_TATAR";
		if (541217603 == value) return "Lang_Tag_KASHUBIAN";
		if (541872963 == value) return "Lang_Tag_CHURCH_SLAVONIC";
		if (542724931 == value) return "Lang_Tag_CZECH";
		if (541545539 == value) return "Lang_Tag_CHITTAGONIAN";
		if (541807939 == value) return "Lang_Tag_SAN_BLAS_KUNA";
		if (541999428 == value) return "Lang_Tag_DANISH";
		if (542261572 == value) return "Lang_Tag_DARGWA";
		if (542654788 == value) return "Lang_Tag_DAYI";
		if (542262084 == value) return "Lang_Tag_WOODS_CREE";
		if (542459204 == value) return "Lang_Tag_GERMAN";
		if (542066500 == value) return "Lang_Tag_DOGRI1";
		if (542263108 == value) return "Lang_Tag_DOGRI2";
		if (541542468 == value) return "Lang_Tag_DHANGU";
		if (542198084 == value) return "Lang_Tag_DIMLI";
		if (542525764 == value) return "Lang_Tag_DIVEHI_DHIVEHI_MALDIVIAN";
		if (542263876 == value) return "Lang_Tag_ZARMA";
		if (810699332 == value) return "Lang_Tag_DJAMBARRPUYNGU";
		if (541544004 == value) return "Lang_Tag_DANGME";
		if (541740612 == value) return "Lang_Tag_DAN";
		if (541806148 == value) return "Lang_Tag_DINKA";
		if (541676100 == value) return "Lang_Tag_DARI";
		if (541742404 == value) return "Lang_Tag_DHUWAL";
		if (542004548 == value) return "Lang_Tag_DUNGAN";
		if (542005828 == value) return "Lang_Tag_DZONGKHA";
		if (541672005 == value) return "Lang_Tag_EBIRA";
		if (542262085 == value) return "Lang_Tag_EASTERN_CREE";
		if (542065733 == value) return "Lang_Tag_EDO";
		if (541673029 == value) return "Lang_Tag_EFIK";
		if (541871173 == value) return "Lang_Tag_GREEK";
		if (541805893 == value) return "Lang_Tag_EASTERN_MANINKAKAN";
		if (541544005 == value) return "Lang_Tag_ENGLISH";
		if (542790213 == value) return "Lang_Tag_ERZYA";
		if (542135109 == value) return "Lang_Tag_SPANISH";
		if (542462789 == value) return "Lang_Tag_CENTRAL_YUPIK";
		if (541676613 == value) return "Lang_Tag_ESTONIAN";
		if (542201157 == value) return "Lang_Tag_BASQUE";
		if (541808197 == value) return "Lang_Tag_EVENKI";
		if (542004805 == value) return "Lang_Tag_EVEN";
		if (541415237 == value) return "Lang_Tag_EWE";
		if (541999430 == value) return "Lang_Tag_FRENCH_ANTILLEAN";
		if (810434886 == value) return "Lang_Tag_FANG";
		if (542261574 == value) return "Lang_Tag_PERSIAN";
		if (542392646 == value) return "Lang_Tag_FANTI";
		if (542001478 == value) return "Lang_Tag_FINNISH";
		if (541674054 == value) return "Lang_Tag_FIJIAN";
		if (541412422 == value) return "Lang_Tag_DUTCH_FLEMISH";
		if (542133574 == value) return "Lang_Tag_FE_FE";
		if (541412934 == value) return "Lang_Tag_FOREST_NENETS";
		if (542003014 == value) return "Lang_Tag_FON";
		if (542330694 == value) return "Lang_Tag_FAROESE";
		if (541151814 == value) return "Lang_Tag_FRENCH";
		if (541282886 == value) return "Lang_Tag_CAJUN_FRENCH";
		if (541676102 == value) return "Lang_Tag_FRISIAN";
		if (541872710 == value) return "Lang_Tag_FRIULIAN";
		if (542134854 == value) return "Lang_Tag_ARPITAN";
		if (541152326 == value) return "Lang_Tag_FUTA";
		if (541873478 == value) return "Lang_Tag_FULAH";
		if (542528838 == value) return "Lang_Tag_NIGERIAN_FULFULDE";
		if (541344071 == value) return "Lang_Tag_GA";
		if (541409607 == value) return "Lang_Tag_SCOTTISH_GAELIC_GAELIC";
		if (541540679 == value) return "Lang_Tag_GAGAUZ";
		if (541868359 == value) return "Lang_Tag_GALICIAN";
		if (542261575 == value) return "Lang_Tag_GARSHUNI";
		if (542589255 == value) return "Lang_Tag_GARHWALI";
		if (542786887 == value) return "Lang_Tag_GEEZ";
		if (541608263 == value) return "Lang_Tag_GITHABUL";
		if (541870407 == value) return "Lang_Tag_GILYAK";
		if (810305863 == value) return "Lang_Tag_KIRIBATI_GILBERTESE";
		if (542133063 == value) return "Lang_Tag_KPELLE_GUINEA";
		if (541805639 == value) return "Lang_Tag_GILAKI";
		if (542788935 == value) return "Lang_Tag_GUMUZ";
		if (542002759 == value) return "Lang_Tag_GUMATJ";
		if (541544263 == value) return "Lang_Tag_GOGO";
		if (542003015 == value) return "Lang_Tag_GONDI";
		if (542003783 == value) return "Lang_Tag_GREENLANDIC";
		if (542069319 == value) return "Lang_Tag_GARO";
		if (541152583 == value) return "Lang_Tag_GUARANI";
		if (541283655 == value) return "Lang_Tag_WAYUU";
		if (541480263 == value) return "Lang_Tag_GUPAPUYNGU";
		if (541742407 == value) return "Lang_Tag_GUJARATI";
		if (542790983 == value) return "Lang_Tag_GUSII";
		if (541671752 == value) return "Lang_Tag_HAITIAN_HAITIAN_CREOLE";
		if (541868360 == value) return "Lang_Tag_HALAM_FALAM_CHIN";
		if (542261576 == value) return "Lang_Tag_HARAUTI";
		if (542458184 == value) return "Lang_Tag_HAUSA";
		if (542589256 == value) return "Lang_Tag_HAWAIIAN";
		if (542720328 == value) return "Lang_Tag_HAYA";
		if (542785864 == value) return "Lang_Tag_HAZARAGI";
		if (541999688 == value) return "Lang_Tag_HAMMER_BANNA";
		if (542262600 == value) return "Lang_Tag_HERERO";
		if (541870408 == value) return "Lang_Tag_HILIGAYNON";
		if (542001480 == value) return "Lang_Tag_HINDI";
		if (541150536 == value) return "Lang_Tag_HIGH_MARI";
		if (542002504 == value) return "Lang_Tag_HMONG";
		if (542068040 == value) return "Lang_Tag_HIRI_MOTU";
		if (541347400 == value) return "Lang_Tag_HINDKO";
		if (537481032 == value) return "Lang_Tag_HO";
		if (541676104 == value) return "Lang_Tag_HARARI";
		if (542528072 == value) return "Lang_Tag_CROATIAN";
		if (542004552 == value) return "Lang_Tag_HUNGARIAN";
		if (541415752 == value) return "Lang_Tag_ARMENIAN";
		if (809851208 == value) return "Lang_Tag_ARMENIAN_EAST";
		if (541147721 == value) return "Lang_Tag_IBAN";
		if (541213257 == value) return "Lang_Tag_IBIBIO";
		if (542065225 == value) return "Lang_Tag_IGBO";
		if (542067273 == value) return "Lang_Tag_IJO_LANGUAGES";
		if (542065737 == value) return "Lang_Tag_IDO";
		if (541412425 == value) return "Lang_Tag_INTERLINGUE";
		if (542067785 == value) return "Lang_Tag_ILOKANO";
		if (541150793 == value) return "Lang_Tag_INTERLINGUA";
		if (541347401 == value) return "Lang_Tag_INDONESIAN";
		if (541544009 == value) return "Lang_Tag_INGUSH";
		if (542461513 == value) return "Lang_Tag_INUKTITUT";
		if (541806665 == value) return "Lang_Tag_INUPIAT";
		if (1213222985 == value) return "Lang_Tag_PHONETIC_TRANSCRIPTION_IPA";
		if (541676105 == value) return "Lang_Tag_IRISH";
		if (542397001 == value) return "Lang_Tag_IRISH_TRADITIONAL";
		if (541872969 == value) return "Lang_Tag_ICELANDIC";
		if (541938505 == value) return "Lang_Tag_INARI_SAMI";
		if (541152329 == value) return "Lang_Tag_ITALIAN";
		if (542267209 == value) return "Lang_Tag_HEBREW";
		if (541933898 == value) return "Lang_Tag_JAMAICAN_CREOLE";
		if (541999434 == value) return "Lang_Tag_JAPANESE";
		if (542523722 == value) return "Lang_Tag_JAVANESE";
		if (542065226 == value) return "Lang_Tag_LOJBAN";
		if (542393162 == value) return "Lang_Tag_KRYMCHAK";
		if (541673802 == value) return "Lang_Tag_YIDDISH";
		if (541349194 == value) return "Lang_Tag_LADINO";
		if (541873482 == value) return "Lang_Tag_JULA";
		if (541213003 == value) return "Lang_Tag_KABARDIAN";
		if (809648459 == value) return "Lang_Tag_KABYLE";
		if (541278539 == value) return "Lang_Tag_KACHCHI";
		if (541868363 == value) return "Lang_Tag_KALENJIN";
		if (541999435 == value) return "Lang_Tag_KANNADA";
		if (542261579 == value) return "Lang_Tag_KARACHAY";
		if (542392651 == value) return "Lang_Tag_GEORGIAN";
		if (542785867 == value) return "Lang_Tag_KAZAKH";
		if (541410379 == value) return "Lang_Tag_MAKONDE";
		if (541148491 == value) return "Lang_Tag_KABUVERDIANU_CRIOULO";
		if (541214027 == value) return "Lang_Tag_KEBENA";
		if (541803851 == value) return "Lang_Tag_KEKCHI";
		if (541411147 == value) return "Lang_Tag_KHUTSURI_GEORGIAN";
		if (541149259 == value) return "Lang_Tag_KHAKASS";
		if (541804619 == value) return "Lang_Tag_KHANTY_KAZIM";
		if (541935691 == value) return "Lang_Tag_KHMER";
		if (542328907 == value) return "Lang_Tag_KHANTY_SHURISHKAR";
		if (542394443 == value) return "Lang_Tag_KHAMTI_SHAN";
		if (542525515 == value) return "Lang_Tag_KHANTY_VAKHI";
		if (542591051 == value) return "Lang_Tag_KHOWAR";
		if (541804875 == value) return "Lang_Tag_KIKUYU_GIKUYU";
		if (542263627 == value) return "Lang_Tag_KIRGHIZ_KYRGYZ";
		if (542329163 == value) return "Lang_Tag_KISII";
		if (542460235 == value) return "Lang_Tag_KIRMANJKI";
		if (541346379 == value) return "Lang_Tag_SOUTHERN_KIWAI";
		if (542132811 == value) return "Lang_Tag_EASTERN_PWO_KAREN";
		if (542788171 == value) return "Lang_Tag_BUMTHANGKHA";
		if (542001995 == value) return "Lang_Tag_KOKNI";
		if (541936715 == value) return "Lang_Tag_KALMYK";
		if (541216075 == value) return "Lang_Tag_KAMBA";
		if (542002507 == value) return "Lang_Tag_KUMAONI";
		if (542068043 == value) return "Lang_Tag_KOMO";
		if (542330187 == value) return "Lang_Tag_KOMSO";
		if (542788939 == value) return "Lang_Tag_KHORASANI_TURKIC";
		if (542264907 == value) return "Lang_Tag_KANURI";
		if (541347659 == value) return "Lang_Tag_KODAGU";
		if (541609803 == value) return "Lang_Tag_KOREAN_OLD_HANGUL";
		if (541806411 == value) return "Lang_Tag_KONKANI";
		if (542003019 == value) return "Lang_Tag_KIKONGO";
		if (541937483 == value) return "Lang_Tag_KOMI";
		if (810438475 == value) return "Lang_Tag_KONGO";
		if (542134091 == value) return "Lang_Tag_KOMI_PERMYAK";
		if (542265163 == value) return "Lang_Tag_KOREAN";
		if (542330699 == value) return "Lang_Tag_KOSRAEAN";
		if (542789451 == value) return "Lang_Tag_KOMI_ZYRIAN";
		if (541872203 == value) return "Lang_Tag_KPELLE";
		if (541676107 == value) return "Lang_Tag_KRIO";
		if (541807179 == value) return "Lang_Tag_KARAKALPAK";
		if (541872715 == value) return "Lang_Tag_KARELIAN";
		if (541938251 == value) return "Lang_Tag_KARAIM";
		if (542003787 == value) return "Lang_Tag_KAREN";
		if (542397003 == value) return "Lang_Tag_KOORETE";
		if (541610827 == value) return "Lang_Tag_KASHMIRI";
		if (810046283 == value) return "Lang_Tag_RIPUARIAN";
		if (541676363 == value) return "Lang_Tag_KHASI";
		if (541938507 == value) return "Lang_Tag_KILDIN_SAMI";
		if (542593867 == value) return "Lang_Tag_SGAW_KAREN";
		if (541152587 == value) return "Lang_Tag_KUANYAMA";
		if (541676875 == value) return "Lang_Tag_KUI";
		if (541873483 == value) return "Lang_Tag_KULVI";
		if (541939019 == value) return "Lang_Tag_KUMYK";
		if (542266699 == value) return "Lang_Tag_KURDISH";
		if (542463307 == value) return "Lang_Tag_KURUKH";
		if (542725451 == value) return "Lang_Tag_KUY";
		if (541808971 == value) return "Lang_Tag_KORYAK";
		if (542464331 == value) return "Lang_Tag_WESTERN_KAYAH";
		if (541344076 == value) return "Lang_Tag_LADIN";
		if (541606220 == value) return "Lang_Tag_LAHULI";
		if (541802828 == value) return "Lang_Tag_LAK";
		if (541933900 == value) return "Lang_Tag_LAMBANI";
		if (542064972 == value) return "Lang_Tag_LAO";
		if (542392652 == value) return "Lang_Tag_LATIN";
		if (542785868 == value) return "Lang_Tag_LAZ";
		if (542262092 == value) return "Lang_Tag_L_CREE";
		if (541803596 == value) return "Lang_Tag_LADAKHI";
		if (542786892 == value) return "Lang_Tag_LEZGI";
		if (541739340 == value) return "Lang_Tag_LIGURIAN";
		if (541935948 == value) return "Lang_Tag_LIMBURGISH";
		if (542001484 == value) return "Lang_Tag_LINGALA";
		if (542329164 == value) return "Lang_Tag_LISU";
		if (542132812 == value) return "Lang_Tag_LAMPUNG";
		if (541674316 == value) return "Lang_Tag_LAKI";
		if (541150540 == value) return "Lang_Tag_LOW_MARI";
		if (541216076 == value) return "Lang_Tag_LIMBU";
		if (542068044 == value) return "Lang_Tag_LOMBARD";
		if (542592332 == value) return "Lang_Tag_LOMWE";
		if (541937484 == value) return "Lang_Tag_LOMA";
		if (541282892 == value) return "Lang_Tag_LURI";
		if (541217612 == value) return "Lang_Tag_LOWER_SORBIAN";
		if (541938508 == value) return "Lang_Tag_LULE_SAMI";
		if (541611084 == value) return "Lang_Tag_LITHUANIAN";
		if (542790732 == value) return "Lang_Tag_LUXEMBOURGISH";
		if (541152588 == value) return "Lang_Tag_LUBA_LULUA";
		if (541218124 == value) return "Lang_Tag_LUBA_KATANGA";
		if (541545804 == value) return "Lang_Tag_GANDA";
		if (541611340 == value) return "Lang_Tag_LUYIA";
		if (542070092 == value) return "Lang_Tag_LUO";
		if (541677132 == value) return "Lang_Tag_LATVIAN";
		if (541344077 == value) return "Lang_Tag_MADURA";
		if (541540685 == value) return "Lang_Tag_MAGAHI";
		if (541606221 == value) return "Lang_Tag_MARSHALLESE";
		if (541737293 == value) return "Lang_Tag_MAJANG";
		if (541802829 == value) return "Lang_Tag_MAKHUWA";
		if (541868365 == value) return "Lang_Tag_MALAYALAM";
		if (541933901 == value) return "Lang_Tag_MAM";
		if (541999437 == value) return "Lang_Tag_MANSI";
		if (542130509 == value) return "Lang_Tag_MAPUDUNGUN";
		if (542261581 == value) return "Lang_Tag_MARATHI";
		if (542589261 == value) return "Lang_Tag_MARWARI";
		if (541999693 == value) return "Lang_Tag_MBUNDU";
		if (542065229 == value) return "Lang_Tag_MBO";
		if (541606733 == value) return "Lang_Tag_MANCHU";
		if (542262093 == value) return "Lang_Tag_MOOSE_CREE";
		if (541410381 == value) return "Lang_Tag_MENDE";
		if (542262349 == value) return "Lang_Tag_MANDAR";
		if (542000461 == value) return "Lang_Tag_MEEN";
		if (542262605 == value) return "Lang_Tag_MERU";
		if (541148749 == value) return "Lang_Tag_PATTANI_MALAY";
		if (541410893 == value) return "Lang_Tag_MORISYEN";
		if (542001485 == value) return "Lang_Tag_MINANGKABAU";
		if (542787917 == value) return "Lang_Tag_MIZO";
		if (541346637 == value) return "Lang_Tag_MACEDONIAN";
		if (542264141 == value) return "Lang_Tag_MAKASAR";
		if (542591821 == value) return "Lang_Tag_KITUBA";
		if (541412429 == value) return "Lang_Tag_MALE";
		if (541543501 == value) return "Lang_Tag_MALAGASY";
		if (542002253 == value) return "Lang_Tag_MALINKE";
		if (542264397 == value) return "Lang_Tag_MALAYALAM_REFORMED";
		if (542723149 == value) return "Lang_Tag_MALAY";
		if (541347405 == value) return "Lang_Tag_MANDINKA";
		if (541544013 == value) return "Lang_Tag_MONGOLIAN";
		if (541675085 == value) return "Lang_Tag_MANIPURI";
		if (541806157 == value) return "Lang_Tag_MANINKA";
		if (542658125 == value) return "Lang_Tag_MANX";
		if (541609805 == value) return "Lang_Tag_MOHAWK";
		if (541806413 == value) return "Lang_Tag_MOKSHA";
		if (541871949 == value) return "Lang_Tag_MOLDAVIAN";
		if (542003021 == value) return "Lang_Tag_MON";
		if (542265165 == value) return "Lang_Tag_MOROCCAN";
		if (542330701 == value) return "Lang_Tag_MOSSI";
		if (541676109 == value) return "Lang_Tag_MAORI";
		if (541611085 == value) return "Lang_Tag_MAITHILI";
		if (542331981 == value) return "Lang_Tag_MALTESE";
		if (542004557 == value) return "Lang_Tag_MUNDARI";
		if (542332237 == value) return "Lang_Tag_MUSCOGEE";
		if (541873997 == value) return "Lang_Tag_MIRANDESE";
		if (542594893 == value) return "Lang_Tag_HMONG_DAW";
		if (542005581 == value) return "Lang_Tag_MAYAN";
		if (542005837 == value) return "Lang_Tag_MAZANDERANI";
		if (541540686 == value) return "Lang_Tag_NAGA_ASSAMESE";
		if (541606222 == value) return "Lang_Tag_NAHUATL";
		if (541999438 == value) return "Lang_Tag_NANAI";
		if (542130510 == value) return "Lang_Tag_NEAPOLITAN";
		if (542327118 == value) return "Lang_Tag_NASKAPI";
		if (542458190 == value) return "Lang_Tag_NAURUAN";
		if (542523726 == value) return "Lang_Tag_NAVAJO";
		if (542262094 == value) return "Lang_Tag_N_CREE";
		if (541213774 == value) return "Lang_Tag_NDEBELE";
		if (541279310 == value) return "Lang_Tag_NDAU";
		if (541541454 == value) return "Lang_Tag_NDONGA";
		if (542327886 == value) return "Lang_Tag_LOW_SAXON";
		if (542131534 == value) return "Lang_Tag_NEPALI";
		if (542590286 == value) return "Lang_Tag_NEWARI";
		if (541149006 == value) return "Lang_Tag_NGBAKA";
		if (542263118 == value) return "Lang_Tag_NAGARI";
		if (541280334 == value) return "Lang_Tag_NORWAY_HOUSE_CREE";
		if (542329166 == value) return "Lang_Tag_NISI";
		if (542460238 == value) return "Lang_Tag_NIUEAN";
		if (541870926 == value) return "Lang_Tag_NYANKOLE";
		if (542067534 == value) return "Lang_Tag_NKO";
		if (541346894 == value) return "Lang_Tag_DUTCH";
		if (541413198 == value) return "Lang_Tag_NIMADI";
		if (541544270 == value) return "Lang_Tag_NOGAI";
		if (542265166 == value) return "Lang_Tag_NORWEGIAN";
		if (542527310 == value) return "Lang_Tag_NOVIAL";
		if (541938510 == value) return "Lang_Tag_NORTHERN_SAMI";
		if (542069582 == value) return "Lang_Tag_SOTHO_NORTHERN";
		if (541152334 == value) return "Lang_Tag_NORTHERN_TAI";
		if (542069838 == value) return "Lang_Tag_ESPERANTO";
		if (541940046 == value) return "Lang_Tag_NYAMWEZI";
		if (542005582 == value) return "Lang_Tag_NORWEGIAN_NYNORSK";
		if (541153870 == value) return "Lang_Tag_MBEMBE_TIGON";
		if (541672271 == value) return "Lang_Tag_OCCITAN";
		if (542262095 == value) return "Lang_Tag_OJI_CREE";
		if (541215311 == value) return "Lang_Tag_OJIBWAY";
		if (541676111 == value) return "Lang_Tag_ODIA_FORMERLY_ORIYA";
		if (542069327 == value) return "Lang_Tag_OROMO";
		if (542331727 == value) return "Lang_Tag_OSSETIAN";
		if (541147472 == value) return "Lang_Tag_PALESTINIAN_ARAMAIC";
		if (541540688 == value) return "Lang_Tag_PANGASINAN";
		if (541868368 == value) return "Lang_Tag_PALI";
		if (541933904 == value) return "Lang_Tag_PAMPANGAN";
		if (541999440 == value) return "Lang_Tag_PUNJABI";
		if (542130512 == value) return "Lang_Tag_PALPA";
		if (810565968 == value) return "Lang_Tag_PAPIAMENTU";
		if (542327120 == value) return "Lang_Tag_PASHTO";
		if (542458192 == value) return "Lang_Tag_PALAUAN";
		if (541279056 == value) return "Lang_Tag_BOUYEI";
		if (541344592 == value) return "Lang_Tag_PICARD";
		if (541279312 == value) return "Lang_Tag_PENNSYLVANIA_GERMAN";
		if (542263120 == value) return "Lang_Tag_POLYTONIC_GREEK";
		if (541804624 == value) return "Lang_Tag_PHAKE";
		if (541608272 == value) return "Lang_Tag_NORFOLK";
		if (541870416 == value) return "Lang_Tag_FILIPINO";
		if (541543504 == value) return "Lang_Tag_PALAUNG";
		if (541805648 == value) return "Lang_Tag_POLISH";
		if (542330192 == value) return "Lang_Tag_PIEMONTESE";
		if (541216336 == value) return "Lang_Tag_WESTERN_PANJABI";
		if (541609808 == value) return "Lang_Tag_POCOMCHI";
		if (542003024 == value) return "Lang_Tag_POHNPEIAN";
		if (542069328 == value) return "Lang_Tag_PROVENCAL_OLD_PROVENCAL";
		if (541545552 == value) return "Lang_Tag_PORTUGUESE";
		if (542070608 == value) return "Lang_Tag_WESTERN_PWO_KAREN";
		if (542001489 == value) return "Lang_Tag_CHIN";
		if (541283665 == value) return "Lang_Tag_KICHE";
		if (541611345 == value) return "Lang_Tag_QUECHUA_BOLIVIA";
		if (542790993 == value) return "Lang_Tag_QUECHUA";
		if (541677137 == value) return "Lang_Tag_QUECHUA_ECUADOR";
		if (541611857 == value) return "Lang_Tag_QUECHUA_PERU";
		if (541737298 == value) return "Lang_Tag_RAJASTHANI";
		if (542261586 == value) return "Lang_Tag_RAROTONGAN";
		if (542458450 == value) return "Lang_Tag_RUSSIAN_BURIAT";
		if (542262098 == value) return "Lang_Tag_R_CREE";
		if (541738322 == value) return "Lang_Tag_REJANG";
		if (541149522 == value) return "Lang_Tag_RIANG";
		if (541477202 == value) return "Lang_Tag_TARIFIT";
		if (542394706 == value) return "Lang_Tag_RITARUNGO";
		if (542591826 == value) return "Lang_Tag_ARAKWAL";
		if (542330194 == value) return "Lang_Tag_ROMANSH";
		if (542723410 == value) return "Lang_Tag_VLAX_ROMANI";
		if (541937490 == value) return "Lang_Tag_ROMANIAN";
		if (542723922 == value) return "Lang_Tag_ROMANY";
		if (542724946 == value) return "Lang_Tag_RUSYN";
		if (541938770 == value) return "Lang_Tag_ROTUMAN";
		if (541152594 == value) return "Lang_Tag_KINYARWANDA";
		if (542004562 == value) return "Lang_Tag_RUNDI";
		if (542135634 == value) return "Lang_Tag_AROMANIAN";
		if (542332242 == value) return "Lang_Tag_RUSSIAN";
		if (541344083 == value) return "Lang_Tag_SADRI";
		if (541999443 == value) return "Lang_Tag_SANSKRIT";
		if (542327123 == value) return "Lang_Tag_SASAK";
		if (542392659 == value) return "Lang_Tag_SANTALI";
		if (542720339 == value) return "Lang_Tag_SAYISI";
		if (541999955 == value) return "Lang_Tag_SICILIAN";
		if (542065491 == value) return "Lang_Tag_SCOTS";
		if (541803859 == value) return "Lang_Tag_SEKOTA";
		if (541869395 == value) return "Lang_Tag_SELKUP";
		if (541149011 == value) return "Lang_Tag_OLD_IRISH";
		if (542066515 == value) return "Lang_Tag_SANGO";
		if (542328659 == value) return "Lang_Tag_SAMOGITIAN";
		if (541673555 == value) return "Lang_Tag_TACHELHIT";
		if (542001235 == value) return "Lang_Tag_SHAN";
		if (541215059 == value) return "Lang_Tag_SIBE";
		if (541346131 == value) return "Lang_Tag_SIDAMO";
		if (541542739 == value) return "Lang_Tag_SILTE_GURAGE";
		if (542329683 == value) return "Lang_Tag_SKOLT_SAMI";
		if (542722899 == value) return "Lang_Tag_SLOVAK";
		if (542327635 == value) return "Lang_Tag_NORTH_SLAVEY";
		if (541150291 == value) return "Lang_Tag_SLAVEY";
		if (542526547 == value) return "Lang_Tag_SLOVENIAN";
		if (541871443 == value) return "Lang_Tag_SOMALI";
		if (542068051 == value) return "Lang_Tag_SAMOAN";
		if (541150803 == value) return "Lang_Tag_SENA";
		if (809586259 == value) return "Lang_Tag_SHONA";
		if (541347411 == value) return "Lang_Tag_SINDHI";
		if (541609555 == value) return "Lang_Tag_SINHALA_SINHALESE";
		if (541806163 == value) return "Lang_Tag_SONINKE";
		if (541544275 == value) return "Lang_Tag_SODO_GURAGE";
		if (542134099 == value) return "Lang_Tag_SONGE";
		if (542396243 == value) return "Lang_Tag_SOTHO_SOUTHERN";
		if (541675859 == value) return "Lang_Tag_ALBANIAN";
		if (541217363 == value) return "Lang_Tag_SERBIAN";
		if (541348435 == value) return "Lang_Tag_SARDINIAN";
		if (541807187 == value) return "Lang_Tag_SARAIKI";
		if (542265939 == value) return "Lang_Tag_SERER";
		if (541872979 == value) return "Lang_Tag_SOUTH_SLAVEY";
		if (541938515 == value) return "Lang_Tag_SOUTHERN_SAMI";
		if (542200915 == value) return "Lang_Tag_SATERLAND_FRISIAN";
		if (541807955 == value) return "Lang_Tag_SUKUMA";
		if (542004563 == value) return "Lang_Tag_SUNDANESE";
		if (542266707 == value) return "Lang_Tag_SURI";
		if (541152851 == value) return "Lang_Tag_SVAN";
		if (541414995 == value) return "Lang_Tag_SWEDISH";
		if (541153107 == value) return "Lang_Tag_SWADAYA_ARAMAIC";
		if (541808467 == value) return "Lang_Tag_SWAHILI";
		if (542791507 == value) return "Lang_Tag_SWATI";
		if (542398547 == value) return "Lang_Tag_SUTU";
		if (542464083 == value) return "Lang_Tag_UPPER_SAXON";
		if (541874515 == value) return "Lang_Tag_SYLHETI";
		if (542267731 == value) return "Lang_Tag_SYRIAC";
		if (1163024723 == value) return "Lang_Tag_SYRIAC_ESTRANGELA";
		if (1246910803 == value) return "Lang_Tag_SYRIAC_WESTERN";
		if (1314019667 == value) return "Lang_Tag_SYRIAC_EASTERN";
		if (541874771 == value) return "Lang_Tag_SILESIAN";
		if (541213012 == value) return "Lang_Tag_TABASARAN";
		if (541737300 == value) return "Lang_Tag_TAJIKI";
		if (541933908 == value) return "Lang_Tag_TAMIL";
		if (542392660 == value) return "Lang_Tag_TATAR";
		if (542262100 == value) return "Lang_Tag_TH_CREE";
		if (541344852 == value) return "Lang_Tag_DEHONG_DAI";
		if (541869396 == value) return "Lang_Tag_TELUGU";
		if (542393684 == value) return "Lang_Tag_TETUM";
		if (541869908 == value) return "Lang_Tag_TAGALOG";
		if (542000980 == value) return "Lang_Tag_TONGAN";
		if (542263124 == value) return "Lang_Tag_TIGRE";
		if (542721876 == value) return "Lang_Tag_TIGRINYA";
		if (541149268 == value) return "Lang_Tag_THAI";
		if (542394452 == value) return "Lang_Tag_TAHITIAN";
		if (541215060 == value) return "Lang_Tag_TIBETAN";
		if (542525780 == value) return "Lang_Tag_TIV";
		if (541936468 == value) return "Lang_Tag_TURKMEN";
		if (541609300 == value) return "Lang_Tag_TAMASHEK";
		if (542002516 == value) return "Lang_Tag_TEMNE";
		if (541150804 == value) return "Lang_Tag_TSWANA";
		if (541412948 == value) return "Lang_Tag_TUNDRA_NENETS";
		if (541544020 == value) return "Lang_Tag_TONGA";
		if (541347668 == value) return "Lang_Tag_TODO";
		if (809783124 == value) return "Lang_Tag_TOMA";
		if (541675604 == value) return "Lang_Tag_TOK_PISIN";
		if (541807188 == value) return "Lang_Tag_TURKISH";
		if (541545300 == value) return "Lang_Tag_TSONGA";
		if (541741908 == value) return "Lang_Tag_TSHANGLA";
		if (541152596 == value) return "Lang_Tag_TUROYO_ARAMAIC";
		if (541939028 == value) return "Lang_Tag_TULU";
		if (541873492 == value) return "Lang_Tag_TUMBUKA";
		if (542528852 == value) return "Lang_Tag_TUVIN";
		if (541873748 == value) return "Lang_Tag_TUVALU";
		if (541677396 == value) return "Lang_Tag_TWI";
		if (542792020 == value) return "Lang_Tag_TAY";
		if (541940308 == value) return "Lang_Tag_TAMAZIGHT";
		if (542071380 == value) return "Lang_Tag_TZOTZIL";
		if (541934677 == value) return "Lang_Tag_UDMURT";
		if (542264149 == value) return "Lang_Tag_UKRAINIAN";
		if (541216085 == value) return "Lang_Tag_UMBUNDU";
		if (541348437 == value) return "Lang_Tag_URDU";
		if (541217621 == value) return "Lang_Tag_UPPER_SORBIAN";
		if (541546837 == value) return "Lang_Tag_UYGHUR";
		if (541219413 == value) return "Lang_Tag_UZBEK";
		if (541279574 == value) return "Lang_Tag_VENETIAN";
		if (542000470 == value) return "Lang_Tag_VENDA";
		if (542394710 == value) return "Lang_Tag_VIETNAMESE";
		if (541871958 == value) return "Lang_Tag_VOLAPUK";
		if (542069334 == value) return "Lang_Tag_VORO";
		if (537477463 == value) return "Lang_Tag_WA";
		if (541540695 == value) return "Lang_Tag_WAGDI";
		if (542261591 == value) return "Lang_Tag_WARAY_WARAY";
		if (542262103 == value) return "Lang_Tag_WEST_CREE";
		if (541869399 == value) return "Lang_Tag_WELSH";
		if (542002263 == value) return "Lang_Tag_WALLOON";
		if (541477975 == value) return "Lang_Tag_WOLOF";
		if (541938775 == value) return "Lang_Tag_MEWATI";
		if (541344344 == value) return "Lang_Tag_LU";
		if (541477720 == value) return "Lang_Tag_KHENGKHA";
		if (542328920 == value) return "Lang_Tag_XHOSA";
		if (541215320 == value) return "Lang_Tag_MINJANGBAL";
		if (541544280 == value) return "Lang_Tag_SOGA";
		if (541413464 == value) return "Lang_Tag_KPELLE_LIBERIA";
		if (541802841 == value) return "Lang_Tag_SAKHA";
		if (542064985 == value) return "Lang_Tag_YAO";
		if (542130521 == value) return "Lang_Tag_YAPESE";
		if (541147737 == value) return "Lang_Tag_YORUBA";
		if (542262105 == value) return "Lang_Tag_Y_CREE";
		if (541280601 == value) return "Lang_Tag_YI_CLASSIC";
		if (541935961 == value) return "Lang_Tag_YI_MODERN";
		if (541148506 == value) return "Lang_Tag_ZEALANDIC";
		if (541607770 == value) return "Lang_Tag_STANDARD_MOROCCAN_TAMAZIGHT";
		if (541149274 == value) return "Lang_Tag_ZHUANG";
		if (541608026 == value) return "Lang_Tag_CHINESE_HONG_KONG_SAR";
		if (542132314 == value) return "Lang_Tag_CHINESE_PHONETIC";
		if (542328922 == value) return "Lang_Tag_CHINESE_SIMPLIFIED";
		if (542394458 == value) return "Lang_Tag_CHINESE_TRADITIONAL";
		if (541347418 == value) return "Lang_Tag_ZANDE";
		if (541873498 == value) return "Lang_Tag_ZULU";
		if (541153882 == value) return "Lang_Tag_ZAZAKI";

		invalid_code_path();
		return "";
	}
};

template <> struct Reflect<Lang_Tag> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(Lang_Tag), "Lang_Tag", enum_count<Lang_Tag>());
		return &i;
	}
};

enum Font_Gpos : u16;
template <> struct Enum_Info<Font_Gpos> {
	static constexpr size_t get_count() { return 11; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 10; }
	static const Font_Gpos index_value(s64 index) { return (Font_Gpos)(index + 0); }
	static const String string(Font_Gpos value) {
		static const String strings[] = {
			"Font_Gpos_INVALID",
			"Font_Gpos_SINGLE_ADJUSTMENT",
			"Font_Gpos_PAIR_ADJUSTMENT",
			"Font_Gpos_CURSIVE_ATTACHMENT",
			"Font_Gpos_MARKTOBASE_ATTACHMENT",
			"Font_Gpos_MARKTOLIGATURE_ATTACHMENT",
			"Font_Gpos_MARKTOMARK_ATTACHMENT",
			"Font_Gpos_CONTEXT_POSITIONING",
			"Font_Gpos_CHAINED_CONTEXT_POSITIONING",
			"Font_Gpos_EXTENSION_POSITIONING",
			"Font_Gpos_COUNT",
		};
		return strings[value - 0];
	}
};

template <> struct Reflect<Font_Gpos> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(Font_Gpos), "Font_Gpos", enum_count<Font_Gpos>());
		return &i;
	}
};

enum Font_Gsub : u16;
template <> struct Enum_Info<Font_Gsub> {
	static constexpr size_t get_count() { return 10; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 9; }
	static const Font_Gsub index_value(s64 index) { return (Font_Gsub)(index + 0); }
	static const String string(Font_Gsub value) {
		static const String strings[] = {
			"Font_Gsub_INVALID",
			"Font_Gsub_SINGLE",
			"Font_Gsub_MULTIPLE",
			"Font_Gsub_ALTERNATE",
			"Font_Gsub_LIGATURE",
			"Font_Gsub_CONTEXTUAL",
			"Font_Gsub_CHAINING_CONTEXTUAL",
			"Font_Gsub_EXTENSION",
			"Font_Gsub_REVERSE_CHAINING_CONTEXTUAL_SINGLE",
			"Font_Gsub_COUNT",
		};
		return strings[value - 0];
	}
};

template <> struct Reflect<Font_Gsub> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(Font_Gsub), "Font_Gsub", enum_count<Font_Gsub>());
		return &i;
	}
};

enum Type_Id : int;
template <> struct Enum_Info<Type_Id> {
	static constexpr size_t get_count() { return 19; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 18; }
	static const Type_Id index_value(s64 index) { return (Type_Id)(index + 0); }
	static const String string(Type_Id value) {
		static const String strings[] = {
			"Type_Id_UNKNOWN",
			"Type_Id_S8",
			"Type_Id_S16",
			"Type_Id_S32",
			"Type_Id_S64",
			"Type_Id_U8",
			"Type_Id_U16",
			"Type_Id_U32",
			"Type_Id_U64",
			"Type_Id_R32",
			"Type_Id_R64",
			"Type_Id_CHAR",
			"Type_Id_VOID",
			"Type_Id_POINTER",
			"Type_Id_FUNCTION",
			"Type_Id_ENUM",
			"Type_Id_STRUCT",
			"Type_Id_UNION",
			"Type_Id_STATIC_ARRAY",
		};
		return strings[value - 0];
	}
};

template <> struct Reflect<Type_Id> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(Type_Id), "Type_Id", enum_count<Type_Id>());
		return &i;
	}
};

enum Render_Index_Type : int;
template <> struct Enum_Info<Render_Index_Type> {
	static constexpr size_t get_count() { return 4; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 3; }
	static const Render_Index_Type index_value(s64 index) { return (Render_Index_Type)(index + 0); }
	static const String string(Render_Index_Type value) {
		static const String strings[] = {
			"Render_Index_Type_NONE",
			"Render_Index_Type_U8",
			"Render_Index_Type_U16",
			"Render_Index_Type_U32",
		};
		return strings[value - 0];
	}
};

template <> struct Reflect<Render_Index_Type> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(Render_Index_Type), "Render_Index_Type", enum_count<Render_Index_Type>());
		return &i;
	}
};

enum Render_Option : u32;
template <> struct Enum_Info<Render_Option> {
	static constexpr size_t get_count() { return 3; }
	static constexpr s64 get_min_value() { return 1; }
	static constexpr s64 get_max_value() { return 4; }
	static const Render_Option index_value(s64 index) {
		static const Render_Option enum_values[] = {
			(Render_Option)1,
			(Render_Option)2,
			(Render_Option)4,
		};
		assert(index >= 0 && index < static_count(enum_values));
		return enum_values[index];
	}
	static const String string(Render_Option value) {
		if (1 == value) return "Render_Option_DEPTH_TEST";
		if (2 == value) return "Render_Option_BLEND";
		if (4 == value) return "Render_Option_CULL";

		invalid_code_path();
		return "";
	}
};

template <> struct Reflect<Render_Option> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(Render_Option), "Render_Option", enum_count<Render_Option>());
		return &i;
	}
};

enum DPI_AWARENESS : int;
template <> struct Enum_Info<DPI_AWARENESS> {
	static constexpr size_t get_count() { return 4; }
	static constexpr s64 get_min_value() { return -1; }
	static constexpr s64 get_max_value() { return 2; }
	static const DPI_AWARENESS index_value(s64 index) { return (DPI_AWARENESS)(index + -1); }
	static const String string(DPI_AWARENESS value) {
		static const String strings[] = {
			"DPI_AWARENESS_INVALID",
			"DPI_AWARENESS_UNAWARE",
			"DPI_AWARENESS_SYSTEM_AWARE",
			"DPI_AWARENESS_PER_MONITOR_AWARE",
		};
		return strings[value - -1];
	}
};

template <> struct Reflect<DPI_AWARENESS> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(DPI_AWARENESS), "DPI_AWARENESS", enum_count<DPI_AWARENESS>());
		return &i;
	}
};

enum DPI_HOSTING_BEHAVIOR : int;
template <> struct Enum_Info<DPI_HOSTING_BEHAVIOR> {
	static constexpr size_t get_count() { return 3; }
	static constexpr s64 get_min_value() { return -1; }
	static constexpr s64 get_max_value() { return 1; }
	static const DPI_HOSTING_BEHAVIOR index_value(s64 index) { return (DPI_HOSTING_BEHAVIOR)(index + -1); }
	static const String string(DPI_HOSTING_BEHAVIOR value) {
		static const String strings[] = {
			"DPI_HOSTING_BEHAVIOR_INVALID",
			"DPI_HOSTING_BEHAVIOR_DEFAULT",
			"DPI_HOSTING_BEHAVIOR_MIXED",
		};
		return strings[value - -1];
	}
};

template <> struct Reflect<DPI_HOSTING_BEHAVIOR> {
	static constexpr Type_Id id = Type_Id_ENUM;
	static const Type_Info * const info() {
		static const Type_Info_Enum i(sizeof(DPI_HOSTING_BEHAVIOR), "DPI_HOSTING_BEHAVIOR", enum_count<DPI_HOSTING_BEHAVIOR>());
		return &i;
	}
};


template <> struct Reflect<Compile_Info> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "file", 0, 0, 0, Reflect<const char *>::info() },
			{ "line", 8, 0, 0, Reflect<const int>::info() },
			{ "current_function", 16, 0, 0, Reflect<const char *>::info() },
		};
		static const Type_Info_Struct i(24, "Compile_Info", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Handle> {
	static constexpr Type_Id id = Type_Id_UNION;
	static const Type_Info * const info() {
		static const Union_Member union_members[] = {
			{ "h32", 0, 0, Reflect<u32>::info() },
			{ "h64", 0, 0, Reflect<u64>::info() },
			{ "hptr", 0, 0, Reflect<void *>::info() },
		};
		static const Type_Info_Union i(8, "Handle", static_count(union_members), union_members);
		return &i;
	}
};

template <typename T> struct Reflect<ExitScope<T>> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		using Templated_Type = ExitScope<T>;
		static const Struct_Member struct_members[] = {
			{ "lambda", offsetof(Templated_Type, lambda), 0, 0, Reflect<T>::info() },
		};
		static const Type_Info_Struct i(sizeof(Templated_Type), "ExitScope<T>", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<ExitScopeHelp> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Type_Info_Struct i(1, "ExitScopeHelp", 0, 0, 0);
		return &i;
	}
};

template <> struct Reflect<Allocator> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "proc", 0, 0, 0, Reflect<Allocator_Proc>::info() },
			{ "data", 8, 0, 0, Reflect<void *>::info() },
		};
		static const Type_Info_Struct i(16, "Allocator", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Temporary_Memory> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "base", 0, 0, 0, Reflect<u8 *>::info() },
			{ "ptr", 8, 0, 0, Reflect<u8 *>::info() },
			{ "capacity", 16, 0, 0, Reflect<ptrsize>::info() },
		};
		static const Type_Info_Struct i(24, "Temporary_Memory", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Thread_Context> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "id", 0, 0, 0, Reflect<u64>::info() },
			{ "allocator", 8, 0, 0, Reflect<Allocator>::info() },
			{ "temp_memory", 24, 0, 0, Reflect<Temporary_Memory>::info() },
			{ "data", 48, 0, 0, Reflect<void *>::info() },
		};
		static const Type_Info_Struct i(56, "Thread_Context", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Push_Allocator> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "allocator", 0, 0, 0, Reflect<Allocator>::info() },
		};
		static const Type_Info_Struct i(16, "Push_Allocator", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <typename T> struct Reflect<Array_View<T>> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		using Templated_Type = Array_View<T>;
		static const Struct_Member struct_members[] = {
			{ "count", offsetof(Templated_Type, count), 0, 0, Reflect<s64>::info() },
			{ "data", offsetof(Templated_Type, data), 0, 0, Reflect<T *>::info() },
		};
		static const Type_Info_Struct i(sizeof(Templated_Type), "Array_View<T>", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <typename T> struct Reflect<Array<T>> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		using Templated_Type = Array<T>;
		static const Struct_Member struct_members[] = {
			{ "count", offsetof(Templated_Type, count), 0, 0, Reflect<s64>::info() },
			{ "data", offsetof(Templated_Type, data), 0, 0, Reflect<T *>::info() },
			{ "capacity", offsetof(Templated_Type, capacity), 0, 0, Reflect<s64>::info() },
			{ "allocator", offsetof(Templated_Type, allocator), 0, 0, Reflect<Allocator>::info() },
		};
		static const Type_Info_Struct i(sizeof(Templated_Type), "Array<T>", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<String> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "count", 0, 0, 0, Reflect<s64>::info() },
			{ "data", 8, 0, 0, Reflect<utf8 *>::info() },
		};
		static const Type_Info_Struct i(16, "String", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Vec2> {
	static constexpr Type_Id id = Type_Id_UNION;
	static const Type_Info * const info() {
		static const Struct_Member _anonymous_struct_members0[] = {
			{ "x", 0, 0, 0, Reflect<r32>::info() },
			{ "y", 4, 0, 0, Reflect<r32>::info() },
		};
		static const Type_Info_Struct _i_anonymous_struct_0(8, "Vec2::anonymous struct", static_count(_anonymous_struct_members0), _anonymous_struct_members0, 0);
		static const Union_Member union_members[] = {
			{ "anonymous", 0, 0, &_i_anonymous_struct_0 }, 
			{ "m", 0, 0, Reflect<r32 [2]>::info() },
		};
		static const Type_Info_Union i(8, "Vec2", static_count(union_members), union_members);
		return &i;
	}
};

template <> struct Reflect<Vec2s> {
	static constexpr Type_Id id = Type_Id_UNION;
	static const Type_Info * const info() {
		static const Struct_Member _anonymous_struct_members0[] = {
			{ "x", 0, 0, 0, Reflect<s32>::info() },
			{ "y", 4, 0, 0, Reflect<s32>::info() },
		};
		static const Type_Info_Struct _i_anonymous_struct_0(8, "Vec2s::anonymous struct", static_count(_anonymous_struct_members0), _anonymous_struct_members0, 0);
		static const Union_Member union_members[] = {
			{ "anonymous", 0, 0, &_i_anonymous_struct_0 }, 
			{ "m", 0, 0, Reflect<s32 [2]>::info() },
		};
		static const Type_Info_Union i(8, "Vec2s", static_count(union_members), union_members);
		return &i;
	}
};

template <> struct Reflect<Vec3> {
	static constexpr Type_Id id = Type_Id_UNION;
	static const Type_Info * const info() {
		static const Struct_Member _anonymous_struct_members0[] = {
			{ "x", 0, 0, 0, Reflect<r32>::info() },
			{ "y", 4, 0, 0, Reflect<r32>::info() },
			{ "z", 8, 0, 0, Reflect<r32>::info() },
		};
		static const Type_Info_Struct _i_anonymous_struct_0(12, "Vec3::anonymous struct", static_count(_anonymous_struct_members0), _anonymous_struct_members0, 0);
		static const Struct_Member _anonymous_struct_members1[] = {
			{ "r", 0, 0, 0, Reflect<r32>::info() },
			{ "g", 4, 0, 0, Reflect<r32>::info() },
			{ "b", 8, 0, 0, Reflect<r32>::info() },
		};
		static const Type_Info_Struct _i_anonymous_struct_1(12, "Vec3::anonymous struct", static_count(_anonymous_struct_members1), _anonymous_struct_members1, 0);
		static const Struct_Member _anonymous_struct_members2[] = {
			{ "xy", 0, 0, 0, Reflect<Vec2>::info() },
			{ "_ignorez", 8, 0, 0, Reflect<r32>::info() },
		};
		static const Type_Info_Struct _i_anonymous_struct_2(12, "Vec3::anonymous struct", static_count(_anonymous_struct_members2), _anonymous_struct_members2, 0);
		static const Struct_Member _anonymous_struct_members3[] = {
			{ "_ignorex", 0, 0, 0, Reflect<r32>::info() },
			{ "yz", 4, 0, 0, Reflect<Vec2>::info() },
		};
		static const Type_Info_Struct _i_anonymous_struct_3(12, "Vec3::anonymous struct", static_count(_anonymous_struct_members3), _anonymous_struct_members3, 0);
		static const Union_Member union_members[] = {
			{ "anonymous", 0, 0, &_i_anonymous_struct_0 }, 
			{ "anonymous", 0, 0, &_i_anonymous_struct_1 }, 
			{ "anonymous", 0, 0, &_i_anonymous_struct_2 }, 
			{ "anonymous", 0, 0, &_i_anonymous_struct_3 }, 
			{ "m", 0, 0, Reflect<r32 [3]>::info() },
		};
		static const Type_Info_Union i(12, "Vec3", static_count(union_members), union_members);
		return &i;
	}
};

template <> struct Reflect<Vec3s> {
	static constexpr Type_Id id = Type_Id_UNION;
	static const Type_Info * const info() {
		static const Struct_Member _anonymous_struct_members0[] = {
			{ "x", 0, 0, 0, Reflect<s32>::info() },
			{ "y", 4, 0, 0, Reflect<s32>::info() },
			{ "z", 8, 0, 0, Reflect<s32>::info() },
		};
		static const Type_Info_Struct _i_anonymous_struct_0(12, "Vec3s::anonymous struct", static_count(_anonymous_struct_members0), _anonymous_struct_members0, 0);
		static const Struct_Member _anonymous_struct_members1[] = {
			{ "r", 0, 0, 0, Reflect<s32>::info() },
			{ "g", 4, 0, 0, Reflect<s32>::info() },
			{ "b", 8, 0, 0, Reflect<s32>::info() },
		};
		static const Type_Info_Struct _i_anonymous_struct_1(12, "Vec3s::anonymous struct", static_count(_anonymous_struct_members1), _anonymous_struct_members1, 0);
		static const Struct_Member _anonymous_struct_members2[] = {
			{ "xy", 0, 0, 0, Reflect<Vec2s>::info() },
			{ "_ignorez", 8, 0, 0, Reflect<s32>::info() },
		};
		static const Type_Info_Struct _i_anonymous_struct_2(12, "Vec3s::anonymous struct", static_count(_anonymous_struct_members2), _anonymous_struct_members2, 0);
		static const Struct_Member _anonymous_struct_members3[] = {
			{ "_ignorex", 0, 0, 0, Reflect<s32>::info() },
			{ "yz", 4, 0, 0, Reflect<Vec2s>::info() },
		};
		static const Type_Info_Struct _i_anonymous_struct_3(12, "Vec3s::anonymous struct", static_count(_anonymous_struct_members3), _anonymous_struct_members3, 0);
		static const Union_Member union_members[] = {
			{ "anonymous", 0, 0, &_i_anonymous_struct_0 }, 
			{ "anonymous", 0, 0, &_i_anonymous_struct_1 }, 
			{ "anonymous", 0, 0, &_i_anonymous_struct_2 }, 
			{ "anonymous", 0, 0, &_i_anonymous_struct_3 }, 
			{ "m", 0, 0, Reflect<s32 [3]>::info() },
		};
		static const Type_Info_Union i(12, "Vec3s", static_count(union_members), union_members);
		return &i;
	}
};

template <> struct Reflect<Vec4> {
	static constexpr Type_Id id = Type_Id_UNION;
	static const Type_Info * const info() {
		static const Struct_Member _anonymous_struct_members0[] = {
			{ "x", 0, 0, 0, Reflect<r32>::info() },
			{ "y", 4, 0, 0, Reflect<r32>::info() },
			{ "z", 8, 0, 0, Reflect<r32>::info() },
			{ "w", 12, 0, 0, Reflect<r32>::info() },
		};
		static const Type_Info_Struct _i_anonymous_struct_0(16, "Vec4::anonymous struct", static_count(_anonymous_struct_members0), _anonymous_struct_members0, 0);
		static const Struct_Member _anonymous_struct_members1[] = {
			{ "r", 0, 0, 0, Reflect<r32>::info() },
			{ "g", 4, 0, 0, Reflect<r32>::info() },
			{ "b", 8, 0, 0, Reflect<r32>::info() },
			{ "a", 12, 0, 0, Reflect<r32>::info() },
		};
		static const Type_Info_Struct _i_anonymous_struct_1(16, "Vec4::anonymous struct", static_count(_anonymous_struct_members1), _anonymous_struct_members1, 0);
		static const Struct_Member _anonymous_struct_members2[] = {
			{ "xy", 0, 0, 0, Reflect<Vec2>::info() },
			{ "zw", 8, 0, 0, Reflect<Vec2>::info() },
		};
		static const Type_Info_Struct _i_anonymous_struct_2(16, "Vec4::anonymous struct", static_count(_anonymous_struct_members2), _anonymous_struct_members2, 0);
		static const Struct_Member _anonymous_struct_members3[] = {
			{ "xyz", 0, 0, 0, Reflect<Vec3>::info() },
			{ "_ignorew", 12, 0, 0, Reflect<r32>::info() },
		};
		static const Type_Info_Struct _i_anonymous_struct_3(16, "Vec4::anonymous struct", static_count(_anonymous_struct_members3), _anonymous_struct_members3, 0);
		static const Struct_Member _anonymous_struct_members4[] = {
			{ "_ignorex", 0, 0, 0, Reflect<r32>::info() },
			{ "yzw", 4, 0, 0, Reflect<Vec3>::info() },
		};
		static const Type_Info_Struct _i_anonymous_struct_4(16, "Vec4::anonymous struct", static_count(_anonymous_struct_members4), _anonymous_struct_members4, 0);
		static const Struct_Member _anonymous_struct_members5[] = {
			{ "rgb", 0, 0, 0, Reflect<Vec3>::info() },
			{ "alpha", 12, 0, 0, Reflect<r32>::info() },
		};
		static const Type_Info_Struct _i_anonymous_struct_5(16, "Vec4::anonymous struct", static_count(_anonymous_struct_members5), _anonymous_struct_members5, 0);
		static const Union_Member union_members[] = {
			{ "anonymous", 0, 0, &_i_anonymous_struct_0 }, 
			{ "anonymous", 0, 0, &_i_anonymous_struct_1 }, 
			{ "anonymous", 0, 0, &_i_anonymous_struct_2 }, 
			{ "anonymous", 0, 0, &_i_anonymous_struct_3 }, 
			{ "anonymous", 0, 0, &_i_anonymous_struct_4 }, 
			{ "anonymous", 0, 0, &_i_anonymous_struct_5 }, 
			{ "m", 0, 0, Reflect<r32 [4]>::info() },
		};
		static const Type_Info_Union i(16, "Vec4", static_count(union_members), union_members);
		return &i;
	}
};

template <> struct Reflect<Vec4s> {
	static constexpr Type_Id id = Type_Id_UNION;
	static const Type_Info * const info() {
		static const Struct_Member _anonymous_struct_members0[] = {
			{ "x", 0, 0, 0, Reflect<s32>::info() },
			{ "y", 4, 0, 0, Reflect<s32>::info() },
			{ "z", 8, 0, 0, Reflect<s32>::info() },
			{ "w", 12, 0, 0, Reflect<s32>::info() },
		};
		static const Type_Info_Struct _i_anonymous_struct_0(16, "Vec4s::anonymous struct", static_count(_anonymous_struct_members0), _anonymous_struct_members0, 0);
		static const Struct_Member _anonymous_struct_members1[] = {
			{ "r", 0, 0, 0, Reflect<s32>::info() },
			{ "g", 4, 0, 0, Reflect<s32>::info() },
			{ "b", 8, 0, 0, Reflect<s32>::info() },
			{ "a", 12, 0, 0, Reflect<s32>::info() },
		};
		static const Type_Info_Struct _i_anonymous_struct_1(16, "Vec4s::anonymous struct", static_count(_anonymous_struct_members1), _anonymous_struct_members1, 0);
		static const Struct_Member _anonymous_struct_members2[] = {
			{ "xy", 0, 0, 0, Reflect<Vec2s>::info() },
			{ "zw", 8, 0, 0, Reflect<Vec2s>::info() },
		};
		static const Type_Info_Struct _i_anonymous_struct_2(16, "Vec4s::anonymous struct", static_count(_anonymous_struct_members2), _anonymous_struct_members2, 0);
		static const Struct_Member _anonymous_struct_members3[] = {
			{ "xyz", 0, 0, 0, Reflect<Vec3s>::info() },
			{ "_ignorew", 12, 0, 0, Reflect<s32>::info() },
		};
		static const Type_Info_Struct _i_anonymous_struct_3(16, "Vec4s::anonymous struct", static_count(_anonymous_struct_members3), _anonymous_struct_members3, 0);
		static const Struct_Member _anonymous_struct_members4[] = {
			{ "_ignorex", 0, 0, 0, Reflect<s32>::info() },
			{ "yzw", 4, 0, 0, Reflect<Vec3s>::info() },
		};
		static const Type_Info_Struct _i_anonymous_struct_4(16, "Vec4s::anonymous struct", static_count(_anonymous_struct_members4), _anonymous_struct_members4, 0);
		static const Struct_Member _anonymous_struct_members5[] = {
			{ "rgb", 0, 0, 0, Reflect<Vec3s>::info() },
			{ "alpha", 12, 0, 0, Reflect<s32>::info() },
		};
		static const Type_Info_Struct _i_anonymous_struct_5(16, "Vec4s::anonymous struct", static_count(_anonymous_struct_members5), _anonymous_struct_members5, 0);
		static const Union_Member union_members[] = {
			{ "anonymous", 0, 0, &_i_anonymous_struct_0 }, 
			{ "anonymous", 0, 0, &_i_anonymous_struct_1 }, 
			{ "anonymous", 0, 0, &_i_anonymous_struct_2 }, 
			{ "anonymous", 0, 0, &_i_anonymous_struct_3 }, 
			{ "anonymous", 0, 0, &_i_anonymous_struct_4 }, 
			{ "anonymous", 0, 0, &_i_anonymous_struct_5 }, 
			{ "m", 0, 0, Reflect<s32 [4]>::info() },
		};
		static const Type_Info_Union i(16, "Vec4s", static_count(union_members), union_members);
		return &i;
	}
};

template <> struct Reflect<Color_HSV> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "h", 0, 0, 0, Reflect<r32>::info() },
			{ "s", 4, 0, 0, Reflect<r32>::info() },
			{ "v", 8, 0, 0, Reflect<r32>::info() },
		};
		static const Type_Info_Struct i(12, "Color_HSV", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Colorh> {
	static constexpr Type_Id id = Type_Id_UNION;
	static const Type_Info * const info() {
		static const Struct_Member _anonymous_struct_members0[] = {
			{ "r", 0, 0, 0, Reflect<u8>::info() },
			{ "g", 1, 0, 0, Reflect<u8>::info() },
			{ "b", 2, 0, 0, Reflect<u8>::info() },
			{ "a", 3, 0, 0, Reflect<u8>::info() },
		};
		static const Type_Info_Struct _i_anonymous_struct_0(4, "Colorh::anonymous struct", static_count(_anonymous_struct_members0), _anonymous_struct_members0, 0);
		static const Union_Member union_members[] = {
			{ "anonymous", 0, 0, &_i_anonymous_struct_0 }, 
			{ "hex", 0, 0, Reflect<u32>::info() },
		};
		static const Type_Info_Union i(4, "Colorh", static_count(union_members), union_members);
		return &i;
	}
};

template <> struct Reflect<Mat3> {
	static constexpr Type_Id id = Type_Id_UNION;
	static const Type_Info * const info() {
		static const Union_Member union_members[] = {
			{ "rows", 0, 0, Reflect<Vec3 [3]>::info() },
			{ "m", 0, 0, Reflect<r32 [9]>::info() },
			{ "m2", 0, 0, Reflect<r32 [4][4]>::info() },
		};
		static const Type_Info_Union i(64, "Mat3", static_count(union_members), union_members);
		return &i;
	}
};

template <> struct Reflect<Mat4> {
	static constexpr Type_Id id = Type_Id_UNION;
	static const Type_Info * const info() {
		static const Union_Member union_members[] = {
			{ "rows", 0, 0, Reflect<Vec4 [4]>::info() },
			{ "m", 0, 0, Reflect<r32 [16]>::info() },
			{ "m2", 0, 0, Reflect<r32 [4][4]>::info() },
		};
		static const Type_Info_Union i(64, "Mat4", static_count(union_members), union_members);
		return &i;
	}
};

template <> struct Reflect<Quat> {
	static constexpr Type_Id id = Type_Id_UNION;
	static const Type_Info * const info() {
		static const Struct_Member _anonymous_struct_members0[] = {
			{ "x", 0, 0, 0, Reflect<r32>::info() },
			{ "y", 4, 0, 0, Reflect<r32>::info() },
			{ "z", 8, 0, 0, Reflect<r32>::info() },
			{ "w", 12, 0, 0, Reflect<r32>::info() },
		};
		static const Type_Info_Struct _i_anonymous_struct_0(16, "Quat::anonymous struct", static_count(_anonymous_struct_members0), _anonymous_struct_members0, 0);
		static const Struct_Member _anonymous_struct_members1[] = {
			{ "i", 0, 0, 0, Reflect<r32>::info() },
			{ "j", 4, 0, 0, Reflect<r32>::info() },
			{ "k", 8, 0, 0, Reflect<r32>::info() },
			{ "real", 12, 0, 0, Reflect<r32>::info() },
		};
		static const Type_Info_Struct _i_anonymous_struct_1(16, "Quat::anonymous struct", static_count(_anonymous_struct_members1), _anonymous_struct_members1, 0);
		static const Struct_Member _anonymous_struct_members2[] = {
			{ "v4", 0, 0, 0, Reflect<Vec4>::info() },
		};
		static const Type_Info_Struct _i_anonymous_struct_2(16, "Quat::anonymous struct", static_count(_anonymous_struct_members2), _anonymous_struct_members2, 0);
		static const Union_Member union_members[] = {
			{ "anonymous", 0, 0, &_i_anonymous_struct_0 }, 
			{ "anonymous", 0, 0, &_i_anonymous_struct_1 }, 
			{ "anonymous", 0, 0, &_i_anonymous_struct_2 }, 
			{ "m", 0, 0, Reflect<r32 [4]>::info() },
		};
		static const Type_Info_Union i(16, "Quat", static_count(union_members), union_members);
		return &i;
	}
};

template <> struct Reflect<Mm_Rect> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "min", 0, 0, 0, Reflect<Vec2>::info() },
			{ "max", 8, 0, 0, Reflect<Vec2>::info() },
		};
		static const Type_Info_Struct i(16, "Mm_Rect", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Rect> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "x", 0, 0, 0, Reflect<r32>::info() },
			{ "y", 4, 0, 0, Reflect<r32>::info() },
			{ "w", 8, 0, 0, Reflect<r32>::info() },
			{ "h", 12, 0, 0, Reflect<r32>::info() },
		};
		static const Type_Info_Struct i(16, "Rect", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Rects> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "x", 0, 0, 0, Reflect<s32>::info() },
			{ "y", 4, 0, 0, Reflect<s32>::info() },
			{ "w", 8, 0, 0, Reflect<s32>::info() },
			{ "h", 12, 0, 0, Reflect<s32>::info() },
		};
		static const Type_Info_Struct i(16, "Rects", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Sampler_Params> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "wrap_s", 0, 0, 0, Reflect<Texture_Wrap>::info() },
			{ "wrap_t", 4, 0, 0, Reflect<Texture_Wrap>::info() },
			{ "min_filter", 8, 0, 0, Reflect<Texture_Filter>::info() },
			{ "mag_filter", 12, 0, 0, Reflect<Texture_Filter>::info() },
			{ "gen_mipmaps", 16, 0, 0, Reflect<bool>::info() },
			{ "srgb", 17, 0, 0, Reflect<bool>::info() },
		};
		static const Type_Info_Struct i(20, "Sampler_Params", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Framebuffer> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "width", 0, 0, 0, Reflect<int>::info() },
			{ "height", 4, 0, 0, Reflect<int>::info() },
			{ "color_format", 8, 0, 0, Reflect<Color_Format>::info() },
			{ "depth_format", 12, 0, 0, Reflect<Depth_Format>::info() },
			{ "id", 16, 0, 0, Reflect<Handle>::info() },
			{ "color", 24, 0, 0, Reflect<Handle>::info() },
			{ "depth", 32, 0, 0, Reflect<Handle>::info() },
		};
		static const Type_Info_Struct i(40, "Framebuffer", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Texture2d> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "handle", 0, 0, 0, Reflect<Handle>::info() },
			{ "rect", 8, 0, 0, Reflect<Mm_Rect>::info() },
		};
		static const Type_Info_Struct i(24, "Texture2d", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Render_Region> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "viewport", 0, 0, 0, Reflect<Rects>::info() },
			{ "scissor", 16, 0, 0, Reflect<Rects>::info() },
		};
		static const Type_Info_Struct i(32, "Render_Region", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Camera_View_Perspective> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "field_of_view", 0, 0, 0, Reflect<float>::info() },
			{ "aspect_ratio", 4, 0, 0, Reflect<float>::info() },
			{ "far_plane", 8, 0, 0, Reflect<float>::info() },
			{ "near_plane", 12, 0, 0, Reflect<float>::info() },
		};
		static const Type_Info_Struct i(16, "Camera_View_Perspective", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Camera_View_Orthographic> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "left", 0, 0, 0, Reflect<float>::info() },
			{ "right", 4, 0, 0, Reflect<float>::info() },
			{ "top", 8, 0, 0, Reflect<float>::info() },
			{ "bottom", 12, 0, 0, Reflect<float>::info() },
			{ "near", 16, 0, 0, Reflect<float>::info() },
			{ "far", 20, 0, 0, Reflect<float>::info() },
		};
		static const Type_Info_Struct i(24, "Camera_View_Orthographic", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Camera_View_Data> {
	static constexpr Type_Id id = Type_Id_UNION;
	static const Type_Info * const info() {
		static const Union_Member union_members[] = {
			{ "perspective", 0, 0, Reflect<Camera_View_Perspective>::info() },
			{ "orthographic", 0, 0, Reflect<Camera_View_Orthographic>::info() },
		};
		static const Type_Info_Union i(24, "Camera_View_Data", static_count(union_members), union_members);
		return &i;
	}
};

template <> struct Reflect<Camera_View> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "kind", 0, 0, 0, Reflect<Camera_View_Kind>::info() },
			{ "data", 4, 0, 0, Reflect<Camera_View_Data>::info() },
		};
		static const Type_Info_Struct i(28, "Camera_View", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Utf32_Codepoint> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "code", 0, 0, 0, Reflect<utf32>::info() },
			{ "size_in_bytes", 4, 0, 0, Reflect<u8>::info() },
		};
		static const Type_Info_Struct i(8, "Utf32_Codepoint", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<String_Iter> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "codepoint", 0, 0, 0, Reflect<Utf32_Codepoint>::info() },
			{ "index", 8, 0, 0, Reflect<s64>::info() },
		};
		static const Type_Info_Struct i(16, "String_Iter", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<String_Search_Result> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "found", 0, 0, 0, Reflect<bool>::info() },
			{ "start_index", 8, 0, 0, Reflect<s64>::info() },
		};
		static const Type_Info_Struct i(16, "String_Search_Result", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Key_Event> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "symbol", 0, 0, 0, Reflect<Key>::info() },
			{ "state", 4, 0, 0, Reflect<State>::info() },
			{ "repeat", 8, 0, 0, Reflect<u32>::info() },
		};
		static const Type_Info_Struct i(12, "Key_Event", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Mouse_Button_Event> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "position", 0, 0, 0, Reflect<Vec2s>::info() },
			{ "symbol", 8, 0, 0, Reflect<Button>::info() },
			{ "state", 12, 0, 0, Reflect<State>::info() },
		};
		static const Type_Info_Struct i(16, "Mouse_Button_Event", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Mouse_Wheel_Event> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "horizontal", 0, 0, 0, Reflect<s32>::info() },
			{ "vertical", 4, 0, 0, Reflect<s32>::info() },
		};
		static const Type_Info_Struct i(8, "Mouse_Wheel_Event", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Mouse_Cursor_Event> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "x", 0, 0, 0, Reflect<s32>::info() },
			{ "y", 4, 0, 0, Reflect<s32>::info() },
		};
		static const Type_Info_Struct i(8, "Mouse_Cursor_Event", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Mouse_Axis_Event> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "x", 0, 0, 0, Reflect<r32>::info() },
			{ "y", 4, 0, 0, Reflect<r32>::info() },
		};
		static const Type_Info_Struct i(8, "Mouse_Axis_Event", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Window_Resize_Event> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "dimension", 0, 0, 0, Reflect<Vec2s>::info() },
		};
		static const Type_Info_Struct i(8, "Window_Resize_Event", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Text_Event> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "codepoint", 0, 0, 0, Reflect<Utf32_Codepoint>::info() },
		};
		static const Type_Info_Struct i(8, "Text_Event", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Controller_Event> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "index", 0, 0, 0, Reflect<u32>::info() },
		};
		static const Type_Info_Struct i(4, "Controller_Event", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Event> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Union_Member _anonymous_union_members_1[] = {
			{ "key", 0, 0, Reflect<Key_Event>::info() },
			{ "mouse_button", 0, 0, Reflect<Mouse_Button_Event>::info() },
			{ "mouse_wheel", 0, 0, Reflect<Mouse_Wheel_Event>::info() },
			{ "mouse_cursor", 0, 0, Reflect<Mouse_Cursor_Event>::info() },
			{ "mouse_axis", 0, 0, Reflect<Mouse_Axis_Event>::info() },
			{ "window", 0, 0, Reflect<Window_Resize_Event>::info() },
			{ "text", 0, 0, Reflect<Text_Event>::info() },
			{ "controller", 0, 0, Reflect<Controller_Event>::info() },
		};
		static const Type_Info_Union _i_anonymous_union_1(16, "Event::anonymous union", static_count(_anonymous_union_members_1), _anonymous_union_members_1);
		static const Struct_Member struct_members[] = {
			{ "type", 0, 0, 0, Reflect<Event_Types>::info() },
			{ "anonymous", 0, 0, 0, &_i_anonymous_union_1 },
		};
		static const Type_Info_Struct i(20, "Event", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Controller> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "buttons", 0, 0, 0, Reflect<State [14]>::info() },
			{ "left_trigger", 56, 0, 0, Reflect<r32>::info() },
			{ "right_trigger", 60, 0, 0, Reflect<r32>::info() },
			{ "left_thumb", 64, 0, 0, Reflect<Vec2>::info() },
			{ "right_thumb", 72, 0, 0, Reflect<Vec2>::info() },
		};
		static const Type_Info_Struct i(80, "Controller", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<System_Find_File_Info> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "path", 0, 0, 0, Reflect<String>::info() },
			{ "name", 16, 0, 0, Reflect<String>::info() },
			{ "extension", 32, 0, 0, Reflect<String>::info() },
			{ "size", 48, 0, 0, Reflect<u64>::info() },
		};
		static const Type_Info_Struct i(56, "System_Find_File_Info", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<System_File> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "handle", 0, 0, 0, Reflect<Handle>::info() },
			{ "get_cursor", 8, 0, 0, Reflect<System_Get_File_Pointer>::info() },
			{ "set_cursor", 16, 0, 0, Reflect<System_Set_File_Pointer>::info() },
			{ "read", 24, 0, 0, Reflect<System_Read_File>::info() },
			{ "write", 32, 0, 0, Reflect<System_Write_File>::info() },
		};
		static const Type_Info_Struct i(40, "System_File", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<System_Window_State> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "handle", 0, 0, 0, Reflect<void *>::info() },
			{ "size", 8, 0, 0, Reflect<ptrsize>::info() },
		};
		static const Type_Info_Struct i(16, "System_Window_State", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<ImVec2> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "x", 0, 0, 0, Reflect<float>::info() },
			{ "y", 4, 0, 0, Reflect<float>::info() },
		};
		static const Type_Info_Struct i(8, "ImVec2", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<ImVec4> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "x", 0, 0, 0, Reflect<float>::info() },
			{ "y", 4, 0, 0, Reflect<float>::info() },
			{ "z", 8, 0, 0, Reflect<float>::info() },
			{ "w", 12, 0, 0, Reflect<float>::info() },
		};
		static const Type_Info_Struct i(16, "ImVec4", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<ImNewDummy> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Type_Info_Struct i(1, "ImNewDummy", 0, 0, 0);
		return &i;
	}
};

template <typename T> struct Reflect<ImVector<T>> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		using Templated_Type = ImVector<T>;
		static const Struct_Member struct_members[] = {
			{ "Size", offsetof(Templated_Type, Size), 0, 0, Reflect<int>::info() },
			{ "Capacity", offsetof(Templated_Type, Capacity), 0, 0, Reflect<int>::info() },
			{ "Data", offsetof(Templated_Type, Data), 0, 0, Reflect<T *>::info() },
		};
		static const Type_Info_Struct i(sizeof(Templated_Type), "ImVector<T>", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<ImGuiStyle> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "Alpha", 0, 0, 0, Reflect<float>::info() },
			{ "WindowPadding", 4, 0, 0, Reflect<ImVec2>::info() },
			{ "WindowRounding", 12, 0, 0, Reflect<float>::info() },
			{ "WindowBorderSize", 16, 0, 0, Reflect<float>::info() },
			{ "WindowMinSize", 20, 0, 0, Reflect<ImVec2>::info() },
			{ "WindowTitleAlign", 28, 0, 0, Reflect<ImVec2>::info() },
			{ "WindowMenuButtonPosition", 36, 0, 0, Reflect<ImGuiDir>::info() },
			{ "ChildRounding", 40, 0, 0, Reflect<float>::info() },
			{ "ChildBorderSize", 44, 0, 0, Reflect<float>::info() },
			{ "PopupRounding", 48, 0, 0, Reflect<float>::info() },
			{ "PopupBorderSize", 52, 0, 0, Reflect<float>::info() },
			{ "FramePadding", 56, 0, 0, Reflect<ImVec2>::info() },
			{ "FrameRounding", 64, 0, 0, Reflect<float>::info() },
			{ "FrameBorderSize", 68, 0, 0, Reflect<float>::info() },
			{ "ItemSpacing", 72, 0, 0, Reflect<ImVec2>::info() },
			{ "ItemInnerSpacing", 80, 0, 0, Reflect<ImVec2>::info() },
			{ "TouchExtraPadding", 88, 0, 0, Reflect<ImVec2>::info() },
			{ "IndentSpacing", 96, 0, 0, Reflect<float>::info() },
			{ "ColumnsMinSpacing", 100, 0, 0, Reflect<float>::info() },
			{ "ScrollbarSize", 104, 0, 0, Reflect<float>::info() },
			{ "ScrollbarRounding", 108, 0, 0, Reflect<float>::info() },
			{ "GrabMinSize", 112, 0, 0, Reflect<float>::info() },
			{ "GrabRounding", 116, 0, 0, Reflect<float>::info() },
			{ "TabRounding", 120, 0, 0, Reflect<float>::info() },
			{ "TabBorderSize", 124, 0, 0, Reflect<float>::info() },
			{ "ColorButtonPosition", 128, 0, 0, Reflect<ImGuiDir>::info() },
			{ "ButtonTextAlign", 132, 0, 0, Reflect<ImVec2>::info() },
			{ "SelectableTextAlign", 140, 0, 0, Reflect<ImVec2>::info() },
			{ "DisplayWindowPadding", 148, 0, 0, Reflect<ImVec2>::info() },
			{ "DisplaySafeAreaPadding", 156, 0, 0, Reflect<ImVec2>::info() },
			{ "MouseCursorScale", 164, 0, 0, Reflect<float>::info() },
			{ "AntiAliasedLines", 168, 0, 0, Reflect<bool>::info() },
			{ "AntiAliasedFill", 169, 0, 0, Reflect<bool>::info() },
			{ "CurveTessellationTol", 172, 0, 0, Reflect<float>::info() },
			{ "CircleSegmentMaxError", 176, 0, 0, Reflect<float>::info() },
			{ "Colors", 180, 0, 0, Reflect<ImVec4 [50]>::info() },
		};
		static const Type_Info_Struct i(980, "ImGuiStyle", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<ImGuiIO> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "ConfigFlags", 0, 0, 0, Reflect<ImGuiConfigFlags>::info() },
			{ "BackendFlags", 4, 0, 0, Reflect<ImGuiBackendFlags>::info() },
			{ "DisplaySize", 8, 0, 0, Reflect<ImVec2>::info() },
			{ "DeltaTime", 16, 0, 0, Reflect<float>::info() },
			{ "IniSavingRate", 20, 0, 0, Reflect<float>::info() },
			{ "IniFilename", 24, 0, 0, Reflect<const char *>::info() },
			{ "LogFilename", 32, 0, 0, Reflect<const char *>::info() },
			{ "MouseDoubleClickTime", 40, 0, 0, Reflect<float>::info() },
			{ "MouseDoubleClickMaxDist", 44, 0, 0, Reflect<float>::info() },
			{ "MouseDragThreshold", 48, 0, 0, Reflect<float>::info() },
			{ "KeyMap", 52, 0, 0, Reflect<int [22]>::info() },
			{ "KeyRepeatDelay", 140, 0, 0, Reflect<float>::info() },
			{ "KeyRepeatRate", 144, 0, 0, Reflect<float>::info() },
			{ "UserData", 152, 0, 0, Reflect<void *>::info() },
			{ "Fonts", 160, 0, 0, Reflect<ImFontAtlas *>::info() },
			{ "FontGlobalScale", 168, 0, 0, Reflect<float>::info() },
			{ "FontAllowUserScaling", 172, 0, 0, Reflect<bool>::info() },
			{ "FontDefault", 176, 0, 0, Reflect<ImFont *>::info() },
			{ "DisplayFramebufferScale", 184, 0, 0, Reflect<ImVec2>::info() },
			{ "ConfigDockingNoSplit", 192, 0, 0, Reflect<bool>::info() },
			{ "ConfigDockingWithShift", 193, 0, 0, Reflect<bool>::info() },
			{ "ConfigDockingAlwaysTabBar", 194, 0, 0, Reflect<bool>::info() },
			{ "ConfigDockingTransparentPayload", 195, 0, 0, Reflect<bool>::info() },
			{ "ConfigViewportsNoAutoMerge", 196, 0, 0, Reflect<bool>::info() },
			{ "ConfigViewportsNoTaskBarIcon", 197, 0, 0, Reflect<bool>::info() },
			{ "ConfigViewportsNoDecoration", 198, 0, 0, Reflect<bool>::info() },
			{ "ConfigViewportsNoDefaultParent", 199, 0, 0, Reflect<bool>::info() },
			{ "MouseDrawCursor", 200, 0, 0, Reflect<bool>::info() },
			{ "ConfigMacOSXBehaviors", 201, 0, 0, Reflect<bool>::info() },
			{ "ConfigInputTextCursorBlink", 202, 0, 0, Reflect<bool>::info() },
			{ "ConfigWindowsResizeFromEdges", 203, 0, 0, Reflect<bool>::info() },
			{ "ConfigWindowsMoveFromTitleBarOnly", 204, 0, 0, Reflect<bool>::info() },
			{ "ConfigWindowsMemoryCompactTimer", 208, 0, 0, Reflect<float>::info() },
			{ "BackendPlatformName", 216, 0, 0, Reflect<const char *>::info() },
			{ "BackendRendererName", 224, 0, 0, Reflect<const char *>::info() },
			{ "BackendPlatformUserData", 232, 0, 0, Reflect<void *>::info() },
			{ "BackendRendererUserData", 240, 0, 0, Reflect<void *>::info() },
			{ "BackendLanguageUserData", 248, 0, 0, Reflect<void *>::info() },
			{ "GetClipboardTextFn", 256, 0, 0, Reflect<const char *(*)(void *)>::info() },
			{ "SetClipboardTextFn", 264, 0, 0, Reflect<void (*)(void *, const char *)>::info() },
			{ "ClipboardUserData", 272, 0, 0, Reflect<void *>::info() },
			{ "RenderDrawListsFnUnused", 280, 0, 0, Reflect<void *>::info() },
			{ "MousePos", 288, 0, 0, Reflect<ImVec2>::info() },
			{ "MouseDown", 296, 0, 0, Reflect<bool [5]>::info() },
			{ "MouseWheel", 304, 0, 0, Reflect<float>::info() },
			{ "MouseWheelH", 308, 0, 0, Reflect<float>::info() },
			{ "MouseHoveredViewport", 312, 0, 0, Reflect<ImGuiID>::info() },
			{ "KeyCtrl", 316, 0, 0, Reflect<bool>::info() },
			{ "KeyShift", 317, 0, 0, Reflect<bool>::info() },
			{ "KeyAlt", 318, 0, 0, Reflect<bool>::info() },
			{ "KeySuper", 319, 0, 0, Reflect<bool>::info() },
			{ "KeysDown", 320, 0, 0, Reflect<bool [512]>::info() },
			{ "NavInputs", 832, 0, 0, Reflect<float [21]>::info() },
			{ "WantCaptureMouse", 916, 0, 0, Reflect<bool>::info() },
			{ "WantCaptureKeyboard", 917, 0, 0, Reflect<bool>::info() },
			{ "WantTextInput", 918, 0, 0, Reflect<bool>::info() },
			{ "WantSetMousePos", 919, 0, 0, Reflect<bool>::info() },
			{ "WantSaveIniSettings", 920, 0, 0, Reflect<bool>::info() },
			{ "NavActive", 921, 0, 0, Reflect<bool>::info() },
			{ "NavVisible", 922, 0, 0, Reflect<bool>::info() },
			{ "Framerate", 924, 0, 0, Reflect<float>::info() },
			{ "MetricsRenderVertices", 928, 0, 0, Reflect<int>::info() },
			{ "MetricsRenderIndices", 932, 0, 0, Reflect<int>::info() },
			{ "MetricsRenderWindows", 936, 0, 0, Reflect<int>::info() },
			{ "MetricsActiveWindows", 940, 0, 0, Reflect<int>::info() },
			{ "MetricsActiveAllocations", 944, 0, 0, Reflect<int>::info() },
			{ "MouseDelta", 948, 0, 0, Reflect<ImVec2>::info() },
			{ "KeyMods", 956, 0, 0, Reflect<ImGuiKeyModFlags>::info() },
			{ "MousePosPrev", 960, 0, 0, Reflect<ImVec2>::info() },
			{ "MouseClickedPos", 968, 0, 0, Reflect<ImVec2 [5]>::info() },
			{ "MouseClickedTime", 1008, 0, 0, Reflect<double [5]>::info() },
			{ "MouseClicked", 1048, 0, 0, Reflect<bool [5]>::info() },
			{ "MouseDoubleClicked", 1053, 0, 0, Reflect<bool [5]>::info() },
			{ "MouseReleased", 1058, 0, 0, Reflect<bool [5]>::info() },
			{ "MouseDownOwned", 1063, 0, 0, Reflect<bool [5]>::info() },
			{ "MouseDownWasDoubleClick", 1068, 0, 0, Reflect<bool [5]>::info() },
			{ "MouseDownDuration", 1076, 0, 0, Reflect<float [5]>::info() },
			{ "MouseDownDurationPrev", 1096, 0, 0, Reflect<float [5]>::info() },
			{ "MouseDragMaxDistanceAbs", 1116, 0, 0, Reflect<ImVec2 [5]>::info() },
			{ "MouseDragMaxDistanceSqr", 1156, 0, 0, Reflect<float [5]>::info() },
			{ "KeysDownDuration", 1176, 0, 0, Reflect<float [512]>::info() },
			{ "KeysDownDurationPrev", 3224, 0, 0, Reflect<float [512]>::info() },
			{ "NavInputsDownDuration", 5272, 0, 0, Reflect<float [21]>::info() },
			{ "NavInputsDownDurationPrev", 5356, 0, 0, Reflect<float [21]>::info() },
			{ "InputQueueSurrogate", 5440, 0, 0, Reflect<ImWchar16>::info() },
			{ "InputQueueCharacters", 5448, 0, 0, Reflect<ImVector<ImWchar>>::info() },
		};
		static const Type_Info_Struct i(5464, "ImGuiIO", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<ImGuiInputTextCallbackData> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "EventFlag", 0, 0, 0, Reflect<ImGuiInputTextFlags>::info() },
			{ "Flags", 4, 0, 0, Reflect<ImGuiInputTextFlags>::info() },
			{ "UserData", 8, 0, 0, Reflect<void *>::info() },
			{ "EventChar", 16, 0, 0, Reflect<ImWchar>::info() },
			{ "EventKey", 20, 0, 0, Reflect<ImGuiKey>::info() },
			{ "Buf", 24, 0, 0, Reflect<char *>::info() },
			{ "BufTextLen", 32, 0, 0, Reflect<int>::info() },
			{ "BufSize", 36, 0, 0, Reflect<int>::info() },
			{ "BufDirty", 40, 0, 0, Reflect<bool>::info() },
			{ "CursorPos", 44, 0, 0, Reflect<int>::info() },
			{ "SelectionStart", 48, 0, 0, Reflect<int>::info() },
			{ "SelectionEnd", 52, 0, 0, Reflect<int>::info() },
		};
		static const Type_Info_Struct i(56, "ImGuiInputTextCallbackData", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<ImGuiSizeCallbackData> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "UserData", 0, 0, 0, Reflect<void *>::info() },
			{ "Pos", 8, 0, 0, Reflect<ImVec2>::info() },
			{ "CurrentSize", 16, 0, 0, Reflect<ImVec2>::info() },
			{ "DesiredSize", 24, 0, 0, Reflect<ImVec2>::info() },
		};
		static const Type_Info_Struct i(32, "ImGuiSizeCallbackData", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<ImGuiWindowClass> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "ClassId", 0, 0, 0, Reflect<ImGuiID>::info() },
			{ "ParentViewportId", 4, 0, 0, Reflect<ImGuiID>::info() },
			{ "ViewportFlagsOverrideSet", 8, 0, 0, Reflect<ImGuiViewportFlags>::info() },
			{ "ViewportFlagsOverrideClear", 12, 0, 0, Reflect<ImGuiViewportFlags>::info() },
			{ "DockNodeFlagsOverrideSet", 16, 0, 0, Reflect<ImGuiDockNodeFlags>::info() },
			{ "DockNodeFlagsOverrideClear", 20, 0, 0, Reflect<ImGuiDockNodeFlags>::info() },
			{ "DockingAlwaysTabBar", 24, 0, 0, Reflect<bool>::info() },
			{ "DockingAllowUnclassed", 25, 0, 0, Reflect<bool>::info() },
		};
		static const Type_Info_Struct i(28, "ImGuiWindowClass", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<ImGuiPayload> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "Data", 0, 0, 0, Reflect<void *>::info() },
			{ "DataSize", 8, 0, 0, Reflect<int>::info() },
			{ "SourceId", 12, 0, 0, Reflect<ImGuiID>::info() },
			{ "SourceParentId", 16, 0, 0, Reflect<ImGuiID>::info() },
			{ "DataFrameCount", 20, 0, 0, Reflect<int>::info() },
			{ "DataType", 24, 0, 0, Reflect<char [33]>::info() },
			{ "Preview", 57, 0, 0, Reflect<bool>::info() },
			{ "Delivery", 58, 0, 0, Reflect<bool>::info() },
		};
		static const Type_Info_Struct i(64, "ImGuiPayload", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<ImGuiOnceUponAFrame> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "RefFrame", 0, 0, 0, Reflect<int>::info() },
		};
		static const Type_Info_Struct i(4, "ImGuiOnceUponAFrame", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<ImGuiTextFilter::ImGuiTextRange> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "b", 0, 0, 0, Reflect<const char *>::info() },
			{ "e", 8, 0, 0, Reflect<const char *>::info() },
		};
		static const Type_Info_Struct i(16, "ImGuiTextFilter::ImGuiTextRange", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<ImGuiTextFilter> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "InputBuf", 0, 0, 0, Reflect<char [256]>::info() },
			{ "Filters", 256, 0, 0, Reflect<ImVector<ImGuiTextFilter::ImGuiTextRange>>::info() },
			{ "CountGrep", 272, 0, 0, Reflect<int>::info() },
		};
		static const Type_Info_Struct i(280, "ImGuiTextFilter", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<ImGuiTextBuffer> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "Buf", 0, 0, 0, Reflect<ImVector<char>>::info() },
		};
		static const Type_Info_Struct i(16, "ImGuiTextBuffer", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<ImGuiStorage::ImGuiStoragePair> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Union_Member _anonymous_union_members_1[] = {
			{ "val_i", 0, 0, Reflect<int>::info() },
			{ "val_f", 0, 0, Reflect<float>::info() },
			{ "val_p", 0, 0, Reflect<void *>::info() },
		};
		static const Type_Info_Union _i_anonymous_union_1(8, "ImGuiStorage::ImGuiStoragePair::anonymous union", static_count(_anonymous_union_members_1), _anonymous_union_members_1);
		static const Struct_Member struct_members[] = {
			{ "key", 0, 0, 0, Reflect<ImGuiID>::info() },
			{ "anonymous", 0, 0, 0, &_i_anonymous_union_1 },
		};
		static const Type_Info_Struct i(16, "ImGuiStorage::ImGuiStoragePair", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<ImGuiStorage> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "Data", 0, 0, 0, Reflect<ImVector<ImGuiStorage::ImGuiStoragePair>>::info() },
		};
		static const Type_Info_Struct i(16, "ImGuiStorage", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<ImGuiListClipper> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "DisplayStart", 0, 0, 0, Reflect<int>::info() },
			{ "DisplayEnd", 4, 0, 0, Reflect<int>::info() },
			{ "ItemsCount", 8, 0, 0, Reflect<int>::info() },
			{ "StepNo", 12, 0, 0, Reflect<int>::info() },
			{ "ItemsHeight", 16, 0, 0, Reflect<float>::info() },
			{ "StartPosY", 20, 0, 0, Reflect<float>::info() },
		};
		static const Type_Info_Struct i(24, "ImGuiListClipper", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<ImColor> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "Value", 0, 0, 0, Reflect<ImVec4>::info() },
		};
		static const Type_Info_Struct i(16, "ImColor", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<ImDrawCmd> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "ElemCount", 0, 0, 0, Reflect<unsigned int>::info() },
			{ "ClipRect", 4, 0, 0, Reflect<ImVec4>::info() },
			{ "TextureId", 24, 0, 0, Reflect<ImTextureID>::info() },
			{ "VtxOffset", 32, 0, 0, Reflect<unsigned int>::info() },
			{ "IdxOffset", 36, 0, 0, Reflect<unsigned int>::info() },
			{ "UserCallback", 40, 0, 0, Reflect<ImDrawCallback>::info() },
			{ "UserCallbackData", 48, 0, 0, Reflect<void *>::info() },
		};
		static const Type_Info_Struct i(56, "ImDrawCmd", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<ImDrawVert> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "pos", 0, 0, 0, Reflect<ImVec2>::info() },
			{ "uv", 8, 0, 0, Reflect<ImVec2>::info() },
			{ "col", 16, 0, 0, Reflect<ImU32>::info() },
		};
		static const Type_Info_Struct i(20, "ImDrawVert", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<ImDrawChannel> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "_CmdBuffer", 0, 0, 0, Reflect<ImVector<ImDrawCmd>>::info() },
			{ "_IdxBuffer", 16, 0, 0, Reflect<ImVector<ImDrawIdx>>::info() },
		};
		static const Type_Info_Struct i(32, "ImDrawChannel", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<ImDrawListSplitter> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "_Current", 0, 0, 0, Reflect<int>::info() },
			{ "_Count", 4, 0, 0, Reflect<int>::info() },
			{ "_Channels", 8, 0, 0, Reflect<ImVector<ImDrawChannel>>::info() },
		};
		static const Type_Info_Struct i(24, "ImDrawListSplitter", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<ImDrawList> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "CmdBuffer", 0, 0, 0, Reflect<ImVector<ImDrawCmd>>::info() },
			{ "IdxBuffer", 16, 0, 0, Reflect<ImVector<ImDrawIdx>>::info() },
			{ "VtxBuffer", 32, 0, 0, Reflect<ImVector<ImDrawVert>>::info() },
			{ "Flags", 48, 0, 0, Reflect<ImDrawListFlags>::info() },
			{ "_Data", 56, 0, 0, Reflect<const ImDrawListSharedData *>::info() },
			{ "_OwnerName", 64, 0, 0, Reflect<const char *>::info() },
			{ "_VtxCurrentOffset", 72, 0, 0, Reflect<unsigned int>::info() },
			{ "_VtxCurrentIdx", 76, 0, 0, Reflect<unsigned int>::info() },
			{ "_VtxWritePtr", 80, 0, 0, Reflect<ImDrawVert *>::info() },
			{ "_IdxWritePtr", 88, 0, 0, Reflect<ImDrawIdx *>::info() },
			{ "_ClipRectStack", 96, 0, 0, Reflect<ImVector<ImVec4>>::info() },
			{ "_TextureIdStack", 112, 0, 0, Reflect<ImVector<ImTextureID>>::info() },
			{ "_Path", 128, 0, 0, Reflect<ImVector<ImVec2>>::info() },
			{ "_Splitter", 144, 0, 0, Reflect<ImDrawListSplitter>::info() },
		};
		static const Type_Info_Struct i(168, "ImDrawList", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<ImDrawData> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "Valid", 0, 0, 0, Reflect<bool>::info() },
			{ "CmdLists", 8, 0, 0, Reflect<ImDrawList **>::info() },
			{ "CmdListsCount", 16, 0, 0, Reflect<int>::info() },
			{ "TotalIdxCount", 20, 0, 0, Reflect<int>::info() },
			{ "TotalVtxCount", 24, 0, 0, Reflect<int>::info() },
			{ "DisplayPos", 28, 0, 0, Reflect<ImVec2>::info() },
			{ "DisplaySize", 36, 0, 0, Reflect<ImVec2>::info() },
			{ "FramebufferScale", 44, 0, 0, Reflect<ImVec2>::info() },
			{ "OwnerViewport", 56, 0, 0, Reflect<ImGuiViewport *>::info() },
		};
		static const Type_Info_Struct i(64, "ImDrawData", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<ImFontConfig> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "FontData", 0, 0, 0, Reflect<void *>::info() },
			{ "FontDataSize", 8, 0, 0, Reflect<int>::info() },
			{ "FontDataOwnedByAtlas", 12, 0, 0, Reflect<bool>::info() },
			{ "FontNo", 16, 0, 0, Reflect<int>::info() },
			{ "SizePixels", 20, 0, 0, Reflect<float>::info() },
			{ "OversampleH", 24, 0, 0, Reflect<int>::info() },
			{ "OversampleV", 28, 0, 0, Reflect<int>::info() },
			{ "PixelSnapH", 32, 0, 0, Reflect<bool>::info() },
			{ "GlyphExtraSpacing", 36, 0, 0, Reflect<ImVec2>::info() },
			{ "GlyphOffset", 44, 0, 0, Reflect<ImVec2>::info() },
			{ "GlyphRanges", 56, 0, 0, Reflect<const ImWchar *>::info() },
			{ "GlyphMinAdvanceX", 64, 0, 0, Reflect<float>::info() },
			{ "GlyphMaxAdvanceX", 68, 0, 0, Reflect<float>::info() },
			{ "MergeMode", 72, 0, 0, Reflect<bool>::info() },
			{ "RasterizerFlags", 76, 0, 0, Reflect<unsigned int>::info() },
			{ "RasterizerMultiply", 80, 0, 0, Reflect<float>::info() },
			{ "EllipsisChar", 84, 0, 0, Reflect<ImWchar>::info() },
			{ "Name", 86, 0, 0, Reflect<char [40]>::info() },
			{ "DstFont", 128, 0, 0, Reflect<ImFont *>::info() },
		};
		static const Type_Info_Struct i(136, "ImFontConfig", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<ImFontGlyph> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "Codepoint", 0, 0, 0, Reflect<unsigned int>::info() },
			{ "Visible", 3, 0, 0, Reflect<unsigned int>::info() },
			{ "AdvanceX", 4, 0, 0, Reflect<float>::info() },
			{ "X0", 8, 0, 0, Reflect<float>::info() },
			{ "Y0", 12, 0, 0, Reflect<float>::info() },
			{ "X1", 16, 0, 0, Reflect<float>::info() },
			{ "Y1", 20, 0, 0, Reflect<float>::info() },
			{ "U0", 24, 0, 0, Reflect<float>::info() },
			{ "V0", 28, 0, 0, Reflect<float>::info() },
			{ "U1", 32, 0, 0, Reflect<float>::info() },
			{ "V1", 36, 0, 0, Reflect<float>::info() },
		};
		static const Type_Info_Struct i(40, "ImFontGlyph", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<ImFontGlyphRangesBuilder> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "UsedChars", 0, 0, 0, Reflect<ImVector<ImU32>>::info() },
		};
		static const Type_Info_Struct i(16, "ImFontGlyphRangesBuilder", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<ImFontAtlasCustomRect> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "ID", 0, 0, 0, Reflect<unsigned int>::info() },
			{ "Width", 4, 0, 0, Reflect<unsigned short>::info() },
			{ "Height", 6, 0, 0, Reflect<unsigned short>::info() },
			{ "X", 8, 0, 0, Reflect<unsigned short>::info() },
			{ "Y", 10, 0, 0, Reflect<unsigned short>::info() },
			{ "GlyphAdvanceX", 12, 0, 0, Reflect<float>::info() },
			{ "GlyphOffset", 16, 0, 0, Reflect<ImVec2>::info() },
			{ "Font", 24, 0, 0, Reflect<ImFont *>::info() },
		};
		static const Type_Info_Struct i(32, "ImFontAtlasCustomRect", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<ImFontAtlas> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "Locked", 0, 0, 0, Reflect<bool>::info() },
			{ "Flags", 4, 0, 0, Reflect<ImFontAtlasFlags>::info() },
			{ "TexID", 8, 0, 0, Reflect<ImTextureID>::info() },
			{ "TexDesiredWidth", 16, 0, 0, Reflect<int>::info() },
			{ "TexGlyphPadding", 20, 0, 0, Reflect<int>::info() },
			{ "TexPixelsAlpha8", 24, 0, 0, Reflect<unsigned char *>::info() },
			{ "TexPixelsRGBA32", 32, 0, 0, Reflect<unsigned int *>::info() },
			{ "TexWidth", 40, 0, 0, Reflect<int>::info() },
			{ "TexHeight", 44, 0, 0, Reflect<int>::info() },
			{ "TexUvScale", 48, 0, 0, Reflect<ImVec2>::info() },
			{ "TexUvWhitePixel", 56, 0, 0, Reflect<ImVec2>::info() },
			{ "Fonts", 64, 0, 0, Reflect<ImVector<ImFont *>>::info() },
			{ "CustomRects", 80, 0, 0, Reflect<ImVector<ImFontAtlasCustomRect>>::info() },
			{ "ConfigData", 96, 0, 0, Reflect<ImVector<ImFontConfig>>::info() },
			{ "CustomRectIds", 112, 0, 0, Reflect<int [1]>::info() },
		};
		static const Type_Info_Struct i(120, "ImFontAtlas", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<ImFont> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "IndexAdvanceX", 0, 0, 0, Reflect<ImVector<float>>::info() },
			{ "FallbackAdvanceX", 16, 0, 0, Reflect<float>::info() },
			{ "FontSize", 20, 0, 0, Reflect<float>::info() },
			{ "IndexLookup", 24, 0, 0, Reflect<ImVector<ImWchar>>::info() },
			{ "Glyphs", 40, 0, 0, Reflect<ImVector<ImFontGlyph>>::info() },
			{ "FallbackGlyph", 56, 0, 0, Reflect<const ImFontGlyph *>::info() },
			{ "DisplayOffset", 64, 0, 0, Reflect<ImVec2>::info() },
			{ "ContainerAtlas", 72, 0, 0, Reflect<ImFontAtlas *>::info() },
			{ "ConfigData", 80, 0, 0, Reflect<const ImFontConfig *>::info() },
			{ "ConfigDataCount", 88, 0, 0, Reflect<short>::info() },
			{ "FallbackChar", 90, 0, 0, Reflect<ImWchar>::info() },
			{ "EllipsisChar", 92, 0, 0, Reflect<ImWchar>::info() },
			{ "DirtyLookupTables", 94, 0, 0, Reflect<bool>::info() },
			{ "Scale", 96, 0, 0, Reflect<float>::info() },
			{ "Ascent", 100, 0, 0, Reflect<float>::info() },
			{ "Descent", 104, 0, 0, Reflect<float>::info() },
			{ "MetricsTotalSurface", 108, 0, 0, Reflect<int>::info() },
			{ "Used4kPagesMap", 112, 0, 0, Reflect<ImU8 [2]>::info() },
		};
		static const Type_Info_Struct i(120, "ImFont", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<ImGuiPlatformIO> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "Platform_CreateWindow", 0, 0, 0, Reflect<void (*)(ImGuiViewport *)>::info() },
			{ "Platform_DestroyWindow", 8, 0, 0, Reflect<void (*)(ImGuiViewport *)>::info() },
			{ "Platform_ShowWindow", 16, 0, 0, Reflect<void (*)(ImGuiViewport *)>::info() },
			{ "Platform_SetWindowPos", 24, 0, 0, Reflect<void (*)(ImGuiViewport *, ImVec2)>::info() },
			{ "Platform_GetWindowPos", 32, 0, 0, Reflect<ImVec2 (*)(ImGuiViewport *)>::info() },
			{ "Platform_SetWindowSize", 40, 0, 0, Reflect<void (*)(ImGuiViewport *, ImVec2)>::info() },
			{ "Platform_GetWindowSize", 48, 0, 0, Reflect<ImVec2 (*)(ImGuiViewport *)>::info() },
			{ "Platform_SetWindowFocus", 56, 0, 0, Reflect<void (*)(ImGuiViewport *)>::info() },
			{ "Platform_GetWindowFocus", 64, 0, 0, Reflect<bool (*)(ImGuiViewport *)>::info() },
			{ "Platform_GetWindowMinimized", 72, 0, 0, Reflect<bool (*)(ImGuiViewport *)>::info() },
			{ "Platform_SetWindowTitle", 80, 0, 0, Reflect<void (*)(ImGuiViewport *, const char *)>::info() },
			{ "Platform_SetWindowAlpha", 88, 0, 0, Reflect<void (*)(ImGuiViewport *, float)>::info() },
			{ "Platform_UpdateWindow", 96, 0, 0, Reflect<void (*)(ImGuiViewport *)>::info() },
			{ "Platform_RenderWindow", 104, 0, 0, Reflect<void (*)(ImGuiViewport *, void *)>::info() },
			{ "Platform_SwapBuffers", 112, 0, 0, Reflect<void (*)(ImGuiViewport *, void *)>::info() },
			{ "Platform_GetWindowDpiScale", 120, 0, 0, Reflect<float (*)(ImGuiViewport *)>::info() },
			{ "Platform_OnChangedViewport", 128, 0, 0, Reflect<void (*)(ImGuiViewport *)>::info() },
			{ "Platform_SetImeInputPos", 136, 0, 0, Reflect<void (*)(ImGuiViewport *, ImVec2)>::info() },
			{ "Platform_CreateVkSurface", 144, 0, 0, Reflect<int (*)(ImGuiViewport *, ImU64, const void *, ImU64 *)>::info() },
			{ "Renderer_CreateWindow", 152, 0, 0, Reflect<void (*)(ImGuiViewport *)>::info() },
			{ "Renderer_DestroyWindow", 160, 0, 0, Reflect<void (*)(ImGuiViewport *)>::info() },
			{ "Renderer_SetWindowSize", 168, 0, 0, Reflect<void (*)(ImGuiViewport *, ImVec2)>::info() },
			{ "Renderer_RenderWindow", 176, 0, 0, Reflect<void (*)(ImGuiViewport *, void *)>::info() },
			{ "Renderer_SwapBuffers", 184, 0, 0, Reflect<void (*)(ImGuiViewport *, void *)>::info() },
			{ "Monitors", 192, 0, 0, Reflect<ImVector<ImGuiPlatformMonitor>>::info() },
			{ "MainViewport", 208, 0, 0, Reflect<ImGuiViewport *>::info() },
			{ "Viewports", 216, 0, 0, Reflect<ImVector<ImGuiViewport *>>::info() },
		};
		static const Type_Info_Struct i(232, "ImGuiPlatformIO", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<ImGuiPlatformMonitor> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "MainPos", 0, 0, 0, Reflect<ImVec2>::info() },
			{ "MainSize", 8, 0, 0, Reflect<ImVec2>::info() },
			{ "WorkPos", 16, 0, 0, Reflect<ImVec2>::info() },
			{ "WorkSize", 24, 0, 0, Reflect<ImVec2>::info() },
			{ "DpiScale", 32, 0, 0, Reflect<float>::info() },
		};
		static const Type_Info_Struct i(36, "ImGuiPlatformMonitor", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<ImGuiViewport> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "ID", 0, 0, 0, Reflect<ImGuiID>::info() },
			{ "Flags", 4, 0, 0, Reflect<ImGuiViewportFlags>::info() },
			{ "Pos", 8, 0, 0, Reflect<ImVec2>::info() },
			{ "Size", 16, 0, 0, Reflect<ImVec2>::info() },
			{ "WorkOffsetMin", 24, 0, 0, Reflect<ImVec2>::info() },
			{ "WorkOffsetMax", 32, 0, 0, Reflect<ImVec2>::info() },
			{ "DpiScale", 40, 0, 0, Reflect<float>::info() },
			{ "DrawData", 48, 0, 0, Reflect<ImDrawData *>::info() },
			{ "ParentViewportId", 56, 0, 0, Reflect<ImGuiID>::info() },
			{ "RendererUserData", 64, 0, 0, Reflect<void *>::info() },
			{ "PlatformUserData", 72, 0, 0, Reflect<void *>::info() },
			{ "PlatformHandle", 80, 0, 0, Reflect<void *>::info() },
			{ "PlatformHandleRaw", 88, 0, 0, Reflect<void *>::info() },
			{ "PlatformRequestMove", 96, 0, 0, Reflect<bool>::info() },
			{ "PlatformRequestResize", 97, 0, 0, Reflect<bool>::info() },
			{ "PlatformRequestClose", 98, 0, 0, Reflect<bool>::info() },
		};
		static const Type_Info_Struct i(104, "ImGuiViewport", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Rng> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "state", 0, 0, 0, Reflect<pcg_state_setseq_64>::info() },
		};
		static const Type_Info_Struct i(16, "Rng", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<stbtt__csctx> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "bounds", 0, 0, 0, Reflect<int>::info() },
			{ "started", 4, 0, 0, Reflect<int>::info() },
			{ "first_x", 8, 0, 0, Reflect<float>::info() },
			{ "first_y", 12, 0, 0, Reflect<float>::info() },
			{ "x", 16, 0, 0, Reflect<float>::info() },
			{ "y", 20, 0, 0, Reflect<float>::info() },
			{ "min_x", 24, 0, 0, Reflect<s32>::info() },
			{ "max_x", 28, 0, 0, Reflect<s32>::info() },
			{ "min_y", 32, 0, 0, Reflect<s32>::info() },
			{ "max_y", 36, 0, 0, Reflect<s32>::info() },
			{ "pvertices", 40, 0, 0, Reflect<stbtt_vertex *>::info() },
			{ "num_vertices", 48, 0, 0, Reflect<int>::info() },
		};
		static const Type_Info_Struct i(56, "stbtt__csctx", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<stbtt__hheap_chunk> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "next", 0, 0, 0, Reflect<struct stbtt__hheap_chunk *>::info() },
		};
		static const Type_Info_Struct i(8, "stbtt__hheap_chunk", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<stbtt__hheap> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "head", 0, 0, 0, Reflect<struct stbtt__hheap_chunk *>::info() },
			{ "first_free", 8, 0, 0, Reflect<void *>::info() },
			{ "num_remaining_in_head_chunk", 16, 0, 0, Reflect<int>::info() },
		};
		static const Type_Info_Struct i(24, "stbtt__hheap", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<stbtt__edge> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "x0", 0, 0, 0, Reflect<float>::info() },
			{ "y0", 4, 0, 0, Reflect<float>::info() },
			{ "x1", 8, 0, 0, Reflect<float>::info() },
			{ "y1", 12, 0, 0, Reflect<float>::info() },
			{ "invert", 16, 0, 0, Reflect<int>::info() },
		};
		static const Type_Info_Struct i(20, "stbtt__edge", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<stbtt__active_edge> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "next", 0, 0, 0, Reflect<struct stbtt__active_edge *>::info() },
			{ "fx", 8, 0, 0, Reflect<float>::info() },
			{ "fdx", 12, 0, 0, Reflect<float>::info() },
			{ "fdy", 16, 0, 0, Reflect<float>::info() },
			{ "direction", 20, 0, 0, Reflect<float>::info() },
			{ "sy", 24, 0, 0, Reflect<float>::info() },
			{ "ey", 28, 0, 0, Reflect<float>::info() },
		};
		static const Type_Info_Struct i(32, "stbtt__active_edge", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<stbtt__point> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "x", 0, 0, 0, Reflect<float>::info() },
			{ "y", 4, 0, 0, Reflect<float>::info() },
		};
		static const Type_Info_Struct i(8, "stbtt__point", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<stbrp_context> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "width", 0, 0, 0, Reflect<int>::info() },
			{ "height", 4, 0, 0, Reflect<int>::info() },
			{ "x", 8, 0, 0, Reflect<int>::info() },
			{ "y", 12, 0, 0, Reflect<int>::info() },
			{ "bottom_y", 16, 0, 0, Reflect<int>::info() },
		};
		static const Type_Info_Struct i(20, "stbrp_context", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<stbrp_node> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "x", 0, 0, 0, Reflect<unsigned char>::info() },
		};
		static const Type_Info_Struct i(1, "stbrp_node", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<stbrp_rect> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "x", 0, 0, 0, Reflect<stbrp_coord>::info() },
			{ "y", 4, 0, 0, Reflect<stbrp_coord>::info() },
			{ "id", 8, 0, 0, Reflect<int>::info() },
			{ "w", 12, 0, 0, Reflect<int>::info() },
			{ "h", 16, 0, 0, Reflect<int>::info() },
			{ "was_packed", 20, 0, 0, Reflect<int>::info() },
		};
		static const Type_Info_Struct i(24, "stbrp_rect", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Feature_Indices> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "count", 0, 0, 0, Reflect<u16>::info() },
			{ "ptr", 8, 0, 0, Reflect<u16 *>::info() },
		};
		static const Type_Info_Struct i(16, "Feature_Indices", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Feature_List> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "ptr", 0, 0, 0, Reflect<u8 *>::info() },
			{ "count", 8, 0, 0, Reflect<u16>::info() },
			{ "records", 16, 0, 0, Reflect<u8 *>::info() },
			{ "indices", 24, 0, 0, Reflect<Feature_Indices>::info() },
		};
		static const Type_Info_Struct i(40, "Feature_List", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Lookup_List> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "ptr", 0, 0, 0, Reflect<u8 *>::info() },
			{ "count", 8, 0, 0, Reflect<u16>::info() },
			{ "lookups", 16, 0, 0, Reflect<u8 *>::info() },
		};
		static const Type_Info_Struct i(24, "Lookup_List", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Dynamic_Font> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "info", 0, 0, 0, Reflect<stbtt_fontinfo>::info() },
			{ "glyph_class_defination", 176, 0, 0, Reflect<void *>::info() },
			{ "mark_attach_class_defination", 184, 0, 0, Reflect<void *>::info() },
			{ "mark_glyph_set", 192, 0, 0, Reflect<void *>::info() },
			{ "gsub_feature", 200, 0, 0, Reflect<Feature_List>::info() },
			{ "gsub_lookup", 240, 0, 0, Reflect<Lookup_List>::info() },
		};
		static const Type_Info_Struct i(264, "Dynamic_Font", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Glyph_Info> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "id", 0, 0, 0, Reflect<u32>::info() },
			{ "cluster", 4, 0, 0, Reflect<int>::info() },
			{ "codepoints", 8, 0, 0, Reflect<u32 [10]>::info() },
			{ "codepoint_count", 48, 0, 0, Reflect<int>::info() },
		};
		static const Type_Info_Struct i(52, "Glyph_Info", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Glyph_Range> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "infos", 0, 0, 0, Reflect<Glyph_Info *>::info() },
			{ "count", 8, 0, 0, Reflect<s32>::info() },
			{ "current", 12, 0, 0, Reflect<s32>::info() },
			{ "max", 16, 0, 0, Reflect<s32>::info() },
			{ "skip", 20, 0, 0, Reflect<s32>::info() },
		};
		static const Type_Info_Struct i(24, "Glyph_Range", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Grapheme_Cluster> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "codepoint", 0, 0, 0, Reflect<utf32>::info() },
			{ "prop", 4, 0, 0, Reflect<Ucd_Grapheme_Property>::info() },
			{ "syllable", 8, 0, 0, Reflect<Ucd_Indic_Syllable>::info() },
			{ "value", 12, 0, 0, Reflect<int>::info() },
		};
		static const Type_Info_Struct i(16, "Grapheme_Cluster", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Font_Shape> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "glyph_infos", 0, 0, 0, Reflect<Glyph_Info *>::info() },
			{ "codepoints", 8, 0, 0, Reflect<Array_View<Grapheme_Cluster>>::info() },
			{ "glyph_count", 24, 0, 0, Reflect<u32>::info() },
		};
		static const Type_Info_Struct i(32, "Font_Shape", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Istream> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "base", 0, 0, 0, Reflect<u8 *>::info() },
			{ "end", 8, 0, 0, Reflect<u8 *>::info() },
			{ "current", 16, 0, 0, Reflect<u8 *>::info() },
		};
		static const Type_Info_Struct i(24, "Istream", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Ostream::Bucket> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "data", 0, 0, 0, Reflect<u8 [4096]>::info() },
			{ "filled", 4096, 0, 0, Reflect<s64>::info() },
			{ "prev", 4104, 0, 0, Reflect<Ostream::Bucket *>::info() },
			{ "next", 4112, 0, 0, Reflect<Ostream::Bucket *>::info() },
		};
		static const Type_Info_Struct i(4120, "Ostream::Bucket", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Ostream> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "head", 0, 0, 0, Reflect<Ostream::Bucket>::info() },
			{ "tail", 4120, 0, 0, Reflect<Ostream::Bucket *>::info() },
			{ "allocator", 4128, 0, 0, Reflect<Allocator>::info() },
		};
		static const Type_Info_Struct i(4144, "Ostream", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Type_Info> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "id", 0, 0, 0, Reflect<Type_Id>::info() },
			{ "size", 8, 0, 0, Reflect<ptrsize>::info() },
			{ "name", 16, 0, 0, Reflect<String>::info() },
			{ "hash", 32, 0, 0, Reflect<u32>::info() },
		};
		static const Type_Info_Struct i(40, "Type_Info", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Struct_Member> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "name", 0, 0, 0, Reflect<String>::info() },
			{ "offset", 16, 0, 0, Reflect<ptrsize>::info() },
			{ "attribute_count", 24, 0, 0, Reflect<ptrsize>::info() },
			{ "attributes", 32, 0, 0, Reflect<const String *>::info() },
			{ "info", 40, 0, 0, Reflect<const Type_Info *const>::info() },
		};
		static const Type_Info_Struct i(48, "Struct_Member", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Type_Info_Struct> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "member_count", 40, 0, 0, Reflect<ptrsize>::info() },
			{ "members", 48, 0, 0, Reflect<const Struct_Member *const>::info() },
			{ "base", 56, 0, 0, Reflect<const Type_Info *const>::info() },
		};
		static const Type_Info_Struct i(64, "Type_Info_Struct", static_count(struct_members), struct_members, Reflect<struct Type_Info>::info());
		return &i;
	}
};

template <> struct Reflect<Union_Member> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "name", 0, 0, 0, Reflect<String>::info() },
			{ "attribute_count", 16, 0, 0, Reflect<ptrsize>::info() },
			{ "attributes", 24, 0, 0, Reflect<const String *>::info() },
			{ "info", 32, 0, 0, Reflect<const Type_Info *>::info() },
		};
		static const Type_Info_Struct i(40, "Union_Member", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Type_Info_Union> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "member_count", 40, 0, 0, Reflect<ptrsize>::info() },
			{ "members", 48, 0, 0, Reflect<const Union_Member *>::info() },
		};
		static const Type_Info_Struct i(56, "Type_Info_Union", static_count(struct_members), struct_members, Reflect<struct Type_Info>::info());
		return &i;
	}
};

template <> struct Reflect<Type_Info_Enum> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "count", 40, 0, 0, Reflect<ptrsize>::info() },
		};
		static const Type_Info_Struct i(48, "Type_Info_Enum", static_count(struct_members), struct_members, Reflect<struct Type_Info>::info());
		return &i;
	}
};

template <> struct Reflect<Type_Info_Pointer> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "pointer_to", 40, 0, 0, Reflect<const Type_Info *const>::info() },
		};
		static const Type_Info_Struct i(48, "Type_Info_Pointer", static_count(struct_members), struct_members, Reflect<struct Type_Info>::info());
		return &i;
	}
};

template <> struct Reflect<Type_Info_Static_Array> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "count", 40, 0, 0, Reflect<const size_t>::info() },
			{ "type", 48, 0, 0, Reflect<const Type_Info *const>::info() },
		};
		static const Type_Info_Struct i(56, "Type_Info_Static_Array", static_count(struct_members), struct_members, Reflect<struct Type_Info>::info());
		return &i;
	}
};

template <> struct Reflect<Type_Info_Function> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "return_type", 40, 0, 0, Reflect<const Type_Info *const>::info() },
			{ "arguments_count", 48, 0, 0, Reflect<ptrsize>::info() },
			{ "arguments", 56, 0, 0, Reflect<const Type_Info *const *>::info() },
		};
		static const Type_Info_Struct i(64, "Type_Info_Function", static_count(struct_members), struct_members, Reflect<struct Type_Info>::info());
		return &i;
	}
};

template <typename T> struct Reflect<Enum_Info<T>> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		using Templated_Type = Enum_Info<T>;
		static const Type_Info_Struct i(sizeof(Templated_Type), "Enum_Info", 0, 0, 0);
		return &i;
	}
};

template <typename T> struct Reflect<Reflect<T>> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		using Templated_Type = Reflect<T>;
		static const Type_Info_Struct i(sizeof(Templated_Type), "Reflect", 0, 0, 0);
		return &i;
	}
};

template <> struct Reflect<Reflect<u8>> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Type_Info_Struct i(1, "Reflect<u8>", 0, 0, 0);
		return &i;
	}
};

template <> struct Reflect<Reflect<u16>> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Type_Info_Struct i(1, "Reflect<u16>", 0, 0, 0);
		return &i;
	}
};

template <> struct Reflect<Reflect<u32>> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Type_Info_Struct i(1, "Reflect<u32>", 0, 0, 0);
		return &i;
	}
};

template <> struct Reflect<Reflect<u64>> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Type_Info_Struct i(1, "Reflect<u64>", 0, 0, 0);
		return &i;
	}
};

template <> struct Reflect<Reflect<s8>> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Type_Info_Struct i(1, "Reflect<s8>", 0, 0, 0);
		return &i;
	}
};

template <> struct Reflect<Reflect<s16>> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Type_Info_Struct i(1, "Reflect<s16>", 0, 0, 0);
		return &i;
	}
};

template <> struct Reflect<Reflect<s32>> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Type_Info_Struct i(1, "Reflect<s32>", 0, 0, 0);
		return &i;
	}
};

template <> struct Reflect<Reflect<s64>> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Type_Info_Struct i(1, "Reflect<s64>", 0, 0, 0);
		return &i;
	}
};

template <> struct Reflect<Reflect<char>> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Type_Info_Struct i(1, "Reflect<char>", 0, 0, 0);
		return &i;
	}
};

template <> struct Reflect<Reflect<void>> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Type_Info_Struct i(1, "Reflect<void>", 0, 0, 0);
		return &i;
	}
};

template <> struct Reflect<Reflect<r32>> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Type_Info_Struct i(1, "Reflect<r32>", 0, 0, 0);
		return &i;
	}
};

template <> struct Reflect<Reflect<r64>> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Type_Info_Struct i(1, "Reflect<r64>", 0, 0, 0);
		return &i;
	}
};

template <> struct Reflect<stbsp__context> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "buf", 0, 0, 0, Reflect<char *>::info() },
			{ "count", 8, 0, 0, Reflect<int>::info() },
			{ "length", 12, 0, 0, Reflect<int>::info() },
			{ "tmp", 16, 0, 0, Reflect<char [512]>::info() },
		};
		static const Type_Info_Struct i(528, "stbsp__context", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Gfx_Platform_Info> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "vendor", 0, 0, 0, Reflect<const u8 *>::info() },
			{ "renderer", 8, 0, 0, Reflect<const u8 *>::info() },
			{ "version", 16, 0, 0, Reflect<const u8 *>::info() },
			{ "shading_lang_version", 24, 0, 0, Reflect<const u8 *>::info() },
		};
		static const Type_Info_Struct i(32, "Gfx_Platform_Info", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Gfx_Platform> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "backend", 8, 0, 0, Reflect<Render_Backend>::info() },
			{ "info", 16, 0, 0, Reflect<Gfx_Platform_Info>::info() },
		};
		static const Type_Info_Struct i(48, "Gfx_Platform", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Quad_Vertex> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "position", 0, 0, 0, Reflect<Vec2>::info() },
			{ "tex_coord", 8, 0, 0, Reflect<Vec2>::info() },
			{ "color", 16, 0, 0, Reflect<Color4>::info() },
		};
		static const Type_Info_Struct i(32, "Quad_Vertex", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Quad_Buffer> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "transform", 0, 0, 0, Reflect<Mat4>::info() },
		};
		static const Type_Info_Struct i(64, "Quad_Buffer", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Quad> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "v", 0, 0, 0, Reflect<Vec2 [4]>::info() },
			{ "t", 32, 0, 0, Reflect<Vec2 [4]>::info() },
			{ "color", 64, 0, 0, Reflect<Color4 [4]>::info() },
			{ "texture", 128, 0, 0, Reflect<Handle>::info() },
		};
		static const Type_Info_Struct i(136, "Quad", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Quad_Sort_Key> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "key", 0, 0, 0, Reflect<u32>::info() },
			{ "index", 4, 0, 0, Reflect<u32>::info() },
		};
		static const Type_Info_Struct i(8, "Quad_Sort_Key", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Quad_Render_Range> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "texture", 0, 0, 0, Reflect<Handle>::info() },
			{ "index", 8, 0, 0, Reflect<u32>::info() },
			{ "count", 12, 0, 0, Reflect<u32>::info() },
		};
		static const Type_Info_Struct i(16, "Quad_Render_Range", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Attribute_Layout> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "index", 0, 0, 0, Reflect<GLuint>::info() },
			{ "count", 4, 0, 0, Reflect<GLint>::info() },
			{ "offset", 8, 0, 0, Reflect<ptrsize>::info() },
		};
		static const Type_Info_Struct i(16, "Attribute_Layout", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Uniform> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "location", 0, 0, 0, Reflect<GLint>::info() },
			{ "type", 4, 0, 0, Reflect<int>::info() },
		};
		static const Type_Info_Struct i(8, "Uniform", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Shader> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "id", 0, 0, 0, Reflect<GLuint>::info() },
			{ "option", 4, 0, 0, Reflect<Render_Options>::info() },
			{ "in_attributes", 8, 0, 0, Reflect<Attribute_Layout [10]>::info() },
			{ "in_attribute_count", 168, 0, 0, Reflect<u32>::info() },
			{ "stride", 176, 0, 0, Reflect<ptrsize>::info() },
			{ "uniforms", 184, 0, 0, Reflect<Uniform [10]>::info() },
			{ "uniform_count", 264, 0, 0, Reflect<u32>::info() },
			{ "textures", 268, 0, 0, Reflect<Uniform [5]>::info() },
			{ "texture_count", 308, 0, 0, Reflect<u32>::info() },
		};
		static const Type_Info_Struct i(312, "Shader", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<Gfx_Platform_OpenGL> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "platform_handle", 48, 0, 0, Reflect<Handle>::info() },
			{ "multisamples", 56, 0, 0, Reflect<s32>::info() },
			{ "framebuffer_w", 60, 0, 0, Reflect<s32>::info() },
			{ "framebuffer_h", 64, 0, 0, Reflect<s32>::info() },
			{ "vertex_array", 68, 0, 0, Reflect<GLuint>::info() },
			{ "color_renderbuffer", 72, 0, 0, Reflect<GLuint>::info() },
			{ "depth_renderbuffer", 76, 0, 0, Reflect<GLuint>::info() },
			{ "framebuffer", 80, 0, 0, Reflect<GLuint>::info() },
			{ "current_shader", 88, 0, 0, Reflect<Shader *>::info() },
			{ "current_render_index_type", 96, 0, 0, Reflect<GLenum>::info() },
			{ "set_swap_interval_func", 104, 0, 0, Reflect<int (*)(int)>::info() },
			{ "get_swap_interval_func", 112, 0, 0, Reflect<int (*)()>::info() },
		};
		static const Type_Info_Struct i(120, "Gfx_Platform_OpenGL", static_count(struct_members), struct_members, Reflect<struct Gfx_Platform>::info());
		return &i;
	}
};

template <> struct Reflect<DPI_AWARENESS_CONTEXT__> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "unused", 0, 0, 0, Reflect<int>::info() },
		};
		static const Type_Info_Struct i(4, "DPI_AWARENESS_CONTEXT__", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<_MODEMDEVCAPS> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "dwActualSize", 0, 0, 0, Reflect<DWORD>::info() },
			{ "dwRequiredSize", 4, 0, 0, Reflect<DWORD>::info() },
			{ "dwDevSpecificOffset", 8, 0, 0, Reflect<DWORD>::info() },
			{ "dwDevSpecificSize", 12, 0, 0, Reflect<DWORD>::info() },
			{ "dwModemProviderVersion", 16, 0, 0, Reflect<DWORD>::info() },
			{ "dwModemManufacturerOffset", 20, 0, 0, Reflect<DWORD>::info() },
			{ "dwModemManufacturerSize", 24, 0, 0, Reflect<DWORD>::info() },
			{ "dwModemModelOffset", 28, 0, 0, Reflect<DWORD>::info() },
			{ "dwModemModelSize", 32, 0, 0, Reflect<DWORD>::info() },
			{ "dwModemVersionOffset", 36, 0, 0, Reflect<DWORD>::info() },
			{ "dwModemVersionSize", 40, 0, 0, Reflect<DWORD>::info() },
			{ "dwDialOptions", 44, 0, 0, Reflect<DWORD>::info() },
			{ "dwCallSetupFailTimer", 48, 0, 0, Reflect<DWORD>::info() },
			{ "dwInactivityTimeout", 52, 0, 0, Reflect<DWORD>::info() },
			{ "dwSpeakerVolume", 56, 0, 0, Reflect<DWORD>::info() },
			{ "dwSpeakerMode", 60, 0, 0, Reflect<DWORD>::info() },
			{ "dwModemOptions", 64, 0, 0, Reflect<DWORD>::info() },
			{ "dwMaxDTERate", 68, 0, 0, Reflect<DWORD>::info() },
			{ "dwMaxDCERate", 72, 0, 0, Reflect<DWORD>::info() },
			{ "abVariablePortion", 76, 0, 0, Reflect<BYTE [1]>::info() },
		};
		static const Type_Info_Struct i(80, "_MODEMDEVCAPS", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

template <> struct Reflect<_MODEMSETTINGS> {
	static constexpr Type_Id id = Type_Id_STRUCT;
	static const Type_Info * const info() {
		static const Struct_Member struct_members[] = {
			{ "dwActualSize", 0, 0, 0, Reflect<DWORD>::info() },
			{ "dwRequiredSize", 4, 0, 0, Reflect<DWORD>::info() },
			{ "dwDevSpecificOffset", 8, 0, 0, Reflect<DWORD>::info() },
			{ "dwDevSpecificSize", 12, 0, 0, Reflect<DWORD>::info() },
			{ "dwCallSetupFailTimer", 16, 0, 0, Reflect<DWORD>::info() },
			{ "dwInactivityTimeout", 20, 0, 0, Reflect<DWORD>::info() },
			{ "dwSpeakerVolume", 24, 0, 0, Reflect<DWORD>::info() },
			{ "dwSpeakerMode", 28, 0, 0, Reflect<DWORD>::info() },
			{ "dwPreferredModemOptions", 32, 0, 0, Reflect<DWORD>::info() },
			{ "dwNegotiatedModemOptions", 36, 0, 0, Reflect<DWORD>::info() },
			{ "dwNegotiatedDCERate", 40, 0, 0, Reflect<DWORD>::info() },
			{ "abVariablePortion", 44, 0, 0, Reflect<BYTE [1]>::info() },
		};
		static const Type_Info_Struct i(48, "_MODEMSETTINGS", static_count(struct_members), struct_members, 0);
		return &i;
	}
};

