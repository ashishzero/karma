#include "modules/core/systems.h"
#include "modules/gfx/renderer.h"

#include "imconfig.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_dx11.h"

static bool               window_is_active = true;
static bool               mouse_pressed[3];
static Cursor_Kind        mouse_cursor[ImGuiMouseCursor_COUNT];
static ImGuiDockNodeFlags dockspace_flags;
ImGuiWindowFlags          dockspace_main_window_flags;

typedef void (*ImGui_Create_Context)(void *, void *);
typedef void (*ImGui_Destroy_Context)();
typedef void (*ImGui_New_Frame)();
typedef void (*ImGui_Render_Frame)(ImDrawData *draw_data);

static ImGui_Create_Context		imgui_create_context;
static ImGui_Destroy_Context	imgui_destroy_context;
static ImGui_Render_Frame		imgui_render_frame;
static ImGui_New_Frame			imgui_new_frame;

static void imgui_set_renderer_backend(Render_Backend backend) {
	switch (backend) {
		case Render_Backend_NONE: {
			imgui_create_context	= [](void *, void *) {};
			imgui_destroy_context	= []() {};
			imgui_new_frame			= []() {};
			imgui_render_frame		= [](ImDrawData *) {};
		} break;

		case Render_Backend_OPENGL: {
			imgui_create_context	= [](void *, void *) { ImGui_ImplOpenGL3_Init("#version 420"); };
			imgui_destroy_context	= ImGui_ImplOpenGL3_Shutdown;
			imgui_new_frame			= ImGui_ImplOpenGL3_NewFrame;
			imgui_render_frame		= ImGui_ImplOpenGL3_RenderDrawData;
		} break;

		case Render_Backend_DIRECTX11: {
			imgui_create_context	= [](void * device, void *device_context) { ImGui_ImplDX11_Init((ID3D11Device *)device, (ID3D11DeviceContext *)device_context); };
			imgui_destroy_context	= ImGui_ImplDX11_Shutdown;
			imgui_new_frame			= ImGui_ImplDX11_NewFrame;
			imgui_render_frame		= ImGui_ImplDX11_RenderDrawData;
		} break;

		invalid_default_case();
	}
}

void ImGuiEx::RefreshRenderingContext() {
	imgui_destroy_context();
	imgui_set_renderer_backend(gfx_render_backend());
	imgui_create_context(gfx_render_device(), gfx_render_context());
}

