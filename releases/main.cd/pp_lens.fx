Texture TargetMap;
sampler2D g_samSrcColor = sampler_state { texture = <TargetMap>; MipFilter = Linear;	};

float4 vecSkill1;//

float4 postprocessing_lense(float2 Tex : TEXCOORD0) : COLOR0 
{  
	float2 pos = {	vecSkill1.x/100. , vecSkill1.y/100. };
	pos *= 2.;	pos -= 1;
	float3 midCoord = float3(1.8 * (2.0 * Tex.xy - 1.0 - pos), 0.5);
	float off = 0.1 * exp2(-dot(midCoord.xy, midCoord.xy));
	return tex2D(g_samSrcColor, Tex.xy - off * normalize(midCoord).xy);
}

technique PostProcess 
{
	pass p1 
	{
		AlphaBlendEnable = false;	
		VertexShader = null;
		PixelShader = compile ps_2_0 postprocessing_lense();
	}
}