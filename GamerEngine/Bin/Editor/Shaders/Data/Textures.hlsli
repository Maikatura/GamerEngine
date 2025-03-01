#ifndef TEXTURES_HLSLI
#define TEXTURES_HLSLI


Texture2D albedoTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D materialTexture : register(t2);
Texture2D vertexNormalTexture : register(t3);
Texture2D worldPositionTexture : register(t4);
Texture2D ambientOcclusionTexture : register(t5);
Texture2D viewPositionTexture : register(t6);
Texture2D viewNormalTexture : register(t7);
Texture2D ssaoTexture: register(t8);

Texture2D blueNoiseTexture : register(t9);

TextureCube environmentTexture		:	register(t10);

Texture2D dirLightShadowTexture		:	register(t19);
Texture2D shadowMap[20]				:	register(t20);
TextureCube shadowCubeTexture[20]	:	register(t40);

Texture2D fullscreenDepth : register(t120);


#endif