void ImGuiEx::Initialize() {
	IMGUI_CHECKVERSION();

	ImGui::SetAllocatorFunctions(
		[](size_t size, void *user_data) -> void * {
			return memory_allocate(size);
		},
		[](void *ptr, void *user_data) {
			memory_free(ptr);
		});

	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	// Setup back-end capabilities flags
	ImGuiIO &io = ImGui::GetIO();
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors; // We can honor GetMouseCursor() values (optional)
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;  // We can honor io.WantSetMousePos requests (optional, rarely used)
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.BackendPlatformName = "Karma";
	io.IniFilename         = 0;

	auto   user_name   = system_get_user_name();
	String ini_content = system_read_entire_file(tprintf("dev/%s_config.ini", tto_cstring(user_name)));

	defer {
		memory_free(user_name.data);
		memory_free(ini_content.data);
	};

	if (ini_content.count) {
		ImGui::LoadIniSettingsFromMemory((char *)ini_content.data, ini_content.count);
	}

	ImFontConfig config;
	config.SizePixels  = 14;
	config.OversampleH = 2;
	config.OversampleV = 2;
	config.PixelSnapH  = true;

	io.Fonts->AddFontFromFileTTF("dev/GeneraleStation-Regular.otf", 14, &config);
	io.Fonts->Build();

	ImGuiStyle *style = &ImGui::GetStyle();
	ImVec4 *colors = style->Colors;

	style->ChildRounding     = 0;
	style->PopupRounding     = 0;
	style->ScrollbarRounding = 0;
	style->TabRounding       = 0;
	style->WindowRounding    = 0;
	style->WindowBorderSize  = 1;
	style->FrameBorderSize   = 0;
	
	style->FrameRounding = 0;
	style->GrabRounding  = 0;

	colors[ImGuiCol_Text] = srgb_to_linear(vec4(1.00f, 1.00f, 1.00f, 1.00f));
	colors[ImGuiCol_TextDisabled] = srgb_to_linear(vec4(0.50f, 0.50f, 0.50f, 1.00f));
	colors[ImGuiCol_WindowBg] = srgb_to_linear(vec4(0.06f, 0.05f, 0.07f, 1.00f));
	colors[ImGuiCol_PopupBg] = srgb_to_linear(vec4(0.07f, 0.07f, 0.09f, 1.00f));
	colors[ImGuiCol_ChildBg] = srgb_to_linear(vec4(0.00f, 0.00f, 0.00f, 0.00f));

	colors[ImGuiCol_Border] = srgb_to_linear(vec4(0.80f, 0.80f, 0.83f, 0.88f));
	colors[ImGuiCol_BorderShadow] = srgb_to_linear(vec4(0.92f, 0.91f, 0.88f, 0.00f));
	colors[ImGuiCol_FrameBg] = srgb_to_linear(vec4(0.10f, 0.09f, 0.12f, 1.00f));
	colors[ImGuiCol_FrameBgHovered] = srgb_to_linear(vec4(0.24f, 0.23f, 0.29f, 1.00f));
	colors[ImGuiCol_FrameBgActive] = srgb_to_linear(vec4(0.56f, 0.56f, 0.58f, 1.00f));
	colors[ImGuiCol_TitleBg] = srgb_to_linear(vec4(0.10f, 0.09f, 0.12f, 1.00f));
	colors[ImGuiCol_TitleBgCollapsed] = srgb_to_linear(vec4(0.10f, 0.09f, 0.12f, 0.75f));
	colors[ImGuiCol_TitleBgActive] = srgb_to_linear(vec4(0.07f, 0.07f, 0.09f, 1.00f));
	colors[ImGuiCol_MenuBarBg] = srgb_to_linear(vec4(0.10f, 0.09f, 0.12f, 1.00f));
	colors[ImGuiCol_ScrollbarBg] = srgb_to_linear(vec4(0.10f, 0.09f, 0.12f, 1.00f));
	colors[ImGuiCol_ScrollbarGrab] = srgb_to_linear(vec4(0.80f, 0.80f, 0.83f, 0.31f));
	colors[ImGuiCol_ScrollbarGrabHovered] = srgb_to_linear(vec4(0.56f, 0.56f, 0.58f, 1.00f));
	colors[ImGuiCol_ScrollbarGrabActive] = srgb_to_linear(vec4(0.06f, 0.05f, 0.07f, 1.00f));
	colors[ImGuiCol_CheckMark] = srgb_to_linear(vec4(0.80f, 0.80f, 1.0f, 1.0f));
	colors[ImGuiCol_SliderGrab] = srgb_to_linear(vec4(0.80f, 0.80f, 0.83f, 0.31f));
	colors[ImGuiCol_SliderGrabActive] = srgb_to_linear(vec4(0.06f, 0.05f, 0.07f, 1.00f));
	colors[ImGuiCol_Button] = srgb_to_linear(vec4(0.10f, 0.09f, 0.12f, 1.00f));
	colors[ImGuiCol_ButtonHovered] = srgb_to_linear(vec4(0.24f, 0.23f, 0.29f, 1.00f));
	colors[ImGuiCol_ButtonActive] = srgb_to_linear(vec4(0.56f, 0.56f, 0.58f, 1.00f));
	colors[ImGuiCol_Header] = srgb_to_linear(vec4(0.10f, 0.09f, 0.12f, 1.00f));
	colors[ImGuiCol_HeaderHovered] = srgb_to_linear(vec4(0.56f, 0.56f, 0.58f, 1.00f));
	colors[ImGuiCol_HeaderActive] = srgb_to_linear(vec4(0.06f, 0.05f, 0.07f, 1.00f));
	colors[ImGuiCol_ResizeGrip] = srgb_to_linear(vec4(0.00f, 0.00f, 0.00f, 0.00f));
	colors[ImGuiCol_ResizeGripHovered] = srgb_to_linear(vec4(0.56f, 0.56f, 0.58f, 1.00f));
	colors[ImGuiCol_ResizeGripActive] = srgb_to_linear(vec4(0.06f, 0.05f, 0.07f, 1.00f));
	colors[ImGuiCol_PlotLines] = srgb_to_linear(vec4(0.40f, 0.39f, 0.38f, 0.63f));
	colors[ImGuiCol_PlotLinesHovered] = srgb_to_linear(vec4(0.25f, 1.00f, 0.00f, 1.00f));
	colors[ImGuiCol_PlotHistogram] = srgb_to_linear(vec4(0.40f, 0.39f, 0.38f, 0.63f));
	colors[ImGuiCol_PlotHistogramHovered] = srgb_to_linear(vec4(0.25f, 1.00f, 0.00f, 1.00f));
	colors[ImGuiCol_TextSelectedBg] = srgb_to_linear(vec4(0.25f, 1.00f, 0.00f, 0.43f));
	colors[ImGuiCol_DragDropTarget] = srgb_to_linear(vec4(1.00f, 1.00f, 0.00f, 0.90f));
	colors[ImGuiCol_NavHighlight] = srgb_to_linear(vec4(0.26f, 0.59f, 0.98f, 1.00f));
	colors[ImGuiCol_NavWindowingHighlight] = srgb_to_linear(vec4(1.00f, 1.00f, 1.00f, 0.70f));
	colors[ImGuiCol_NavWindowingDimBg] = srgb_to_linear(vec4(0.80f, 0.80f, 0.80f, 0.20f));
	colors[ImGuiCol_ModalWindowDimBg] = srgb_to_linear(vec4(0.80f, 0.80f, 0.80f, 0.35f));

	colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
	colors[ImGuiCol_SeparatorHovered] = srgb_to_linear(vec4(0.10f, 0.40f, 0.75f, 0.78f));
	colors[ImGuiCol_SeparatorActive] = srgb_to_linear(vec4(0.10f, 0.40f, 0.75f, 1.00f));

	colors[ImGuiCol_Tab] = lerp(colors[ImGuiCol_Header], colors[ImGuiCol_TitleBgActive], 0.80f);
	colors[ImGuiCol_TabHovered] = colors[ImGuiCol_HeaderHovered];
	colors[ImGuiCol_TabActive] = lerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f);
	colors[ImGuiCol_TabUnfocused] = lerp(colors[ImGuiCol_Tab], colors[ImGuiCol_TitleBg], 0.80f);
	colors[ImGuiCol_TabUnfocusedActive] = lerp(colors[ImGuiCol_TabActive], colors[ImGuiCol_TitleBg], 0.40f);
	colors[ImGuiCol_DockingPreview] = srgb_to_linear(vec4(1.5f, 1.0f, 1.0f, 1.0f));
	colors[ImGuiCol_DockingEmptyBg] = srgb_to_linear(vec4(1.5f, 1.0f, 1.0f, 1.00f));

	// Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
	io.KeyMap[ImGuiKey_Tab]        = Key_TAB;
	io.KeyMap[ImGuiKey_LeftArrow]  = Key_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = Key_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow]    = Key_UP;
	io.KeyMap[ImGuiKey_DownArrow]  = Key_DOWN;
	io.KeyMap[ImGuiKey_PageUp]     = Key_PAGE_UP;
	io.KeyMap[ImGuiKey_PageDown]   = Key_PAGE_DOWN;
	io.KeyMap[ImGuiKey_Home]       = Key_HOME;
	io.KeyMap[ImGuiKey_End]        = Key_END;
	io.KeyMap[ImGuiKey_Insert]     = Key_INSERT;
	io.KeyMap[ImGuiKey_Delete]     = Key_DELETE;
	io.KeyMap[ImGuiKey_Backspace]  = Key_BACKSPACE;
	io.KeyMap[ImGuiKey_Space]      = Key_SPACE;
	io.KeyMap[ImGuiKey_Enter]      = Key_RETURN;
	io.KeyMap[ImGuiKey_Escape]     = Key_ESCAPE;
	io.KeyMap[ImGuiKey_A]          = Key_A;
	io.KeyMap[ImGuiKey_C]          = Key_C;
	io.KeyMap[ImGuiKey_V]          = Key_V;
	io.KeyMap[ImGuiKey_X]          = Key_X;
	io.KeyMap[ImGuiKey_Y]          = Key_Y;
	io.KeyMap[ImGuiKey_Z]          = Key_Z;

	io.SetClipboardTextFn = [](void *user_data, const char *text) {
		system_set_clipboard_text(String(text, strlen(text)));
	};

	io.GetClipboardTextFn = [](void *user_data) -> const char * {
		String string = system_get_clipboard_text();
		if (string.count) {
			char *data = (char *)ImGui::MemAlloc(string.count + 1);
			memcpy(data, string.data, string.count);
			data[string.count] = 0;
			memory_free(string.data);
			return data;
		}
		return 0;
	};
	io.ClipboardUserData = 0;

	mouse_cursor[ImGuiMouseCursor_Arrow]      = Cursor_Kind_ARROW;
	mouse_cursor[ImGuiMouseCursor_TextInput]  = Cursor_Kind_IBEAM;
	mouse_cursor[ImGuiMouseCursor_ResizeAll]  = Cursor_Kind_SIZE_ALL;
	mouse_cursor[ImGuiMouseCursor_ResizeNS]   = Cursor_Kind_SIZE_NS;
	mouse_cursor[ImGuiMouseCursor_ResizeEW]   = Cursor_Kind_SIZE_EW;
	mouse_cursor[ImGuiMouseCursor_ResizeNESW] = Cursor_Kind_SIZE_NESW;
	mouse_cursor[ImGuiMouseCursor_ResizeNWSE] = Cursor_Kind_SIZE_NWSE;
	mouse_cursor[ImGuiMouseCursor_Hand]       = Cursor_Kind_HAND;

	io.BackendRendererName = "Karma Renderer";
	io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

	imgui_set_renderer_backend(gfx_render_backend());
	imgui_create_context(gfx_render_device(), gfx_render_context());

	dockspace_flags |= ImGuiDockNodeFlags_PassthruCentralNode;
	dockspace_flags |= ImGuiDockNodeFlags_AutoHideTabBar;
	//dockspace_flags |= ImGuiDockNodeFlags_NoDockingInCentralNode;

	dockspace_main_window_flags |= ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoMove;
	dockspace_main_window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
	dockspace_main_window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	dockspace_main_window_flags |= ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize;
	dockspace_main_window_flags |= ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMouseInputs;
	dockspace_main_window_flags |= ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus;
}

