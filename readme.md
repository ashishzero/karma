# कर्म (karma)

![Logo](res/logo/karma.png)

## Directory Structure
```bash
├── bin					  # Contains intermediate build files
├── res					  # Contains resources
│   ├── fonts			  # Fonts used in games
│   ├── logo			  # Branding
│   ├── misc			  # Miscellaneous files
│   ├── shaders			  # Shaders (they are written in our format)
│   ├── ucd				  # Unicode Database (https://www.unicode.org/ucd/)
├── run_tree			  # This is the directory where the executable is ran from
│   ├── data			  # Data required for executable
│   ├── imgui			  # Contain files used by IMGUI
│   ├── *				  # Debugging files and executables
├── src					  # Karma source code
│   ├── .generated		  # Generated files by Reflection
│		├── readme.txt	  # Instructions how to generate reflection of code and use it
│   ├── glad			  # External library (https://github.com/Dav1dde/glad)
│   ├── imgui			  # External library (https://github.com/ocornut/imgui)
│   ├── pcg				  # External library (https://github.com/imneme/pcg-c-basic)
│   ├── opengl			  # OpenGL specific code
│   ├── windows			  # Windows specific code
│   ├── prebuild.bat	  # Windows Batch file for Reflection (more at .generated/readme.txt)
│   ├── *.cpp, *.h		  # Source and header CPP files
├── tools				  # Tools that are used by Karma
│   ├── reflection		  # Generated files by Reflection
│   ├── ucd				  # Unicode Database To Code Generator
├── *.vcxproj*			  # Visual Studio Project Files
├── readme.md
├── todo.txt
├── .gitattributes
└── .gitignore
```

## Build Process (Windows)
- Check `src/build.cpp` file for build configuration
- The source files that are included in `src/build.cpp` are excluded from the build process
- `src/prebuild.bat` is run before the build process to generate reflection
- The executable is ran from `run_tree` directory

## Sample Program

```cpp
#include "karma.h"        // shared
#include "systems.h"      // windows
#include "gfx_renderer.h" // rendering
#include "length_string.h"
#include "lin_maths.h"
#include "imgui/imgui.h"

int system_main() { // Entry point
	Handle platform = system_create_window(u8"Karma", 1280, 720, System_Window_Show_NORMAL);
	gfx_create_context(platform, Render_Backend_OPENGL, 1, 4);
	ImGui::Initialize();

	Handle quad_shader = gfx_create_shader("./data/quad.fx");

	Render_Region region = {};

	Event event;
	bool  running = true;

	const float dt = 1.0f / 60.0f;

	float t           = 0.0f;
	float accumulator = 0.0f;
	float frame_time  = 0.0f;

	u64 frequency = system_get_frequency();
	u64 counter   = system_get_counter();

	r32 window_w = 0, window_h = 0;

	Vec2 position = vec2(250, 250);
	Vec2 dimension = vec2(200, 200);
	Color4 box_color = vec4(0.88f, 0.8f, 0.02f);

	while (running) {
		auto new_counter = system_get_counter();
		auto counts      = new_counter - counter;
		counter          = new_counter;

		frame_time = ((1000000.0f * (r32)counts) / (r32)frequency) / 1000000.0f;
		accumulator += frame_time;
		accumulator = min_value(accumulator, 0.2f);

		while (system_poll_events(&event)) {
			if (ImGui::HandleEvent(event)) continue;

			if (event.type & Event_Type_EXIT) {
				running = false;
				break;
			}

			if (event.type & Event_Type_WINDOW_RESIZE) {
				s32 w = event.window.dimension.x;
				s32 h = event.window.dimension.y;
				gfx_resize_framebuffer(w, h);
				region.viewport = { 0, 0, w, h };
				region.scissor  = region.viewport;
				window_w        = (r32)w;
				window_h        = (r32)h;
				continue;
			}

			if ((event.type & Event_Type_KEY_UP) && event.key.symbol == Key_ESCAPE) {
				system_request_quit();
				break;
			}
		}

		while (accumulator >= dt) {
			// simulate here

			t += dt;
			accumulator -= dt;
		}

		float alpha = accumulator / dt;

		ImGui::UpdateFrame(frame_time);

		gfx_frame(0, region, Clear_Flag_ALL, hex_to_color4(colorh(0x00, 0x45, 0x4f)));
		auto view = orthographic_view(0, window_w, window_h, 0);

		gfx2d_begin(vec2(0), 1, view, quad_shader);
		gfx2d_no_texture();
		gfx2d_color(box_color);
		gfx2d_rect(position, dimension);
		gfx2d_end();

		// ImGui Rendering here
		ImGui::Begin("Edit");
		ImGui::DragFloat2("Position", position.m);
		ImGui::DragFloat2("Dimension", dimension.m);
		ImGui::ColorEdit4("Color", box_color.m);
		ImGui::End();

		ImGui::RenderFrame();

		gfx_present();

		reset_temporary_memory();
	}

	ImGui::Shutdown();
	gfx_destroy_context();

	return 0;
}
```