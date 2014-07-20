// envmap.fx - environment mapping FFP

texture entSkin1;
texture mtlSkin1;
matrix matMtl;

technique envmap
{
	pass one
	{
      AddressU[0] = Clamp;  // don't wrap around edges
      AddressV[0] = Clamp;

      TexCoordIndex[0] = CameraSpaceReflectionVector;
      TextureTransformFlags[0] = Count3;
      TextureTransform[0] = <matMtl>; // transform camera space back to world space

      Texture[0] = <mtlSkin1>;
      ColorArg1[0] = Texture;
      ColorArg2[0] = Diffuse;
      ColorOp[0] = Modulate2x;

      Texture[1] = <entSkin1>;
		TextureFactor = 0x40FFFFFF; // blend by 25% (= 0x40)
      ColorArg1[1] = Texture;
      ColorArg2[1] = Current;
		ColorOp[1] = BlendFactorAlpha; 
	}
}

technique fallback { pass one { } }
