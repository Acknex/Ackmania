Texture TargetMap;
sampler2D g_samSrcColor = sampler_state { texture = <TargetMap>; MipFilter = Linear;	};

float4 vecViewPort;

float4 postprocessing_sobel(float2 Tex : TEXCOORD0) : COLOR0 
{   
	float2 texCoord0 = Tex.xy + float2(-1.333 * vecViewPort.z,  0.0);
	float2 texCoord1 = Tex.xy + float2( 1.333 * vecViewPort.z,  0.0);
	float2 texCoord2 = Tex.xy + float2( 0.0, -1.333 * vecViewPort.w);
	float2 texCoord3 = Tex.xy + float2( 0.0,  1.333 * vecViewPort.w);
	float4 texCoord4 = Tex.xyyx + float4(-1.333 * vecViewPort.z, -1.25 * vecViewPort.w, -1.333 * vecViewPort.w, -1.25 * vecViewPort.z);
	float4 texCoord5 = Tex.xyyx + float4(-1.333 * vecViewPort.z,  1.25 * vecViewPort.w, -1.333 * vecViewPort.w,  1.25 * vecViewPort.z);
	float4 texCoord6 = Tex.xyyx + float4( 1.333 * vecViewPort.z, -1.25 * vecViewPort.w,  1.333 * vecViewPort.w, -1.25 * vecViewPort.z);
	float4 texCoord7 = Tex.xyyx + float4( 1.333 * vecViewPort.z,  1.25 * vecViewPort.w,  1.333 * vecViewPort.w,  1.25 * vecViewPort.z);
	
	float4 sobelX = 0.;
	float4 sobelY = 0.;

	sobelX += tex2D(g_samSrcColor, texCoord0) * 18.0;
	sobelX -= tex2D(g_samSrcColor, texCoord1) * 18.0;

	sobelY += tex2D(g_samSrcColor, texCoord2) * 18.0;
	sobelY -= tex2D(g_samSrcColor, texCoord3) * 18.0;

	sobelX += tex2D(g_samSrcColor, texCoord4.xy) * 15.0;
	sobelX += tex2D(g_samSrcColor, texCoord5.xy) * 15.0;
	sobelX -= tex2D(g_samSrcColor, texCoord6.xy) * 15.0;
	sobelX -= tex2D(g_samSrcColor, texCoord7.xy) * 15.0;

	sobelY += tex2D(g_samSrcColor, texCoord4.wz) * 15.0;
	sobelY += tex2D(g_samSrcColor, texCoord5.wz) * 15.0;
	sobelY -= tex2D(g_samSrcColor, texCoord6.wz) * 15.0;
	sobelY -= tex2D(g_samSrcColor, texCoord7.wz) * 15.0;
	
	sobelX /= 12.0;
	sobelY /= 12.0;
	
	return sqrt(sobelX * sobelX + sobelY * sobelY);
}

technique PostProcess 
{
	pass p1 
	{
		AlphaBlendEnable = false;	
		VertexShader = null;
		PixelShader = compile ps_2_0 postprocessing_sobel();
	}
}