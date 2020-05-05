#include "../gfx_renderer.h"
#include "../systems.h"

#include "imconfig.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"

static bool               window_is_active = true;
static bool               mouse_pressed[3];
static Cursor_Kind        mouse_cursor[ImGuiMouseCursor_COUNT];
static ImGuiDockNodeFlags dockspace_flags;
ImGuiWindowFlags          dockspace_main_window_flags;
static bool               show_overlay;
static r32                cpu_frame_time;
static r32                event_frame_time;
static r32                simulate_frame_time;
static r32                render_frame_time;
static r32                gpu_frame_time;

void ImGui::Initialize() {
	IMGUI_CHECKVERSION();

	ImGui::SetAllocatorFunctions(
		[](size_t size, void *user_data) -> void * {
			return mallocate(size);
		},
		[](void *ptr, void *user_data) {
			mfree(ptr);
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
	String ini_content = system_read_entire_file(tprintf("imgui/%s_config.ini", tto_cstring(user_name)));

	defer {
		mfree(user_name.data);
		mfree(ini_content.data);
	};

	if (ini_content) {
		ImGui::LoadIniSettingsFromMemory((char *)ini_content.data, ini_content.count);
	}

	ImFontConfig config;
	config.SizePixels  = 16;
	config.OversampleH = 4;
	config.OversampleV = 4;
	config.PixelSnapH  = true;
	io.Fonts->AddFontFromFileTTF("imgui/Roboto/RobotoMono-Medium.ttf", 16, &config);

	static const ImWchar devanagari_ranges[] = {
		0x0900,
		0x097F,
		0,
	};

	//ImVector<ImWchar>        ranges;
	//ImFontGlyphRangesBuilder builder;
	//builder.AddRanges(io.Fonts->GetGlyphRangesDefault());
	//builder.AddRanges(devanagari_ranges);
	////builder.AddRanges(io.Fonts->GetGlyphRangesJapanese());
	//builder.BuildRanges(&ranges);
	//
	//io.Fonts->AddFontFromFileTTF("c:/windows/fonts/mangal.ttf", 24, NULL, ranges.Data);
	//io.Fonts->Build();

	auto &style             = ImGui::GetStyle();
	style.ChildRounding     = 0;
	style.PopupRounding     = 0;
	style.ScrollbarRounding = 0;
	style.TabRounding       = 0;
	style.WindowRounding    = 0;
	style.WindowBorderSize  = 1;
	style.FrameBorderSize   = 0;

	style.FrameRounding = 0;
	style.GrabRounding  = 0;

	ImVec4 *colors                         = style.Colors;
	colors[ImGuiCol_Text]                  = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
	colors[ImGuiCol_TextDisabled]          = ImVec4(0.36f, 0.42f, 0.47f, 1.00f);
	colors[ImGuiCol_WindowBg]              = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	colors[ImGuiCol_PopupBg]               = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border]                = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
	colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg]               = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.12f, 0.20f, 0.28f, 1.00f);
	colors[ImGuiCol_FrameBgActive]         = ImVec4(0.09f, 0.12f, 0.14f, 1.00f);
	colors[ImGuiCol_TitleBg]               = ImVec4(0.09f, 0.12f, 0.14f, 0.65f);
	colors[ImGuiCol_TitleBgActive]         = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg]             = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
	colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.02f, 0.02f, 0.02f, 0.39f);
	colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.18f, 0.22f, 0.25f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.09f, 0.21f, 0.31f, 1.00f);
	colors[ImGuiCol_CheckMark]             = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrab]            = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
	colors[ImGuiCol_Button]                = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_ButtonHovered]         = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
	colors[ImGuiCol_ButtonActive]          = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
	colors[ImGuiCol_Header]                = ImVec4(0.20f, 0.25f, 0.29f, 0.55f);
	colors[ImGuiCol_HeaderHovered]         = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_HeaderActive]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Separator]             = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
	colors[ImGuiCol_SeparatorActive]       = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
	colors[ImGuiCol_ResizeGrip]            = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_Tab]                   = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	colors[ImGuiCol_TabHovered]            = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_TabActive]             = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_TabUnfocused]          = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	colors[ImGuiCol_PlotLines]             = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram]         = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget]        = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

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
		if (string) {
			char *data = (char *)ImGui::MemAlloc(string.count + 1);
			memcpy(data, string.data, string.count);
			data[string.count] = 0;
			mfree(string.data);
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

	if (gfx_render_backend() == Render_Backend_OPENGL) ImGui_ImplOpenGL3_Init("#version 150");

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

void ImGui::Shutdown() {
	String ini_content;
	size_t size       = 0;
	ini_content.data  = (utf8 *)ImGui::SaveIniSettingsToMemory(&size);
	ini_content.count = (s64)size;

	auto user_name = system_get_user_name();
	system_write_entire_file(tprintf("imgui/%s_config.ini", tto_cstring(user_name)), ini_content);

	mfree(user_name.data);

	ImGui_ImplOpenGL3_DestroyDeviceObjects();
	ImGui::DestroyContext();
}

bool ImGui::HandleEvent(const Event &event) {
	if ((event.type & Event_Type_KEY_UP) && event.key.symbol == Key_F1) {
		show_overlay = !show_overlay;
	}

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

void ImGui::UpdateFrame(r32 dt, r32 event_time, r32 simulate_time, r32 render_time, r32 gpu_time) {
	if (gfx_render_backend() == Render_Backend_OPENGL) ImGui_ImplOpenGL3_NewFrame();

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
		io.MouseDown[0] = mouse_pressed[0] || (system_button(Button_LEFT) == State_DOWN);
		io.MouseDown[1] = mouse_pressed[1] || (system_button(Button_RIGHT) == State_DOWN);
		io.MouseDown[2] = mouse_pressed[2] || (system_button(Button_MIDDLE) == State_DOWN);
		auto cursorp    = system_get_cursor_position();
		io.MousePos     = ImVec2((r32)cursorp.x, (r32)cursorp.y);
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

	cpu_frame_time      = 0.8f * cpu_frame_time + 0.2f * 1000.0f * dt;
	event_frame_time    = 0.8f * event_frame_time + 0.2f * event_time;
	simulate_frame_time = 0.8f * simulate_frame_time + 0.2f * simulate_time;
	render_frame_time   = 0.8f * render_frame_time + 0.2f * render_time;
	gpu_frame_time      = 0.8f * gpu_frame_time + 0.2f * gpu_time;

	// Overlay
	if (show_overlay) {
		ImVec2 work_area_pos = viewport->GetWorkPos();
		ImGui::SetNextWindowPos(ImVec2(work_area_pos.x + 10, work_area_pos.y + 10), ImGuiCond_Always, ImVec2(0, 0));
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::SetNextWindowBgAlpha(0.95f);
		if (ImGui::Begin("Information Overlay", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav)) {
			float x = ImGui::CalcTextSize("#############").x;
			float y = ImGui::GetCursorPosY();

			ImGui::TextUnformatted("Frame time");
			ImGui::TextUnformatted("Event time");
			ImGui::TextUnformatted("Simulate time");
			ImGui::TextUnformatted("Render time");
			ImGui::TextUnformatted("GPU time");

			ImGui::SetCursorPosY(y);
			ImGui::SetCursorPosX(x);
			ImGui::Text(": %.3f ms, %d fps", cpu_frame_time, (int)(1000.0f / cpu_frame_time));
			ImGui::SetCursorPosX(x);
			ImGui::Text(": %.3f ms", event_frame_time);
			ImGui::SetCursorPosX(x);
			ImGui::Text(": %.3f ms", simulate_frame_time);
			ImGui::SetCursorPosX(x);
			ImGui::Text(": %.3f ms", render_frame_time);
			ImGui::SetCursorPosX(x);
			ImGui::Text(": %.3f ms", gpu_frame_time);
		}
		ImGui::End();
	}
}

void ImGui::RenderFrame() {
	ImGui::Render();

	if (gfx_render_backend() == Render_Backend_OPENGL) ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
