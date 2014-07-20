// Chrome mapping
// (c) jcl / oP group 2009  Version 1.1
// based on a shader by ello on the Gamestudio Wiki

#include <transform>
#include <pos>
#include <fog>
#include <normal>
#include <sun>
#include <tangent>
#include <vecskill>
#include <color>

float4x4 matView;

texture mtlSkin1;   // chrome reflection map
texture entSkin1;
texture entSkin2;
texture entSkin3;

sampler chromemap = sampler_state { Texture = <mtlSkin1>; MipFilter = Linear; };
sampler sSkin = sampler_state { Texture = <entSkin1>; MipFilter = Linear; };
sampler sSkin2 = sampler_state { Texture = <entSkin2>; MipFilter = Linear;	};
sampler sSkin3 = sampler_state { Texture = <entSkin3>; MipFilter = Linear;	};

struct chromeOut
{
	float4 Pos:    POSITION;
	float  Fog:		FOG;
	float2 Tex1:   TEXCOORD0;	
	float3 Light:	TEXCOORD1;
	float3 Color:  TEXCOORD2;	
	float3 m1: TEXCOORD3;
	float3 m2: TEXCOORD4;
	float3 m3: TEXCOORD5;
};

chromeOut chrome_VS (
   in float4 inPos 	: POSITION,
   in float3 inNormal: NORMAL,
   in	float2 inTex1	: TEXCOORD0,
   in float3 inTangent: TEXCOORD2
)
{
   chromeOut Out;
	
	Out.Pos = DoTransform(inPos);
	Out.Fog = DoFog(inPos);
	float3 N = DoNormal(inNormal);
	Out.Tex1 = inTex1;
	Out.Light = (DoAmbient()+DoSunLight(N)*vecDiffuse)*(1-vecSpecular)*4;
	Out.Color = (vecColor.xyz+vecSunColor.xyz*vecDiffuse)*vecSpecular*4;
	CreateTangents(inNormal,inTangent);
	float facScale = DoDefault(vecSkill41.x*0.005,0.25); 
	Out.m1 = matTangent[0]*facScale;
	Out.m2 = matTangent[1]*facScale;
	Out.m3 = matTangent[2]*facScale;
	
	return Out;
}


float4 DoChrome(chromeOut In,float3 Normalmap): COLOR
{
	float3x3 matTangent = float3x3(In.m1,In.m2,In.m3);
	float3 Normal = mul(Normalmap,matTangent);
	float2 Lookup = mul(Normal,matView);

	float3 chrome = tex2D(chromemap,Lookup)*In.Color;
	float4 color = tex2D(sSkin,In.Tex1)*float4(In.Light.xyz,1); 
	return float4(pow(chrome,fPower*0.1)*color.a,1)+color; 
}

float4 chrome_PS (chromeOut In): COLOR
{
	float3 Normalmap = tex2D(sSkin2,In.Tex1)*2-1;
   return DoChrome(In,Normalmap);
}

float4 chromeLM_PS (chromeOut In): COLOR
{
	float3 Normalmap = tex2D(sSkin3,In.Tex1)*2-1;
   return DoChrome(In,Normalmap);
}

technique chrome
{
	pass one
	{
		VertexShader=compile vs_2_0 chrome_VS();
		PixelShader=compile ps_2_0 chrome_PS();
	}
}

technique chrome_lm
{
	pass one
	{
		VertexShader=compile vs_2_0 chrome_VS();
		PixelShader=compile ps_2_0 chromeLM_PS();
	}
}

technique fallback { pass one { } }