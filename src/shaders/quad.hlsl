cbuffer constants : register(b0) {
	row_major float4x4 projection;
}

struct Vs_In {
	float3 position  : POSITION;
	float2 tex_coord : TEX_COORD;
	float4 color     : COLOR;
};

struct Vs_Out {
	float4 position  : SV_POSITION;
	float2 tex_coord : TEX_COORD;
	float4 color     : COLOR;
};

Texture2D    u_texture : register(t0);
SamplerState u_sampler : register(s0);

Vs_Out vs_main(Vs_In input) {
	Vs_Out output;
	output.position  = mul(float4(input.position, 1.0f), projection);
	output.tex_coord = input.tex_coord;
	output.color     = input.color;
	return output;
}

float4 ps_main(Vs_Out input) : SV_TARGET {
	return u_texture.Sample(u_sampler, input.tex_coord) * input.color;
}
