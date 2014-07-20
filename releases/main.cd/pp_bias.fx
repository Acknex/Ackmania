#include <srctex>

float4 pp_bias(float3 Tex : TEXCOORD0) : COLOR0 
{   
	float bias = vecSkill1.x/10.;
	float gradient = vecSkill1.y/25.;
	float g = bias * lerp(1.0,Tex.x,gradient);
	return tex2Dbias(src,float4(Tex.xyz,g));
}

technique { pass { PixelShader = compile ps_2_0 pp_bias(); } }