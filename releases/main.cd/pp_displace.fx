#include <srctex>

float4 pp_displacement(float2 Tex: TEXCOORD0): COLOR0 
{   
	float2 disp = vecSkill1.x/5000.*(tex2D(src,Tex.xy).xy-(float2)0.5);
	return tex2D(src,Tex.xy+disp);
}

technique { pass { PixelShader = compile ps_2_0 pp_displacement(); } }