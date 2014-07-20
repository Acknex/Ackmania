Texture TargetMap;
sampler2D g_samSrcColor = sampler_state { texture = <TargetMap>; MipFilter = Linear;	};

float4 vecSkill1;

float4 postprocessing_desaturate(float2 Tex : TEXCOORD0) : COLOR0 
{   
	float desat = vecSkill1.x/100.;
	float3 srcColor = tex2D(g_samSrcColor, Tex.xy).xyz; 
	float3 grayXfer = float3(0.3,0.59,0.11);
	float gray = dot(grayXfer,srcColor).xxx;
	float3 result = lerp(srcColor,gray,desat);
	return float4(result,1);
}

technique PostProcess 
{
	pass p1 
	{
		AlphaBlendEnable = false;	
		VertexShader = null;
		PixelShader = compile ps_2_0 postprocessing_desaturate();
	}
}