void ImGuiEx::Shutdown() {
	String ini_content;
	size_t size       = 0;
	ini_content.data  = (utf8 *)ImGui::SaveIniSettingsToMemory(&size);
	ini_content.count = (s64)size;

	auto user_name = system_get_user_name();
	system_write_entire_file(tprintf("dev/%s_config.ini", tto_cstring(user_name)), ini_content);

	memory_free(user_name.data);

	imgui_destroy_context();
	imgui_set_renderer_backend(Render_Backend_NONE);

	ImGui::DestroyContext();
}

bool ImGuiEx::HandleEvent(const Event &event) {
	ImGuiIO &io = ImGui::GetIO();

	switch (event.type) {
		case Event_Type_WINDOW_ACTIVE: {
			window_is_active = true;
			return false;
		} break;

		case Event_Type_WINDOW_INACTIVE: {
			window_is_active = false;
			return false;
		} break;

		case Event_Type_MOUSE_WHEEL: {
			if (event.mouse_wheel.horizontal > 0) io.MouseWheelH += 1;
			if (event.mouse_wheel.horizontal < 0) io.MouseWheelH -= 1;
			if (event.mouse_wheel.vertical > 0) io.MouseWheel += 1;
			if (event.mouse_wheel.vertical < 0) io.MouseWheel -= 1;
			return io.WantCaptureMouse;
		} break;

		case Event_Type_MOUSE_BUTTON_DOWN: {
			if (event.mouse_button.symbol == Button_LEFT) mouse_pressed[0] = true;
			if (event.mouse_button.symbol == Button_RIGHT) mouse_pressed[1] = true;
			if (event.mouse_button.symbol == Button_MIDDLE) mouse_pressed[2] = true;
			return io.WantCaptureMouse;
		} break;

		case Event_Type_TEXT_INPUT: {
			io.AddInputCharacter(event.text.codepoint.code);
			return io.WantTextInput;
		} break;

		case Event_Type_KEY_UP:
		case Event_Type_KEY_DOWN: {
			auto key = event.key.symbol;
			IM_ASSERT(key >= 0 && key < IM_ARRAYSIZE(io.KeysDown));
			io.KeysDown[key] = event.type == Event_Type_KEY_DOWN;
			io.KeyShift      = ((system_get_key_mods() & Key_Mod_SHIFT) != 0);
			io.KeyCtrl       = ((system_get_key_mods() & Key_Mod_CTRL) != 0);
			io.KeyAlt        = ((system_get_key_mods() & Key_Mod_ALT) != 0);
			io.KeySuper      = 0;
			return io.WantCaptureKeyboard;
		} break;
	}
	return false;
}

