Texture TargetMap;
sampler2D g_samSrcColor = sampler_state { texture = <TargetMap>; MipFilter = Linear;	};

float4 vecViewPort;

float4 postprocessing_erode(float2 Tex : TEXCOORD0) : COLOR0 
{   
   float4 erode = tex2D(g_samSrcColor, Tex.xy);
	erode = min( erode, tex2D( g_samSrcColor, Tex.xy+float2(-vecViewPort.z,-vecViewPort.w) ) );
	erode = min( erode, tex2D( g_samSrcColor, Tex.xy+float2(0,-vecViewPort.w) ) );
	erode = min( erode, tex2D( g_samSrcColor, Tex.xy+float2(vecViewPort.z,-vecViewPort.w)));
	erode = min( erode, tex2D( g_samSrcColor, Tex.xy+float2(-vecViewPort.z,0) ) );
	erode = min( erode, tex2D( g_samSrcColor, Tex.xy+float2(vecViewPort.z,0) ) );
	erode = min( erode, tex2D( g_samSrcColor, Tex.xy+float2(-vecViewPort.z,vecViewPort.w) ) );
	erode = min( erode, tex2D( g_samSrcColor, Tex.xy+float2(0,vecViewPort.w) ) );
	return min( erode, tex2D( g_samSrcColor, Tex.xy+float2(vecViewPort.z,vecViewPort.w) ) );
}

technique PostProcess 
{
	pass p1 
	{
		AlphaBlendEnable = false;	
		VertexShader = null;
		PixelShader = compile ps_2_0 postprocessing_erode();
	}
}
