@ vec2 tex_coord;

::Vertex

const vec2 vertices[] = {
	vec2(-1, -1), vec2(-1, 1), vec2(1, 1), vec2(-1, -1), vec2(1, 1), vec2(1, -1)
};
const vec2 tex_coords[] = {
	vec2(0, 0), vec2(0, 1), vec2(1, 1), vec2(0, 0), vec2(1, 1), vec2(1, 0)
};

void main() {
	tex_coord   = tex_coords[gl_VertexID];
	gl_Position = vec4(vertices[gl_VertexID], 0, 1);
}

::Fragment

layout (location = 0) out vec4 pixel_color;

layout (binding = 0) uniform sampler2D tex;

//
// ACES Tone mapping curve
// https://64.github.io/tonemapping/
//

const mat3 aces_input_mat = {
	{ 0.59719, 0.07600, 0.02840 },
	{ 0.35458, 0.90834, 0.13383 }, 
	{ 0.04823, 0.01566, 0.83777 }
};

const mat3 aces_output_mat = {
	{ 1.60475, -0.10208, -0.00327},
	{ -0.53108, 1.10813, -0.07276 },
	{ -0.07367, -0.00605, 1.07602 }
};

vec3 rrt_and_odt_fit(vec3 v) {
	vec3 a = v * (v + 0.0245786) - 0.000090537;
	vec3 b = v * (0.983729 * v + 0.4329510) + 0.238081;
	return a / b;
}

vec3 aces_fitted(vec3 color) {
	color = aces_input_mat * color;
	color = rrt_and_odt_fit(color);
	color = aces_output_mat * color;
	return clamp(color, 0, 1);
}

void main() {
	vec4 sampled = texture(tex, tex_coord);
	sampled.xyz  = aces_fitted(sampled.xyz);
	pixel_color  = sampled;
}
