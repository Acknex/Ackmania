#include <transform>
#include <fog>
#include <pos>
#include <normal>
#include <light>
#include <vecskill>
#include <color>

float vecSunColor;
float fAlbedo;

texture entSkin1;	// texture
texture entSkin2;	// normal map
texture mtlSkin1;

//vecSkill41.z: float facEnvBump = 0.8;
//vecSkill41.w: float reflectivity = 0.5;

sampler sBaseTex = sampler_state { Texture = <entSkin1>; MipFilter = Linear;	};
sampler sBump = sampler_state { Texture = <entSkin2>; MipFilter = None;	};
sampler sEnviro = sampler_state { Texture = <mtlSkin1>; MipFilter = Linear;	};

struct bumpOut
{
	float4 Pos:    	POSITION;
	float  Fog:			FOG;
	float4 Ambient:	COLOR0;
	float2 Tex:   		TEXCOORD0;
	float3 PosWorld: 	TEXCOORD1;	
	float2 Vecskill:  TEXCOORD2;
	float3 Normal: 	TEXCOORD3;	
};

bumpOut perPixel_VS(
in float4 inPos: 		POSITION, 
in float3 inNormal:	NORMAL,
in float2 inTex: 		TEXCOORD0,
in float3 inTangent: TEXCOORD2)
{
	bumpOut Out;

	Out.Pos	= DoTransform(inPos);
	Out.Tex	= inTex;
	Out.Fog	= DoFog(inPos);
	Out.Ambient = DoAmbient();	

	float facBump = DoDefault(vecSkill41.x*0.01,50*0.01);
	float facTex = DoDefault(vecSkill41.y*0.01,50*0.01);
	Out.Vecskill.x = facBump;
	Out.Vecskill.y = facTex;
	
	Out.PosWorld = DoPos(inNormal);
	Out.Normal = DoNormal(inNormal);
	
	return Out;		
}


float4 diffuseBump_PS(bumpOut In): COLOR
{
	float4 base = tex2D(sBaseTex,In.Tex);
	float3 bumpNormal = tex2D(sBump,In.Tex)*2-1;
	
	float facBump = In.Vecskill.x;
	float3 cubeTexCoord = normalize(reflect(In.PosWorld - vecViewPos,In.Normal));
	float3 newCubeTexCoord = cubeTexCoord + (bumpNormal - cubeTexCoord )*facBump;
	float4 environment = texCUBE(sEnviro,newCubeTexCoord)*(vecColor+vecSunColor*vecDiffuse)*base.w;	
	float facTex = In.Vecskill.y;
	float4 resultingBase = lerp(environment,base,facTex);
			
	return resultingBase * (0.5+In.Ambient);
}

technique bump
{
	pass One
	{		
		VertexShader = compile vs_2_0 perPixel_VS();
		PixelShader = compile ps_2_0 diffuseBump_PS();
	}
}

technique fallback { pass one { } }