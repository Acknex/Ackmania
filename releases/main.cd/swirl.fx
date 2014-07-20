float4 vecViewPort;
float4 vecTime;

float4 vecSkill1;
float4 vecSkill5;

float g_menuFxSpeed_var;

Texture TargetMap;
sampler2D smpSource = sampler_state
{
	texture = <TargetMap>;
	AddressU = MIRROR;
	AddressV = MIRROR;
};

float3 crazy (float2 tc)
{
	float3 color = 0;
	
	float2 pos = tc;
	float3 time = g_menuFxSpeed_var * vecTime.w * float3(1.2, -0.9, 1.0);
	
	color.rgb = 
		0.5 + 
		0.5 * sin(time + float3(0, 1, 2) + float3(10, -10, 5) * pos.y) * 
		cos(time + float3(3, 2, 1) - float3(-6, 8, -2) * pos.x);
	
	color.rgb = pow(0.5 + color.rgb, 2) - 0.5;
	
	return color;
}

float4 PS (float2 Tex : TEXCOORD0, float2 vpos : VPOS) : COLOR0 
{
	float radius = vecSkill1.x;
	float angle = vecSkill1.y;
	float2 center = vecSkill1.zw;

	float aspect = vecViewPort.x / vecViewPort.y;
	
	float2 tc = Tex.xy;
	tc -= center;
	
	float dist = length(float2(tc.x, tc.y / aspect));
	
	if (dist < radius) 
	{
		float percent = (radius - dist) / radius;
		float theta = percent * percent * angle * 8.0;
		float s = sin(theta);
		float c = cos(theta);
		tc = float2(dot(tc, float2(c, -s)), dot(tc, float2(s, c)));
	}
	
	tc += center;
	
	float blend = vecSkill5.x;
	
	float3 swirlColor = tex2D(smpSource, tc).rgb;
	float3 crazyColor = crazy(tc);
	
	return float4(lerp(swirlColor, crazyColor, blend), 1.0);
}

technique t
{
	pass p
	{
		PixelShader = compile ps_3_0 PS();
	}
}