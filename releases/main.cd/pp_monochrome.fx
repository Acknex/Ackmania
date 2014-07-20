Texture TargetMap;
sampler2D g_samSrcColor = sampler_state { texture = <TargetMap>; MipFilter = Linear;	};

float4 MyShader( float2 Tex : TEXCOORD0 ) : COLOR0 
{
   float4 Color = tex2D( g_samSrcColor, Tex.xy);
	Color.rgb = (Color.r+Color.g+Color.b)/3.0f; 				  
	return float4(Color.xyz*2.,Color.w);
}

technique PostProcess 
{
	pass p1 
	{
		AlphaBlendEnable = false;
		VertexShader = null;
		PixelShader = compile ps_2_0 MyShader();
	}
}