void ImGuiEx::UpdateFrame(r32 dt) {
	imgui_new_frame();

	ImGuiIO &io = ImGui::GetIO();
	IM_ASSERT(io.Fonts->IsBuilt() && "Font atlas not built! It is generally built by the renderer back-end. Missing call to renderer _NewFrame() function? e.g. ImGui_ImplOpenGL3_NewFrame().");

	// Setup display size (every frame to accommodate for window resizing)
	auto size                  = system_get_client_size();
	io.DisplaySize             = ImVec2((float)size.x, (float)size.y);
	io.DisplayFramebufferScale = ImVec2(1, 1);
	io.DeltaTime               = dt;

	// If a mouse press event came, always pass it as "mouse held this frame",
	// so we don't miss click-release sys_events that are shorter than 1 frame.
	if (window_is_active) {
		io.MouseDown[0] = mouse_pressed[0] || (system_button(Button_LEFT) == Key_State_DOWN);
		io.MouseDown[1] = mouse_pressed[1] || (system_button(Button_RIGHT) == Key_State_DOWN);
		io.MouseDown[2] = mouse_pressed[2] || (system_button(Button_MIDDLE) == Key_State_DOWN);
		io.MousePos     = system_get_cursor_position_y_inverted();
	}

	mouse_pressed[0] = mouse_pressed[1] = mouse_pressed[2] = false;


	if (!(io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)) {
		ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
		if (io.MouseDrawCursor || imgui_cursor == ImGuiMouseCursor_None) {
			system_hide_cursor();
		} else {
			auto cursor = imgui_cursor ? mouse_cursor[imgui_cursor] : Cursor_Kind_ARROW;
			system_set_cursor_kind(cursor);
			system_show_cursor();
		}
	}

	ImGui::NewFrame();

	// Docking
	ImGuiViewport *viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->GetWorkPos());
	ImGui::SetNextWindowSize(viewport->GetWorkSize());
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace", 0, dockspace_main_window_flags);
	ImGui::PopStyleVar();
	ImGui::DockSpace(ImGui::GetID("Playground"), ImVec2(0.0f, 0.0f), dockspace_flags);
	ImGui::End();
}

