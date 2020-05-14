#include "karma.h"        // shared
#include "systems.h"      // windows
#include "gfx_renderer.h" // rendering
#include "length_string.h"
#include "lin_maths.h"
#include "imgui/imgui.h"
//#include "physics.h"

#include<time.h>
#include<stdlib.h>
#define NUM_RIGID_BODIES 20
#define PI 3.14

int initial = 0;
int xy = 2000;

typedef struct {
	float width;
	float height;
	float mass;
	float momentofInertia;
} BoxShape;


//=========two dimensional rigid body=========#

typedef struct {
	Vec2 position;
	Vec2 velocity;
	Vec2 linear_velocity;
	Vec2 linear_acceleration;
	float angular_velocity;
	float angle;
	Vec2 force;
	float torque;
	BoxShape shape;
	Mm_Rect min_max_points;
	bool collided;
}RigidBody;

RigidBody rigidBodies[NUM_RIGID_BODIES];

int random_number_generator(int n) {

	//srand(int(time(NULL)));
	return (rand() % n + 1);
}

int neg_random_number_generator(int n) {

	//srand(int(time(NULL)));
	return (rand() % n - n + 1);
}


void CalculateBoxInertia(BoxShape *boxShape) {
	float m = boxShape->mass;
	float h = boxShape->height;
	float w = boxShape->width;
	boxShape->momentofInertia = m * (w*w + h * h) / 2;

}

void ComputeForceandTorque(RigidBody *rigidBody) {

	Vec2 f;
	//f = { r32(2000), r32(2000) };
	f = { r32(neg_random_number_generator(2 + xy)), r32(neg_random_number_generator(1 + xy)) };
	//if(initial==1)
	//	 f = { r32(1500), r32(2000) };
	//else
	//	 f = { r32(1500), r32(-2000) };

	rigidBody->force = f;
	Vec2 r = { rigidBody->shape.width / 2, rigidBody->shape.height / 2 };
	rigidBody->torque = r.x*f.y - r.y*f.x;
	xy += 1;
	initial += 1;

}

void InitializeRigidBodies() {
	int x = 0;
	for (int i = 0; i < NUM_RIGID_BODIES; i++) {

		RigidBody *rigidBody = &rigidBodies[i];
		ComputeForceandTorque(rigidBody);
		rigidBody->position = { r32(random_number_generator(350 + x)), r32(random_number_generator(650 + x)) };
		//if (i == 0)
		//	rigidBody->position = { 500,100 };
		//if (i==1)
		//	rigidBody->position = { 500,500 };
		rigidBody->angle = float(random_number_generator(360) / 360.0f * PI * 2);
		rigidBody->linear_velocity = { 0, 0 };
		rigidBody->angular_velocity = 0;
		rigidBody->collided = false;

		BoxShape shape;
		shape.mass = float(random_number_generator(5000));

		printf("%.2f\t", shape.mass);
		shape.width = 20 * float(random_number_generator(2));
		shape.height = 20 * float(random_number_generator(4));
		CalculateBoxInertia(&shape);
		rigidBody->shape = shape;

		//rigidBody->linear_acceleration = { rigidBody->force.x / rigidBody->shape.mass, rigidBody->force.y / rigidBody->shape.mass };
		rigidBody->linear_acceleration.x = rigidBody->force.x / rigidBody->shape.mass;
		rigidBody->linear_acceleration.y = rigidBody->force.y / rigidBody->shape.mass;

		rigidBody->min_max_points.min = { rigidBody->position.x, rigidBody->position.y };
		rigidBody->min_max_points.max = { rigidBody->position.x + rigidBody->shape.width, rigidBody->position.y + rigidBody->shape.height };


		x += 5;
	}
}

void update_min_max(RigidBody *rigidBody) {
	for (int i = 0; i < NUM_RIGID_BODIES; i++) {
		RigidBody *rigidBody = &rigidBodies[i];
		rigidBody->min_max_points.min = { rigidBody->position.x, rigidBody->position.y };
		rigidBody->min_max_points.max = { rigidBody->position.x + rigidBody->shape.width, rigidBody->position.y + rigidBody->shape.height };

	}

}

bool AABB_collision_check(RigidBody *a, RigidBody *b)
{
	float d1x = b->min_max_points.min.x - a->min_max_points.max.x;
	//float d1x = b->min.x - a->max.x;
	float d1y = b->min_max_points.min.y - a->min_max_points.max.y;
	//float d1y = b->min.y - a->max.y;

	float d2x = a->min_max_points.min.x - b->min_max_points.max.x;
	//float d2x = a->min.x - b->max.x;
	float d2y = a->min_max_points.min.y - b->min_max_points.max.y;
	//float d2y = a->min.y - b->max.y;

	if (d1x > 0.0f || d1y > 0.0f) {
		system_trace("Not Collided\n");
		return false;
	}
	if (d2x > 0.0f || d2y > 0.0f) {
		system_trace("Not Collided\n");
		return false;
	}
	system_trace("Collided\n");
	return true;

}



