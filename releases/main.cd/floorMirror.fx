	#include <transform>
	#include <fog>
	#include <pos>
	#include <normal>
	
	float fAlbedo;

	texture entSkin1;
	texture mtlSkin2;

	sampler sSurfTex = sampler_state
	{
		Texture = <entSkin1>;
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

	struct out_water_mirror // Output to the pixelshader fragment
	{
		float4 Pos	: POSITION;
		float2  Albedo: COLOR0;
		float  Fog	: FOG;
		float2 Tex0	: TEXCOORD0;
		float3 Tex1	: TEXCOORD1;
	};

	out_water_mirror vs_water_mirror
	(
		in float4 inPos		: POSITION,
		in float3 inNormal	: NORMAL,
		in float4 inTex0	: TEXCOORD0
	)
	{
		out_water_mirror Out;
		
		Out.Pos = DoTransform(inPos); // transform to screen coordinates
		Out.Fog = DoFog(inPos);

// bump and reflection coordinates
		Out.Tex0 = inTex0;
		Out.Tex1 = Out.Pos.xyw;

// color and transparency
		Out.Albedo.x = fAlbedo;
		Out.Albedo.y = 1.0 - Out.Albedo.x;
		
		return Out;
	}
	

#ifndef SHADERMODEL1
	float4 ps_water_mirror20(out_water_mirror In): COLOR
	{

		float2 Coord = 0.5 * (1.0 + In.Tex1.xy/In.Tex1.z);
		return In.Albedo.x*tex2D(sMirrorTex,Coord) + In.Albedo.y*tex2D(sSurfTex,In.Tex0);
	}
#endif

//////////////////////////////////////////////////////////////////
	technique water_mirror
	{
		pass one
		{
			AlphaBlendEnable = True;

			sampler[0] = (sSurfTex);
			sampler[1] = (sMirrorTex);

			VertexShader = compile vs_1_1 vs_water_mirror();
			PixelShader = compile ps_2_0 ps_water_mirror20();			
		}		
	}

	technique fallback { pass one { } }
