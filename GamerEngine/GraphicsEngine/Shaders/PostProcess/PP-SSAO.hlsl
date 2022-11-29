#include "PostProcessStructs.hlsli"
#include "../Data/DeferredShaderStructs.hlsli"
#include "../Data/PBRFunctions.hlsli"



float SSAO(float2 screenUV, float uv, float pxViewPos, float3 pxViewNrm, float scale, float bias, float intensity)
{
	const float3 diff	= GetViewPosition(screenUV + uv).xyz - pxViewPos;
	const float3 v		= normalize(diff);
	const float d		= length(diff) * scale;
	const float occ		= max(0.0f, dot(pxViewNrm, v) - bias) * 1.0f / (1.0f + d) * intensity;

	return occ;
}

DeferredPixelOutput main(DeferredVertexToPixel input)
{
	DeferredPixelOutput result;

	// SSAO SETTINGS
	float intensity		= 0.35f;
	float scale			= 0.05f;
	float bias			= 0.05f;
	float radius		= 0.002f;
	float offset		= 0.707f;

	int noiseWidth = 0;
	int noiseHeight = 0;
	int numMipmaps = 0;
	blueNoiseTexture.GetDimensions(0, noiseWidth, noiseHeight, numMipmaps);
	float2 randomUVScale = FB_Resolution / float2(noiseWidth, noiseHeight);
	float2 random = GetRandom(input.myUV, randomUVScale);

	const float4 pxPos = GetViewPosition(input.myUV);
	const float4 pxNrm = GetViewNormal(input.myUV);

	float2 frustrumCorners[4] = { float2(1,0), float2(-1,0),float2(0,1),float2(0,-1) };

	float occlusion = 0.0f;
	for (int i = 0; i < 4; i++)
	{
		const uint idx = i % 4;
		const float2 coord1 = reflect(frustrumCorners[idx], random) * radius;
		const float2 coord2 = float2(coord1.x * offset - coord1.y * offset, coord1.x * offset + coord1.y * offset);

		occlusion += SSAO(input.myUV, coord1 * .25f, pxPos.xyz, pxNrm.xyz, scale, bias, intensity);
		occlusion += SSAO(input.myUV, coord2 * .5f, pxPos.xyz, pxNrm.xyz, scale, bias, intensity);
		occlusion += SSAO(input.myUV, coord1 * .75f, pxPos.xyz, pxNrm.xyz, scale, bias, intensity);
		occlusion += SSAO(input.myUV, coord2, pxPos.xyz, pxNrm.xyz, scale, bias, intensity);
	}

	result.myColor.rgb = float3(1 - occlusion, 0, 0);
	result.myColor.a = 1;
	return result;
}
