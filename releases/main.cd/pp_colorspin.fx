#include <view>

Texture TargetMap;
sampler2D g_samSrcColor = sampler_state { texture = <TargetMap>; MipFilter = Linear;	};

float4 postprocessing_colorSpin(float2 Tex : TEXCOORD0) : COLOR0 
{   
	float3 texCol = tex2D(g_samSrcColor, Tex.xy).xyz - 0.5;
	texCol = DoView(texCol) + 0.5;
	return float4(texCol,1);
}  


technique PostProcess 
{
	pass p1 
	{
		AlphaBlendEnable = false;	
		VertexShader = null;
		PixelShader = compile ps_1_0 postprocessing_colorSpin();
	}
}