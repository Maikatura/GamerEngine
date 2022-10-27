#include "ShaderStructs.hlsli"
#include "PBRFunctions.hlsli"

PixelOutput main(VertexToPixel input)
{
	PixelOutput result = { 0,0,0,0, 0 };

	const float4 albedo = albedoTexture.Sample(defaultSampler, input.myUV);
	// TGA weirdness R:unused | G:norm.y | B:AO | A:norm.x
	const float3 normalMap = normalTexture.Sample(defaultSampler, input.myUV).agb;
	// TGA weirdness R:metal | G:rough | B:emissive | A:emissiveStr
	const float4 materialMap = materialTexture.Sample(defaultSampler, input.myUV);

	const float ambientOcclusion = normalMap.z;
	//AO = 1;
	const float metalness = materialMap.r;
	const float roughness = materialMap.g;
	const float emissive = materialMap.b;
	const float emissiveStr = materialMap.a;

	const float3 toEye = normalize(FB_CamTranslation.xyz - input.myVxPosition.xyz);
	const float3 specularColor = lerp((float3).04f, albedo, metalness);
	const float3 diffuseColor = lerp((float3).01f, albedo, 1 - metalness);

	const float3x3 TBN = float3x3(
		normalize(input.myTangent),
		normalize(input.myBinormal),
		normalize(input.myNormal)
		);

	float3 pixelNormal = normalMap;
	pixelNormal.z = 0;
	pixelNormal = pixelNormal * 2 - 1;
	pixelNormal.z = sqrt(1 - saturate(pixelNormal.x + pixelNormal.x + pixelNormal.y + pixelNormal.y));
	pixelNormal = normalize(pixelNormal);
	pixelNormal = normalize(mul(pixelNormal, TBN));
	

	const float3 ambientLighting = EvaluateAmbience(
		environmentTexture,
		pixelNormal,
		input.myNormal,
		toEye,
		roughness,
		ambientOcclusion,
		diffuseColor,
		specularColor
	);

	const float3 directLighting = EvaluateDirectionalLight(
		diffuseColor,
		specularColor,
		pixelNormal,
		roughness,
		LB_Color,
		LB_Intensity,
		-LB_Direction,
		toEye
	);

	

#ifdef _DEBUG
	switch(FB_RenderMode)
	{
		default: 
		case 0: // Default
		{
			result.myColor.rgb = LinearToGamma(ambientLighting + directLighting);
			result.myColor.a = 1;
			break;
		}
		case 1: // UV1
		{
			result.myColor = float4(input.myUV.r, input.myUV.g, 0, 1);
			break;
		}
		case 2: // VertexColor
		{
			result.myColor = float4(
				input.myVxColor.r, 
				input.myVxColor.g,
				input.myVxColor.b,
				input.myVxColor.a
				);
			break;
		}
		case 3:
		{
			result.myColor = float4(
				input.myVxColor2.r,
				input.myVxColor2.g,
				input.myVxColor2.b,
				input.myVxColor2.a
				);
			break;
		}
		case 4:
		{
			result.myColor = float4(
				input.myVxColor3.r,
				input.myVxColor3.g,
				input.myVxColor3.b,
				input.myVxColor3.a
				);
			break;
		}
		case 5:
		{
			result.myColor = float4(
				input.myVxColor4.r,
				input.myVxColor4.g,
				input.myVxColor4.b,
				input.myVxColor4.a
				);
			break;
		}
		case 6:
		{
			float3 debugNormal = input.myNormal;
			const float signedLength = (debugNormal.r + debugNormal.g + debugNormal.b) / 3;
			if(signedLength < 0)
			{
				debugNormal = float3(1 - abs(debugNormal));
			}
			result.myColor.rgb = debugNormal;
			result.myColor.a = 1;
			break;
		}
		case 7:
		{
			float3 debugNormal = pixelNormal;
			const float signedLength = (debugNormal.r + debugNormal.g + debugNormal.b) / 3;
			if(signedLength < 0)
			{
				debugNormal = float3(1 - abs(debugNormal));
			}
			result.myColor.rgb = debugNormal;
			result.myColor.a = 1;
			break;
		}
		case 8:
		{
			const float fresnel = saturate(1 - dot(input.myNormal, toEye));
			result.myColor.rgb = lerp(albedo, float3(1,1,1), pow(fresnel, 2));
			result.myColor.a = 1;
			break;
		}
		case 9:
		{
			result.myColor.rgb = ambientOcclusion;
			result.myColor.a = 1;
			break;
		}
		case 10:
		{
			result.myColor.rgb = roughness;
			result.myColor.a = 1;
			break;
		}
		case 11:
		{
			result.myColor.rgb = metalness;
			result.myColor.a = 1;
			break;
		}
		case 12:
		{
			result.myColor.rgb = emissive * emissiveStr;
			result.myColor.a = 1;
			break;
		}
	}
#else
	result.myColor.rgb = saturate(ambientLighting + directLighting);
	result.myColor.a = 1;
#endif

	result.myID = OB_ID;
	


	

	return result;
}