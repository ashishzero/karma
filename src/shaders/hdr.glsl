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

void main() {
	pixel_color  = texture(tex, tex_coord);
}
