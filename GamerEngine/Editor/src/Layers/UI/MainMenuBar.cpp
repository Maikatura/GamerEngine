#include "Editor.pch.h"
#include "MainMenuBar.h"
#include "ImGui/imgui.h"
#include <string>
#include <Layers/LayerHeader.h>

#include "Renderer/GraphicsEngine.h"
#include "Time.hpp"
#include "Renderer/Debugger/ConsoleHelper.h"
#include "Fonts/IconsForkAwesome.h"
#include "Handlers/FileDialog.h"
#include "Renderer/Scene/Scene.h"
#include "Renderer/Scene/SceneManager.h"
#include "Renderer/Scene/SceneSerializer.h"
#include "Snapshots/SnapshotManager.h"


MainMenuBar::MainMenuBar(EditorLayers& aLayer) : Layer("MainMenuBar"), myLayers(aLayer)
{ }

bool MainMenuBar::OnImGuiRender()
{
    if(ImGui::BeginMainMenuBar())
    {
        if(ImGui::BeginMenu(ICON_FK_FILE" File"))
        {
            if(ImGui::MenuItem("Open"))
            {
                std::string path = FileDialog::OpenFile("Scene File (*.csf)\0*.csf\0");
                SceneManager::LoadScene(path);
                ConsoleHelper::Log(LogType::Info, std::string("Open Scene from '" + SceneManager::GetScene()->GetPath() + "'"));
            }

            if(ImGui::MenuItem("Save"))
            {
                std::string path = FileDialog::SaveFile("Scene File (*.csf)\0*.csf\0");
                SceneManager::SaveScene(path + ".csf");
                ConsoleHelper::Log(LogType::Info, std::string("Saved scene to '" + SceneManager::GetScene()->GetPath() + "'"));
            }

            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("View"))
        {
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


        if(ImGui::BeginMenu("Help"))
        {
            if(ImGui::MenuItem("Help Panel"))
            {
                myLayers.AddLayer(std::make_shared<HelpPanel>());
            }

            ImGui::EndMenu();
        }

        ImGuiIO& io = ImGui::GetIO();
        auto windowWidth = io.DisplaySize.x;

        float size = 16.0f;
        float padding = 4.0f;




        float textWidth = 0.0f;
        if (!myLayers.ShouldRunEngine())
        {
            std::string playText = ICON_FK_PLAY;
            playText += " Play";

        	textWidth = ImGui::CalcTextSize(playText.c_str()).x;
            ImGui::SetCursorPosX((windowWidth * 0.5f) - (size + padding) - (textWidth * 0.5f));
            if(ImGui::Button(playText.c_str()))
            {
                myLayers.SetShouldEngineRun(true);
                mySnapshot = SnapshotManager(&SceneManager::GetScene()->GetRegistry());
                mySnapshot.CreateSnapshot();
            }
        }
        else
        {

            std::string stopText = ICON_FK_STOP;
            stopText += " Stop";

            textWidth = ImGui::CalcTextSize(stopText.c_str()).x;
            ImGui::SetCursorPosX((windowWidth * 0.5f) - (size + padding) - (textWidth * 0.5f));
            if(ImGui::Button(stopText.c_str()))
            {
                myLayers.SetShouldEngineRun(false);
                ConsoleHelper::Log(LogType::Info, "Stopped game");
                mySnapshot.RestoreSnapShot();
            }
        }
       

        std::string resumeOrPause = (GraphicsEngine::Get()->GetPauseState() == true) ? "Resume" : "Pause";

        textWidth = ImGui::CalcTextSize(resumeOrPause.c_str()).x;
        ImGui::SetCursorPosX((windowWidth * 0.5f) + (size + padding));

        if(ImGui::Button(resumeOrPause.c_str()))
        {
            GraphicsEngine::Get()->SetPauseState(!GraphicsEngine::Get()->GetPauseState());
            ConsoleHelper::Log(LogType::Info, "Paused/Resumed game");
        }



		std::string fps = "FPS: " + std::to_string(Time::GetFPS());

       
    	textWidth = ImGui::CalcTextSize(fps.c_str()).x + 6.0f;

        ImGui::SetCursorPosX((windowWidth - textWidth ));

    	ImGui::Text(fps.c_str(), false);

        ImGui::EndMainMenuBar();
    }
	return true;
}
