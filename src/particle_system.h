#pragma once
#include "random.h"
#include "gfx_renderer.h"

struct Particle_Emitter_Property {
	enum Kind {
		POINT,
		LINE,
		ELLIPSE,
		TRIANGLE,
		RECT,
	};
	/*
	 * If kind is POINT, neither of a, b, or c is used
	 * if kind is LINE, the a is used offset from center
	 * if kind is ELLIPSE, a.x and a.y are used as width and height of the ellipse
	 * if kind is TRIANGLE, a, b, c are used as 3 corner of the triangle around the origin
	 * if kind is RECT, then a is used as width and height of the rectangle
	*/
	Kind                 kind;
	Distribution_Control control;
	Vec2                 a, b, c; // TODO: Perhaps we can remove TRIANGLE, that way we only have to store 'a'

	Random_Distribution density;
	r32                 volume_factor;

	Random_Distribution scale_a;
	Random_Distribution scale_b;

	Random_Distribution spin;
	Random_Distribution rotation;

	Random_Distribution initial_velocity_x;
	Random_Distribution initial_velocity_y;
	Random_Distribution force_x;
	Random_Distribution force_y;

	Random_Distribution drag;

	Color4 color_a;
	Color4 color_b;
	r32    opacity;
	r32    intensity; // TODO: Make this per particle!

	Random_Distribution life_span;
	s32                 emission_rate; // in particles per milli secs
	r32                 fade_in;       // in secs
	r32                 fade_out;      // in secs
};

struct Particle {
	r32    density;
	Vec2   position;
	r32    scale_a;
	r32    scale_b;
	r32    rotation;
	Vec2   velocity;
	r32    drag;
	r32    spin;
	Color4 color_a;
	Color4 color_b;
	r32    life_span; // in secs
	r32    life;      // in secs
	Vec2   external_force;
};

struct Particle_Emitter {
	Texture2d_Handle          *texture;
	Mm_Rect                   texture_rect;
	Vec2                      position;
	Particle_Emitter_Property properties; // TODO: perhaps we should we same emitter for multiple particle system
	Particle *                particles;
	int                       particles_count;
	int                       emit_count;
	r32                       time_elapsed; // in secs
};

Particle *particle_emitter_find_dead(Particle_Emitter *emitter) {
	for (int i = 0; i < emitter->particles_count; ++i) {
		auto particle = emitter->particles + i;
		if (particle->life > particle->life_span) return particle;
	}

	// If we don't find a dead particle, we just return the last particle
	return emitter->particles + emitter->emit_count;
}

void particle_create_new(Particle_Emitter_Property &properties, Vec2 center, Particle *particle) {
	switch (properties.kind) {
		case Particle_Emitter_Property::POINT: {
			particle->position = center;
		} break;

		case Particle_Emitter_Property::LINE: {
			auto value         = random_get_zero_to_one(properties.control);
			particle->position = value * (properties.a) + center;
		} break;

		case Particle_Emitter_Property::ELLIPSE: {
			auto value_a         = random_get_zero_to_one(properties.control);
			auto value_b         = random_get_zero_to_one(properties.control) * MATH_PI * 2;
			particle->position.x = sqrtf(value_a) * properties.a.x * cosf(value_b) + center.x;
			particle->position.y = sqrtf(value_a) * properties.a.y * sinf(value_b) + center.y;
		} break;

		case Particle_Emitter_Property::TRIANGLE: {
			// https://www.cs.princeton.edu/~funk/tog02.pdf, Section 4.2
			auto value_a       = sqrtf(random_get_zero_to_one(properties.control));
			auto value_b       = random_get_zero_to_one(properties.control);
			particle->position = (1 - value_a) * properties.a + (value_a * (1 - value_b)) * properties.b + (value_a * value_b) * properties.c;
			particle->position += center;
		} break;

		case Particle_Emitter_Property::RECT: {
			auto value_x         = random_get_zero_to_one(properties.control);
			auto value_y         = random_get_zero_to_one(properties.control);
			particle->position.x = value_x * properties.a.x + center.x;
			particle->position.y = value_y * properties.a.y + center.y;
		} break;

			invalid_default_case();
	}

	particle->density        = random_get_range(properties.density);
	particle->scale_a        = random_get_range(properties.scale_a);
	particle->scale_b        = random_get_range(properties.scale_b);
	particle->rotation       = random_get_range(properties.rotation);
	particle->velocity.x     = random_get_range(properties.initial_velocity_x);
	particle->velocity.y     = random_get_range(properties.initial_velocity_y);
	particle->drag           = random_get_range(properties.drag);
	particle->spin           = random_get_range(properties.spin);
	particle->color_a        = properties.color_a;
	particle->color_b        = properties.color_b;
	particle->life_span      = random_get_range(properties.life_span);
	particle->life           = 0;
	particle->external_force = vec2(0);

	assert(particle->life_span != 0);
}

