float4x4 matWorldViewProj;
float4x4 matWorld;
float fAmbient;
float4 vecSkill41;
float4 vecViewDir;

void DiffuseVS( 
   in float4 InPos: POSITION, 
   in float3 InNormal: NORMAL, 
   in float2 InTex: TEXCOORD0, 
   out float4 OutPos: POSITION, 
   out float2 OutTex: TEXCOORD0, 
   out float3 OutNormal: TEXCOORD1) 
{ 
   OutTex = float2(0,InPos.y/256.0); 
InPos.xyz *= vecSkill41.x;
   OutPos = mul(InPos, matWorldViewProj); 
   OutNormal = normalize(mul(InNormal, matWorld));
} 
    
float4 DiffusePS( 
   in float2 InTex: TEXCOORD0, 
   in float3 InNormal: TEXCOORD1): COLOR 
{ 
	float4 Color = float4(1,0.25+InTex.y*0.7,InTex.y*0.3,1);
   float Diffuse = 0.5+0.7*saturate(dot(-vecViewDir, normalize(InNormal)))+fAmbient; 
   float4 final = Color*Diffuse;
   final.a = (0.05*Diffuse+InTex.y*InTex.y*0.2)*vecSkill41.x;
   
   return final;
} 
 
technique DiffuseTechnique 
{ 
   pass P0 
   { 
   zwriteenable = false;
   alphablendenable = true;
      VertexShader = compile vs_2_0 DiffuseVS(); 
      PixelShader  = compile ps_2_0 DiffusePS(); 
   } 
} 
