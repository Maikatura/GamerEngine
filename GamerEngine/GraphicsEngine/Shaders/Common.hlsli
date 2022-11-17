float3 LinearToGamma(float3 aColor)
{
    return pow(abs(aColor), 1.0 / 2.2);
}

float3 GammaToLinear(float3 aColor)
{
    return pow(abs(aColor), 2.2);
}

int GetNumMips(TextureCube cubeTex)
{
    int iWidth = 0;
    int iheight = 0;
    int numMips = 0;
    cubeTex.GetDimensions(0, iWidth, iheight, numMips);
    return numMips;
}

bool GetShadowPixel(Texture2D aShadowMap, float4x4 aLightView, float4x4 aLightProjection, float3 aWorldPosition, bool aCastShadows)
{
	if(!aCastShadows) return false;


	float shadowBias = 0.00001f;

	float4 w2lView = mul(aLightView, float4(aWorldPosition, 1));
	float4 v2lProj = mul(aLightProjection, w2lView);

	float2 projectedTexCoord;
	projectedTexCoord.x = v2lProj.x / v2lProj.w / 2.0f + 0.5f;
	projectedTexCoord.y = -v2lProj.y / v2lProj.w / 2.0f + 0.5f;

	if(saturate(projectedTexCoord.x) == projectedTexCoord.x && saturate(projectedTexCoord.y) == projectedTexCoord.y)
	{
		float vDepth = (v2lProj.z / v2lProj.w) - shadowBias;
		float lDepth = aShadowMap.Sample(defaultSampler, projectedTexCoord).r;
		if(lDepth < vDepth)
		{
			return true;
		}
	}
	return false;
}

bool GetShadowPixel(TextureCube aShadowMap, float4x4 aLightView[6], float4x4 aLightProjection, float aRange, float3 aLightPosition, float3 aWorldPosition, bool aCastShadows)
{
	if(!aCastShadows) return false;

	float shadowBias = 0.00001f;

	for(int face = 0; face < 6; face++)
	{
		float4 w2lView = mul(aLightView[face], float4(aWorldPosition, 1));
		float4 v2lProj = mul(aLightProjection, w2lView);

		float3 fragToLight = aWorldPosition - aLightPosition;

		float2 lightUV;
		lightUV.x = v2lProj.x / v2lProj.w / 2.f + .5f;
		lightUV.y = -v2lProj.y / v2lProj.w / 2.f + .5f;
		
		//float vDepth = ((length(fragToLight) / aRange) / v2lProj.w) - aBias;
		if(saturate(lightUV.x) == lightUV.x && saturate(lightUV.y) == lightUV.y && w2lView.z > 0)
		{
			float vDepth = (v2lProj.z / v2lProj.w) - shadowBias;
			float lDepth = aShadowMap.Sample(defaultSampler, fragToLight).r;
			if(lDepth < vDepth) return true;
		}
	}
	return false;
}