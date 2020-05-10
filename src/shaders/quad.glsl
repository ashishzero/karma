@ vec2 tex_coord;
@ vec4 color;

::Vertex

layout (location = 0) in vec3 POSITION;
layout (location = 1) in vec2 TEX_COORD;
layout (location = 2) in vec4 COLOR;

layout(row_major, std140, binding = 2) uniform Constants {
	mat4 projection;
};

void main() {
	tex_coord   = TEX_COORD;
	color       = COLOR;
	gl_Position = projection * vec4(POSITION, 1);
}

::Fragment

layout (location = 0) out vec4 pixel_color;

uniform sampler2D tex;

void main() {
	vec4 sampled = texture(tex, tex_coord);
	pixel_color  = sampled * color;
}
