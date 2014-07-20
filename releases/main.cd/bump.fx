// Lambert bump mapping
// (c) oP group 2008  Version 2.1

#include <bump_vs>
#include <phong>

texture entSkin1;	// texture
texture entSkin2;	// normal map or lightmap
texture entSkin3;	// normal map on blocks

sampler sBaseTex = sampler_state { Texture = <entSkin1>; MipFilter = Linear;	};
sampler sSkin2 = sampler_state { Texture = <entSkin2>; MipFilter = None;	};
sampler sSkin3 = sampler_state { Texture = <entSkin3>; MipFilter = None;	};


float3 DoDiffuse(bumpOut In,float3 Normal)
{
	float3 Diffuse = In.Diffuse1 * saturate(dot(normalize(In.LightDir1.xyz),Normal));
	Diffuse += In.Diffuse2 * saturate(dot(normalize(In.LightDir2.xyz),Normal));
	Diffuse += In.Diffuse3 * saturate(dot(normalize(In.LightDir3.xyz),Normal));
	return Diffuse * vecDiffuse.xyz;
}

float4 diffuseBump_PS(bumpOut In): COLOR
{
	float4 Base = tex2D(sBaseTex,In.Tex12.xy);
	float3 Normalmap = tex2D(sSkin2,In.Tex12.xy)*2-1;
	float3 Diffuse = DoDiffuse(In,Normalmap);	
	return Base * DoColor(Diffuse,In.Ambient);
}

float4 diffuseBumpLM_PS(bumpOut In): COLOR
{
	float4 Base = tex2D(sBaseTex,In.Tex12.xy);
	float4 Lightmap = tex2D(sSkin2,In.Tex12.zw);
	float3 Normalmap = tex2D(sSkin3,In.Tex12.xy)*2-1;
	float3 Diffuse = DoDiffuse(In,Normalmap);	
	return Base * DoLightmap(Diffuse,Lightmap,In.Ambient);
}

technique bump
{
	pass one
	{		
		VertexShader = compile vs_2_0 bump_VS();
		PixelShader = compile ps_2_0 diffuseBump_PS();
	}
}

technique bump_lm
{
	pass one
	{		
		VertexShader = compile vs_2_0 bump_VS();
		PixelShader = compile ps_2_0 diffuseBumpLM_PS();
	}
}

technique fallback { pass one { } }