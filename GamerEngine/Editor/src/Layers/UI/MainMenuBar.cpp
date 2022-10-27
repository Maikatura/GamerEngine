#include "Editor.pch.h"
#include "MainMenuBar.h"
#include "ImGui/imgui.h"
#include <string>
#include <Layers/LayerHeader.h>

#include "GraphicsEngine.h"
#include "Time.hpp"
#include "Debugger/ConsoleHelper.h"
#include "Scene/Scene.h"
#include "Snapshots/SnapshotManager.h"


MainMenuBar::MainMenuBar(EditorLayers& aLayer) : myLayers(aLayer)
{ }

bool MainMenuBar::OnImGuiRender()
{
    

    if(ImGui::BeginMainMenuBar())
    {
        if(ImGui::BeginMenu("File"))
        {
            if(ImGui::MenuItem("Open"))
            {

            }

            if(ImGui::MenuItem("Save"))
            {
                // TODO : FIX A POSTMASTER THAT SEND THE OPEN COMMAND TO THE KeybindShortcuts to open the file prompt to save
            }

            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("View"))
        {
            if(ImGui::BeginMenu("Windows"))
            {

                if(ImGui::MenuItem(EditorNames::HierarchyName.c_str()))
                {
                    myLayers.AddLayer(std::make_shared<Hierarchy>());
                }

                if(ImGui::MenuItem(EditorNames::InspectorName.c_str()))
                {
                    myLayers.AddLayer(std::make_shared<Inspector>());
                }

                if(ImGui::MenuItem("Networking"))
                {
                    myLayers.AddLayer(std::make_shared<NetworkingLayer>());
                }

            	ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("Tools"))
        {
            // EditorNames::SettingsName.c_str()
            if(ImGui::MenuItem("Tools 1 & 2"))
            {
                myLayers.AddLayer(std::make_shared<EditorSettingsPanel>());
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

        ImGuiIO& io = ImGui::GetIO();
        auto windowWidth = io.DisplaySize.x;

        float size = 16;
        float padding = 4;




        float textWidth = 0.0f;
        if (!myLayers.myShouldRunEngine)
        {
        	textWidth = ImGui::CalcTextSize("Play").x;
            ImGui::SetCursorPosX((windowWidth * 0.5) - (size + padding));
            if(ImGui::Button("Play"))
            {
            	myLayers.myShouldRunEngine = true;

                mySnapshot = SnapshotManager(&GraphicsEngine::Get()->GetScene()->GetRegistry());
                mySnapshot.CreateSnapshot();
            }
        }
        else
        {
            textWidth = ImGui::CalcTextSize("Stop").x;
            ImGui::SetCursorPosX((windowWidth * 0.5) - (size + padding));
            if(ImGui::Button("Stop"))
            {
                myLayers.myShouldRunEngine = false;
                ConsoleHelper::Log(LogType::Info, "Stopped game");
                mySnapshot.RestoreSnapShot();
            }
        }
       

        textWidth = ImGui::CalcTextSize("Pause").x;
        ImGui::SetCursorPosX((windowWidth * 0.5) + (size + padding));
        if(ImGui::Button("Pause"))
        {
            myLayers.myShouldRunEngine = !myLayers.myShouldRunEngine;
            ConsoleHelper::Log(LogType::Info, "Paused game");
        }



		std::string fps = "FPS: " + std::to_string(Time::GetFPS());

       
    	textWidth = ImGui::CalcTextSize(fps.c_str()).x + 6.0f;

        ImGui::SetCursorPosX((windowWidth - textWidth ));

    	ImGui::Text(fps.c_str(), false);

        ImGui::EndMainMenuBar();
    }
	return true;
}
