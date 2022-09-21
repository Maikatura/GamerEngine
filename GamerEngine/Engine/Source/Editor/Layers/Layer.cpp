#include "GraphicsEngine.pch.h"
#include "Layer.h"

void Layer::OnAttach()
{
    
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

bool Layer::OnDetach()
{
    return false;
}

bool Layer::OnShutdown()
{
    return false;
}
