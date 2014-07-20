#include <define>
#include <transform>
#include <fog>
#include <pos>
#include <normal>
#include <lights>
#include <vecskill>
#include <color>

texture entSkin1;	// texture
texture entSkin2;	// lightmap
sampler sBaseTex = sampler_state { Texture = <entSkin1>; MipFilter = Linear;	};
sampler sSkin2 = sampler_state { Texture = <entSkin2>; MipFilter = Linear;	};

struct toonOut
{
	float4 Pos:    POSITION;
	float  Fog:		FOG;
	float4 Ambient:COLOR1;
	float2 Tex:   	TEXCOORD0;
	float2 LM: TEXCOORD1;
	float4 Param: TEXCOORD3;
};

toonOut toon_VS (
   in float4 inPos: 	POSITION, 
   in float3 inNormal:	NORMAL,
   in float2 inTex: 	TEXCOORD0,
   in float2 inLM: 	TEXCOORD1
   )
{
	toonOut Out;
	Out.Pos	= DoTransform(inPos);
	Out.Tex	= inTex;
	Out.LM	= inLM;
	Out.Fog	= DoFog(inPos);
	Out.Ambient = 2*DoAmbient();	

	float3 P = DoPos(inPos);
// rotate and normalize the normal
	float3 N = DoNormal(inNormal);

	float3 Color = 0; 
	for (int i=0; i<iLights; i++)  // Add dynamic lights
		Color += DoLight(P,N,i);

   Out.Param.x = length(Color*vecDiffuse);
   Out.Param.y = DoDefault(vecSkill41.z*0.015,0.75); // shadow threshold
   Out.Param.z = DoDefault(vecSkill41.x*(0.3/50),0.3); // shadow brightness
   Out.Param.w = DoDefault(vecSkill41.w*(0.05/50),0.05); // shadow edge

	return Out;		
}

float4 toon_PS(toonOut In): COLOR
{
	float4 base = tex2D(sBaseTex,In.Tex);
	
	float light;
	if (In.Param.x > In.Param.y)
		light = 1.0;
	else if (In.Param.x > In.Param.y-In.Param.w)	
		light = lerp(1.0,In.Param.z,(In.Param.y-In.Param.x)/(In.Param.w)); 
	else
		light = In.Param.z;
	
	return base * light * In.Ambient;
}

float4 toonLM_PS(toonOut In): COLOR
{
	float4 base = tex2D(sBaseTex,In.Tex);
	float4 lightmap = tex2D(sSkin2,In.LM);
	
	In.Param.x += length(lightmap.xyz);
	float light; 
	if (In.Param.x > In.Param.y)
		light = 1.0;
	else if (In.Param.x > In.Param.y-In.Param.w)	
		light = lerp(1.0,In.Param.z,(In.Param.y-In.Param.x)/(In.Param.w)); 
	else
		light = In.Param.z;
	
	return base * light * In.Ambient;
}

technique toon
{
	pass one
	{		
		VertexShader = compile vs_2_0 toon_VS();
		PixelShader = compile ps_2_0 toon_PS();
	}
	pass two 
	{
		CULLMODE=CW;
		vertexShaderConstant[0] = <matWorldViewProj>;
		vertexShaderConstant[16] = <vecSkill41.y*0.03>;
		
		VertexShader =	asm
		{
			vs_1_0
			dcl_position v0
			dcl_normal v3
			dcl_texcoord v7
			mov r0,v0
			mul r1,c16.x,v3
// Scale the normal
			add r0.xyz,r0.xyz,r1.xyz
			m4x4 oPos,r0,c0
// Transform position to clip space
			mov oD0, c0
			mov r0,c0
		};
	}
}

technique toon_lm
{
	pass one
	{		
		VertexShader = compile vs_2_0 toon_VS();
		PixelShader = compile ps_2_0 toonLM_PS();
	}
}

technique fallback { pass one { } }
