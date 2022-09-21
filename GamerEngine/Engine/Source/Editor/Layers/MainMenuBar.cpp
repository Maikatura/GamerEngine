#include "MainMenuBar.h"
#include "ImGui/imgui.h"
#include <string>
#include <Layers/LayerHeader.h>

#include "Time.hpp"

MainMenuBar::MainMenuBar(EditorLayers* aLayer)
{
    myLayers = aLayer;
}

bool MainMenuBar::OnImGuiRender()
{


    if(ImGui::BeginMainMenuBar())
    {
        if(ImGui::BeginMenu("File"))
        {
            if(ImGui::MenuItem("Nothing for now"))
            {
                //Do something
            }

            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("View"))
        {
            if(ImGui::BeginMenu("Windows"))
            {

                if(ImGui::MenuItem(EditorNames::HierarchyName.c_str()))
                {

                }

                if(ImGui::MenuItem(EditorNames::PropertiesName.c_str()))
                {

                }

            	ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("Help"))
        {
            if(ImGui::MenuItem(EditorNames::SettingsName.c_str()))
            {
                myLayers->AddLayer(std::make_shared<EditorSettingsPanel>());
            }

            ImGui::EndMenu();
        }

		std::string fps = "FPS: " + std::to_string(Time::GetFPS());
        ImGui::BeginMenu(fps.c_str(), false);

        ImGui::EndMainMenuBar();
    }
	return true;
}
