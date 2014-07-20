Texture TargetMap;
sampler2D g_samSrcColor = sampler_state { texture = <TargetMap>; MipFilter = Linear;	};
                    
float4  g_SepiaColor1     = {0.2, 0.05, 0.0, 1.0};   
float4  g_SepiaColor2     = {1.0, 0.9,  0.5, 1.0}; 

float4 postprocessing_sepia( float2 Tex : TEXCOORD0 ) : COLOR0 
{
   float4 Color = tex2D( g_samSrcColor, Tex.xy);
	float  sepiaLerp = dot( float3(0.3, 0.59, 0.11),Color );
	return lerp(g_SepiaColor1, g_SepiaColor2, sepiaLerp);
}

technique PostProcess 
{
	pass p1 
	{
		AlphaBlendEnable = false;	
		VertexShader = null;
		PixelShader = compile ps_2_0 postprocessing_sepia();
	}
}