#include "Editor.pch.h"
#include <Layers/NetworkingLayer.h>



bool NetworkingLayer::OnImGuiRender()
{

	ImGui::Begin("Networking");

    if (ImGui::Button("Host"))
    {
        StartNetworking();
    }

	ImGui::End();


	return true;
}

void NetworkingLayer::Listen()
{
}

void NetworkingLayer::StartNetworking()
{

}
