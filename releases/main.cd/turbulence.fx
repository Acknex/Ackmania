#include <define>
#include <transform>
#include <fog>
#include <pos>
#include <normal>
#include <lights>
#include <vecskill>
#include <color>

float4 vecTime;

struct turbOut
{
	float4 Pos: POSITION;
	float  Fog:	FOG;
	float4 Color:   COLOR0;
	float2 Tex: TEXCOORD0;
	float2 LM:  TEXCOORD1;
};

turbOut turb_VS (
   in float4 inPos: 	POSITION, 
   in float3 inNormal:	NORMAL,
   in float2 inTex: 	TEXCOORD0,
   in float2 inLM: 	TEXCOORD1
   )
{
	turbOut Out;
	Out.Pos	= DoTransform(inPos);
	float fCycle = vecTime.w*DoDefault(vecSkill41.y*(0.1/50),0.1);
	float fAmp = DoDefault(vecSkill41.x*(0.005/50),0.005);
	float fMod = 1+DoDefault(vecSkill41.z*(25./50),25);
	Out.Tex.x = inTex.x + fAmp*sin(inTex.y*fMod + fCycle);
	Out.Tex.y = inTex.y - fAmp*sin(inTex.x*fMod + fCycle);
	Out.LM	= inLM;
	Out.Fog	= DoFog(inPos);

	float3 P = DoPos(inPos);
	float3 N = DoNormal(inNormal);

	float3 Color = 0; 
	for (int i=0; i<8; i++)  // Add 8 dynamic lights
		Color += DoLight(P,N,i);

   Out.Color = 0.5*DoAmbient() + 0.5*float4(Color,1)*vecDiffuse;

	return Out;		
}


technique turb
{
	pass one
	{		
		VertexShader = compile vs_2_0 turb_VS();
	}
}

technique fallback { pass one { } }
