// Occlusion mapping
// (c) oP group 2008  Version 2.1
// based on a shader by nVidia

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

//// ray-intersect functions for relief mapping //////////

float ray_intersect(			// use linear and binary search
      sampler heightMap,float2 dp,float2 ds)
{
   const int linear_search_steps=15;
 // current size of search window
   float size = 1.0/linear_search_steps;
 // current depth position
   float depth = 0.0;
 // search front to back for first point inside object
   for( int i=0;i<linear_search_steps-1;i++ ) {
      float2 uv = dp+ds*depth;
   	float4 t = tex2D(heightMap,uv);
		if (depth<t.w)
		    depth += size;
   }
   
   const int binary_search_steps=5;
// recurse around first point (depth) for closest match
   for( int i=0;i<binary_search_steps;i++ ) {
   	size *= 0.5;
      float2 uv = dp+ds*depth;
	   float4 t = tex2D(heightMap,uv);
		if (depth<t.w)
		    depth += (2*size);
		depth -= size;
   }
   return depth;
}

float3 DoPom(tangentOut In,sampler sNormal,out float2 offsetUV)
{
	matTangent = float3x3(In.Tangent0,In.Tangent1,In.Tangent2);

	float3 View = normalize(In.PosView);

// ray intersect in view direction
	const float Depth = 0.1;
	float3 s  = float3(dot(View,In.Tangent0),-dot(View,In.Tangent1),-dot(View,In.Tangent2));
	s *= Depth/s.z;
	float d  = ray_intersect(sNormal,In.Tex12.xy,s.xy);
   offsetUV = s.xy*d;

// normal map on rgb, height map on w
	float4 normalMap = tex2D(sNormal,In.Tex12.xy + offsetUV)*2-1;

// transform normal to world space
	float3 Normal = normalize(mul(normalMap.xyz,matTangent));
	View = In.PosView + View*d*Depth/s.z;

	float fLight = DoShine(In.Light1.xyz-In.PosView,Normal);
	float3 Diffuse = In.Diffuse1 * saturate(fLight);

	fLight += DoShine(In.Light2.xyz-In.PosView,Normal);
	Diffuse += In.Diffuse2 * saturate(fLight);

	return Diffuse * vecDiffuse;
}

float4 specPom_PS(tangentOut In) : COLOR
{
	float2 offsetUV;
	float3 Diffuse = DoPom(In,sSkin2,offsetUV);
	float4 Basemap = tex2D(sBaseTex,In.Tex12.xy + offsetUV);
	return Basemap * DoColor(Diffuse,In.Ambient);
}

float4 specPomLM_PS(tangentOut In) : COLOR
{
	float2 offsetUV;
	float3 Diffuse = DoPom(In,sSkin3,offsetUV);
	float4 Basemap = tex2D(sBaseTex,In.Tex12.xy + offsetUV);
	float4 Lightmap = tex2D(sSkin2,In.Tex12.zw);
   return Basemap * DoLightmap(Diffuse,Lightmap,In.Ambient);	
}

/////////////////Techniques/////////////////

technique specPom
{
	pass p0
	{
		VertexShader = compile vs_2_0 tangent_VS();
		PixelShader  = compile ps_2_a specPom_PS();
	}
}

technique specPom_lm
{
	pass p0
	{
		VertexShader = compile vs_2_0 tangent_VS();
		PixelShader  = compile ps_2_a specPomLM_PS();
	}
}
