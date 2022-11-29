#include "Data/DeferredShaderStructs.hlsli"

GBufferOutput main(VertexToPixel input)
{
	GBufferOutput result;

	const float4 albedo = albedoTexture.Sample(defaultSampler, input.myUV);
	if(albedo.a == 0)
	{
		discard;
		result.Albedo = 0;
		result.Normal = 0;
		result.Material = 0;
		result.VertexNormal = 0;
		result.WorldPosition = 0;
		result.AmbientOcclusion = 0;
		return result;
	}

	// TGA weirdness R:unused | G:norm.y | B:AO | A:norm.x
	const float3 normalMap = normalTexture.Sample(defaultSampler, input.myUV).agb;
	// TGA weirdness R:metal | G:rough | B:emissive | A:emissiveStr
	const float4 materialMap = materialTexture.Sample(defaultSampler, input.myUV);

	const float ambientOcclusion = normalMap.z;

	const float3x3 TBN = float3x3(
		normalize(input.myTangent),
		normalize(input.myBinormal),
		normalize(input.myNormal)
		);

	float3 pixelNormal = normalMap;
	pixelNormal.z = 0;
	pixelNormal = pixelNormal * 2 - 1;
	pixelNormal.z = sqrt(1 - saturate(pixelNormal.x + pixelNormal.x + pixelNormal.y + pixelNormal.y));
	pixelNormal.y *= -1;
	pixelNormal = normalize(pixelNormal);
	pixelNormal = normalize(mul(pixelNormal, TBN));

	result.Albedo = albedo;
	result.Normal = float4(pixelNormal, 1.0f);
	result.Material = materialMap;
	result.VertexNormal = float4(normalize(input.myNormal), 1.0f);
	result.WorldPosition = input.myVxPosition;
	result.AmbientOcclusion = ambientOcclusion;


	return result;

}
