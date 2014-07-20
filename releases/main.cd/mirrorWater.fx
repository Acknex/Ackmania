	#include <transform>
	#include <fog>
	#include <pos>
	#include <normal>
	#include <vecskill>
	
	float4 vecTime;
	float4 vecSunColor;	
	float4 vecColor;	
	float fAlbedo;
	float fAlpha;

	texture entSkin1;
	texture mtlSkin1;
	texture mtlSkin2;

	sampler sBumpTex = sampler_state
	{
		Texture = <mtlSkin1>;
		MipFilter = Linear;
		MinFilter = Linear;
		MagFilter = Linear;
		AddressU = Wrap;
		Addressv = Wrap;
	};

	sampler sMirrorTex = sampler_state
	{
		Texture = <mtlSkin2>;
		MipFilter = Linear;
		MinFilter = Linear;
		MagFilter = Linear;
		AddressU = Clamp;
		Addressv = Clamp;
	};

	struct out_mirror // Output to the pixelshader fragment
	{
		float4 Pos	: POSITION;
		float  Fog	: FOG;
		float4 Color: COLOR0;
		float2 Tex0	: TEXCOORD0;
		float3 Tex1	: TEXCOORD1;
		float3  Albedo: TEXCOORD2;
	};

out_mirror vs_water_mirror
(
	in float4 inPos		: POSITION,
	in float3 inNormal	: NORMAL,
	in float4 inTex0	: TEXCOORD0
)
{
	out_mirror Out;
	
	Out.Pos = DoTransform(inPos); // transform to screen coordinates
	Out.Fog = DoFog(inPos);

// bump and reflection coordinates
   float2 Speed;
   Speed.x = DoDefault(vecSkill41.x * 0.000002,0.0001);
   Speed.y = DoDefault(vecSkill41.y * 0.000002,0.0001);
	Out.Tex0 = (inTex0 + Speed*vecTime.w) * DoDefault(vecSkill41.w*0.05,50*0.05);
	Out.Tex1 = Out.Pos.xyw;

// color and transparency
	Out.Albedo.x = fAlbedo;
	Out.Albedo.y = 1.0 - fAlbedo;
	Out.Albedo.z = DoDefault(vecSkill41.z*0.001,50*0.001);
	Out.Color = float4(vecColor.xyz+vecSunColor.xyz,fAlpha);
#ifdef MIRROR_FRESNEL
	float4 P = DoPos(inPos); // vector world position
	float3 vecToView = normalize(vecViewPos-P); // direction towards camera
	float3 N = DoNormal(inNormal); // normal world orientation
	Out.Color.a = 0.5 + fAlpha * (1.0 - dot(vecToView,N));
#endif		
	return Out;
}
	

	float4 ps_water_mirror20(out_mirror In): COLOR
	{

		float4 Bump = tex2D(sBumpTex,In.Tex0)*2-1;
		float2 Coord = 0.5 * (1.0 + In.Tex1.xy/In.Tex1.z) + Bump.xy*In.Albedo.z;
		return tex2D(sMirrorTex,Coord) * In.Color;
		//return float4(0,0,1,0.5);
	}

//////////////////////////////////////////////////////////////////
	technique water_mirror
	{
		pass one
		{
		   ZWriteEnable = False;
			AlphaBlendEnable = True;
		   SrcBlend = SRCALPHA;
		   DestBlend = INVSRCALPHA;
			VertexShader = compile vs_2_0 vs_water_mirror();
			PixelShader = compile ps_2_0 ps_water_mirror20();			
		}		
	}

	technique fallback { pass one { } }
