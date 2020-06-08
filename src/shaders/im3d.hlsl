cbuffer constants : register(b0) {
	row_major float4x4 projection;
}

struct Vs_In {
	float3 position : POSITION;
	float2 tex_coord : TEX_COORD;
	float3 normal : NORMAL;
	float4 color : COLOR;
};

struct Vs_Out {
	float4 position : SV_POSITION;
	float2 tex_coord : TEX_COORD;
	float3 normal : NORMAL;
	float4 color : COLOR;
};

Texture2D    u_texture : register(t0);
SamplerState u_sampler : register(s0);

Vs_Out vs_main(Vs_In input) {
	Vs_Out output;
	output.position  = mul(projection, float4(input.position, 1.0f));
	output.tex_coord = input.tex_coord;
	output.normal    = input.normal;
	output.color     = input.color;
	return output;
}

struct Ps_Out {
	float4 pixel : SV_TARGET0;
	float4 bloom : SV_TARGET1;
};

float luminance(float3 color) {
	return dot(color, float3(0.2126, 0.7152, 0.0722));
}

Ps_Out ps_main(Vs_Out input) {
	float2 tex_coord = input.tex_coord;

	Ps_Out output;

	output.pixel = u_texture.Sample(u_sampler, tex_coord) * input.color;

	float brightness = luminance(output.pixel.rgb);

	if (brightness > 1) {
		output.bloom = output.pixel;
	} else {
		output.bloom = float4(0, 0, 0, 0);
	}

	return output;
}
