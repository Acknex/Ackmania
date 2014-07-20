// Blinn / Phong bump mapping
// (c) oP group 2008  Version 2.1

#include <tangent_vs>
#include <phong>

bool REQUIRE_HEIGHT;

texture entSkin1;	// texture
texture entSkin2;	// normal map or lightmap
texture entSkin3;	// normal map on blocks

sampler sBaseTex = sampler_state { Texture = <entSkin1>; MipFilter = Linear;	};
sampler sSkin2 = sampler_state { Texture = <entSkin2>; MipFilter = Linear;	};
sampler sSkin3 = sampler_state { Texture = <entSkin3>; MipFilter = Linear;	};

///////////////////////////////////////////////////////////////////////

float3 DoSpecular(tangentOut In,float3 View,float3 Normal,float fSpecular)
{
	float fLight = DoShine(In.Light1.xyz-View,Normal);
	float3 Diffuse = DoPhong(In.Diffuse1,fLight,fLight,fSpecular);

	fLight = DoShine(In.Light2.xyz-View,Normal);
	Diffuse += DoPhong(In.Diffuse2,fLight,fLight,fSpecular);

	return Diffuse;
}

float3 DoSpecParallax(tangentOut In,sampler sNormal,out float2 offsetUV)
{
	matTangent = float3x3(In.Tangent0,In.Tangent1,In.Tangent2);

	float3 View = normalize(In.PosView);

// normal map on rgb, height map on w
	float4 normalMap = tex2D(sNormal,In.Tex12.xy)*2-1;

// texture coordinates with height offset
	offsetUV = normalMap.w * mul(View,matTangent).xy * 0.02;

// transform normal to world space
	return normalize(mul(normalMap.xyz,matTangent));
}

float4 specParallax_PS(tangentOut In) : COLOR
{
	float2 offsetUV;
	float3 Normal = DoSpecParallax(In,sSkin2,offsetUV);
	float4 Basemap = tex2D(sBaseTex,In.Tex12.xy + offsetUV);
	float3 Diffuse = DoSpecular(In,normalize(In.PosView),Normal,Basemap.w);
	return Basemap * DoColor(Diffuse,In.Ambient);
}

float4 specParallaxLM_PS(tangentOut In) : COLOR
{
	float2 offsetUV;
	float3 Normal = DoSpecParallax(In,sSkin3,offsetUV);
	float4 Basemap = tex2D(sBaseTex,In.Tex12.xy + offsetUV);
	float4 Lightmap = tex2D(sSkin2,In.Tex12.zw);
	float3 Diffuse = DoSpecular(In,normalize(In.PosView),Normal,Basemap.w);
   return Basemap * DoLightmap(Diffuse,Lightmap,In.Ambient);	
}

/////////////////Techniques/////////////////

technique specParallax
{
	pass p0
	{
      ZWriteEnable = True;
      AlphaBlendEnable = False;

		VertexShader = compile vs_2_0 tangent_VS();
		PixelShader  = compile ps_2_0 specParallax_PS();
	}
}

technique specParallax_lm
{
	pass p0
	{
      ZWriteEnable = True;
      AlphaBlendEnable = False;

		VertexShader = compile vs_2_0 tangent_VS();
		PixelShader  = compile ps_2_0 specParallaxLM_PS();
	}
}
