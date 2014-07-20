Texture TargetMap;
sampler2D g_samSrcColor = sampler_state { texture = <TargetMap>; MipFilter = Linear;	};

float4 vecSkill1;

float4 postprocessing_bleach(float2 Tex : TEXCOORD0) : COLOR0 
{   
	float opacity  = vecSkill1.x/50.;
   float4 base = tex2D(g_samSrcColor, Tex.xy);
   float3 lumCoeff = float4(0.25,0.65,0.1,0);
	float lum = dot(lumCoeff,base.rgb);
	float3 blend = lum.rrr;
   float L = min(1,max(0,10*(lum- 0.45)));
   float3 result1 = 2.0f * base.rgb * blend;
   float3 result2 = 1.0f - 2.0f*(1.0f-blend)*(1.0f-base.rgb);
	float3 newColor = lerp(result1,result2,L);
   float A2 = opacity * base.a;
   float3 mixRGB = A2 * newColor.rgb;
   mixRGB += ((1.0f-A2) * base.rgb);
	return float4(mixRGB,base.a);
}

technique PostProcess 
{
	pass p1 
	{
		AlphaBlendEnable = false;	
		VertexShader = null;
		PixelShader = compile ps_2_0 postprocessing_bleach();
	}
}