#include "Editor.pch.h"
#include <ImGui/imgui.h>
#include <Layers/Debug/ConsoleWindow.h>


bool ConsoleWindow::OnImGuiRender()
{
	ImGui::Begin("Console");



	ImGui::End();


	return true;
}
