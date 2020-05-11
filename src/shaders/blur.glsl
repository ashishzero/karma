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
// http://rastergrid.com/blog/2010/09/efficient-gaussian-blur-with-linear-sampling/
//

const float weight[5] = { 0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216 };
 
void main(void) {
    vec2 tex_size = 1.0 / textureSize(tex, 0);
    vec4 sampled = texture(tex, tex_coord);
    vec3 result = sampled.rgb * weight[0];

#ifdef BLUR_HORIZONTAL
    for(int i = 1; i < 5; ++i) {
        result += texture(tex, tex_coord + vec2(tex_size.x * i, 0.0)).rgb * weight[i];
        result += texture(tex, tex_coord - vec2(tex_size.x * i, 0.0)).rgb * weight[i];
    }
#endif

#ifdef BLUR_VERTICAL
    for(int i = 1; i < 5; ++i) {
        result += texture(tex, tex_coord + vec2(0.0, tex_size.y * i)).rgb * weight[i];
        result += texture(tex, tex_coord - vec2(0.0, tex_size.y * i)).rgb * weight[i];
    }
#endif

    pixel_color = vec4(result, sampled.a);
}
