#define MAX_LIGHTS 8
#include "Data/LightBuffer.hlsli"
#include "Data/ShaderStructs.hlsli"
#include "Data/PBRFunctions.hlsli"

PixelOutput main(VertexToPixel input)
{
	PixelOutput result = { 0,0,0,0 };




	const float4 albedo = albedoTexture.Sample(defaultSampler, input.myUV);

	if(albedo.a == 0)
	{
		discard;
		result.myColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
		return result;
	}


	// TGA weirdness R:unused | G:norm.y | B:AO | A:norm.x
	const float3 normalMap = normalTexture.Sample(defaultSampler, input.myUV).agb;
	// TGA weirdness R:metal | G:rough | B:emissive | A:emissiveStr
	const float4 materialMap = materialTexture.Sample(defaultSampler, input.myUV);

	const float ambientOcclusion = normalMap.z;
	const float ssaoValue = saturate(SSAOTexture.Sample(defaultSampler, input.myUV).r);

	//AO = 1;
	const float metalness = materialMap.r;
	const float roughness = materialMap.g;
	const float emissive = materialMap.b;
	const float emissiveStr = materialMap.a;

	const float3 toEye = normalize(FB_CamTranslation.xyz - input.myVxPosition.xyz);
	const float3 specularColor = lerp((float3)0.04f, albedo, metalness);
	const float3 diffuseColor = lerp((float3)0.01f, albedo, 1 - metalness);

	const float3x3 TBN = float3x3(
		normalize(input.myTangent),
		normalize(input.myBinormal),
		normalize(input.myNormal)
		);

	float3 pixelNormal = CalculatePixelNormal(normalMap, TBN);


	float3 ambientLighting = EvaluateAmbience(
		environmentTexture,
		pixelNormal,
		input.myNormal,
		toEye,
		roughness,
		ambientOcclusion,
		diffuseColor,
		specularColor
	);

	ambientLighting *= ssaoValue;

	float shadow = .001f;

	float3 directLighting = 0;
	float3 pointLight = 0;
	float3 spotLight = 0;

	float3 dirTemp = EvaluateDirectionalLight(
		diffuseColor,
		specularColor,
		pixelNormal,
		roughness,
		LB_DirectionalLight.Color,
		LB_DirectionalLight.Intensity,
		-LB_DirectionalLight.Direction,
		toEye
	);

	if(LB_DirectionalLight.CastShadows)
	{
		if(GetShadowPixel(dirLightShadowMap, LB_DirectionalLight.LightView, LB_DirectionalLight.LightProjection, pixelNormal.xyz, LB_DirectionalLight.CastShadows))
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

#endif
			[unroll(20)]
			for(unsigned int l = 0; l < LB_NumLights; l++)
			{
				const LightData Light = LB_Lights[l];
				switch(Light.LightType)
				{

					case 1:
						break;

					case 2:
					{
						float3 pointTemp = EvaluatePointLight(diffuseColor,
							specularColor, pixelNormal, roughness, Light.Color, Light.Intensity * 100,
							Light.Range * 100, Light.Position, toEye, input.myNormal.xyz);

						if(Light.CastShadows)
						{
							float4x4 plViews[6] = { Light.LightView, Light.LightView1, Light.LightView2, Light.LightView3, Light.LightView4, Light.LightView5 };
							if(GetShadowPixel(shadowCubeTexture[l], plViews, Light.LightProjection, Light.Range, Light.Position, input.myNormal.xyz, Light.CastShadows))
							{
								pointTemp *= shadow;
							}
						}
						pointLight += pointTemp;
						break;
					}
					case 3:
					{
						float3 spotTemp = EvaluateSpotLight(diffuseColor,
							specularColor, pixelNormal, roughness, Light.Color, Light.Intensity * 100,
							Light.Range * 100, Light.Position, Light.Direction, Light.SpotOuterRadius * (3.1451f / 180.0f),
							Light.SpotInnerRadius * (3.1451f / 180.0f), toEye, input.myNormal.xyz);

						if(Light.CastShadows)
						{
							if(GetShadowPixel(shadowMap[l], Light.LightView, Light.LightProjection, input.myNormal.xyz, Light.CastShadows))
							{
								spotTemp *= shadow;
							}
						}
						spotLight += spotTemp;
						break;
					}
				}
			}
			float3 emissiveColor = emissive * albedo.xyz * emissiveStr;
			result.myColor.rgb =(directLighting + ambientLighting + pointLight + spotLight + emissiveColor);
			result.myColor.a = 1;


#ifdef _DEBUG
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
			result.myColor.rgb = lerp(albedo, float3(1, 1, 1), pow(fresnel, 2));
			result.myColor.a = 1;
			break;
		}
		case 9:
		{
			result.myColor.rgb = ambientOcclusion * ssaoValue;
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
