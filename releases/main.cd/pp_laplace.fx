Texture TargetMap;
sampler2D g_samSrcColor = sampler_state { texture = <TargetMap>; MipFilter = Linear;	};

float4 vecViewPort;

float4 postprocessing_laplace(float2 Tex : TEXCOORD0) : COLOR0 
{   
	float4 laplace = 0.;
	laplace -= tex2D( g_samSrcColor , Tex.xy + float2(-1.3333333 * vecViewPort.z,  0.0) )*3.;
	laplace -= tex2D( g_samSrcColor , Tex.xy + float2( 1.3333333 * vecViewPort.z,  0.0) )*3.;
	laplace -= tex2D( g_samSrcColor , Tex.xy + float2( 0.0, -1.3333333 * vecViewPort.w) )*3.;
	laplace -= tex2D( g_samSrcColor , Tex.xy + float2( 0.0,  1.3333333 * vecViewPort.w) )*3.;
	laplace -= tex2D( g_samSrcColor , Tex.xy + float2(-vecViewPort.z, -vecViewPort.w) );
	laplace -= tex2D( g_samSrcColor , Tex.xy + float2( vecViewPort.z, -vecViewPort.w) );
	laplace -= tex2D( g_samSrcColor , Tex.xy + float2( vecViewPort.z,  vecViewPort.w) );
	laplace -= tex2D( g_samSrcColor , Tex.xy + float2(-vecViewPort.z,  vecViewPort.w) );
	laplace += tex2D(g_samSrcColor, Tex.xy) * 16.0;
	laplace /= 8.0;
	return laplace + 0.5;
}

technique PostProcess 
{
	pass p1 
	{
		AlphaBlendEnable = false;	
		VertexShader = null;
		PixelShader = compile ps_2_0 postprocessing_laplace();
	}
}
