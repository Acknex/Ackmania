	#include <transform>
	#include <fog>
	#include <pos>
	#include <normal>
	
	float4 vecTime;
	float4 vecSkill41; // Wind_X, Wind_Y, Ripple, Scale
	float4 vecColor;	

	texture entSkin1;
	texture mtlSkin2;

	sampler sBumpTex = sampler_state
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
		float4 Color: COLOR0;
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
		Out.Tex0 = (inTex0 + vecSkill41.xy * vecTime.w) * vecSkill41.w;
#ifdef SHADERMODEL1
		Out.Tex1.xy = 0.5 * (1.0 + (Out.Pos/Out.Pos.w));
#else
		Out.Tex1 = Out.Pos.xyw;
#endif		

// color and transparency
		Out.Color = vecColor;
#ifdef MIRROR_FRESNEL
		float4 P = DoPos(inPos); // vector world position
		float3 N = DoNormal(inNormal); // normal world orientation
		float4 vecToView = normalize(vecViewPos-P); // direction towards camera
		Out.Color.a = 0.5 + vecColor.a * (1.0 - dot(vecToView,N));
#endif		
		return Out;
	}
	

#ifndef SHADERMODEL1
	float4 ps_water_mirror20(out_water_mirror In): COLOR
	{

		float4 Bump = tex2D(sBumpTex,In.Tex0);
		float2 Coord = 0.5 * (1.0 + In.Tex1.xy/In.Tex1.z) + Bump.xy*vecSkill41.z;
		return tex2D(sMirrorTex,Coord) * In.Color;
	}
#endif

//////////////////////////////////////////////////////////////////
	technique water_mirror
	{
		pass one
		{
			AlphaBlendEnable = True;

			sampler[0] = (sBumpTex);
			sampler[1] = (sMirrorTex);

			VertexShader = compile vs_1_1 vs_water_mirror();
#ifndef SHADERMODEL1
			PixelShader = compile ps_2_0 ps_water_mirror20();			
#else
// bump matrix - required for texbem
			BumpEnvMat00[1] = (vecSkill41.z);
			BumpEnvMat11[1] = (vecSkill41.z);
			BumpEnvMat01[1] = 0.0;
			BumpEnvMat10[1] = 0.0;

// We need an asm shader here - for some reason,
// ps_1_1 HLSL tex2D won't sample modified coordinates
			PixelShader = asm
			{
				ps_1_1
				tex t0         // sample Stage0 Texture rg 
				texbem t1,t0   // multiply rg with BumpEnvMat to uv, 
							   // and sample Stage1 Texture using uv+t1
				mul r0,v0,t1   // multiply Stage1 Texture by Color + Alpha
			};
#endif
		}		
	}

	technique fallback { pass one { } }
