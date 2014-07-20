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

sampler chromemap = sampler_state { Texture = <mtlSkin1>; MipFilter = Linear; };
sampler sSkin = sampler_state { Texture = <entSkin1>; MipFilter = Linear; };
sampler sSkin2 = sampler_state { Texture = <entSkin2>; MipFilter = Linear;	};

struct chromeOut
{
	float4 Pos:    POSITION;
	float  Fog:		FOG;
	float2 Tex1:   TEXCOORD0;	
	float3 Light:	TEXCOORD1;
	float3 Color:  TEXCOORD2;	
	float2 Lookup: TEXCOORD3;
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
	float facScale = DoDefault(vecSkill41.x*0.005,0.25); 
	Out.Lookup = mul(N,matView)*facScale;
	
	return Out;
}


float4 chrome_PS (chromeOut In): COLOR
{
	float3 chrome = tex2D(chromemap,In.Lookup)*In.Color;
	float4 color = tex2D(sSkin,In.Tex1)*float4(In.Light.xyz,1); 
	return float4(pow(chrome,fPower*0.1)*color.a,1)+color; 
}

technique chrome
{
	pass one
	{
		VertexShader = compile vs_2_0 chrome_VS();
		PixelShader = compile ps_2_0 chrome_PS();
	}
}

technique fallback { pass one { } }