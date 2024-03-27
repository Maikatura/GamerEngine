#include "Data/ShaderStructs.hlsli"
#include "Data/Common.hlsli"
#include "Data/Samplers.hlsli"

SamplerState ImageSamplers[8] : register(s1);
Texture2D ImageTextures[32] : register(t96);

// Median and LinearStep functions Moved to the math header file

float2 SafeNormalize(in float2 v)
{
    float len = length(v);
    len = (len > 0.f) ? 1.f / len : 0.f;
    return v * len;
}

float ScreenPxRange(float2 uvs)
{
    const float screenPxRange = 2;
    float2 unitRange = float2(screenPxRange, screenPxRange) / float2(212, 212);
    float2 screenTexSize = float2(1.0, 1.0) / fwidth(uvs);
    return max(0.5 * dot(unitRange, screenTexSize), 1.0);
}

float Median(float3 rgb)
{
    return max(min(rgb.r, rgb.g), min(max(rgb.r, rgb.g), rgb.b));
}

PixelOutput main(RectVertexOutput input)
{
    const uint FLAG_MSDF_FONT = 1u;
    const uint FLAG_DRAW_BORDER = 2u;
    const uint FLAG_ANTIALIASED = 4u;

    
    PixelOutput output;
    output.Color = float4(1, 1, 1, 1);

    // TODO: Replace this 
    SamplerState chosenSampler = linearClampSampler;
    if (input.ImageIndex == ~0u)
    {
        output.Color = float4(1, 0, 1, input.Color.a);
        return output;
    }
    switch (input.ImageIndex)
    {
        default:
            break;
        case 0:
            output.Color = ImageTextures[0].SampleLevel(chosenSampler, input.TextureCoordinates, 0);
            break;
        case 1:
            output.Color = ImageTextures[1].SampleLevel(chosenSampler, input.TextureCoordinates, 0);
            break;
        case 2:
            output.Color = ImageTextures[2].SampleLevel(chosenSampler, input.TextureCoordinates, 0);
            break;
        case 3:
            output.Color = ImageTextures[3].SampleLevel(chosenSampler, input.TextureCoordinates, 0);
            break;
        case 4:
            output.Color = ImageTextures[4].SampleLevel(chosenSampler, input.TextureCoordinates, 0);
            break;
        case 5:
            output.Color = ImageTextures[5].SampleLevel(chosenSampler, input.TextureCoordinates, 0);
            break;
        case 6:
            output.Color = ImageTextures[6].SampleLevel(chosenSampler, input.TextureCoordinates, 0);
            break;
        case 7:
            output.Color = ImageTextures[7].SampleLevel(chosenSampler, input.TextureCoordinates, 0);
            break;
        case 8:
            output.Color = ImageTextures[8].SampleLevel(chosenSampler, input.TextureCoordinates, 0);
            break;
        case 9:
            output.Color = ImageTextures[9].SampleLevel(chosenSampler, input.TextureCoordinates, 0);
            break;
        case 10:
            output.Color = ImageTextures[10].SampleLevel(chosenSampler, input.TextureCoordinates, 0);
            break;
        case 11:
            output.Color = ImageTextures[11].SampleLevel(chosenSampler, input.TextureCoordinates, 0);
            break;
        case 12:
            output.Color = ImageTextures[12].SampleLevel(chosenSampler, input.TextureCoordinates, 0);
            break;
        case 13:
            output.Color = ImageTextures[13].SampleLevel(chosenSampler, input.TextureCoordinates, 0);
            break;
        case 14:
            output.Color = ImageTextures[14].SampleLevel(chosenSampler, input.TextureCoordinates, 0);
            break;
        case 15:
            output.Color = ImageTextures[15].SampleLevel(chosenSampler, input.TextureCoordinates, 0);
            break;
        case 16:
            output.Color = ImageTextures[16].SampleLevel(chosenSampler, input.TextureCoordinates, 0);
            break;
        case 17:
            output.Color = ImageTextures[17].SampleLevel(chosenSampler, input.TextureCoordinates, 0);
            break;
        case 18:
            output.Color = ImageTextures[18].SampleLevel(chosenSampler, input.TextureCoordinates, 0);
            break;
        case 19:
            output.Color = ImageTextures[19].SampleLevel(chosenSampler, input.TextureCoordinates, 0);
            break;
        case 20:
            output.Color = ImageTextures[20].SampleLevel(chosenSampler, input.TextureCoordinates, 0);
            break;
        case 21:
            output.Color = ImageTextures[21].SampleLevel(chosenSampler, input.TextureCoordinates, 0);
            break;
        case 22:
            output.Color = ImageTextures[22].SampleLevel(chosenSampler, input.TextureCoordinates, 0);
            break;
        case 23:
            output.Color = ImageTextures[23].SampleLevel(chosenSampler, input.TextureCoordinates, 0);
            break;
        case 24:
            output.Color = ImageTextures[24].SampleLevel(chosenSampler, input.TextureCoordinates, 0);
            break;
        case 25:
            output.Color = ImageTextures[25].SampleLevel(chosenSampler, input.TextureCoordinates, 0);
            break;
        case 26:
            output.Color = ImageTextures[26].SampleLevel(chosenSampler, input.TextureCoordinates, 0);
            break;
        case 27:
            output.Color = ImageTextures[27].SampleLevel(chosenSampler, input.TextureCoordinates, 0);
            break;
        case 28:
            output.Color = ImageTextures[28].SampleLevel(chosenSampler, input.TextureCoordinates, 0);
            break;
        case 29:
            output.Color = ImageTextures[29].SampleLevel(chosenSampler, input.TextureCoordinates, 0);
            break;
        case 30:
            output.Color = ImageTextures[30].SampleLevel(chosenSampler, input.TextureCoordinates, 0);
            break;
        case 31:
            output.Color = ImageTextures[31].SampleLevel(chosenSampler, input.TextureCoordinates, 0);
            break;
    }
    const uint msdfRect = input.SamplingData;
    if (msdfRect & FLAG_MSDF_FONT)
    {
        if (msdfRect & FLAG_DRAW_BORDER)
        {
            const float smoothing = 0.05f;
            const float outlineSmoothing = 0.05f;
            const float outlineWidth = 0.44f;
            const float textWidth = 0.5f;
            const float3 outlineColor = float3(0, 0, 0);
        
            float sd = Median(output.Color.rgb);
            float outline = output.Color.a;
        
            sd = output.Color.a;
        
            sd = smoothstep(textWidth - smoothing, textWidth + smoothing, sd);
            outline = smoothstep(outlineWidth - outlineSmoothing, outlineWidth + outlineSmoothing, outline);
            output.Color = float4(lerp(outlineColor, input.Color.rgb, sd), outline);
        }
        else
        {
            float sd = Median(output.Color.rgb);
            float screenPxDist = ScreenPxRange(input.TextureCoordinates) * (sd - 0.5f);
            float opacity = clamp(screenPxDist + 0.5f, 0.f, 1.f);

            float3 bgColor = float3(0, 0, 0);
            float3 fgColor = float3(1, 1, 1);
            output.Color.rgb = lerp(bgColor, fgColor, opacity);
            output.Color.a = (output.Color.r + output.Color.g + output.Color.b) > 0.5;
            output.Color.rgb = fgColor * input.Color.rgb;
        }
    }
    else
    {
        output.Color.rgb = output.Color.rgb * input.Color.rgb;
    }

    output.Color.rgb = LinearToSRGB(output.Color.rgb);
    output.Color.a *= input.Color.a;
    return output;
}
