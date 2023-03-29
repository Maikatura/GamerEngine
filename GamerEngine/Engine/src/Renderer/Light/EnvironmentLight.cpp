#include "GraphicsEngine.pch.h"
#include <Renderer/Light/EnvironmentLight.h>


void EnvironmentLight::SetAsResource(Microsoft::WRL::ComPtr<ID3D11Buffer> aLightBuffer)
{
	if (myTexture)
	{
		myTexture->SetAsResource(10);
	}
}

void EnvironmentLight::SetData(TransformComponent* data)
{
}
