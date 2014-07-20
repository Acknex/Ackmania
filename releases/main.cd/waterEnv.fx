#include <transform>
#include <fog>
#include <pos>
#include <normal>
	
float4 vecTime;
float4 vecSkill41; // Wind_X, Wind_Y, Ripple, Scale
float4 vecColor;	

texture entSkin1;
texture mtlSkin2;

sampler sBumpTex = sampler_state { Texture = (entSkin1); MipFilter = Linear;	};
sampler sCubeTex = sampler_state { Texture = (mtlSkin2); MipFilter = Linear;	};

struct out_water_env // Output to the pixelshader
{
	float4 Pos	: POSITION;
	float4 Color: COLOR0;
	float  Fog	: FOG;
	float2 Tex0	: TEXCOORD0;
	float4 TanToCube0:	TEXCOORD1;
	float4 TanToCube1:	TEXCOORD2;
	float4 TanToCube2:	TEXCOORD3;
};

out_water_env vs_water_env
(
	in float4 inPos		: POSITION,
	in float3 inNormal	: NORMAL,
	in float2 inTex0	: TEXCOORD0,
    in float3 inTangent	: TEXCOORD2
)
{
	out_water_env Out;
	
	Out.Pos = DoTransform(inPos); // transform to screen coordinates
	Out.Fog = DoFog(inPos);
// bump and reflection coordinates
	Out.Tex0 = (inTex0 + vecSkill41.xy * vecTime.w) * (vecSkill41.w);
	
	float3x3 ObjToTan;
	ObjToTan[0] = (vecSkill41.z/33.) * inTangent;
	ObjToTan[1] = (vecSkill41.z/33.) * cross(inTangent, inNormal);
	ObjToTan[2] = inNormal;

	Out.TanToCube0.xyz = mul(ObjToTan,matWorld[0].xyz);
	Out.TanToCube1.xyz = mul(ObjToTan,matWorld[1].xyz);
	Out.TanToCube2.xyz = mul(ObjToTan,matWorld[2].xyz);
	float4 P = DoPos(inPos); // vector world position
	float4 vecToView = normalize(vecViewPos-P); // direction towards camera

	Out.TanToCube0.w = vecToView.x;
	Out.TanToCube1.w = vecToView.y;
	Out.TanToCube2.w = vecToView.z;

// color and transparency
	Out.Color = vecColor;
#ifdef ENVIRO_FRESNEL
	float3 N = DoNormal(inNormal); // normal world orientation
	Out.Color.a = 0.5 + vecColor.a * (1.0 - dot(vecToView,N));
#endif		
	return Out;
}
	
//////////////////////////////////////////////////////////////////
technique water_env
{
	pass one
	{
		AlphaBlendEnable = True;

		sampler[0] = (sBumpTex);
		sampler[3] = (sCubeTex);
		VertexShader = compile vs_1_1 vs_water_env();

		PixelShader = asm
		{
			ps_1_1
			tex t0		// Normalmap
			texm3x3pad   t1,t0_bx2	// TanToCube0
			texm3x3pad   t2,t0_bx2	// TanToCube1
			texm3x3vspec t3,t0_bx2	// TanToCube2, lookup in CubeTex
			mul r0,v0,t3   // multiply Stage3 Texture by Color + Alpha
		};			
	}
}

technique fallback { pass one { } }