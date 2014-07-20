	#include <transform>
	#include <fog>
	#include <pos>
	#include <normal>
	#include <lights>
	#include <color>
	
	float fAlbedo;

	texture entSkin1;
	texture mtlSkin2;

   sampler sSurfTex = sampler_state { Texture = <entSkin1>; MipFilter = Linear;	};

	sampler sMirrorTex = sampler_state
	{
		Texture = <mtlSkin2>;
		MipFilter = Linear;
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
		float2  Albedo: TEXCOORD2;
	};

out_mirror vs_floor_mirror
(
	in float4 inPos		: POSITION,
	in float3 inNormal	: NORMAL,
	in float2 inTex0	: TEXCOORD0
)
{
	out_mirror Out;
	
	Out.Pos = DoTransform(inPos); // transform to screen coordinates
	Out.Fog = DoFog(inPos);
// bump and reflection coordinates
	Out.Tex0 = inTex0;
	Out.Tex1 = Out.Pos.xyw;

// color and transparency
	Out.Albedo.x = fAlbedo;
	Out.Albedo.y = 1.0 - fAlbedo;

   float3 P = DoPos(inPos);
   float3 N = DoNormal(inNormal);
	float3 Color = 0; 
	for (int i=0; i<8; i++)  // Add 8 dynamic lights
		Color += DoLight(P,N,i);

   Out.Color = DoAmbient() + float4(Color,1)*vecDiffuse;

	return Out;
}
	

	float4 ps_floor_mirror20(out_mirror In): COLOR
	{

		float2 Coord = 0.5 * (1.0 + In.Tex1.xy/In.Tex1.z);
		return In.Color*float4(In.Albedo.x*tex2D(sMirrorTex,Coord).xyz + In.Albedo.y*tex2D(sSurfTex,In.Tex0).xyz,1);
	}

//////////////////////////////////////////////////////////////////
	technique water_mirror
	{
		pass one
		{
			VertexShader = compile vs_2_0 vs_floor_mirror();
			PixelShader = compile ps_2_0 ps_floor_mirror20();			
		}		
	}

	technique fallback { pass one { } }
