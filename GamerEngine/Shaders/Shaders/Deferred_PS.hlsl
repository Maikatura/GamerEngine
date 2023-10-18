#include "Data/ShaderStructs.hlsli"
#include "Data/Textures.hlsli"

#include "Data/PBRFunctions.hlsli"
#include "Data/Samplers.hlsli"
#include "Data/ShadowFunctions.hlsli"
#include "Data/LightBuffer.hlsli"

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

float ResolveShadow(LightData light, Texture2D aShadowMap, float3 projectedTexCoord, float interleavedGradientNoise)
{
	static const int vogelSampleCount = 16;

	if(light.SmoothShadows)
	{
		return SampleShadowsVogelDisk(aShadowMap, projectedTexCoord, interleavedGradientNoise, light.VogalDiskScale, vogelSampleCount);
	}
	else
	{
		return aShadowMap.SampleLevel(pointClampSampler, projectedTexCoord.xy, 0) >= projectedTexCoord.z;
	}
}

float ResolveShadowCube(LightData light, TextureCube aShadowMap, float projectedDepth, float3 fragToLight, float4 worldToLightView, float interleavedGradientNoise)
{
	static const int vogelSampleCount = 16;

	if(light.SmoothShadows)
	{
		return SampleShadowCubeVogelDisk(aShadowMap, projectedDepth, worldToLightView, light.Range, fragToLight, interleavedGradientNoise, light.VogalDiskScale, vogelSampleCount);
	}
	else
	{
		float depth = aShadowMap.SampleLevel(pointClampSampler, fragToLight, 0).r;
		if(depth < projectedDepth)
		{
			return saturate(worldToLightView.z / (light.Range * light.Range));
		}
		return 1.f;
	}
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



DeferredPixelOutput main(DeferredVertexToPixel input)
{
	DeferredPixelOutput result;

	float4 albedo = albedoTexture.Sample(defaultSampler, input.myUV);
	if(albedo.a == 0)
	{
		discard;
		result.myColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
		return result;
	}

	//Albedo less versions of light render modes
	[flatten]
	if(FB_RenderMode >= 16 && FB_RenderMode <= 19)
	{
		albedo = 1.0f;
	}

	const float3 normal = normalTexture.Sample(defaultSampler, input.myUV).rgb;
	const float4 material = materialTexture.Sample(defaultSampler, input.myUV).rgba;
	const float3 vertexNormal = vertexNormalTexture.Sample(defaultSampler, input.myUV).rgb;
	const float4 worldPosition = worldPositionTexture.Sample(defaultSampler, input.myUV).rgba;
	const float ambientOcclusion = ambientOcclusionTexture.Sample(defaultSampler, input.myUV).r;
	const float ssao = saturate(ssaoTexture.Sample(defaultSampler, input.myUV).r);

	const float metalness = material.r;
	const float roughness = material.g;
	const float emissive = material.b;
	const float emissiveIntensity = material.a;

	const float3 toEye = normalize(FB_CamTranslation.xyz - worldPosition.xyz);
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
    ambientLighting *= 0.2f;

	float3 pointLight = 0;
	float3 spotLight = 0;

	//float interleavedGradientNoise = InterleavedGradientNoise(input.myUV * float2(512, 512));

    float3 directLighting = EvaluateDirectionalLight(
		diffuseColor,
		specularColor,
		normal,
		roughness,
		LB_DirectionalLight.Color.rgb,
		LB_DirectionalLight.Intensity,
		-LB_DirectionalLight.Direction,
		toEye
	);

	float directShadow = 1.0f;

	if(LB_DirectionalLight.CastShadows)
	{
        const float4 worldToLightView = mul(worldPosition, LB_DirectionalLight.LightView[0]);
        const float4 lightViewToLightProj = mul(worldToLightView, LB_DirectionalLight.LightProjection);

        float4 projectedTexCoord = float4(0.0f, 0.0f, 0.0f, 0.0f);

        projectedTexCoord.xy = lightViewToLightProj.xy / (2.0f * lightViewToLightProj.w) + float2(0.5f, -0.5f);

        bool validTexCoord = saturate(projectedTexCoord) == projectedTexCoord;
        bool inFrontOfLight = worldToLightView.z >= 0.0f;

        if (validTexCoord && inFrontOfLight)
        {
            const float shadowBias = 0.00008f;
            float viewDepth = lightViewToLightProj.z / lightViewToLightProj.w - shadowBias;
            projectedTexCoord.z = viewDepth;
    
            float lightDepth = dirLightShadowTexture.SampleLevel(pointClampSampler, projectedTexCoord.xy, 0).r;
    
            if (lightDepth < viewDepth)
            {
                float flaking = 0.8f;
                float shadow = SampleShadowsPCF16(dirLightShadowTexture, projectedTexCoord, 1.0f / 8192.0f);
                directShadow *= saturate(shadow * (1 - flaking) + flaking);
                directLighting *= shadow;
            }
        }
    }

	//[unroll(20)]
	//for(unsigned int l = 0; l < LB_NumLights; l++)
	//{
	//	LightData Light = LB_Lights[l];
	//	switch(Light.LightType)
	//	{
	//		case 2:
	//		{
	//			float3 pointTemp = EvaluatePointLight(diffuseColor,
	//				specularColor, normal, roughness, Light.Color, Light.Intensity,
	//				Light.Range, Light.Position, toEye, worldPosition.xyz);

	//			if(Light.CastShadows)
	//			{

	//				if(GetShadowPixel(shadowCubeTexture[l], Light.LightView, Light.LightProjection, Light.Range, Light.Position, worldPosition.xyz))
	//				{
	//					const float shadow = 0.001f;
	//					pointTemp *= shadow;
	//				}
	//			}
	//			pointLight += pointTemp;
	//			break;
	//		}
	//		case 3:
	//		{
	//			float3 spotTemp = EvaluateSpotLight(diffuseColor,
	//				specularColor, normal, roughness, Light.Color, Light.Intensity,
	//				Light.Range, Light.Position, Light.Direction, Light.SpotOuterRadius * (3.1451f / 180.0f),
	//				Light.SpotInnerRadius * (3.1451f / 180.0f), toEye, worldPosition.xyz);

	//			if(Light.CastShadows)
	//			{
	//				if(GetShadowPixel(shadowMap[l], Light.LightView[0], Light.LightProjection, worldPosition.xyz))
	//				{
	//					const float shadow = 0.001f;
	//					spotTemp *= shadow;
	//				}
	//			}
	//			spotLight += spotTemp;
	//			break;
	//		}
	//	}
	//}

    float emissiveStrength = 0.0f;
	float3 emissiveColor = emissive * emissiveIntensity * albedo.xyz * emissiveStrength;
    float3 finalColor = directLighting + ambientLighting + emissiveColor + ((pointLight + spotLight)/* * directShadow*/);


	switch(FB_RenderMode)
	{
		default:
		case 0:// RenderMode::Default:
			result.myColor.rgb = finalColor;
			result.myColor.a = 1.0f;
			break;
		case 1://RenderMode::TexCoord1:
			result.myColor.rgb = float3(input.myUV.xy, 0.0f);
			result.myColor.a = 1.0f;
			break;
		case 2://RenderMode::VertexColor:
			result.myColor.rgb = albedo.rgb; //Albedo is set to VertexColor in GBuffer when RenderMode::VertexColor
			result.myColor.a = 1.0f;
			break;
		case 3://RenderMode::VertexNormal:
		{
			float3 debugNormal = vertexNormal;
			float signedLength = (debugNormal.r + debugNormal.g + debugNormal.b) / 3;

			if(signedLength < 0)
			{
				debugNormal = float3(1 - abs(debugNormal));
			}
			result.myColor.rgb = debugNormal;
			result.myColor.a = 1.0f;
			break;
		}
		case 4://RenderMode::PixelNormal:
		{
			float3 debugNormal = normal;
			float signedLength = (debugNormal.r + debugNormal.g + debugNormal.b) / 3;

			if(signedLength < 0)
			{
				debugNormal = float3(1 - abs(debugNormal));
			}
			result.myColor.rgb = debugNormal;
			result.myColor.a = 1.0f;
			break;
		}
		case 5://RenderMode::AlbedoMap:
		{
			float3 baseMyColorMapSample = albedoTexture.Sample(defaultSampler, input.myUV).rgb;
			result.myColor = float4(baseMyColorMapSample, 1.0f);
			result.myColor.a = 1.0f;
			break;
		}
		case 6://RenderMode::NormalMap:
		{
			result.myColor = float4(normal.r, normal.g, 1.0f, 1.0f); //normal is set to NormalMap in GBuffer when RenderMode::NormalMap
			result.myColor.a = 1.0f;
			break;
		}
		case 7://RenderMode::DirectLight:
			result.myColor.rgb = directLighting;
			result.myColor.a = 1.0f;
			break;
		case 8://RenderMode::AmbientLight:
			result.myColor.rgb = finalColor;
			//result.myColor.rgb = ambientLighting;
			result.myColor.a = 1.0f;
			break;
		case 9://RenderMode::PointLight:
			result.myColor.rgb = pointLight;
			result.myColor.a = 1.0f;
			break;
		case 10://RenderMode::SpotLight:
			result.myColor.rgb = spotLight;
			result.myColor.a = 1.0f;
			break;
		case 11://RenderMode::AmbientOcclusion:
			result.myColor.rgb = ambientOcclusion;
			result.myColor.a = 1.0f;
			break;
		case 12: //RenderMode::SSAO:
			result.myColor.rgb = ssao;
			result.myColor.a = 1.0f;
			break;
		case 13://RenderMode::Metalness:
			result.myColor.rgb = metalness;
			result.myColor.a = 1.0f;
			break;
		case 14://RenderMode::Roughness:
			result.myColor.rgb = roughness;
			result.myColor.a = 1.0f;
			break;
		case 15://RenderMode::Emission:
			result.myColor.rgb = emissiveColor;
			result.myColor.a = 1.0f;
			break;
		case 16://RenderMode::DirectLightNoAlbedo:
			result.myColor.rgb = directLighting;
			result.myColor.a = 1.0f;
			break;
		case 17://RenderMode::AmbientLightNoAlbedo:
			//result.myColor.rgb = finalColor;
			result.myColor.rgb = ambientLighting;
			result.myColor.a = 1.0f;
			break;
		case 18://RenderMode::PointLightNoAlbedo:
			result.myColor.rgb = pointLight;
			result.myColor.a = 1.0f;
			break;
		case 19://RenderMode::SpotLightNoAlbedo:
			result.myColor.rgb = spotLight;
			result.myColor.a = 1.0f;
			break;
	}


	return result;
}
