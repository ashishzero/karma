#pragma once
#include "pcg/pcg_basic.h"
#include "lin_maths.h"

struct Random_Series {
	pcg_state_setseq_64 state;
};

inline Random_Series random_init(u64 state, u64 seq) {
	Random_Series rng;
	pcg32_srandom_r(&rng.state, state, seq);
	return rng;
}

inline u32 random_get(Random_Series *rng) {
	return pcg32_random_r(&rng->state);
}

inline u32 random_get_bound(Random_Series *rng, u32 bound) {
	return pcg32_boundedrand_r(&rng->state, bound);
}

inline u32 random_get_range(Random_Series *rng, u32 min, u32 max) {
	return min + pcg32_boundedrand_r(&rng->state, max - min);
}

inline r32 random_get_zero_to_one(Random_Series *rng) {
	return (float)ldexpf((float)pcg32_random_r(&rng->state), -32);
}

inline r32 random_get_bound(Random_Series *rng, float bound) {
	return random_get_zero_to_one(rng) * bound;
}

inline r32 random_r32_range(Random_Series *rng, float min, float max) {
	return min + random_get_zero_to_one(rng) * (max - min);
}

inline Vec2 random_vec2_zero_to_one(Random_Series *rng) {
	return vec2(random_get_zero_to_one(rng), random_get_zero_to_one(rng));
}

inline Vec3 random_vec3_zero_to_one(Random_Series *rng) {
	return vec3(random_get_zero_to_one(rng), random_get_zero_to_one(rng), random_get_zero_to_one(rng));
}

inline Vec4 random_vec4_zero_to_one(Random_Series *rng) {
	return vec4(random_get_zero_to_one(rng), random_get_zero_to_one(rng), random_get_zero_to_one(rng), random_get_zero_to_one(rng));
}

inline Color3 random_color_hsv(Random_Series *rng, float s, float v) {
	Color3 color;
	color.x = random_get_zero_to_one(rng);
	color.y = s;
	color.z = v;
	return color;
}

inline Color3 random_color3(Random_Series *rng, float s, float v) {
	return hsv_to_rgb(random_color_hsv(rng, s, v));
}

inline Color4 random_color4(Random_Series *rng, float s, float v) {
	return vec4(hsv_to_rgb(random_color_hsv(rng, s, v)), random_get_zero_to_one(rng));
}

//
//
//

enum Distribution_Control {
	Distribution_Control_UNIFORM,
	Distribution_Control_SQUARED,
	Distribution_Control_CUBED,
	Distribution_Control_FOURTH,
	Distribution_Control_ONE_MINUS_SQUARED,
	Distribution_Control_ONE_MINUS_CUBED,
	Distribution_Control_ONE_MINUS_FOURTH,
	Distribution_Control_TWO_DICE,
	Distribution_Control_THREE_DICE,
	Distribution_Control_FOUR_DICE,
	Distribution_Control_TWO_DICE_SQUARED,
	Distribution_Control_THREE_DICE_SQUARED,
	Distribution_Control_FOUR_DICE_SQUARED,
	Distribution_Control_EXTREMES,
};

struct Random_Distribution {
	Distribution_Control control;
	r32                  min, max;
};

inline Random_Distribution random_distribution(Distribution_Control control, r32 min, r32 max) {
	Random_Distribution result;
	result.control = control;
	result.min     = min;
	result.max     = max;
	return result;
}

inline r32 random_get_zero_to_one(Random_Series *rng, Distribution_Control control) {
	r32 x = random_get_zero_to_one(rng);

	switch (control) {
		case Distribution_Control_UNIFORM: break;
		case Distribution_Control_SQUARED: {
			x *= x;
		} break;
		case Distribution_Control_CUBED: {
			x = x * x * x;
		} break;
		case Distribution_Control_FOURTH: {
			x *= x;
			x *= x;
		} break;
		case Distribution_Control_ONE_MINUS_SQUARED: {
			x = 1 - x * x;
		} break;
		case Distribution_Control_ONE_MINUS_CUBED: {
			x = 1 - x * x * x;
		} break;
		case Distribution_Control_ONE_MINUS_FOURTH: {
			x = 1 - x * x * x * x;
		} break;
		case Distribution_Control_TWO_DICE: {
			r32 x0 = random_get_zero_to_one(rng);
			x      = (x + x0) * 0.5f;
		} break;
		case Distribution_Control_THREE_DICE: {
			r32 x0 = random_get_zero_to_one(rng);
			r32 x1 = random_get_zero_to_one(rng);
			x      = (x + x0 + x1) * (1.0f / 3.0f);
		} break;
		case Distribution_Control_FOUR_DICE: {
			r32 x0 = random_get_zero_to_one(rng);
			r32 x1 = random_get_zero_to_one(rng);
			r32 x2 = random_get_zero_to_one(rng);
			x      = (x + x0 + x1 + x2) * 0.25f;
		} break;
		case Distribution_Control_TWO_DICE_SQUARED: {
			r32 x0 = random_get_zero_to_one(rng);
			x      = (x + x0) * 0.5f;
			x *= x;
		} break;
		case Distribution_Control_THREE_DICE_SQUARED: {
			r32 x0 = random_get_zero_to_one(rng);
			r32 x1 = random_get_zero_to_one(rng);
			x      = (x + x0 + x1) * (1.0f / 3.0f);
			x      = x * x * x;
		} break;
		case Distribution_Control_FOUR_DICE_SQUARED: {
			r32 x0 = random_get_zero_to_one(rng);
			r32 x1 = random_get_zero_to_one(rng);
			r32 x2 = random_get_zero_to_one(rng);
			x      = (x + x0 + x1 + x2) * 0.25f;
			x *= x;
			x *= x;
		} break;
		case Distribution_Control_EXTREMES: {
#if 0		// This is much extreme, but powf is expensive and maybe we'll never need this extreme
			x = x - 0.5f;
			r32 sign = sgn(x);
			x = sign * sign * sign * powf(3.0f * fabsf(x), 1.0f / 3.0f);
			x = (x + 1) * 0.5f;
#else
			x = x * x - 1;
			if (random_get(rng) & 1) x = -x;
			x = (x + 1) * 0.5f;
#endif
		} break;

			invalid_default_case();
	}

	return x;
}

inline r32 random_get_range(Random_Series *rng, Distribution_Control control, r32 min, r32 max) {
	return random_get_zero_to_one(rng, control) * (max - min) + min;
}

inline r32 random_get_range(Random_Series *rng, Random_Distribution dist) {
	return random_get_range(rng, dist.control, dist.min, dist.max);
}
