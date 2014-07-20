// Light counting test shader
// (c) oP group 2009  Version 2.2

/* Shader for test purposes; indicates how many dynamic lights are
   in range of the bounding box of a certain block.
   black - 0 lights in range
   grey - 1
   blue - 2
   green - 3
   cyan - 4
   red - 5
   magenta - 6
   yellow - 7
   white - 8 or more lights in range
*/

//////////////////////////////////////////////////////////////////////
#include <define>
#include <transform>
#include <pos>
#include <normal>
#include <lights>
#include <color>

bool AUTORELOAD;

#define LIGHTS_PER_BLOCK   // display all lights on the block, even if the vertex is out of range

struct lightOut
{
	float4 Pos: POSITION;
	float4 Ambient:  COLOR0;
	float3 Diffuse:  TEXCOORD0;
	float3 PosWorld: TEXCOORD1; 		
};

lightOut lightCount_VS(vertexIn In)
{
	lightOut Out;

	Out.Pos	= DoTransform(In.Pos);
	Out.Ambient = DoAmbient();	

   Out.PosWorld = DoPos(In.Pos);
   float3 Normal = DoNormal(In.Normal);
	int lights = 0;
	for (int i=0; i < 8; i++)
#ifdef LIGHTS_PER_BLOCK
      if (vecLightPos[i].w > 0.f) lights++;
#else	
      if (DoLightFactorN(vecLightPos[i],Out.PosWorld,Normal) > 0.) lights++;
#endif
      
   if (lights >= 8)   
      Out.Diffuse = float3(1,1,1);
   else if (lights >= 7)   
      Out.Diffuse = float3(1,1,0);
   else if (lights >= 6)   
      Out.Diffuse = float3(1,0,1);
   else if (lights >= 5)   
      Out.Diffuse = float3(1,0,0);
   else if (lights >= 4)   
      Out.Diffuse = float3(0,1,1);
   else if (lights >= 3)   
      Out.Diffuse = float3(0,1,0);
   else if (lights >= 2)   
      Out.Diffuse = float3(0,0,1);
   else  if (lights >= 1)   
      Out.Diffuse = float3(0.5,0.5,0.5);
   else   
      Out.Diffuse = float3(0,0,0);

	return Out;		
}


float4 lightCount_PS(lightOut In): COLOR
{
   
	return float4(In.Diffuse,1);
}

technique lightCount
{
	pass one
	{		
		VertexShader = compile vs_2_0 lightCount_VS();
		PixelShader = compile ps_2_0 lightCount_PS();
	}
}

technique fallback { pass one { } }