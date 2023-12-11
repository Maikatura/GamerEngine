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

// Inline function for shadow sampling
inline bool SampleShadow(Texture2D aShadowMap, float4x4 aLightView, float4x4 aLightProjection, float3 aWorldPosition)
{
    float4 w2lView = mul(aLightView, float4(aWorldPosition, 1));
    float4 v2lProj = mul(aLightProjection, w2lView);

    float2 projectedTexCoord;
    projectedTexCoord.x = v2lProj.x / v2lProj.w / 2.0f + 0.5f;
    projectedTexCoord.y = -v2lProj.y / v2lProj.w / 2.0f + 0.5f;

    if (saturate(projectedTexCoord.x) == projectedTexCoord.x && saturate(projectedTexCoord.y) == projectedTexCoord.y)
    {
        float vDepth = (v2lProj.z / v2lProj.w) - SHADOW_BIAS;
        float lDepth = aShadowMap.Sample(pointClampSampler, projectedTexCoord).r;
        if (lDepth < vDepth)
        {
            return true;
        }
    }
    return false;
}

PixelOutput main(VertexToPixel input)
{
	PixelOutput result = { 0,0,0,0 };

	const float4 albedo = albedoTexture.Sample(defaultSampler, input.UV);

	if(albedo.a <= 0.05f)
	{
		discard;
		result.Color = float4(0.0f, 0.0f, 0.0f, 0.0f);
		return result;
	}


    const float3 normalMap = normalTexture.Sample(defaultSampler, input.UV).rgb;
    const float4 material = materialTexture.Sample(defaultSampler, input.UV).rgba;

    const float3 worldPosition = input.WorldPosition;
    const float ambientOcclusion = normalMap.b;
    const float ssao = saturate(ssaoTexture.Sample(defaultSampler, input.UV).r);

    const float metalness = material.r;
    const float roughness = material.g;
    const float emissive = material.b;
    const float emissiveStr = material.a;

    const float3 toEye = normalize(FB_CamTranslation.xyz - worldPosition);

	float3 specularColor = lerp(float3(0.04f, 0.04f, 0.04f), albedo, metalness);
	float3 diffuseColor = lerp(float3(0.0f, 0.0f, 0.0f), albedo, 1 - metalness);

    float3 normal = normalize(input.Normal);
    float3 tangent = normalize(input.Tangent);
    float3 binormal = normalize(input.Binormal);

    // Calculate the TBN matrix (Tangent, Binormal, Normal)
    float3x3 TBN = float3x3(tangent, binormal, normal);

    float3 sampledNormal = normalize(normalMap.xyz);
    float3 worldNormal = mul(sampledNormal, TBN);

    float3 pixelNormal = normalize(input.WorldNormal);

	float3 ambientLighting = EvaluateAmbience(
		environmentTexture,
		pixelNormal,
		input.WorldPosition,
		toEye,
		roughness,
		ambientOcclusion,
		diffuseColor,
		specularColor
	);

	//ambientLighting *= ssao;

	float3 pointLight = 0;
	float3 spotLight = 0;

	//float interleavedGradientNoise = InterleavedGradientNoise(input.UV * float2(512, 512));

	float3 directLighting = EvaluateDirectionalLight(
		diffuseColor,
		specularColor,
		pixelNormal,
		roughness,
		LB_DirectionalLight.Color.rgb,
		LB_DirectionalLight.Intensity,
		-LB_DirectionalLight.Direction,
		toEye
	);

	float directShadow = 1.0f;


    if (LB_DirectionalLight.CastShadows)
    {
        const float4 worldToLightView = mul(worldPosition, LB_DirectionalLight.LightView[0]);
        const float4 lightViewToLightProj = mul(worldToLightView, LB_DirectionalLight.LightProjection);

        float4 projectedTexCoord = float4(0.0f, 0.0f, 0.0f, 0.0f);

        projectedTexCoord.xy = lightViewToLightProj.xy / (2.0f * lightViewToLightProj.w) + float2(0.5f, -0.5f);

        bool validTexCoord = saturate(projectedTexCoord) == projectedTexCoord;
        bool inFrontOfLight = worldToLightView.z >= 0.0f;

        if (validTexCoord && inFrontOfLight)
        {
            const float shadowBias = 0.008f;
            float viewDepth = lightViewToLightProj.z / lightViewToLightProj.w - shadowBias;
            projectedTexCoord.z = viewDepth;
    
            float lightDepth = dirLightShadowTexture.SampleLevel(pointClampSampler, projectedTexCoord.xy, 0).r;
    
            if (lightDepth < viewDepth)
            {
                float flaking = 0.8f;
                float shadow = SampleShadowsPCF16(dirLightShadowTexture, projectedTexCoord.xyz, 1.0f / 8192.0f);
                directShadow *= saturate(shadow * (1 - flaking) + flaking);
                directLighting *= shadow;
            }
        }
    }

	[unroll(20)]
	for(unsigned int l = 0; l < LB_NumLightsPoint; l++)
	{
		LightData Light = LB_LightsPoint[l];
		switch(Light.LightType)
		{
			case 2:
			{
				float3 pointTemp = EvaluatePointLight(diffuseColor,
					specularColor, pixelNormal, roughness, Light.Color, Light.Intensity,
					Light.Range, Light.Position, toEye, worldPosition.xyz);

				if(Light.CastShadows)
				{
					
					if(GetShadowPixel(shadowCubeTexture[l], Light.LightView, Light.LightProjection, Light.Range, Light.Position, worldPosition.xyz , .05f, Light.CastShadows))
					{
						const float shadow = 0.05f;
						pointTemp *= shadow;
					}
				}
				pointLight += pointTemp;
				break;
			}
			
		}
	}

	[unroll(20)]
	for(unsigned int l = 0; l < LB_NumLightsSpot; l++)
	{
		LightData Light = LB_LightsSpot[l];
		switch(Light.LightType)
		{
			case 3:
			{
				float3 spotTemp = EvaluateSpotLight(diffuseColor,
					specularColor, pixelNormal, roughness, Light.Color, Light.Intensity,
					Light.Range, Light.Position, Light.Direction, Light.SpotOuterRadius * (3.1451f / 180.0f),
					Light.SpotInnerRadius * (3.1451f / 180.0f), toEye, worldPosition.xyz);

				if(Light.CastShadows)
				{
				
					if(GetShadowPixel(shadowMap[l], Light.LightView[0], Light.LightProjection, worldPosition.xyz, .0001f, Light.CastShadows))
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
	float3 finalColor = directLighting + ambientLighting + emissiveColor +((pointLight + spotLight));


	switch(FB_RenderMode)
	{
		default:
		case 0:// RenderMode::Default:
			result.Color.rgb = finalColor;
			result.Color.a = 1.0f;
			break;
		case 1://RenderMode::TexCoord1:
			result.Color.rgb = float3(input.UV.xy, 0.0f);
			result.Color.a = 1.0f;
			break;
		case 2://RenderMode::VertexColor:
			result.Color.rgb = albedo.rgb; //Albedo is set to VertexColor in GBuffer when RenderMode::VertexColor
			result.Color.a = 1.0f;
			break;
		case 3://RenderMode::VertexNormal:
		{
			float3 debugNormal = pixelNormal;
			float signedLength = (debugNormal.r + debugNormal.g + debugNormal.b) / 3;

			if(signedLength < 0)
			{
				debugNormal = float3(1 - abs(debugNormal));
			}
			result.Color.rgb = debugNormal;
			result.Color.a = 1.0f;
			break;
		}
		case 4://RenderMode::PixelNormal:
		{
			float3 debugNormal = normalMap;
			float signedLength = (debugNormal.r + debugNormal.g + debugNormal.b) / 3;

			if(signedLength < 0)
			{
				debugNormal = float3(1 - abs(debugNormal));
			}
			result.Color.rgb = debugNormal;
			result.Color.a = 1.0f;
			break;
		}
		case 5://RenderMode::AlbedoMap:
		{
			float3 basemyColorMapSample = albedoTexture.Sample(defaultSampler, input.UV).rgb;
			result.Color = float4(basemyColorMapSample, 1.0f);
			result.Color.a = 1.0f;
			break;
		}
		case 6://RenderMode::NormalMap:
		{
			result.Color = float4(normalMap.r, normalMap.g, 1.0f, 1.0f); //normal is set to NormalMap in GBuffer when RenderMode::NormalMap
			result.Color.a = 1.0f;
			break;
		}
		case 7://RenderMode::DirectLight:
			result.Color.rgb = directLighting;
			result.Color.a = 1.0f;
			break;
		case 8://RenderMode::AmbientLight:
			result.Color.rgb = ambientLighting;
			result.Color.a = 1.0f;
			break;
		case 9://RenderMode::PointLight:
			result.Color.rgb = pointLight;
			result.Color.a = 1.0f;
			break;
		case 10://RenderMode::SpotLight:
			result.Color.rgb = spotLight;
			result.Color.a = 1.0f;
			break;
		case 11://RenderMode::AmbientOcclusion:
			result.Color.rgb = ambientOcclusion;
			result.Color.a = 1.0f;
			break;
		case 12: //RenderMode::SSAO:
			result.Color.rgb = ssao;
			result.Color.a = 1.0f;
			break;
		case 13://RenderMode::Metalness:
			result.Color.rgb = metalness;
			result.Color.a = 1.0f;
			break;
		case 14://RenderMode::Roughness:
			result.Color.rgb = roughness;
			result.Color.a = 1.0f;
			break;
		case 15://RenderMode::Emission:
			result.Color.rgb = emissiveColor;
			result.Color.a = 1.0f;
			break;
		case 16://RenderMode::DirectLightNoAlbedo:
			result.Color.rgb = directLighting;
			result.Color.a = 1.0f;
			break;
		case 17://RenderMode::AmbientLightNoAlbedo:
			result.Color.rgb = ambientLighting;
			result.Color.a = 1.0f;
			break;
		case 18://RenderMode::PointLightNoAlbedo:
			result.Color.rgb = pointLight;
			result.Color.a = 1.0f;
			break;
		case 19://RenderMode::SpotLightNoAlbedo:
			result.Color.rgb = spotLight;
			result.Color.a = 1.0f;
			break;
	}



	return result;
}
