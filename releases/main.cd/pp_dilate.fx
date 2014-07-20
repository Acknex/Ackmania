#include <srctex>

float4 pp_dilate(float2 Tex : TEXCOORD0) : COLOR0 
{   
  float4 dilate = tex2D(src, Tex.xy);
	dilate = max( dilate, tex2D( src, Tex.xy+float2(-vecViewPort.z,-vecViewPort.w) ) );
	dilate = max( dilate, tex2D( src, Tex.xy+float2(0,-vecViewPort.w) ) );
	dilate = max( dilate, tex2D( src, Tex.xy+float2(vecViewPort.z,-vecViewPort.w)));
	dilate = max( dilate, tex2D( src, Tex.xy+float2(-vecViewPort.z,0) ) );
	dilate = max( dilate, tex2D( src, Tex.xy+float2(vecViewPort.z,0) ) );
	dilate = max( dilate, tex2D( src, Tex.xy+float2(-vecViewPort.z,vecViewPort.w) ) );
	dilate = max( dilate, tex2D( src, Tex.xy+float2(0,vecViewPort.w) ) );
	dilate = max( dilate, tex2D( src, Tex.xy+float2(vecViewPort.z,vecViewPort.w) ) );

	return dilate;
}

technique { pass { PixelShader = compile ps_2_0 pp_dilate(); } }