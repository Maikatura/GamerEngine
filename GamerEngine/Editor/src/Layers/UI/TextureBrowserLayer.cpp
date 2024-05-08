#include "Editor.pch.h"
#include "TextureBrowserLayer.h"

TextureBrowserLayer::TextureBrowserLayer() : Layer("Texture Browser", false, true)
{
}

void TextureBrowserLayer::OnImGuiRender()
{
	Layer::OnImGuiRender();

	
	BeginMenu();


	const int size = 512;

	auto textures = TextureAssetHandler::GetTextureList();
	for (auto texture : textures)
	{
		ImGui::Image(texture->GetSRV().Get(), { size, size });
	}

	EndMenu();
}
