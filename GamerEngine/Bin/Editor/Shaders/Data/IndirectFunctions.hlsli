#ifndef INDIRECT_FUNCTIONS_HLSLI
#define INDIRECT_FUNCTIONS_HLSLI

#include "Samplers.hlsli"
#include "Common.hlsli"

#define PI 3.14159265358979323846f

float InterleavedGradientNoise(float2 position_screen)
{
    float3 magic = float3(0.06711056f, 0.00583715f, 52.9829189f);
    return frac(magic.z * frac(dot(position_screen, magic.xy)));
}

float2 VogelDiskSample(int sampleIndex, int samplesCount, float phi)
{
    float GoldenAngle = 2.4f;

    float r = sqrt(sampleIndex + 0.5f) / sqrt(samplesCount);
    float theta = sampleIndex * GoldenAngle + phi;

    float sine, cosine;
    sincos(theta, sine, cosine);

    return float2(r * cosine, r * sine);
}

float3 VogelDiskSampleCube(int sampleIndex, int samplesCount, float phi)
{
    float GoldenAngle = 2.4f;

    float r = sqrt(sampleIndex + 0.5f) / sqrt(samplesCount);
    float theta = sampleIndex * GoldenAngle + phi;

    float sine, cosine;
    sincos(theta, sine, cosine);

    return float3(r * cosine, r * sine, r * cosine * sine);
}

float3 CalculateIndirectLighting(float lKernelSize, float lIntensity, float3 fWorldPos, float3 fAlbedoColor, float3 fWorldNormal, float3 lightSpacePosPostW, float gradient, float3 gLightPos, Texture2D noiseMap, Texture2D positionMap, Texture2D normalMap, Texture2D fluxMap)
{
    const int NUM_SAMPLES = 64;
    const float TEXEL_SIZE = 1.f / 512.f;

    float3 N = normalize(fWorldNormal);
    float3 indirect = float3(0.f, 0.f, 0.f);

#ifdef _DEBUG
    [loop]
#else 
    [unroll]
#endif
    for(int i = 0; i < NUM_SAMPLES; ++i)
    {
        float2 offset = POISSON_SAMPLES[i];

        // This adds per fragment noise, which spreads bounce out better, but adds temporal flickering
        // Potential fix: https://cs.dartmouth.edu/wjarosz/publications/mara17towards.pdf

        //float2 random = VogelDiskSample(i, NUM_SAMPLES, InterleavedGradientNoise(fWorldPos.xy)) * 0.4f;
        //float2 texCoords = lightSpacePosPostW.xy + random + (offset * TEXEL_SIZE * lKernelSize);

        float2 texCoords = lightSpacePosPostW.xy + offset * TEXEL_SIZE * lKernelSize;
        if(texCoords.x < 0 || texCoords.y < 0 || texCoords.x > 1 || texCoords.y > 1)
        {
            continue;
        }
        float3 flux = fluxMap.SampleLevel(defaultSampler, texCoords, 0).xyz;
        if(flux.r + flux.g + flux.b <= 0)
        {
            continue;
        }
        float3 worldPos = positionMap.SampleLevel(defaultSampler, texCoords, 0).xyz;
        float3 worldNormal = normalMap.SampleLevel(defaultSampler, texCoords, 0).xyz;
        float3 lightContrib = flux * max(0.0f, dot(worldNormal, fWorldPos - worldPos)) * max(0.0f, dot(N, worldPos - fWorldPos)) / pow(length(fWorldPos - worldPos), 4);
        lightContrib *= offset.x * offset.x;

        float3 cDiff = fAlbedoColor / PI;
        indirect += lightContrib * cDiff;
    }
    return saturate(indirect * lIntensity);
}

float3 CalculateIndirectLightingCubeMap(float lKernelSize, float lIntensity, float3 fWorldPos, float3 fAlbedoColor, float3 fWorldNormal, float3 gLightPos, Texture2D noiseMap, TextureCube positionMap, TextureCube normalMap, TextureCube fluxMap)
{
    const int NUM_SAMPLES = 64;

    float3 N = normalize(fWorldNormal);
    float3 indirect = float3(0.f, 0.f, 0.f);

    for(int i = 0; i < NUM_SAMPLES; ++i)
    {
        float3 offset = POISSON_SAMPLES_3D[i];
        float3 wPos = fWorldPos + offset * lKernelSize;
        float3 texCoords = (wPos - gLightPos);

        float3 flux = fluxMap.SampleLevel(defaultSampler, texCoords, 0).xyz;
        if(flux.r + flux.g + flux.b <= 0)
        {
            continue;
        }
        float3 worldPos = positionMap.SampleLevel(defaultSampler, texCoords, 0).xyz;
        float3 worldNormal = normalMap.SampleLevel(defaultSampler, texCoords, 0).xyz;
        float3 lightContrib = flux * max(0.0f, dot(worldNormal, fWorldPos - worldPos)) * max(0.0f, dot(N, worldPos - fWorldPos)) / pow(length(fWorldPos - worldPos), 4);
        lightContrib *= offset.x * offset.x;

        float3 cDiff = fAlbedoColor / PI;
        indirect += lightContrib * cDiff;
    }
    return saturate(indirect * lIntensity);
}
#endif