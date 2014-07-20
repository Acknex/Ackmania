// Tree animation shader

float4 vecTime;

#include <transform>
#include <fog>
#include <pos>
#include <normal>
#include <lights>
#include <texture>
#include <color>
#include <vecskill>

struct vsOut
{
	float4 Pos: POSITION;
	float  Fog:	FOG;		
	float4 Ambient:COLOR;
	float2 Tex: TEXCOORD0;
};

vsOut animTree_VS (
   in float4 inPos: 	POSITION, 
   in float3 inNormal:	NORMAL,
   in float2 inTex: 	TEXCOORD0)
{
	vsOut Out;

	Out.Tex	= DoTexture(inTex);
	Out.Fog	= DoFog(inPos);
	
	Out.Ambient = DoAmbient();	

	float3 P = DoPos(inPos);
	float3 N = DoNormal(inNormal);
	for (int i=0; i<iLights; i++)  // Add dynamic lights
		Out.Ambient.xyz += DoLight(P,N,i)*0.5*vecDiffuse;
	
	float force_x = DoDefault(vecSkill41.x*(0.1/50),0.1); 
	float force_y = DoDefault(vecSkill41.y*(0.1/50),0.1);
	float offs = vecTime.w+0.2*(P.x+P.y+P.z);
	float speed = sin(offs * DoDefault(vecSkill41.z*(0.05/50),0.05));
	
	if(inPos.y > 0 ) // move only upper part of tree
	{
		inPos.x += speed * force_x * inPos.y;
		inPos.z += speed * force_y * inPos.y;
		inPos.y -= 0.1*abs(speed*(force_x+force_y)) * inPos.y;
	}
	Out.Pos = DoTransform(inPos);
   
	return Out;		
}

technique animTree
{
	pass { VertexShader = compile vs_2_0 animTree_VS(); }
}

technique animTree_lod1
{
	pass { VertexShader = compile vs_2_0 animTree_VS(); }
}

technique animTree_lod2
{
	pass { VertexShader = compile vs_2_0 animTree_VS(); }
}

technique animTree_lod3 { pass { } }

technique fallback { pass { } }