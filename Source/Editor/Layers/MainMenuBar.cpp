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
                    myLayers->AddLayer(std::make_shared<Hierarchy>());
                }

                if(ImGui::MenuItem(EditorNames::InspectorName.c_str()))
                {
                    myLayers->AddLayer(std::make_shared<Inspector>());
                }

            	ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("Tools"))
        {
            // EditorNames::SettingsName.c_str()
            if(ImGui::MenuItem("Tools 1"))
            {
                myLayers->AddLayer(std::make_shared<EditorSettingsPanel>());
            }

            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("Help"))
        {
            if(ImGui::MenuItem("NO ELP"))
            {
                //myLayers->AddLayer(std::make_shared<EditorSettingsPanel>());
            }

            ImGui::EndMenu();
        }

        

		std::string fps = "FPS: " + std::to_string(Time::GetFPS());
        ImGui::BeginMenu(fps.c_str(), false);

        ImGui::EndMainMenuBar();
    }
	return true;
}
