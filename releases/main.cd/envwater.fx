#include <define>
#include <transform>
#include <fog>
#include <pos>
#include <normal>
#include <tangent>
#include <vecskill>
	
float4 vecTime;
float4 vecColor;	
float4 vecSunColor;	
float fAlpha;

texture mtlSkin1;
texture mtlSkin2;

sampler sBumpTex = sampler_state { Texture = <mtlSkin1>; MipFilter = None;	};
samplerCUBE sCubeTex = sampler_state { Texture = <mtlSkin2>; MipFilter = Linear;	};

struct out_water_env // Output to the pixelshader
{
	float4 Pos	: POSITION;
	float4 Color: COLOR0;
	float  Fog	: FOG;
	float2 Tex	: TEXCOORD0;
	float3 Eye	: TEXCOORD4;
	float3 m0:	TEXCOORD5;
	float3 m1:	TEXCOORD6;
	float3 m2:	TEXCOORD7;
};

out_water_env vs_water_env
(
	in float4 inPos: POSITION,
	in float3 inNormal: NORMAL,
	in float2 inTex: TEXCOORD0,
   in float3 inTangent: TEXCOORD2
)
{
	out_water_env Out;
	
	Out.Pos = DoTransform(inPos); // transform to screen coordinates
	Out.Fog = DoFog(inPos);
// bump and reflection coordinates
   float2 Speed;
   Speed.x = DoDefault(vecSkill41.x * 0.000001,0.00005);
   Speed.y = DoDefault(vecSkill41.y * 0.000001,0.00005);
	Out.Tex = (inTex + Speed*vecTime.w) * DoDefault(vecSkill41.w*0.3,0.3*50);

	CreateTangents(inNormal,inTangent);
	float fScale = DoDefault(vecSkill41.z*0.01,50*0.01);
	Out.m0 = matTangent[0]*fScale;
	Out.m1 = matTangent[1]*fScale;
	Out.m2 = matTangent[2]*fScale;

	float4 P = DoPos(inPos); // vector world position
	Out.Eye = normalize(P-vecViewPos); // direction towards camera

// color and transparency
	Out.Color = float4(vecColor.xyz+vecSunColor.xyz,fAlpha);
#ifdef ENVIRO_FRESNEL
	float3 N = DoNormal(inNormal); // normal world orientation
	Out.Color.a = 0.5 + fAlpha * (1.0 - dot(Out.Eye,N));
#endif		
	return Out;
}
	
float4 envWater_PS(out_water_env In): COLOR
{
	float3 Normalmap = tex2D(sBumpTex,In.Tex)*2-1;
	float3x3 matTangent = float3x3(In.m0,In.m1,In.m2);
	float3 normal = mul(matTangent,Normalmap);
	float3 vec = reflect(In.Eye,normal);
	float4 Environment = texCUBE(sCubeTex,vec);
   return Environment*In.Color;	
}

	
	
//////////////////////////////////////////////////////////////////
technique water_env
{
	pass one
	{
	   ZWriteEnable = False;
	   AlphaBlendEnable = True;
	   SrcBlend = SRCALPHA;
	   DestBlend = INVSRCALPHA;
		VertexShader = compile vs_2_0 vs_water_env();
		PixelShader = compile ps_2_0 envWater_PS();
	}
}

technique fallback { pass one { } }