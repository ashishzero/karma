#pragma once
#include "pcg/pcg_basic.h"
#include "lin_maths.h"

typedef pcg_state_setseq_64 Random_Series;

extern "C" pcg32_random_t pcg32_global;

inline void random_init_global(u64 state, u64 seq) {
	pcg32_srandom(state, seq);
}

inline Random_Series random_init(u64 state, u64 seq) {
	Random_Series rng;
	pcg32_srandom_r(&rng, state, seq);
	return rng;
}

inline u32 random_get(Random_Series *rng = &pcg32_global) {
	return pcg32_random_r(rng);
}

inline u32 random_get_bound(u32 bound, Random_Series *rng = &pcg32_global) {
	return pcg32_boundedrand_r(rng, bound);
}

inline u32 random_get_range(u32 min, u32 max, Random_Series *rng = &pcg32_global) {
	return min + pcg32_boundedrand_r(rng, max - min);
}

inline r32 random_get_zero_to_one(Random_Series *rng = &pcg32_global) {
	return (float)ldexpf((float)pcg32_random_r(rng), -32);
}

inline r32 random_get_bound(float bound, Random_Series *rng = &pcg32_global) {
	return random_get_zero_to_one(rng) * bound;
}

inline r32 random_r32_range(float min, float max, Random_Series *rng = &pcg32_global) {
	return min + random_get_zero_to_one(rng) * (max - min);
}

inline Vec2 random_vec2_zero_to_one(Random_Series *rng = &pcg32_global) {
	return vec2(random_get_zero_to_one(rng), random_get_zero_to_one(rng));
}

inline Vec3 random_vec3_zero_to_one(Random_Series *rng = &pcg32_global) {
	return vec3(random_get_zero_to_one(rng), random_get_zero_to_one(rng), random_get_zero_to_one(rng));
}

inline Vec4 random_vec4_zero_to_one(Random_Series *rng = &pcg32_global) {
	return vec4(random_get_zero_to_one(rng), random_get_zero_to_one(rng), random_get_zero_to_one(rng), random_get_zero_to_one(rng));
}

inline Color3 random_color_hsv(float s, float v, Random_Series *rng = &pcg32_global) {
	Color3 color;
	color.x = random_get_zero_to_one(rng);
	color.y = s;
	color.z = v;
	return color;
}

inline Color3 random_color3(float s, float v, Random_Series *rng = &pcg32_global) {
	return hsv_to_rgb(random_color_hsv(s, v, rng));
}

inline Color4 random_color4(float s, float v, Random_Series *rng = &pcg32_global) {
	return vec4(hsv_to_rgb(random_color_hsv(s, v, rng)), random_get_zero_to_one(rng));
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

inline r32 random_get_zero_to_one(Distribution_Control control, Random_Series *rng = &pcg32_global) {
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

inline r32 random_get_range(Distribution_Control control, r32 min, r32 max, Random_Series *rng = &pcg32_global) {
	return random_get_zero_to_one(control, rng) * (max - min) + min;
}

inline r32 random_get_range(Random_Distribution dist, Random_Series *rng = &pcg32_global) {
	return random_get_range(dist.control, dist.min, dist.max, rng);
}
