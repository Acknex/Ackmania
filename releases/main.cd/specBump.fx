//////////////////////////////////////////////////////////////////////
// Blinn / Phong bump mapping
// (c) oP group 2010  Version 2.3
//////////////////////////////////////////////////////////////////////

#include <bump_vs>
#include <phong>

bool REQUIRE_NORMAL;

texture entSkin1;	// texture
texture entSkin2;	// normal map or lightmap
texture entSkin3;	// normal map on blocks

sampler sBaseTex = sampler_state { Texture = <entSkin1>; MipFilter = Linear;	};
sampler sSkin2 = sampler_state { Texture = <entSkin2>; MipFilter = None;	};
sampler sSkin3 = sampler_state { Texture = <entSkin3>; MipFilter = None;	};

float3 DoSpecular(bumpOut In,float3 Normal,float fSpecular)
{
#ifdef BLINN
	float3 viewDir = -normalize(In.ViewDir);	
			
	float fLight = dot(In.LightDir1.xyz,Normal);
	float3 R = reflect(In.LightDir1.xyz,Normal);
	float3 Diffuse = DoPhong(In.Diffuse1,fLight,dot(R,viewDir),fSpecular);

	fLight = dot(In.LightDir2.xyz,Normal);
	R = reflect(In.LightDir2.xyz,Normal);
	Diffuse += DoPhong(In.Diffuse2,fLight,dot(R,viewDir),fSpecular);

	fLight = dot(In.LightDir3.xyz,Normal);
	R = reflect(In.LightDir3.xyz,Normal);
	Diffuse += DoPhong(In.Diffuse3,fLight,dot(R,viewDir),fSpecular);
	
#else // PHONG			
	float fLight = dot(In.LightDir1.xyz,Normal);
	float3 Diffuse = DoPhong(In.Diffuse1,fLight,fLight,fSpecular);		

	fLight = dot(In.LightDir2.xyz,Normal);
	Diffuse += DoPhong(In.Diffuse2,fLight,fLight,fSpecular);		

	fLight = dot(In.LightDir3.xyz,Normal);
	Diffuse += DoPhong(In.Diffuse3,fLight,fLight,fSpecular);
#endif

	return Diffuse;
}

float4 specBump_PS(bumpOut In): COLOR
{
	float4 Base = tex2D(sBaseTex,In.Tex12.xy);
	float3 Normalmap = tex2D(sSkin2,In.Tex12.xy)*2-1;
  float3 Diffuse = DoSpecular(In,Normalmap,Base.w);	
	return Base * DoColor(Diffuse,In.Ambient);
}

float4 specBumpLM_PS(bumpOut In): COLOR
{
	float4 Base = tex2D(sBaseTex,In.Tex12.xy);
	float4 Lightmap = tex2D(sSkin2,In.Tex12.zw);
	float3 Normalmap = tex2D(sSkin3,In.Tex12.xy)*2-1;
  float3 Diffuse = DoSpecular(In,Normalmap,Base.w);	
	return Base * DoLightmap(Diffuse,Lightmap,In.Ambient);
}


technique spec
{
	pass one
	{		
      ZWriteEnable = True;
      AlphaBlendEnable = False;

		VertexShader = compile vs_2_0 bump_VS();
		PixelShader = compile ps_2_0 specBump_PS();
	}
}

technique spec_lm
{
	pass one
	{		
		ZWriteEnable = True;
		AlphaBlendEnable = False;
		
		VertexShader = compile vs_2_0 bump_VS();
		PixelShader = compile ps_2_0 specBumpLM_PS();
	}
}

technique fallback { pass one { } }