Texture TargetMap;
sampler2D g_samSrcColor = sampler_state { texture = <TargetMap>; MipFilter = Linear;	};

float4 vecViewPort;
float4 vecSkill1;

float4 postprocessing_sharpen( float2 Tex : TEXCOORD0 ) : COLOR0 
{
	float facSharpen = vecSkill1.x/20.;
   float4 Color;
   Color = tex2D( g_samSrcColor, Tex.xy);
	Color -= tex2D( g_samSrcColor, Tex.xy+vecViewPort.zw)*facSharpen;
	Color += tex2D( g_samSrcColor, Tex.xy-vecViewPort.zw)*facSharpen;
	return Color;
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