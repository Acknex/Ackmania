Texture TargetMap;
sampler2D g_samSrcColor = sampler_state { texture = <TargetMap>; MipFilter = Linear;	};

float4 vecViewPort;

float4 postprocessing_median(float2 Tex : TEXCOORD0) : COLOR0 
{   
   float4 sample0 = tex2D(g_samSrcColor, Tex.xy);
	float4 sample1 = tex2D( g_samSrcColor, Tex.xy - vecViewPort.zw );
	float4 sample2 = tex2D( g_samSrcColor, Tex.xy + vecViewPort.zw );
	
	float4 max0 = max(sample0, sample1);	
	float4 max1 = max(sample1, sample2);	
	float4 max2 = max(sample2, sample0);
	
	return min(min(max0, max1), max2);
}

technique PostProcess 
{
	pass p1 
	{
		AlphaBlendEnable = false;	
		VertexShader = null;
		PixelShader = compile ps_2_0 postprocessing_median();
	}
}