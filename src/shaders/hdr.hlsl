
struct Vs_In {
	uint id : SV_VertexID;
};

struct Vs_Out {
	float4 position  : SV_POSITION;
	float2 tex_coord : TEX_COORD;
};

Texture2D    u_texture : register(t0);
SamplerState u_sampler : register(s0);

static const float2 vertices[] = {
	{ -1, -1 }, { -1, 1 }, { 1, 1 }, { -1, -1 }, { 1, 1 }, { 1, -1 }
};
static const float2 tex_coords[] = {
	{ 0, 0 }, { 0, 1 }, { 1, 1 }, { 0, 0 }, { 1, 1 }, { 1, 0 }
};

Vs_Out vs_main(Vs_In input) {
	Vs_Out output;
	output.position  = float4(vertices[input.id], 0, 1);
	output.tex_coord = tex_coords[input.id];
	return output;
}

float4 ps_main(Vs_Out input) :
	SV_TARGET {
	float2 tex_coord = input.tex_coord;
	tex_coord.y      = 1.0f - tex_coord.y;
	return u_texture.Sample(u_sampler, tex_coord);
}