void ImGuiEx::RenderFrame() {
	ImGui::Render();
	imgui_render_frame(ImGui::GetDrawData());
}

//
//
//
#include "imgui_internal.h"

Vec2 ImGuiEx::MouseCursorReverse() {
	Vec2 c = ImGui::GetIO().MousePos;
	r32 y = (r32)system_get_client_size().y;
	c.y = y - c.y;
	return c;
}

bool ImGuiEx::LabelCheckbox(const char *label, bool *v, bool enabled) {
	ImGuiWindow *window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext &g = *GImGui;
	const ImGuiStyle &style = g.Style;
	const float w = ImGui::CalcItemWidth();

	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
	const ImRect value_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y * 2));
	const ImRect total_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w + (label_size.x > 0.0f ? style.ItemInnerSpacing.x : 0.0f), style.FramePadding.y * 2) + label_size);
	ImGui::ItemSize(total_bb, style.FramePadding.y);
	if (!ImGui::ItemAdd(total_bb, 0))
		return false;

	const float square_sz = ImGui::GetFrameHeight();
	const ImRect check_bb(value_bb.Min, value_bb.Min + ImVec2(square_sz, square_sz));
	ImGuiID id = window->GetID(null_tprintf("%s##button_checkbox", label));

	if (!ImGui::ItemAdd(check_bb, id))
		return false;

	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(check_bb, id, &hovered, &held);
	if (pressed && enabled)
	{
		*v = !(*v);
		ImGui::MarkItemEdited(id);
	}

	ImU32 button_color;
	if (enabled) {
		button_color = ImGui::GetColorU32((held && hovered) ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg);
	} else {
		button_color = ImGui::GetColorU32(ImGuiCol_TextDisabled);
	}

	ImGui::RenderNavHighlight(check_bb, id);
	ImGui::RenderFrame(check_bb.Min, check_bb.Max, button_color, true, style.FrameRounding);
	ImU32 check_col = ImGui::GetColorU32(ImGuiCol_CheckMark);
	bool mixed_value = (window->DC.ItemFlags & ImGuiItemFlags_MixedValue) != 0;
	if (mixed_value)
	{
		// Undocumented tristate/mixed/indeterminate checkbox (#2644)
		ImVec2 pad(ImMax(1.0f, IM_FLOOR(square_sz / 3.6f)), ImMax(1.0f, IM_FLOOR(square_sz / 3.6f)));
		window->DrawList->AddRectFilled(check_bb.Min + pad, check_bb.Max - pad, check_col, style.FrameRounding);
	}
	else if (*v)
	{
		const float pad = ImMax(1.0f, IM_FLOOR(square_sz / 6.0f));
		ImGui::RenderCheckMark(window->DrawList, check_bb.Min + ImVec2(pad, pad), check_col, square_sz - pad * 2.0f);
	}

	if (g.LogEnabled)
		ImGui::LogRenderedText(&check_bb.Min, mixed_value ? "[~]" : *v ? "[x]" : "[ ]");

	if (label_size.x > 0.0f)
		ImGui::RenderText(ImVec2(value_bb.Max.x + style.ItemInnerSpacing.x, value_bb.Min.y + style.FramePadding.y), label);

	IMGUI_TEST_ENGINE_ITEM_INFO(id, label, window->DC.ItemFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
	return pressed && enabled;
}

