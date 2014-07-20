#include <srctex>

float4 pp_blur( float2 Tex : TEXCOORD0 ) : COLOR0 
{
	float facBlur = 0.05*vecSkill1.x;
	float4 Color = tex2D(src,Tex.xy);
	Color += tex2D(src,Tex.xy+vecViewPort.zw*facBlur);
	Color += tex2D(src,Tex.xy+vecViewPort.zw*facBlur*2);
	Color += tex2D(src,Tex.xy+vecViewPort.zw*facBlur*3);
	return Color/4;
}

technique { pass { PixelShader = compile ps_2_0 pp_blur(); } }