void PrintRigidBodies() {
	for (int i = 0; i < NUM_RIGID_BODIES; i++) {
		RigidBody *rigidBody = &rigidBodies[i];
		gfx2d_rect(vec2(rigidBody->position.x, rigidBody->position.y), vec2(rigidBody->shape.width, rigidBody->shape.height));
	}
}



int system_main() { // Entry point
	Handle platform = system_create_window(u8"Karma", 1280, 720, System_Window_Show_NORMAL);
	gfx_create_context(platform, Render_Backend_OPENGL, 1, 4);
	ImGui::Initialize();

	Handle quad_shader = gfx_create_shader("./data/quad.fx");

	Render_Region region = {};

	Event event;
	bool  running = true;

	const float dt = 1.0f / 60.0f;

	float t = 0.0f;
	float accumulator = 0.0f;
	float frame_time = 0.0f;

	u64 frequency = system_get_frequency();
	u64 counter = system_get_counter();

	r32 window_w = 0, window_h = 0;

	Vec2 position = vec2(250, 250);
	Vec2 dimension = vec2(200, 200);
	Color4 box_color = vec4(0.88f, 0.8f, 0.02f);

	float x = 500, y = 200;


	InitializeRigidBodies();

	while (running) {
		auto new_counter = system_get_counter();
		auto counts = new_counter - counter;
		counter = new_counter;

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
				region.scissor = region.viewport;
				window_w = (r32)w;
				window_h = (r32)h;
				continue;
			}

			if ((event.type & Event_Type_KEY_UP) && event.key.symbol == Key_ESCAPE) {
				system_request_quit();
				break;
			}
		}

		while (accumulator >= dt) {
			// simulate here
			//gfx2d_rect(vec2(x, y), vec2(250, 250));
			for (int i = 0; i < NUM_RIGID_BODIES; i++) {
				RigidBody *rigidBody = &rigidBodies[i];

				if (rigidBody->collided == false) {
					rigidBody->linear_velocity.x += rigidBody->linear_acceleration.x * dt;
					rigidBody->linear_velocity.y += rigidBody->linear_acceleration.y * dt;
				}

				for (int j = i + 1; j < NUM_RIGID_BODIES  && j !=i; j++) {
					RigidBody *rigidBody2 = &rigidBodies[j];
					if (AABB_collision_check(rigidBody, rigidBody2)) {

						Vec2 prev_velocity_rigidBody = rigidBody->linear_velocity;
						Vec2 prev_velocity_rigidBody2 = rigidBody2->linear_velocity;

						rigidBody2->linear_velocity.x = ((2 * rigidBody->shape.mass * prev_velocity_rigidBody.x) / (rigidBody->shape.mass + rigidBody2->shape.mass) + ((rigidBody2->shape.mass - rigidBody->shape.mass) / (rigidBody2->shape.mass + rigidBody->shape.mass)) * prev_velocity_rigidBody2.x);
						rigidBody2->linear_velocity.y = ((2 * rigidBody->shape.mass * prev_velocity_rigidBody.y) / (rigidBody->shape.mass + rigidBody2->shape.mass) + ((rigidBody2->shape.mass - rigidBody->shape.mass) / (rigidBody2->shape.mass + rigidBody->shape.mass)) * prev_velocity_rigidBody2.y);

						rigidBody->linear_velocity.x = (((rigidBody->shape.mass - rigidBody2->shape.mass) / (rigidBody->shape.mass + rigidBody2->shape.mass)) * prev_velocity_rigidBody.x + (2 * rigidBody2->shape.mass * prev_velocity_rigidBody2.x) / (rigidBody->shape.mass + rigidBody2->shape.mass));
						rigidBody->linear_velocity.y = (((rigidBody->shape.mass - rigidBody2->shape.mass) / (rigidBody->shape.mass + rigidBody2->shape.mass)) * prev_velocity_rigidBody.y + (2 * rigidBody2->shape.mass * prev_velocity_rigidBody2.y) / (rigidBody->shape.mass + rigidBody2->shape.mass));

						if (rigidBody->linear_velocity.x > 80.0f)
							rigidBody->linear_velocity.x = 80.0f;
						if (rigidBody->linear_velocity.x < -80.0f)
							rigidBody->linear_velocity.x = -80.0f;
						if (rigidBody->linear_velocity.y > 80.0f)
							rigidBody->linear_velocity.y = 80.0f;
						if (rigidBody->linear_velocity.y < -80.0f)
							rigidBody->linear_velocity.y = -80.0f;

						if (rigidBody2->linear_velocity.x > 80.0f)
							rigidBody2->linear_velocity.x = 80.0f;
						if (rigidBody2->linear_velocity.x < -80.0f)
							rigidBody2->linear_velocity.x = -80.0f;
						if (rigidBody2->linear_velocity.y > 80.0f)
							rigidBody2->linear_velocity.y = 80.0f;
						if (rigidBody2->linear_velocity.y < -80.0f)
							rigidBody2->linear_velocity.y = -80.0f;

						rigidBody->linear_acceleration.x = rigidBody->linear_velocity.x / dt;
						rigidBody->linear_acceleration.y = rigidBody->linear_velocity.y / dt;


						rigidBody->force.x = rigidBody->linear_acceleration.x * rigidBody->shape.mass;
						rigidBody->force.y = rigidBody->linear_acceleration.y * rigidBody->shape.mass;



						rigidBody2->linear_acceleration.x = rigidBody2->linear_velocity.x / dt;
						rigidBody2->linear_acceleration.y = rigidBody2->linear_velocity.y / dt;

						rigidBody2->force.x = rigidBody2->linear_acceleration.x * rigidBody2->shape.mass;
						rigidBody2->force.y = rigidBody2->linear_acceleration.y * rigidBody2->shape.mass;

						rigidBody2->position.x += rigidBody2->linear_velocity.x * dt;
						rigidBody2->position.y += rigidBody2->linear_velocity.y * dt;
						update_min_max(rigidBody2);

						rigidBody2->collided = true;



					}

				}

				if (rigidBody->collided == false) {
					rigidBody->position.x += rigidBody->linear_velocity.x * dt;
					rigidBody->position.y += rigidBody->linear_velocity.y * dt;
				}
				float angularAcceleration = rigidBody->torque / rigidBody->shape.momentofInertia;
				rigidBody->angular_velocity += angularAcceleration * dt;
				rigidBody->angle += rigidBody->angular_velocity * dt;
				if (rigidBody->linear_velocity.x > 80.0f)
					rigidBody->linear_velocity.x = 80.0f;
				if (rigidBody->linear_velocity.x < -80.0f)
					rigidBody->linear_velocity.x = -80.0f;
				if (rigidBody->linear_velocity.y > 80.0f)
					rigidBody->linear_velocity.y = 80.0f;
				if (rigidBody->linear_velocity.y < -80.0f)
					rigidBody->linear_velocity.y = -80.0f;

				if (rigidBody->position.x <= 0)
				{
					rigidBody->force.x *= -1;
					rigidBody->linear_acceleration.x *= -1;
					rigidBody->linear_velocity.x *= -1;
				}
				if (rigidBody->position.x >= window_w)
				{
					rigidBody->force.x *= -1;
					rigidBody->linear_acceleration.x *= -1;
					rigidBody->linear_velocity.x *= -1;
				}
				if (rigidBody->position.y <= 0)
				{
					rigidBody->force.y *= -1;
					rigidBody->linear_acceleration.y *= -1;
					rigidBody->linear_velocity.y *= -1;
				}
				if (rigidBody->position.y >= window_h)
				{
					rigidBody->force.y *= -1;
					rigidBody->linear_acceleration.y *= -1;
					rigidBody->linear_velocity.y *= -1;
				}
				update_min_max(rigidBody);

			}

			for (int i = 0; i < NUM_RIGID_BODIES; i++) {
				RigidBody *rigidBody = &rigidBodies[i];
				rigidBody->collided = false;

			}

			x += 1;
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
		//gfx2d_rect(position, dimension);
		
		PrintRigidBodies();
		gfx2d_end();

		// ImGui Rendering here
		ImGui::Begin("Edit");
		static int index = 0;
		RigidBody &rb = rigidBodies[index];
		ImGui::DragInt("Index", &index);
		ImGui::DragFloat3("Position", rb.position.m);
		ImGui::DragFloat2("Velocity", rb.velocity.m);
		ImGui::DragFloat2("Linear Velocity", rb.linear_velocity.m);
		ImGui::DragFloat2("Linear Acceleration", rb.linear_acceleration.m);
		ImGui::DragFloat("Angular Velocity", &rb.angular_velocity);
		ImGui::DragFloat("Angle", &rb.angle);
		ImGui::DragFloat2("Force", rb.force.m);
		ImGui::DragFloat("Torque", &rb.torque);
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