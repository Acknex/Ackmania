Texture TargetMap;
sampler2D g_samSrcColor = sampler_state { texture = <TargetMap>; MipFilter = Linear;	};

float4 vecSkill1;

float4 postprocessing_posterize(float2 Tex : TEXCOORD0) : COLOR0 
{   
	float nColors = vecSkill1.x/5.;
	float gamma = vecSkill1.y/100.;
   float4 texCol = tex2D(g_samSrcColor, Tex.xy);
	texCol.xyz = pow(texCol.xyz, gamma);
	texCol.xyz *= nColors;
	texCol.xyz = floor(texCol.xyz);
	texCol.xyz /= nColors;
	texCol.xyz = pow(texCol.xyz,1.0/gamma);
	return texCol;
}  

technique PostProcess 
{
	pass p1 
	{
		AlphaBlendEnable = false;	
		VertexShader = null;
		PixelShader = compile ps_2_0 postprocessing_posterize();
	}
}