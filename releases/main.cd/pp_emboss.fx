Texture TargetMap;
sampler2D g_samSrcColor = sampler_state { texture = <TargetMap>; MipFilter = Linear;	};

float4 vecViewPort;

float4 postprocessing_emboss( float2 Tex : TEXCOORD0 ) : COLOR0 
{
   float4 Color = float4(0.5,0.5,0.5,1.0);
	Color -= tex2D( g_samSrcColor, Tex.xy-vecViewPort.zw)*2.0f;
	Color += tex2D( g_samSrcColor, Tex.xy+vecViewPort.zw)*2.0f;	
	Color.rgb = (Color.r+Color.g+Color.b)/3.0f;		  
	return Color;
}

technique PostProcess 
{
	pass p1 
	{
		AlphaBlendEnable = false;
		VertexShader = null;
		PixelShader = compile ps_2_0 postprocessing_emboss();
	}
}