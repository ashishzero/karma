                             f      #version 330
out vec2 tex_coord;
out vec4 color;


layout (location = 0) in vec2 vertex_position;
layout (location = 1) in vec2 vertex_tex_coord;
layout (location = 2) in vec4 vertex_color;

uniform mat4 projection;

void main() {
	tex_coord = vertex_tex_coord;
	color = vertex_color;
	gl_Position = projection * vec4(vertex_position, 0, 1);
}
      #version 330
in vec2 tex_coord;
in vec4 color;


layout (location = 0) out vec4 pixel_color;

uniform sampler2D tex;

void main() {
	// TODO: Clean this up, this is for font rendering
	float a     = texture(tex, tex_coord).r;
	pixel_color = vec4(1, 1, 1, a) * color;
}
       
       projection                 tex   