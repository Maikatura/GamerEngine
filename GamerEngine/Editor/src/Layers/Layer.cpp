#include "Editor.pch.h"
#include "Layer.h"

Layer::Layer(const std::string& aLayerName, bool aAlwaysOpen, bool aSaveLayerSettings)
{
    myLayerName = aLayerName;
    myIsAlwaysOpen = aAlwaysOpen;
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

bool Layer::OnImGuiRender()
{
    return false;
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

bool Layer::HasBeenAdded()
{
    return hasInited;
}

bool Layer::BeginMenu(bool* isOpen, ImGuiWindowFlags aFlag)
{
    return ImGui::Begin(myLayerName.c_str(), isOpen, aFlag);
}

void Layer::EndMenu()
{
    ImGui::End();
}
