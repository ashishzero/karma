#include "modules/core/thread_pool.h"

#include "scene.h"
#include "modules/gfx/renderer.h"

int karma_user_shankar() {
#ifdef INIT_THREAD_POOL
	if (!async_initialize(2, mega_bytes(32), context.allocator)) {
		system_fatal_error("Thread could not be created");
	}
#endif

	scene_prepare(Scene_Run_Method_CLIENT, Render_Backend_DIRECTX11, System_Window_Show_NORMAL);

	Scene* scene = scene_create();

	scene_load_resources();

	bool loaded = scene_load_level(scene, "rokkenjima");
	assert(loaded);

	scene_loop(scene);

	while (scene_running()) {
		scene_frame_begin(scene);

		scene_frame_simulate(scene);

		scene_begin_drawing();

		scene_render(scene);

		const auto VIEW_HEIGHT = 10.0f;
		const auto VIEW_WIDTH = 16.0f/9.0f * VIEW_HEIGHT;

		auto view = orthographic_view(0.0f, VIEW_WIDTH, VIEW_HEIGHT, 0.0f);

		im2d_begin(view);
		im2d_rect(vec2(5), vec2(4), vec4(1));
		im2d_end();

		scene_end_drawing();

		scene_frame_end(scene);
	}

	scene_destroy(scene);

	scene_shutdown();

	return 0;
}
