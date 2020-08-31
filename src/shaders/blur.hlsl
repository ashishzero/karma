
struct Vs_In {
	uint id : SV_VertexID;
};

struct Vs_Out {
	float4 position : SV_POSITION;
	float2 tex_coord : TEX_COORD;
};

Texture2D    u_texture : register(t0);
SamplerState u_sampler : register(s0);

static const float2 vertices[] = {
	{ -1, -1 }, { -1, 1 }, { 1, 1 }, { -1, -1 }, { 1, 1 }, { 1, -1 }
};
static const float2 tex_coords[] = {
	{ 0, 1 }, { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 }
};

Vs_Out vs_main(Vs_In input) {
	Vs_Out output;
	output.position  = float4(vertices[input.id], 0, 1);
	output.tex_coord = tex_coords[input.id];
	return output;
}

static const float weight[5] = { 0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216 };

float4 ps_main_h(Vs_Out input) :
	SV_TARGET {
	float2 tex_coord = input.tex_coord;
	tex_coord.y      = 1.0f - tex_coord.y;

	float2 tex_size;
	u_texture.GetDimensions(tex_size.x, tex_size.y);
	tex_size = 1.0f / tex_size;

	float4 sampled = u_texture.Sample(u_sampler, tex_coord);
	float3 result  = sampled.xyz * weight[0];

	for (int i = 1; i < 5; ++i) {
		result += u_texture.Sample(u_sampler, tex_coord + float2(tex_size.x * i, 0.0f)).xyz * weight[i];
		result += u_texture.Sample(u_sampler, tex_coord - float2(tex_size.x * i, 0.0f)).xyz * weight[i];
	}

	return float4(result, sampled.w);
}

float4 ps_main_v(Vs_Out input) :
	SV_TARGET {
	float2 tex_coord = input.tex_coord;

	float2 tex_size;
	u_texture.GetDimensions(tex_size.x, tex_size.y);
	tex_size = 1.0f / tex_size;

	float4 sampled = u_texture.Sample(u_sampler, tex_coord);
	float3 result  = sampled.xyz * weight[0];

	for (int i = 1; i < 5; ++i) {
		result += u_texture.Sample(u_sampler, tex_coord - float2(0.0f, tex_size.y * i)).xyz * weight[i];
		result += u_texture.Sample(u_sampler, tex_coord + float2(0.0f, tex_size.y * i)).xyz * weight[i];
	}

	return float4(result, sampled.w);
}
