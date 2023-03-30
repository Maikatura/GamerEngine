#include "Editor.pch.h"
#include "Layer.h"

Layer::Layer(const std::string& aLayerName, bool aAlwaysOpen, bool aSaveLayerSettings, const std::string& aCategory)
{
    myLayerName = aLayerName;
    myIsAlwaysOpen = aAlwaysOpen;
    mySaveLayer = aSaveLayerSettings;
    myCategory = aCategory;
}

void Layer::OnAttach()
{
    hasInited = true;
}

bool Layer::OnRender()
{
    return false;
}

bool Layer::EndFrame()
{
    return false;
}

void Layer::OnImGuiRender()
{
    
}

void Layer::OnUpdate()
{
}

bool Layer::OnDetach()
{
    return false;
}

bool Layer::OnShutdown()
{
    return false;
}

bool Layer::ShouldBeSaved()
{
    return mySaveLayer;
}

std::string Layer::GetCategory()
{
    return myCategory;
}

std::string Layer::GetLayerName()
{
    return myLayerName;
}

void Layer::SetOpen(bool aOpenState)
{
    myIsOpen = aOpenState;
}

bool Layer::IsOpen()
{
    return myIsOpen;
}

bool Layer::HasBeenAdded()
{
    return hasInited;
}

bool Layer::BeginMenu(ImGuiWindowFlags aFlag)
{
	return ImGui::Begin(myLayerName.c_str(), &myIsOpen, aFlag);
}

void Layer::EndMenu()
{
    ImGui::End();
}
