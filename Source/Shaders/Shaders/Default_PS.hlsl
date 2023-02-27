#define MAX_LIGHTS 8
#include "Data/ShaderStructs.hlsli"
#include "Data/BufferData.hlsli"

#include "Data/PBRFunctions.hlsli"
#include "Data/Samplers.hlsli"
#include "Data/ShadowFunctions.hlsli"
#include "Data/LightBuffer.hlsli"

float ResolveShadow(LightData light, Texture2D aShadowMap, float3 projectedTexCoord, float interleavedGradientNoise)
{
	static const int vogelSampleCount = 16;

	return aShadowMap.SampleLevel(pointClampSampler, projectedTexCoord.xy, 0) >= projectedTexCoord.z;

}

float ResolveShadowCube(LightData light, TextureCube aShadowMap, float projectedDepth, float3 fragToLight, float4 worldToLightView, float interleavedGradientNoise)
{
	static const int vogelSampleCount = 16;


	float depth = aShadowMap.SampleLevel(pointClampSampler, fragToLight, 0).r;
	if(depth < projectedDepth)
	{
		return saturate(worldToLightView.z / (light.Range * light.Range));
	}
	return 1.f;
}

float4 GetViewPosition(float2 uv)
{
	const float4 worldPosition = float4(ambientOcclusionTexture.Sample(defaultSampler, uv).rgb, 1);
	const float4 viewPosition = mul(FB_ToView, worldPosition);
	return viewPosition;
}

float4 GetViewNormal(float2 uv)
{
	const float4 worldNormal = float4(normalTexture.Sample(defaultSampler, uv).rgb, 0);
	const float4 viewNormal = mul(FB_ToView, worldNormal);
	return viewNormal;
}

float2 GetRandom(float2 uv, float2 uvScale)
{
	const float3 random = 2.0f * blueNoiseTexture.Sample(pointWrapSampler, uv * uvScale).rgb - 1.0f;
	return random.xy;
}


bool GetShadowPixel(Texture2D aShadowMap, float4x4 aLightView, float4x4 aLightProjection, float3 aWorldPosition)
{

	float shadowBias = 0.01f;

	float4 w2lView = mul(aLightView, float4(aWorldPosition, 1));
	float4 v2lProj = mul(aLightProjection, w2lView);

	float2 projectedTexCoord;
	projectedTexCoord.x = v2lProj.x / v2lProj.w / 2.0f + 0.5f;
	projectedTexCoord.y = -v2lProj.y / v2lProj.w / 2.0f + 0.5f;

	if(saturate(projectedTexCoord.x) == projectedTexCoord.x && saturate(projectedTexCoord.y) == projectedTexCoord.y)
	{
		float vDepth = (v2lProj.z / v2lProj.w) - shadowBias;
		float lDepth = aShadowMap.Sample(pointClampSampler, projectedTexCoord).r;
		if(lDepth < vDepth)
		{
			return true;
		}
	}
	return false;
}

bool GetShadowPixel(TextureCube aShadowMap, float4x4 aLightView[6], float4x4 aLightProjection, float aRange, float3 aLightPosition, float3 aWorldPosition)
{
	float shadowBias = 0.01f;

	[unroll(6)]
	for(int face = 0; face < 6; face++)
	{
		float4 w2lView = mul(aLightView[face], float4(aWorldPosition, 1));
		float4 v2lProj = mul(aLightProjection, w2lView);

		float3 fragToLight = aWorldPosition - aLightPosition;

		float2 lightUV = 0;
		lightUV.x = v2lProj.x / v2lProj.w / 2.f + .5f;
		lightUV.y = -v2lProj.y / v2lProj.w / 2.f + .5f;

		if(saturate(lightUV.x) == lightUV.x && saturate(lightUV.y) == lightUV.y && w2lView.z > 0)
		{
			float vDepth = (v2lProj.z / v2lProj.w) - shadowBias;
			float lDepth = aShadowMap.Sample(pointClampSampler, fragToLight).r;
			if(lDepth < vDepth) return true;
		}
	}
	return false;
}

