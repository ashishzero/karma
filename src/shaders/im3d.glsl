@ vec2 tex_coord;
@ vec3 normal;
@ vec4 color;

::Vertex

layout (location = 0) in vec3 POSITION;
layout (location = 1) in vec2 TEX_COORD;
layout (location = 2) in vec3 NORMAL;
layout (location = 3) in vec4 COLOR;

layout(row_major, std140, binding = 2) uniform Constants {
	mat4 projection;
};

void main() {
	tex_coord   = TEX_COORD;
	color       = COLOR;
	normal		= NORMAL;
	gl_Position = projection * vec4(POSITION, 1);
}

::Fragment

layout (location = 0) out vec4 pixel_color;
layout (location = 1) out vec4 bright_color;

layout (binding = 0) uniform sampler2D tex;

float luminance(vec3 color) {
	return dot(color, vec3(0.2126, 0.7152, 0.0722));
}

void main() {
	vec4 sampled = texture(tex, tex_coord);
	pixel_color  = sampled * color;

	float brightness = luminance(pixel_color.rgb);

	if (brightness > 1) {
		bright_color = pixel_color;
	} else {
		bright_color = vec4(0, 0, 0, 0);
	}
}
