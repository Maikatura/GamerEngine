#include "DeferredShaderStructs.hlsli"
#include "PBRFunctions.hlsli"

DeferredPixelOutput main(DeferredVertexToPixel input)
{
	DeferredPixelOutput result;

	const float4 albedo = albedoTexture.Sample(defaultSampler, input.myUV);
	if (albedo.a < 0.05f)
	{
		discard;
		result.myColor = 0;
		return result;
	}

	const float3 pixelNormalMap = normalTexture.Sample(defaultSampler, input.myUV).rgb;
	const float4 materialMap = materialTexture.Sample(defaultSampler, input.myUV);
	const float3 vertexNormal = vertexNormalTexture.Sample(defaultSampler, input.myUV).rgb;
	const float4 worldPosition = worldPositionTexture.Sample(defaultSampler, input.myUV);
	const float ambientOcclusion = ambientOcclusionTexture.Sample(defaultSampler, input.myUV).r;

	const float metalness = materialMap.r;
	const float roughness = materialMap.g;
	const float emissive = materialMap.b;
	const float emissiveStr = materialMap.a;

	const float3 toEye = normalize(FB_CamTranslation.xyz - worldPosition.xyz);
	const float3 specularColor = lerp((float3).04f, albedo, metalness);
	const float3 diffuseColor = lerp((float3).01f, albedo, 1 - metalness);


	const float3 ambientLighting = EvaluateAmbience(
		environmentTexture,
		pixelNormalMap,
		vertexNormal,
		toEye,
		roughness,
		ambientOcclusion,
		diffuseColor,
		specularColor
	);

	const float3 directLighting = EvaluateDirectionalLight(
		diffuseColor,
		specularColor,
		pixelNormalMap,
		roughness,
		LB_Color,
		LB_Intensity,
		-LB_Direction,
		toEye
	);

	result.myColor.rgb = saturate(ambientLighting + directLighting) + (emissive * emissiveStr);
	result.myColor.a = 1;


	return result;
}