void particle_emitter_update_single_particle(Particle_Emitter *emitter, Particle *particle, r32 dt) {
	particle->life += dt;
	r32 t = particle->life / particle->life_span;
	if (t <= 1) {
		// TODO: Perhaps we should store the inverse of density and volume, instead of just density and volumne
		r32  scale  = lerp(particle->scale_a, particle->scale_b, t) + 0.001f; // TODO: Should we do this to prevent division by 0?
		r32  volume = scale * emitter->properties.volume_factor;
		r32  mass   = (particle->density * volume);
		r32  imass  = 1.0f / mass;
		Vec2 force;
		force.x = random_get_range(emitter->properties.force_x);
		force.y = random_get_range(emitter->properties.force_y);
		force += particle->external_force;
		Vec2 acceleration = force * imass;
		particle->velocity += dt * acceleration;
		particle->velocity *= powf(0.5f, particle->drag * dt); // TODO: powf is expensive!
		particle->position += dt * particle->velocity;
		particle->rotation += dt * particle->spin;
	}
}

void particle_emitter_update_particles(Particle_Emitter *emitter, r32 dt) {
	for (int particle_index = 0; particle_index < emitter->emit_count; ++particle_index) {
		particle_emitter_update_single_particle(emitter, emitter->particles + particle_index, dt);
	}

	emitter->time_elapsed += dt;
	while (emitter->time_elapsed >= 0.001f) {
		for (int i = 0; i < emitter->properties.emission_rate; ++i) {
			// TODO: Should we replace the old particle, or perhaps we should not limit the particles count?
			auto new_particle = particle_emitter_find_dead(emitter);
			if (new_particle) {
				particle_create_new(emitter->properties, emitter->position, new_particle);
			} else {
				break;
			}
		}
		emitter->time_elapsed -= 0.001f;
	}
}

Particle_Emitter_Property particle_emitter_default_property() {
	Particle_Emitter_Property props;
	props.kind               = Particle_Emitter_Property::LINE;
	props.control            = Distribution_Control_UNIFORM;
	props.a                  = vec2(260, 0);
	props.b                  = vec2(0);
	props.c                  = vec2(0);
	props.density            = random_distribution(Distribution_Control_UNIFORM, 1.1f, 1.2f);
	props.volume_factor      = 0.05f;
	props.scale_a            = random_distribution(Distribution_Control_UNIFORM, 50.0f, 50.0f);
	props.scale_b            = random_distribution(Distribution_Control_UNIFORM, 0.0f, 0.0f);
	props.spin               = random_distribution(Distribution_Control_UNIFORM, 0.1f, 0.5f);
	props.rotation           = random_distribution(Distribution_Control_UNIFORM, 0.0f, 0.0f);
	props.initial_velocity_x = random_distribution(Distribution_Control_UNIFORM, -10, 10);
	props.initial_velocity_y = random_distribution(Distribution_Control_UNIFORM, 65, 120);
	props.force_x            = random_distribution(Distribution_Control_UNIFORM, 0, 0);
	props.force_y            = random_distribution(Distribution_Control_UNIFORM, 90, 180);
	props.drag               = random_distribution(Distribution_Control_UNIFORM, 0.0f, 0.0f);
	props.color_a            = vec4(0.2f, 1.0f, 1.0f, 0.0f);
	props.color_b            = vec4(0.0f, 1.0f, 1.0f, 1.0f);
	props.opacity            = 1;
	props.intensity          = 4.5f;
	props.life_span          = random_distribution(Distribution_Control_UNIFORM, 1.5f, 2.0f);
	props.emission_rate      = 50;
	props.fade_in            = 0.06f;
	props.fade_out           = 0.3f;

	return props;
}

Particle_Emitter particle_emitter_create(Texture2d_Handle *texture, Mm_Rect tex_rect, int max_partices, int emit_count) {
	Particle_Emitter emitter;
	emitter.texture         = texture;
	emitter.texture_rect    = tex_rect;
	emitter.position        = vec2(250, 250);
	emitter.properties      = particle_emitter_default_property();
	emitter.particles_count = max_partices;
	emitter.emit_count      = emit_count;
	emitter.particles       = (Particle *)mallocate(max_partices * sizeof(Particle));
	emitter.time_elapsed    = 0;

	// TODO: Cleanup
	memset(emitter.particles, 0, sizeof(Particle) * emitter.particles_count);

	return emitter;
}
