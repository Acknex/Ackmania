Texture TargetMap;
sampler2D g_samSrcColor = sampler_state { texture = <TargetMap>; MipFilter = Linear;	};

float4 postprocessing_negative( float2 Tex : TEXCOORD0 ) : COLOR0 
{
   float3 Color = 1. - tex2D( g_samSrcColor, Tex.xy).xyz;
   return float4(Color,1.);
}

technique PostProcess 
{
	pass p1 
	{
		AlphaBlendEnable = false;
		VertexShader = null;
		PixelShader = compile ps_2_0 postprocessing_negative();
	}
}