#include "Editor.pch.h"
#include "TextureBrowserLayer.h"

TextureBrowserLayer::TextureBrowserLayer() : Layer("Texture Browser", false, true)
{
}

void TextureBrowserLayer::OnImGuiRender()
{
	Layer::OnImGuiRender();

	
	BeginMenu();

	auto textures = TextureAssetHandler::GetTextureList();
	for (auto texture : textures)
	{
		ImGui::Image(texture->GetSRV().Get(), { 512, 512 });
	}

	EndMenu();
}
