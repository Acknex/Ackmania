float4 vecSkill1;

Texture TargetMap;
sampler2D smpSrc = sampler_state {
	Texture = <TargetMap>;
	AddressU = Clamp;
	AddressV = Clamp;
};

float4 process_sepia(float2 Tex: TEXCOORD0): COLOR 
{
	float x = Tex.x;
	float scale = 0.1f + 0.4f * Tex.x;
	
	Tex.y = 0.5f + 0.4f * ((1.0f / scale) * (Tex.y - 0.5f));
	
	Tex.y += 0.08f * (1.0f - Tex.x) * sin(30 * (1.0f - Tex.x) * (1.0f - 0.3f * Tex.x) + 0.5f * vecSkill1.x);
	
	Tex.x = 1.0f - pow(1.0f - Tex.x, 1.7f);

	float4 Color = tex2D(smpSrc, Tex.xy);
	
	float offset = 0.2 * vecSkill1.x;
	
	float4 ColorYes = float4(0, 0, 0, 1);
	
	ColorYes.r = sin(05.0 * offset + 0.4 * x + 0);
	ColorYes.g = sin(07.5 * offset + 0.6 * x + 1);
	ColorYes.b = sin(10.0 * offset + 0.8 * x + 2);
	ColorYes.rgb = 0.5f + 0.5f * ColorYes.rgb;
	
	float4 ColorNo = float4(1.0f - ColorYes.rgb, 1.0f);
	
	return tex2D(smpSrc, Tex.xy); // float4(lerp(ColorYes, ColorNo, Color.r).rgb, Color.a);
}

technique Sepia {
	pass p1 {
		AlphaBlendEnable = false;	
		PixelShader = compile ps_3_0 process_sepia();
	}
}   
