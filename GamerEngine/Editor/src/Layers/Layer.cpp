#include "Editor.pch.h"
#include "Layer.h"

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
