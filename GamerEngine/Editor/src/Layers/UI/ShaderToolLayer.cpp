#include "Editor.pch.h"
#include "ShaderToolLayer.h"

ShaderToolLayer::ShaderToolLayer() : Layer("Shader Tools")
{
}

void ShaderToolLayer::OnImGuiRender()
{
    BeginMenu();

    
    
    EndMenu();
}

void ShaderToolLayer::OnBeginFrame()
{
    Layer::OnBeginFrame();
}
