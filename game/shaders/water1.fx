float4x4 matWorldViewProj;
float4x4 matWorld;
float4 vecAmbient;
float4 vecSunDir;
float4 vecTime;
texture entSkin1;

sampler ColorMapSampler = sampler_state 
{ 
   Texture = <entSkin1>; 
   AddressU  = Clamp; 
   AddressV  = Clamp; 
}; 
    
void DiffuseVS( 
   in float4 InPos: POSITION, 
   in float3 InNormal: NORMAL, 
   in float2 InTex: TEXCOORD0, 
   out float4 OutPos: POSITION, 
   out float2 OutTex: TEXCOORD0, 
   out float3 OutNormal: TEXCOORD1) 
{ 
InPos.y += 16*sin(vecTime.w*0.05+InPos.x/64+InPos.z/64);
   OutPos = mul(InPos, matWorldViewProj); 
   OutNormal = normalize(mul(InNormal, matWorld));
   OutTex = InTex; 
} 
    
float4 DiffusePS( 
   in float2 InTex: TEXCOORD0, 
   in float3 InNormal: TEXCOORD1): COLOR 
{ 
   float4 Color = tex2D(ColorMapSampler, InTex); 
   float Diffuse = 0.5+0.5*saturate(dot(-vecSunDir, normalize(InNormal))); 
   float4 final = Color*Diffuse; asdadasd
   
   return final;
} 
 
technique DiffuseTechnique 
{ 
   pass P0 
   { 
      VertexShader = compile vs_3_0 DiffuseVS(); 
      PixelShader  = compile ps_3_0 DiffusePS(); 
   } 
} 