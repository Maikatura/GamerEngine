#include "Editor.pch.h"
#include <ImGui/imgui.h>
#include <Layers/Debug/ConsoleWindow.h>


ConsoleWindow::ConsoleWindow() : Layer("Console Window")
{
}

void ConsoleWindow::OnImGuiRender()
{
	BeginMenu();
	
	EndMenu();
}