PixelOutput main(VertexToPixel input)
{
	PixelOutput result = { 0,0,0,0 };

	const float4 albedo = albedoTexture.Sample(defaultSampler, input.myUV);

	if(albedo.a <= 0.05f)
	{
		discard;
		result.myColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
		return result;
	}


	const float3 normal = normalTexture.Sample(defaultSampler, input.myUV).rgb;
	const float4 material = materialTexture.Sample(defaultSampler, input.myUV).rgba;
	const float3 vertexNormal = vertexNormalTexture.Sample(defaultSampler, input.myUV).rgb;
	const float4 worldPosition = worldPositionTexture.Sample(defaultSampler, input.myUV).rgba;
	const float ambientOcclusion = normal.b;
	const float ssao = saturate(SSAOTexture.Sample(defaultSampler, input.myUV).r);

	const float metalness = material.r;
	const float roughness = material.g;
	const float emissive = material.b;
	const float emissiveStr = material.a;

	const float3 toEye = normalize(FB_CamTranslation.xyz - input.myPosition.xyz);
	float3 specularColor = lerp(float3(0.04f, 0.04f, 0.04f), albedo.rgb, metalness);
	float3 diffuseColor = lerp(float3(0.0f, 0.0f, 0.0f), albedo.rgb, 1 - metalness);



	float3 ambientLighting = EvaluateAmbience(
		environmentTexture,
		normal,
		vertexNormal,
		toEye,
		roughness,
		ambientOcclusion,
		diffuseColor,
		specularColor
	);

	ambientLighting *= ssao;

	float3 pointLight = 0;
	float3 spotLight = 0;
	float3 directLighting = 0;

	float directShadow = 1.0f;
	float pointLightShadow = 1.0f;

#ifdef _DEBUG

	switch(FB_RenderMode)
	{
		default:
			break;
		case 0: // Default
		{
#endif
			[unroll(20)]
			for(unsigned int l = 0; l < LB_NumLights; l++)
			{
				LightData Light = LB_Lights[l];
				switch(Light.LightType)
				{
					case 1:
					{
						directLighting = EvaluateDirectionalLight(
							diffuseColor,
							specularColor,
							normal,
							roughness,
							Light.Color,
							Light.Intensity * 0.1f,
							-Light.Direction,
							toEye
						);

						if(LB_DirectionalLight.CastShadows)
						{
							const float4 worldToLightView = mul(Light.LightView[0], worldPosition);
							const float4 lightViewToLightProj = mul(Light.LightProjection, worldToLightView);

							float3 projectedTexCoord;
							projectedTexCoord.x = lightViewToLightProj.x / lightViewToLightProj.w / 2.0f + 0.5f;
							projectedTexCoord.y = -lightViewToLightProj.y / lightViewToLightProj.w / 2.0f + 0.5f;

							if(saturate(projectedTexCoord.x) == projectedTexCoord.x && saturate(projectedTexCoord.y) == projectedTexCoord.y && worldToLightView.z >= 0)
							{
								const float shadowBias = 0.0001f;
								const float viewDepth = (lightViewToLightProj.z / lightViewToLightProj.w) - shadowBias;
								projectedTexCoord.z = viewDepth;

								float flaking = 0.8f;
								float shadow = ResolveShadow(LB_DirectionalLight, dirLightShadowMap, projectedTexCoord, 0.0015f);
								directShadow *= saturate(shadow * (1 - flaking) + flaking);
								directLighting *= shadow;
							}
						}
						//directLighting += tempDir;

						break;
					}
					case 2:
					{

						float3 pointTemp = EvaluatePointLight(diffuseColor,
							specularColor, normal, roughness, Light.Color, Light.Intensity,
							Light.Range, Light.Position, toEye, worldPosition.xyz);

						if(Light.CastShadows)
						{

							if(GetShadowPixel(shadowCubeTexture[l], Light.LightView, Light.LightProjection, Light.Range, Light.Position, worldPosition.xyz))
							{
								const float shadow = 0.001f;
								pointTemp *= shadow;
							}
						}
						pointLight += pointTemp;

						break;
					}
					case 3:
					{
						float3 spotTemp = EvaluateSpotLight(diffuseColor,
							specularColor, normal, roughness, Light.Color, Light.Intensity,
							Light.Range, Light.Position, Light.Direction, Light.SpotOuterRadius * (3.1451f / 180.0f),
							Light.SpotInnerRadius * (3.1451f / 180.0f), toEye, worldPosition.xyz);

						if(Light.CastShadows)
						{
							if(GetShadowPixel(shadowMap[l], Light.LightView[0], Light.LightProjection, worldPosition.xyz))
							{
								const float shadow = 0.001f;
								spotTemp *= shadow;
							}
						}
						spotLight += spotTemp;
						break;
					}
				}
			}

			float emissiveStrength = 0.0f;
			float3 emissiveColor = emissive * emissiveStr * albedo.xyz * emissiveStrength;
			float3 finalColor = directLighting + ambientLighting + emissiveColor + ((pointLight + spotLight));

			result.myColor.rgb = finalColor;
			result.myColor.a = 1.0f;

#ifdef _DEBUG
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
			float3 debugNormal = normal;
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
			result.myColor.rgb = ambientOcclusion * ssao;
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
			float4 color = albedoTexture.Sample(defaultSampler, input.myUV);
			float luminance = dot(color.rgb, float3(.2f, .75f, .6f));
			float cutOff = .82f;
			luminance = saturate(luminance - cutOff);
			result.myColor = color * luminance * (1.0f / cutOff);
			result.myColor.a = 1;
			break;
		}
		case 14:
		{
			result.myColor.rgb = (GetViewNormal(input.myUV) + 1.0f) / 2.0f;
			result.myColor.a = 1;
			break;
		}
		case 15:
		{
			result.myColor.rgb = (GetViewPosition(input.myUV) + 1.0f) / 2.0f;
			result.myColor.a = 1;
			break;
		}
	}

#endif



	return result;
}
