Texture TargetMap;
sampler2D g_samSrcColor = sampler_state { texture = <TargetMap>; MipFilter = Linear;	};

float4 vecSkill1;

float4 postprocessing_colorshift( float2 Tex : TEXCOORD0 ) : COLOR0 
{
	float3 facShift = { vecSkill1.x/10. , vecSkill1.y/10. , vecSkill1.z/10. };
	float4 color =tex2D( g_samSrcColor, Tex.xy);
   return float4(color.xyz*facShift,color.w);
}

technique PostProcess 
{
	pass p1 
	{
		AlphaBlendEnable = false;
		VertexShader = null;
		PixelShader = compile ps_2_0 postprocessing_colorshift();
	}
}