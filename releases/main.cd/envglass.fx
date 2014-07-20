// Environment glass shader
// (c) oP group 2009
// based on a shader by Ichiro on the Gamestudio Wiki

#include <transform>
#include <fog>
#include <pos>
#include <normal>
#include <tangent>
#include <vecskill>

float4 vecLight;
float fAlpha;
float fAlbedo;

texture entSkin1;
texture entSkin2;
texture mtlSkin1;

samplerCUBE sEnvironment = sampler_state { Texture = <mtlSkin1>; MipFilter = LINEAR; };
sampler sBase = sampler_state { Texture = <entSkin1>;	MipFilter = LINEAR; };
sampler sNormal = sampler_state { Texture = <entSkin2>; MipFilter = LINEAR; };

// vertex shader output structure
struct VS_OUTPUT
{
	float4 Pos: POSITION;
	float  Fog:  FOG;
	float2 Tex: TEXCOORD0;      
	float3 Eye: TEXCOORD1;
	float3 Norm: TEXCOORD2;
   float factor: TEXCOORD6;
   float3 rainbow: TEXCOORD7;
};


VS_OUTPUT VS(
   float4 inPos  : POSITION,
   float3 inNormal : NORMAL,
   float2 inTex  : TEXCOORD0,
   float4 inTangent : TEXCOORD2) 
{
	VS_OUTPUT Out;
	float4 P = DoPos(inPos);
	float3 N = DoNormal(inNormal);
	Out.Pos  = DoTransform(inPos);
	Out.Fog = DoFog(inPos);
	Out.Tex = inTex;	
	Out.Eye = P - vecViewPos;
	Out.Norm = N;
	
	Out.factor = DoDefault(vecSkill41.y*0.005,0.25); // Texture	
	
	float fSpread = DoDefault(vecSkill41.x*0.0005,0.025);
	Out.rainbow.z = 1.0-2*fSpread;
	Out.rainbow.y = Out.rainbow.z-fSpread;
	Out.rainbow.x = Out.rainbow.y-fSpread;
	return Out;
}

float4 envglass_PS ( VS_OUTPUT In) : COLOR
{
	float4 OutColor;
	float4 color = tex2D(sBase, In.Tex);
	color.a *= In.factor;
	float3 norm = In.Norm;
	float3 vec = reflect(normalize(In.Eye), norm);
	float4 Environment = texCUBE(sEnvironment,vec);
// generate a rainbow effect		
	float3 vec_red = refract(normalize(In.Eye), norm, In.rainbow.x);//0.95);
	float3 vec_green = refract(normalize(In.Eye), norm, In.rainbow.y); //0.9525);
	float3 vec_blue = refract(normalize(In.Eye), norm, In.rainbow.z); //0.955);
	float4 EnvRefract = float4(texCUBE(sEnvironment,vec_red).r,
	   texCUBE(sEnvironment,vec_green).g,
	   texCUBE(sEnvironment,vec_blue).b,1);
	float fresnel = 1-(dot(norm,-normalize(In.Eye)));
	fresnel = pow(fresnel,1.5);
	OutColor.rgb = 2*(fAlpha*lerp(color.rgb,EnvRefract,1-color.a) + fAlbedo*Environment.rgb*fresnel);
	OutColor.a = color.a + pow(fresnel,0.25);
	return OutColor;
}

technique glas
{
	pass one
	{
		VertexShader = compile vs_2_0 VS();
		PixelShader  = compile ps_2_0 envglass_PS();
	}
}

technique fallback { pass one { } }