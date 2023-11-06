#ifndef COMMON_HLSLI
#define COMMON_HLSLI

//Frostbyte accurate linear to srgb 
float3 LinearToSRGB(float3 aColor)
{
    //Frostbyte accurate linear to srgb
    float3 sRGBLo = aColor * 12.92f;
    float3 sRGBHi = pow(abs(aColor), 1.f / 2.4f) * 1.055f - 0.055f;
    float3 sRGB = (aColor <= 0.0031308f) ? sRGBLo : sRGBHi;

    return sRGB;
}

//Frostbyte accurate srgb to linear 
float3 SRGBToLinear(float3 aColor)
{
    float3 linearRGBLo = aColor / 12.92f;
    float3 linearRGBHi = pow(abs((aColor + 0.055f) / 1.055f), 2.4f);
    float3 linearRGB = (aColor <= 0.04045f) ? linearRGBLo : linearRGBHi;

    return linearRGB;
}

int GetNumMips(TextureCube aCubeTex)
{
    int iWidth = 0;
    int iheight = 0;
    int numMips = 0;
    aCubeTex.GetDimensions(0, iWidth, iheight, numMips);
    return numMips;
}

int2 GetSize(Texture2D aTexture)
{
    int width = 0;
    int height = 0;
    int mips = 0;
    aTexture.GetDimensions(0, width, height, mips);
    return int2(width, height);
}
#endif