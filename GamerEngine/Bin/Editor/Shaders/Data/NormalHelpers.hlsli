#ifndef NORMAL_HELPERS_HLSLI
#define NORMAL_HELPERS_HLSLI

float3 UnpackNormal(float3 aNormal)
{
    aNormal.z = 0;
    aNormal = 2.0f * aNormal - 1;
    aNormal.z = sqrt(1 - saturate(dot(aNormal.xy, aNormal.xy)));

    return normalize(aNormal);
}

float3x3 ComposeTBN(float3 aTangent, float3 aBinormal, float3 aNormal)
{
    return float3x3
	(
        normalize(aTangent),
        normalize(aBinormal),
        normalize(aNormal)
    );
}

float3 CalculatePixelNormal(float3 aPackedNormal, float3 aTangent, float3 aBinormal, float3 aVertexNormal)
{
    return normalize(mul(UnpackNormal(aPackedNormal), ComposeTBN(aTangent, aBinormal, aVertexNormal)));
}

float3 CalculatePixelNormal(float3 aPackedNormal, float3x3 aTBN)
{
    return normalize(mul(UnpackNormal(aPackedNormal), aTBN));
}

#endif