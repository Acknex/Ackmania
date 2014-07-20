Texture TargetMap;
sampler2D smpSource = sampler_state
{
	texture = <TargetMap>;
	AddressU = MIRROR;
	AddressV = MIRROR;
};

float redness_var;

float4 PS (float2 Tex : TEXCOORD0, float2 vpos : VPOS) : COLOR0 
{
	float3 color = tex2D(smpSource, Tex.xy).rgb;
	float3 redColor = lerp(color, float3(1,0,0), redness_var/100);
	return float4(redColor, 1.0);
}

technique t
{
	pass p
	{
		PixelShader = compile ps_3_0 PS();
	}
}