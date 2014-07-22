float4 vecTime;
texture TargetMap;
sampler smpSource = sampler_state { texture = <TargetMap>; };

float3 HSVtoRGB(float3 HSV)
{
    float3 RGB = 0;
    float C = HSV.z * HSV.y;
    float H = HSV.x * 6;
    float X = C * (1 - abs(fmod(H, 2) - 1));
    if (HSV.y != 0)
    {
        float I = floor(H);
        if (I == 0) { RGB = float3(C, X, 0); }
        else if (I == 1) { RGB = float3(X, C, 0); }
        else if (I == 2) { RGB = float3(0, C, X); }
        else if (I == 3) { RGB = float3(0, X, C); }
        else if (I == 4) { RGB = float3(X, 0, C); }
        else { RGB = float3(C, 0, X); }
    }
    float M = HSV.z - C;
    return RGB + M;
}

float3 RGBtoHSV(float3 RGB)
{
    float3 HSV = 0;
    float M = min(RGB.r, min(RGB.g, RGB.b));
    HSV.z = max(RGB.r, max(RGB.g, RGB.b));
    float C = HSV.z - M;
    if (C != 0)
    {
        HSV.y = C / HSV.z;
        float3 D = (((HSV.z - RGB) / 6) + (C / 2)) / C;
        if (RGB.r == HSV.z)
            HSV.x = D.b - D.g;
        else if (RGB.g == HSV.z)
            HSV.x = (1.0/3.0) + D.r - D.b;
        else if (RGB.b == HSV.z)
            HSV.x = (2.0/3.0) + D.g - D.r;
        if ( HSV.x < 0.0 ) { HSV.x += 1.0; }
        if ( HSV.x > 1.0 ) { HSV.x -= 1.0; }
    }
    return HSV;
}

float4 ps(float2 Tex : TEXCOORD0) : COLOR0
{
	float4 Color = tex2D(smpSource,Tex.xy); 
	
	float3 hsv = RGBtoHSV(Color.rgb);
	
	hsv.x = frac(hsv.x + 0.05 * vecTime.w);
	
	Color.rgb = HSVtoRGB(hsv);
	
	return Color;
}

technique
{
	pass
	{
		PixelShader  = compile ps_3_0 ps();
	}
}