#include "GraphicsEngine.pch.h"
#include <Light/EnvironmentLight.h>


void EnvironmentLight::SetAsResource(Microsoft::WRL::ComPtr<ID3D11Buffer> aLightBuffer)
{
	if (myTexture)
	{
		myTexture->SetAsResource(10);
	}
}
