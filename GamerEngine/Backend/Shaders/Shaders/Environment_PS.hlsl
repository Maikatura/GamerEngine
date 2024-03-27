
#include "Data/ShaderStructs.hlsli"
#include "Data/Textures.hlsli"

#include "Data/PBRFunctions.hlsli"
#include "Data/Samplers.hlsli"
#include "Data/ShadowFunctions.hlsli"
#include "Data/LightBuffer.hlsli"

PixelOutput main(FullscreenVertexToPixel input)
{
    PixelOutput result;

    const float4 albedo = albedoTexture.Sample(defaultSampler, input.UV);
    if (albedo.a <= 0.05f)
    {
        result.Color.rgb = 1;
        discard;
        return result;
    }

    const float3 normal = normalTexture.Sample(defaultSampler, input.UV).rgb;
    const float4 material = materialTexture.Sample(defaultSampler, input.UV);
    const float3 vertexNormal = vertexNormalTexture.Sample(defaultSampler, input.UV).rgb;
    const float4 worldPosition = worldPositionTexture.Sample(defaultSampler, input.UV);
    const float ambientOcclusion = ambientOcclusionTexture.Sample(defaultSampler, input.UV).r;

    const float metalness = material.r;
    const float roughness = material.g;
    const float emissve = material.b;
    const float emissiveStr = material.a;

    const float3 toEye = normalize(FB_CamTranslation.xyz - worldPosition.xyz);
    const float3 specularColor = lerp((float3) 0.04f, albedo.rgb, metalness);
    const float3 diffuseColor = lerp((float3) 0.00f, albedo.rgb, 1 - metalness);


    float3 pointLight = 0;
    float3 spotLight = 0;

   // for (unsigned int i = 0; i < LB_NumLights; i++)
   // {
   //     const LightData Light = LB_Lights[i];
   //     switch (Light.LightType)
   //     {
   //         default:
   //             break;
   //
   //         case 0: // This could be directional light
   //             break;
   //
   //         case 1:
	//		
   //             pointLight += EvaluatePointLight(diffuseColor, specularColor, normal, material.g, Light.Color, Light.Intensity, Light.Range, Light.Position, toEye, worldPosition.xyz);
   //             break;
   //
   //         case 2: //Spot Light
   //             break;
   //
   //         case 3: //Area Lights, etc
   //             break;
   //     }
   // }

    const float3 ambientLightning = EvaluateAmbience(
		environmentTexture,
		normal,
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
		normal,
		roughness,
		LB_DirectionalLight.Color,
		LB_DirectionalLight.Intensity,
		-LB_DirectionalLight.Direction,
		toEye
	);
    float3 finalColor = directLighting + ambientLightning + ((pointLight + spotLight));
    result.Color.rgb = finalColor;
    result.Color.a = 1;

    return result;
}
