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
	colors[ImGuiCol_CheckMark] = srgb_to_linear(vec4(0.80f, 0.80f, 0.83f, 0.31f));
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
