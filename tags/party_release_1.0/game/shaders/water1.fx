float4x4 matWorldViewProj;
float4x4 matWorld;
float4 vecAmbient;
float4 vecSunDir;
float4 vecTime;
texture entSkin1;

sampler ColorMapSampler = sampler_state 
{ 
   Texture = <entSkin1>; 
   AddressU  = Wrap; 
   AddressV  = Wrap; 
}; 
    
void DiffuseVS( 
   in float4 InPos: POSITION, 
   in float3 InNormal: NORMAL, 
   in float2 InTex: TEXCOORD0, 
   out float4 OutPos: POSITION, 
   out float2 OutTex: TEXCOORD0, 
   out float3 OutData: TEXCOORD1) 
{ 
float fac = 12*sin(vecTime.w*0.05+InPos.x/32)*sin(vecTime.w*0.075+InPos.z/32);
InPos.y += fac;
   OutPos = mul(InPos, matWorldViewProj); 
   //OutNormal = mul(InNormal, matWorld);
   OutTex = InTex*3;
   OutData = float3(sin(vecTime.w*0.075),float2(floor(vecTime.w*0.05)/4,floor(vecTime.w*0.0125)/4) ); //floor(InPos.x/32+vecTime.w*0.2),floor(InPos.z/32+vecTime.w*0.2));
} 
    
float4 DiffusePS( 
   in float2 InTex: TEXCOORD0, 
   in float3 InData: TEXCOORD1): COLOR 
{ 
   float4 Color = tex2D(ColorMapSampler, InTex);
   float Diffuse = tex2D(ColorMapSampler, InTex+InData.yz).b*(0.5+0.25*InData.x)*0+1.2;
   float4 final = Color*Diffuse;
   
   return final;
} 
 
technique DiffuseTechnique 
{ 
   pass P0 
   { 
      VertexShader = compile vs_2_0 DiffuseVS(); 
      PixelShader  = compile ps_2_0 DiffusePS(); 
   } 
} 