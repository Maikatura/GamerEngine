#define MAX_LIGHTS 96
#include "LightBuffer.hlsli"
#include "DeferredShaderStructs.hlsli"
#include "ShaderStructs.hlsli"
#include "PBRFunctions.hlsli"

DeferredPixelOutput main(DeferredVertexToPixel input)
{
	DeferredPixelOutput result;

	const float4 albedo = albedoTexture.Sample(defaultSampler, input.myUV);
	if(albedo.a < 0.05f)
	{
		discard;
		result.myColor.rgb = float3(0, 0, 0);
		result.myColor.a = 0;
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

	float3 directLighting = 0;
	float3 pointLight = 0;
	float3 spotLight = 0;





	float shadow = .001f;
	float shadowBias = 0.00001f;


	float3 dirTemp = EvaluateDirectionalLight(
		diffuseColor,
		specularColor,
		pixelNormalMap,
		roughness,
		LB_DirectionalLight.Color,
		LB_DirectionalLight.Intensity,
		-LB_DirectionalLight.Direction,
		toEye
	);

	if(LB_DirectionalLight.CastShadows)
	{
		if(GetShadowPixel(dirLightShadowMap, LB_DirectionalLight.LightView, LB_DirectionalLight.LightProjection, worldPosition.xyz, LB_DirectionalLight.CastShadows))
		{
			dirTemp *= shadow;
		}
	}

	directLighting += dirTemp;

#ifdef _DEBUG

	switch(FB_RenderMode)
	{
		default:
			break;
		case 0: // Default
		{



			[unroll(20)]
			for(unsigned int l = 0; l < LB_NumLights; l++)
			{
				const LightData Light = LB_Lights[l];
				switch(Light.LightType)
				{

					case 1:
						break;

					case 2:
						float3 pointTemp = EvaluatePointLight(diffuseColor,
							specularColor, pixelNormalMap, roughness, Light.Color, Light.Intensity * 100,
							Light.Range * 100, Light.Position, toEye, worldPosition.xyz);

						if(Light.CastShadows)
						{
							float4x4 plViews[6] = { Light.LightView, Light.LightView1, Light.LightView2, Light.LightView3, Light.LightView4, Light.LightView5 };
							if(GetShadowPixel(shadowCubeTexture[l], plViews, Light.LightProjection, Light.Range, Light.Position, worldPosition.xyz, Light.CastShadows))
							{
								pointTemp *= shadow;
							}
						}
						pointLight += pointTemp;
						break;

					case 3:
						float3 spotTemp = EvaluateSpotLight(diffuseColor,
							specularColor, pixelNormalMap, roughness, Light.Color, Light.Intensity * 100,
							Light.Range * 100, Light.Position, Light.Direction, Light.SpotOuterRadius * (3.1451f / 180.0f),
							Light.SpotInnerRadius * (3.1451f / 180.0f), toEye, worldPosition.xyz);

						if(Light.CastShadows)
						{
							if(GetShadowPixel(shadowMap[l], Light.LightView, Light.LightProjection, worldPosition.xyz, Light.CastShadows))
							{
								spotTemp *= shadow;
							}
						}
						spotLight += spotTemp;
						break;
				}
			}

			result.myColor.rgb = LinearToGamma(ambientLighting + directLighting + pointLight + spotLight) + (emissive * albedo * emissiveStr);
			result.myColor.a = albedo.a;
			break;
		}
		case 1: // UV1
		{
			result.myColor = float4(input.myUV.r, input.myUV.g, 0, 1);
			break;
		}
		case 6:
		{
			float3 debugNormal = vertexNormal;
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
			float3 debugNormal = pixelNormalMap;
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
			const float fresnel = saturate(1 - dot(vertexNormal, toEye));
			result.myColor.rgb = lerp(albedo, float3(1, 1, 1), pow(fresnel, 2));
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
		case 13:
		{
			result.myColor.rgb = diffuseColor;
			result.myColor.a = 1;
			break;
		}
	}



#else

	[unroll(20)]
	for(unsigned int l = 0; l < LB_NumLights; l++)
	{
		const LightData Light = LB_Lights[l];
		switch(Light.LightType)
		{

			case 1:
				break;

			case 2:
				float3 pointTemp = EvaluatePointLight(diffuseColor,
					specularColor, pixelNormalMap, roughness, Light.Color, Light.Intensity * 100,
					Light.Range * 100, Light.Position, toEye, worldPosition.xyz);

				if(Light.CastShadows)
				{
					float4x4 plViews[6] = { Light.LightView, Light.LightView1, Light.LightView2, Light.LightView3, Light.LightView4, Light.LightView5 };
					if(GetShadowPixel(shadowCubeTexture[l], plViews, Light.LightProjection, Light.Range, Light.Position, worldPosition.xyz, Light.CastShadows))
					{
						pointTemp *= shadow;
					}
				}
				pointLight += pointTemp;
				break;

			case 3:
				float3 spotTemp = EvaluateSpotLight(diffuseColor,
					specularColor, pixelNormalMap, roughness, Light.Color, Light.Intensity * 100,
					Light.Range * 100, Light.Position, Light.Direction, Light.SpotOuterRadius * (3.1451f / 180.0f),
					Light.SpotInnerRadius * (3.1451f / 180.0f), toEye, worldPosition.xyz);

				if(Light.CastShadows)
				{
					if(GetShadowPixel(shadowMap[l], Light.LightView, Light.LightProjection, worldPosition.xyz, Light.CastShadows))
					{
						spotTemp *= shadow;
					}
				}
				spotLight += spotTemp;
				break;
		}
	}

	result.myColor.rgb = LinearToGamma(ambientLighting + directLighting + pointLight + spotLight) + (emissive * albedo * emissiveStr);
	result.myColor.a = albedo.a;

#endif

	return result;
}
