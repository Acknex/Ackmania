// Blinn / Phong bump mapping
// (c) oP group 2008  Version 2.1

#include <tangent_vs>
#include <phong>

bool REQUIRE_HEIGHT;

texture entSkin1;	// texture
texture entSkin2;	// normal map or lightmap
texture entSkin3;	// normal map on blocks

sampler sBaseTex = sampler_state { Texture = <entSkin1>; MipFilter = Linear; };
sampler sSkin2 = sampler_state { Texture = <entSkin2>; MipFilter = None;	};
sampler sSkin3 = sampler_state { Texture = <entSkin3>; MipFilter = None;	};

///////////////////////////////////////////////////////////////////////////////

float3 DoParallax(tangentOut In,sampler sNormal,out float2 offsetUV)
{
	matTangent = float3x3(In.Tangent0,In.Tangent1,In.Tangent2);

	float3 View = normalize(In.PosView);

// normal map on rgb, height map on w
	float4 normalMap = tex2D(sNormal,In.Tex12.xy)*2-1;

// texture coordinates with height offset
	offsetUV = normalMap.w * mul(View,matTangent).xy * 0.025;

// transform normal to world space
	float3 Normal = normalize(mul(normalMap.xyz,matTangent));

	float fLight = DoShine(In.Light1.xyz-In.PosView,Normal);
	float3 Diffuse = In.Diffuse1 * saturate(fLight);

	fLight += DoShine(In.Light2.xyz-In.PosView,Normal);
	Diffuse += In.Diffuse2 * saturate(fLight);

	return Diffuse * vecDiffuse;
}

float4 parallax_PS(tangentOut In) : COLOR
{
	float2 offsetUV;
	float3 Diffuse = DoParallax(In,sSkin2,offsetUV);
	float4 Basemap = tex2D(sBaseTex,In.Tex12.xy + offsetUV);
	return Basemap * DoColor(Diffuse,In.Ambient);
}

float4 parallaxLM_PS(tangentOut In) : COLOR
{
	float2 offsetUV;
	float3 Diffuse = DoParallax(In,sSkin3,offsetUV);
	float4 Basemap = tex2D(sBaseTex,In.Tex12.xy + offsetUV);
	float4 Lightmap = tex2D(sSkin2,In.Tex12.zw);
   return Basemap * DoLightmap(Diffuse,Lightmap,In.Ambient);	
}

/////////////////Techniques/////////////////

technique parallax
{
	pass p0
	{
		VertexShader = compile vs_2_0 tangent_VS();
		PixelShader  = compile ps_2_0 parallax_PS();
	}
}

technique parallax_lm
{
	pass p0
	{
		VertexShader = compile vs_2_0 tangent_VS();
		PixelShader  = compile ps_2_0 parallaxLM_PS();
	}
}

technique fallback { pass one { } }