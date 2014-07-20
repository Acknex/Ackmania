
//enable: Shadow Map on Blue
//help: Terrain contains a shadow map on the Skin1 blue channel.
//help: With PS 1.1 (GeForce3), sun & dynamic lights are then disabled.
//id: 14
#define SHADOWMAP

#include <define>
#include <transform>
#include <sun>
#include <lights>
#include <fog>
#include <normal>

Texture entSkin1; // Red/green for blending, blue for shadow
Texture entSkin2; // Basic tiled terrain texture
Texture entSkin3; // Red masked tiled texture
Texture entSkin4; // Green masked tiled texture

float4 vecSkill41;
float fAmbient;

sampler sMaskTex = sampler_state { Texture = <entSkin1>; MipFilter = Linear; };
sampler sBaseTex = sampler_state { Texture = <entSkin2>; MipFilter = Linear; };
sampler sRedTex = sampler_state { Texture = <entSkin3>; MipFilter = Linear; };
sampler sGreenTex = sampler_state {	Texture = <entSkin4>; MipFilter = Linear;	};

//////////////////////////////////////////////////////////////////////
struct out_terraintex3 // Output to the pixelshader fragment
{
	float4 Pos		: POSITION;
	float4 Color	: COLOR0;
	float  Fog		: FOG;
	float2 MaskCoord: TEXCOORD0;
	float2 BaseCoord: TEXCOORD1;
	float2 RedCoord : TEXCOORD2;
	float2 GreenCoord: TEXCOORD3;
};

out_terraintex3 vs_terraintex3(
	float4 inPos : POSITION,
	float3 inNormal : NORMAL,
	float2 inTexCoord0 : TEXCOORD0)
{
	out_terraintex3 Out;

	Out.Pos = DoTransform(inPos); // transform to screen coordinates

// rotate and normalize the normal
	float3 N = DoNormal(inNormal);
	float3 P = mul(inPos,matWorld);

	Out.Color = fAmbient; // Add ambient and sun light
	for (int i=0; i<8; i++)  // Add 8 dynamic lights
		Out.Color += DoLight(P,N,i);
	Out.Fog = DoFog(inPos); // Add fog

// scale the texture coordinates for the masked textures
	Out.MaskCoord = inTexCoord0.xy;
	Out.BaseCoord = inTexCoord0.xy * vecSkill41.y;
	Out.RedCoord = inTexCoord0.xy * vecSkill41.z;
	Out.GreenCoord = inTexCoord0.xy * vecSkill41.w;
	return Out;
}

float4 ps_terraintex3(out_terraintex3 In): COLOR
{
// retrieve the pixels for the textures and the masks
	float4 MaskColor = tex2D(sMaskTex,In.MaskCoord);
	float4 BaseColor = tex2D(sBaseTex,In.BaseCoord);
	float4 RedColor = tex2D(sRedTex,In.RedCoord);

// blend the red and green textures over the base texture
	float4 GreenColor = tex2D(sGreenTex,In.GreenCoord);
	float4 BaseRed = lerp(BaseColor,RedColor,MaskColor.r);
	float4 FinalColor = lerp(BaseRed,GreenColor,MaskColor.g);

// Add the vertex light and shadow map
#ifdef SHADOWMAP
//entry: Shadow Map Offset
//cntl: spin 0 1.0 0.05
//help: Light offset (0..1) to be subtracted from the shadow map
	float offset = 0.25;
	FinalColor *= In.Color + (MaskColor.b - offset);
#else
	FinalColor *= In.Color;
#endif
	FinalColor.a = 1.0f;	// prevent transparency
	return FinalColor;
}


technique terraintex3_13
{
	pass one
	{
		sampler[0] = (sMaskTex);
		sampler[1] = (sBaseTex);
		sampler[2] = (sRedTex);
		sampler[3] = (sGreenTex);

		VertexShader = compile vs_2_0 vs_terraintex3();
		PixelShader = compile ps_2_0 ps_terraintex3();
	}
}

// fallback if nothing works
technique fallback { pass one { } }
