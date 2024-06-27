#include "Data/ShaderStructs.hlsli"
#include "Data/Textures.hlsli"

#include "Data/PBRFunctions.hlsli"
#include "Data/Samplers.hlsli"
#include "Data/ShadowFunctions.hlsli"
#include "Data/LightBuffer.hlsli"
#include "Data/Shadows.hlsli"

// Constants
#define SHADOW_BIAS 0.01f
#define SHADOW_MAP_TEXCOORD_SCALE 8192.0f
#define SHADOW_MAP_TEXCOORD_BIAS 0.0001f