#include "Data/Textures.hlsli"
#include "Data/NormalHelpers.hlsli"
#include "Data/Samplers.hlsli"
#include "Data/ShaderStructs.hlsli"

GBufferOutput main(VertexToPixel input)
{
	GBufferOutput result;

    const float4 albedo = albedoTexture.Sample(wrapSampler, input.myUV).rgba;
	if(albedo.a <= 0.05f)
	{
		discard;
		result.Albedo = float4(0.0f, 0.0f, 0.0f, 0.0f);
		result.Normal = float4(0.0f, 0.0f, 0.0f, 0.0f);
		result.Material = float4(0.0f, 0.0f, 0.0f, 0.0f);
		result.VertexNormal = float4(0.0f, 0.0f, 0.0f, 0.0f);
		result.WorldPosition = float4(0.0f, 0.0f, 0.0f, 0.0f);
		result.AmbientOcclusion = 0.0f;
		result.ViewPosition = float4(0.0f, 0.0f, 0.0f, 0.0f);
		result.ViewNormal = float4(0.0f, 0.0f, 0.0f, 0.0f);
		return result;
	}

	// TGA weirdness R:unused | G:norm.y | B:AO | A:norm.x
    const float3 normalMap = normalTexture.Sample(wrapSampler, input.myUV).agb;

    const float4 materialMap = materialTexture.Sample(wrapSampler, input.myUV).rgba;
	const float ambientOcclusion = normalMap.b;


	float3 pixelNormal = CalculatePixelNormal(normalMap, input.myTangent, input.myBinormal, input.myNormal);

	
	result.Albedo = albedo;
    result.Normal = float4(pixelNormal, 1.0f);
    result.Material = float4(materialMap.xyz, 1.0f);;
    result.VertexNormal = float4(normalize(input.myNormal), 1.0f);
	result.WorldPosition = float4(input.myVertexWorldPosition.xyz, 1.0f);
	result.AmbientOcclusion = ambientOcclusion;
	result.ViewPosition = float4(input.myViewPosition.xyz, 1.0f);
    result.ViewNormal = float4(normalize(mul(FB_ToView, float4(result.Normal.xyz, 0.0f)).xyz), 1.0f);

	switch(FB_RenderMode)
	{
		default:
			break;
        case 1: // RenderMode::TexCoords0
            result.Albedo.rg = input.myUV;
            break;
		case 6:// RenderMode::NormalMap
            result.Normal = normalTexture.Sample(wrapSampler, input.myUV).agbb;
			break;
		case 2://RenderMode::VertexColor:
			result.Albedo.rgb = input.myVxColor.rgb;
			result.Albedo.a = 1.0f;
			break;
	}


	return result;

}
