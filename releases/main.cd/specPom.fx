// Phong Occlusion mapping
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

// ray-intersect function for relief mapping //////////
float ray_intersect(			// use linear and binary search
      sampler heightMap,
      float2 dp, 
      float2 ds)
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


float3 DoSpecular(tangentOut In,float3 View,float3 Normal,float fSpecular)
{
	float fLight = DoShine(In.Light1.xyz-View,Normal);
	float3 Diffuse = DoPhong(In.Diffuse1,fLight,fLight,fSpecular);

	fLight = DoShine(In.Light2.xyz-View,Normal);
	Diffuse += DoPhong(In.Diffuse2,fLight,fLight,fSpecular);

	return Diffuse;
}

float3 DoSpecPom(tangentOut In,sampler sNormal,out float2 offsetUV,out float3 View)
{
	matTangent = float3x3(In.Tangent0,In.Tangent1,In.Tangent2);

	View = normalize(In.PosView);

// ray intersect in view direction
	const float Depth = 0.1;
	float3 s  = float3(dot(View,In.Tangent0),-dot(View,In.Tangent1),-dot(View,In.Tangent2));
	s *= Depth/s.z;
	float d  = ray_intersect(sNormal,In.Tex12.xy,s.xy);
   offsetUV = s.xy*d;
	View = normalize(In.PosView + View*d*Depth/s.z);

// normal map on rgb, height map on w
	float3 normalMap = tex2D(sNormal,In.Tex12.xy + offsetUV)*2-1;
	return normalize(mul(normalMap,matTangent));
}

float4 specPom_PS(tangentOut In) : COLOR
{
	float2 offsetUV;
	float3 View;
	float3 Normal = DoSpecPom(In,sSkin2,offsetUV,View);
	float4 Basemap = tex2D(sBaseTex,In.Tex12.xy + offsetUV);
	float3 Diffuse = DoSpecular(In,View,Normal,Basemap.w);
	return Basemap * DoColor(Diffuse,In.Ambient);
}

float4 specPomLM_PS(tangentOut In) : COLOR
{
	float2 offsetUV;
	float3 View;
	float3 Normal = DoSpecPom(In,sSkin3,offsetUV,View);
	float4 Basemap = tex2D(sBaseTex,In.Tex12.xy + offsetUV);
	float3 Diffuse = DoSpecular(In,View,Normal,Basemap.w);
	float4 Lightmap = tex2D(sSkin2,In.Tex12.zw);
   return Basemap * DoLightmap(Diffuse,Lightmap,In.Ambient);	
}

/////////////////Techniques/////////////////

technique specPom
{
	pass p0
	{
      ZWriteEnable = True;
      AlphaBlendEnable = False;

		VertexShader = compile vs_2_0 tangent_VS();
		PixelShader  = compile ps_2_a specPom_PS();
	}
}

technique specPom_lm
{
	pass p0
	{
      ZWriteEnable = True;
      AlphaBlendEnable = False;
      
		VertexShader = compile vs_2_0 tangent_VS();
		PixelShader  = compile ps_2_a specPomLM_PS();
	}
}
