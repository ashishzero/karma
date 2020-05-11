
struct Vs_In {
	uint id : SV_VertexID;
};

struct Vs_Out {
	float4 position : SV_POSITION;
	float2 tex_coord : TEX_COORD;
};

Texture2D    u_texture : register(t0);
Texture2D    u_bloom : register(t1);
SamplerState u_texture_sampler : register(s0);
SamplerState u_bloom_sampler : register(s1);

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

//
// ACES Tone mapping curve
// https://64.github.io/tonemapping/
//

static const float3x3 aces_input_mat = {
	{ 0.59719, 0.35458, 0.04823 },
	{ 0.07600, 0.90834, 0.01566 },
	{ 0.02840, 0.13383, 0.83777 }
};

static const float3x3 aces_output_mat = {
	{ 1.60475, -0.53108, -0.07367 },
	{ -0.10208, 1.10813, -0.00605 },
	{ -0.00327, -0.07276, 1.07602 }
};

float3 rrt_and_odt_fit(float3 v) {
	float3 a = v * (v + 0.0245786f) - 0.000090537f;
	float3 b = v * (0.983729f * v + 0.4329510f) + 0.238081f;
	return a / b;
}

float3 aces_fitted(float3 color) {
	color = mul(aces_input_mat, color);
	color = rrt_and_odt_fit(color);
	color = mul(aces_output_mat, color);
	return saturate(color);
}

float4 ps_main(Vs_Out input) : SV_TARGET {
	float2 tex_coord   = input.tex_coord;
	tex_coord.y        = 1.0f - tex_coord.y;
	float4 sampled     = u_texture.Sample(u_texture_sampler, tex_coord);
	float3 bloom_color = u_bloom.Sample(u_bloom_sampler, tex_coord).xyz;
	sampled.xyz += bloom_color;
	sampled.xyz = aces_fitted(sampled.xyz);
	return sampled;
}
