#include <srctex>

float4 pp_gaussian(float2 Tex : TEXCOORD0) : COLOR0 
{   
	float facBlur = 0.05*vecSkill1.x;
  float4 sampleM  = tex2D(src, Tex.xy);
	float4 sampleB0 = tex2D(src, Tex.xy - vecViewPort.zw*facBlur );
	float4 sampleF0 = tex2D(src, Tex.xy + vecViewPort.zw*facBlur );
	float4 sampleB1 = tex2D(src, Tex.xy - vecViewPort.zw*facBlur*2. );
	float4 sampleF1 = tex2D(src, Tex.xy + vecViewPort.zw*facBlur*2. );
	float4 sampleB2 = tex2D(src, Tex.xy - vecViewPort.zw*facBlur*3. );
	float4 sampleF2 = tex2D(src, Tex.xy + vecViewPort.zw*facBlur*3. );
	
	return 0.1752 * sampleM + 0.1658 * (sampleB0 + sampleF0) + 0.1403 * (sampleB1 + sampleF1) + 0.1063 * (sampleB2 + sampleF2);
}

technique { pass { PixelShader = compile ps_2_0 pp_gaussian(); } }