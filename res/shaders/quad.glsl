.version = 330;
.depth = false;
.cull = true;
.blend = true;

@ vec2 tex_coord;
@ vec4 color;

::Vertex

layout (location = 0) in vec2 vertex_position;
layout (location = 1) in vec2 vertex_tex_coord;
layout (location = 2) in vec4 vertex_color;

uniform mat4 projection;

void main() {
	tex_coord = vertex_tex_coord;
	color = vertex_color;
	gl_Position = projection * vec4(vertex_position, 0, 1);
}

::Fragment

layout (location = 0) out vec4 pixel_color;

uniform sampler2D tex;

void main() {
	// TODO: Clean this up, this is for font rendering
	vec4 sampled = texture(tex, tex_coord);
	pixel_color  = sampled * color;
}