void ImGuiEx::LabelColor(const char *label, const Vec4 &color, int color_flags) {
	ImGuiWindow *window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return;

	ImGuiContext &g = *GImGui;
	const ImGuiStyle &style = g.Style;
	const float w = ImGui::CalcItemWidth();

	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
	const ImRect value_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y * 2));
	const ImRect total_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w + (label_size.x > 0.0f ? style.ItemInnerSpacing.x : 0.0f), style.FramePadding.y * 2) + label_size);
	ImGui::ItemSize(total_bb, style.FramePadding.y);
	if (!ImGui::ItemAdd(total_bb, 0))
		return;

	ImU32 ucolor = ImGui::ColorConvertFloat4ToU32(color);

	const char *desc_id = null_tprintf("%s##button_color", label);
	const ImGuiID id = window->GetID(desc_id);
	float default_size = ImGui::GetFrameHeight();
	ImVec2 size(default_size, default_size);

	const ImRect bb(value_bb.Min, value_bb.Min + size);
	const ImRect next_value_bb(ImVec2(value_bb.Min.x + default_size, value_bb.Min.y), value_bb.Max);
	if (!ImGui::ItemAdd(bb, id))
		return;

	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

	if (color_flags & ImGuiColorEditFlags_NoAlpha)
		color_flags &= ~(ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaPreviewHalf);

	ImVec4 col_rgb = color;
	if (color_flags & ImGuiColorEditFlags_InputHSV)
		ImGui::ColorConvertHSVtoRGB(col_rgb.x, col_rgb.y, col_rgb.z, col_rgb.x, col_rgb.y, col_rgb.z);

	ImVec4 col_rgb_without_alpha(col_rgb.x, col_rgb.y, col_rgb.z, 1.0f);
	float grid_step = ImMin(size.x, size.y) / 2.99f;
	float rounding = ImMin(g.Style.FrameRounding, grid_step * 0.5f);
	ImRect bb_inner = bb;
	float off = 0.0f;
	if ((color_flags & ImGuiColorEditFlags_NoBorder) == 0)
	{
		off = -0.75f; // The border (using Col_FrameBg) tends to look off when color is near-opaque and rounding is enabled. This offset seemed like a good middle ground to reduce those artifacts.
		bb_inner.Expand(off);
	}
	if ((color_flags & ImGuiColorEditFlags_AlphaPreviewHalf) && col_rgb.w < 1.0f)
	{
		float mid_x = IM_ROUND((bb_inner.Min.x + bb_inner.Max.x) * 0.5f);
		ImGui::RenderColorRectWithAlphaCheckerboard(window->DrawList, ImVec2(bb_inner.Min.x + grid_step, bb_inner.Min.y), bb_inner.Max, ImGui::GetColorU32(col_rgb), grid_step, ImVec2(-grid_step + off, off), rounding, ImDrawCornerFlags_TopRight | ImDrawCornerFlags_BotRight);
		window->DrawList->AddRectFilled(bb_inner.Min, ImVec2(mid_x, bb_inner.Max.y), ImGui::GetColorU32(col_rgb_without_alpha), rounding, ImDrawCornerFlags_TopLeft | ImDrawCornerFlags_BotLeft);
	}
	else
	{
		// Because GetColorU32() multiplies by the global style Alpha and we don't want to display a checkerboard if the source code had no alpha
		ImVec4 col_source = (color_flags & ImGuiColorEditFlags_AlphaPreview) ? col_rgb : col_rgb_without_alpha;
		if (col_source.w < 1.0f)
			ImGui::RenderColorRectWithAlphaCheckerboard(window->DrawList, bb_inner.Min, bb_inner.Max, ImGui::GetColorU32(col_source), grid_step, ImVec2(off, off), rounding);
		else
			window->DrawList->AddRectFilled(bb_inner.Min, bb_inner.Max, ImGui::GetColorU32(col_source), rounding, ImDrawCornerFlags_All);
	}
	ImGui::RenderNavHighlight(bb, id);
	if ((color_flags & ImGuiColorEditFlags_NoBorder) == 0)
	{
		if (g.Style.FrameBorderSize > 0.0f)
			ImGui::RenderFrameBorder(bb.Min, bb.Max, rounding);
		else
			window->DrawList->AddRect(bb.Min, bb.Max, ImGui::GetColorU32(ImGuiCol_FrameBg), rounding); // Color button are often in need of some sort of border
	}

	// Drag and Drop Source
	// NB: The ActiveId test is merely an optional micro-optimization, BeginDragDropSource() does the same test.
	if (g.ActiveId == id && !(color_flags & ImGuiColorEditFlags_NoDragDrop) && ImGui::BeginDragDropSource())
	{
		if (color_flags & ImGuiColorEditFlags_NoAlpha)
			ImGui::SetDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_3F, &col_rgb, sizeof(float) * 3, ImGuiCond_Once);
		else
			ImGui::SetDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F, &col_rgb, sizeof(float) * 4, ImGuiCond_Once);
		ImGui::ColorButton(desc_id, color, color_flags);
		ImGui::SameLine();
		ImGui::TextEx("Color");
		ImGui::EndDragDropSource();
	}

	// Tooltip
	if (!(color_flags & ImGuiColorEditFlags_NoTooltip) && hovered)
		ImGui::ColorTooltip(desc_id, &color.x, color_flags & (ImGuiColorEditFlags__InputMask | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaPreviewHalf));

	// Render
	const char *value_text_begin = &g.TempBuffer[0];
	const char *value_text_end = value_text_begin + ImFormatString(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer), " #%08x", ucolor);
	ImGui::RenderTextClipped(next_value_bb.Min, next_value_bb.Max, value_text_begin, value_text_end, NULL, ImVec2(0.0f, 0.5f));
	if (label_size.x > 0.0f)
		ImGui::RenderText(ImVec2(next_value_bb.Max.x + style.ItemInnerSpacing.x, next_value_bb.Min.y + style.FramePadding.y), label);
}
