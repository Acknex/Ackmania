Texture TargetMap;
sampler2D g_samSrcColor = sampler_state { texture = <TargetMap>; MipFilter = Linear;	};

float4 vecViewPort;

float4 postprocessing_sharpen(float2 Tex : TEXCOORD0) : COLOR0 
{   
	float4 sharp00 = tex2D( g_samSrcColor , Tex.xy + float2(-vecViewPort.z, -vecViewPort.w) );
	float4 sharp02 = tex2D( g_samSrcColor , Tex.xy + float2( vecViewPort.z, -vecViewPort.w) );
	float4 sharp20 = tex2D( g_samSrcColor , Tex.xy + float2( vecViewPort.z,  vecViewPort.w) );
	float4 sharp22 = tex2D( g_samSrcColor , Tex.xy + float2(-vecViewPort.z,  vecViewPort.w) );
	float4 sharp11 = tex2D(g_samSrcColor, Tex.xy) * 5.0;
	
	return (sharp11 - sharp00 - sharp02 - sharp20 - sharp22);
}

technique PostProcess 
{
	pass p1 
	{
		AlphaBlendEnable = false;	
		VertexShader = null;
		PixelShader = compile ps_2_0 postprocessing_sharpen();
	}
}