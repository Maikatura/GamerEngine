#include "Editor.pch.h"
#include "MainMenuBar.h"
#include "ImGui/imgui.h"
#include <string>
#include <Layers/LayerHeader.h>

#include "GraphicsEngine.h"
#include "Time.hpp"
#include "Debugger/ConsoleHelper.h"
#include "Fonts/IconsForkAwesome.h"
#include "Handlers/FileDialog.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Scene/SceneSerializer.h"
#include "Snapshots/SnapshotManager.h"
#include "SettingKeybinds.h"
#include <Layers/EditorLayers.h>

MainMenuBar::MainMenuBar() : Layer("MainMenuBar", true, false)
{ }

void MainMenuBar::OnImGuiRender()
{
    RenderMainBar();
    RenderFooter();
}

void MainMenuBar::RenderMainBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu(ICON_FK_FILE" File"))
        {
            if (ImGui::MenuItem("Open"))
            {
                std::string path = FileDialog::OpenFile("Scene File (*.csf)\0*.csf\0");
                SceneManager::Get().LoadScene(path);
                ConsoleHelper::Log(LogType::Info, "Open Scene from '%s'", SceneManager::Get().GetScene()->GetPath().c_str());
            }

            if (ImGui::MenuItem("Save"))
            {
                std::string path = FileDialog::SaveFile("Scene File (*.csf)\0*.csf\0");
                if (!path.empty())
                {
                    SceneManager::Get().SaveScene(path + ".csf");
                    ConsoleHelper::Log(LogType::Info, "Saved scene to '%s'", SceneManager::Get().GetScene()->GetPath().c_str());
                    
                }
                
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View"))
        {
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Tools"))
        {
            // EditorNames::SettingsName.c_str()
            if (ImGui::MenuItem("Tools 1 & 2"))
            {
                //myLayers->AddLayer<EditorSettingsPanel>();
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Windows"))
        {

            for (int i = 0; i < myLayers->myLayers.size(); i++)
            {
                if (myLayers->myLayers[i]->ShouldBeSaved())
                {
                    //if (myLayers.myLayers[i]->GetCategory() != "")
                    //{
                    //    /*if(ImGui::BeginMenu(myLayers.myLayers[i]->GetCategory().c_str()))
                    //    {
                    //        if(ImGui::MenuItemEx(myLayers.myLayers[i]->GetLayerName().c_str(), myLayers.myLayers[i]->IsOpen() ? ICON_FK_CHECK : ""))
                    //        {
                    //            if(myLayers.myLayers[i]->IsOpen())
                    //            {
                    //                myLayers.myLayers[i]->SetOpen(false);
                    //            }
                    //            else
                    //            {
                    //                myLayers.myLayers[i]->SetOpen(true);
                    //            }
                    //        }

                    //        ImGui::EndMenu();*/
                    //    }
                    //    else
                    //    {
                    if (ImGui::MenuItemEx(myLayers->myLayers[i]->GetLayerName().c_str(), myLayers->myLayers[i]->IsOpen() ? ICON_FK_CHECK : ""))
                    {
                        if (myLayers->myLayers[i]->IsOpen())
                        {
                            myLayers->myLayers[i]->SetOpen(false);
                        }
                        else
                        {
                            myLayers->myLayers[i]->SetOpen(true);
                        }
                    }
                    //}
                //}


                }


            }

            ImGui::EndMenu();
        }


        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("Help Panel"))
            {
                myLayers->AddLayer<HelpPanel>();
            }

            ImGui::EndMenu();
        }

        ImGuiIO& io = ImGui::GetIO();
        auto windowWidth = io.DisplaySize.x;

        float size = 16.0f;
        float padding = 4.0f;




        float textWidth = 0.0f;
        if (!myLayers->ShouldRunEngine())
        {
            std::string playText = ICON_FK_PLAY;
            playText += " Play";

            textWidth = ImGui::CalcTextSize(playText.c_str()).x;
            ImGui::SetCursorPosX((windowWidth * 0.5f) - (size + padding) - (textWidth * 0.5f));
            if (ImGui::Button(playText.c_str()))
            {
                myLayers->SetShouldEngineRun(true);
                GraphicsEngine::Get()->SetEngineRunning(true);
                ConsoleHelper::Log(LogType::Info, "Started game");

                SceneManager::Get().SetSceneState(SceneState::Play);
            }
        }
        else
        {
            std::string stopText = ICON_FK_STOP;
            stopText += " Stop";

            textWidth = ImGui::CalcTextSize(stopText.c_str()).x;
            ImGui::SetCursorPosX((windowWidth * 0.5f) - (size + padding) - (textWidth * 0.5f));
            if (ImGui::Button(stopText.c_str()))
            {
                SceneManager::Get().OnRuntimeStop();
                myLayers->SetShouldEngineRun(false);
                GraphicsEngine::Get()->SetEngineRunning(false);
                ConsoleHelper::Log(LogType::Info, "Stopped game");

                SceneManager::Get().SetSceneState(SceneState::Edit);
            }
        }


        std::string resumeOrPause = (GraphicsEngine::Get()->GetPauseState() == true) ? "Resume" : "Pause";

        textWidth = ImGui::CalcTextSize(resumeOrPause.c_str()).x;
        ImGui::SetCursorPosX((windowWidth * 0.5f) + (size + padding));

        if (ImGui::Button(resumeOrPause.c_str()))
        {
            GraphicsEngine::Get()->SetPauseState(!GraphicsEngine::Get()->GetPauseState());

            if (GraphicsEngine::Get()->GetPauseState() == true)
            {
                ConsoleHelper::Log(LogType::Info, "Resumed game");

            }
            else
            {
                ConsoleHelper::Log(LogType::Info, "Paused game");
            }


        }


        std::string topBar = "";
       
        topBar += "\t";

        topBar += "Render Mode: ";
        RenderMode currentRenderMode = GraphicsEngine::Get()->GetRenderMode();

        switch (currentRenderMode)
        {
        case RenderMode::Default:
            topBar += "Default";
            break;
        case RenderMode::UV1:
            topBar += "UV1";
            break;
        case RenderMode::VertexColor:
            topBar += "Vertex Color";
            break;
        case RenderMode::VertexNormal:
            topBar += "Vertex Normal";
            break;
        case RenderMode::PixelNormal:
            topBar += "Pixel Normal";
            break;
        case RenderMode::AlbedoMap:
            topBar += "Albedo Map";
            break;
        case RenderMode::NormalMap:
            topBar += "Normal Map";
            break;
        case RenderMode::DirectLight:
            topBar += "Directonal Light";
            break;
        case RenderMode::AmbientLight:
            topBar += "Ambient Light";
            break;
        case RenderMode::PointLight:
            topBar += "Point Light";
            break;
        case RenderMode::SpotLight:
            topBar += "Spot Light";
            break;
        case RenderMode::AmbientOcclusion:
            topBar += "AO";
            break;
        case RenderMode::SSAO:
            topBar += "SSAO";
            break;
        case RenderMode::Metalness:
            topBar += "Metalness";
            break;
        case RenderMode::Roughness:
            topBar += "Roughness";
            break;
        case RenderMode::Emission:
            topBar += "Emission";
            break;
        case RenderMode::DirectLightNoAlbedo:
            topBar += "Direct Light No Albedo";
            break;
        case RenderMode::AmbientLightNoAlbedo:
            topBar += "Ambient Light No Albedo";
            break;
        case RenderMode::PointLightNoAlbedo:
            topBar += "Point Light No Albedo";
            break;
        case RenderMode::SpotLightNoAlbedo:
            topBar += "Spot Light No Albedo";
            break;
        case RenderMode::DepthBuffer:
            topBar += "Depth Buffer";
            break;
        case RenderMode::COUNT:
        default:
            topBar += "Error";
            break;
        }

        topBar += "\t";
        topBar += "FPS: " + std::to_string(Time::GetFPS());


        textWidth = ImGui::CalcTextSize(topBar.c_str()).x + 6.0f;

        ImGui::SetCursorPosX((windowWidth - textWidth));

        ImGui::Text(topBar.c_str(), false);


        ImGuiViewportP* viewport = (ImGuiViewportP*)(void*)ImGui::GetMainViewport();
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
        float height = ImGui::GetFrameHeight();

        if (ImGui::BeginViewportSideBar("##SecondaryMenuBar", viewport, ImGuiDir_Up, height, window_flags)) {
            if (ImGui::BeginMenuBar()) {
                if(ImGui::Button(SettingKeybinds::GetEditModeName().c_str()))
                {
	                if (SettingKeybinds::GetEditMode() == ImGuizmo::MODE::LOCAL)
	                {
                        SettingKeybinds::SetEditModeType(ImGuizmo::MODE::WORLD);
	                }
                    else
                    {
                        SettingKeybinds::SetEditModeType(ImGuizmo::MODE::LOCAL);
                    }
                }
                ImGui::EndMenuBar();
            }
            ImGui::End();
        }



        ImGui::EndMainMenuBar();
    }
}

void MainMenuBar::RenderFooter()
{
    //const ImGuiViewport* viewport = ImGui::GetMainViewport();

    //// Set position to the bottom of the viewport
    //ImGui::SetNextWindowPos(
    //    ImVec2(viewport->Pos.x,
    //        viewport->Pos.y + viewport->Size.y - ImGui::GetFrameHeight()));

    //// Extend width to viewport width
    //ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, ImGui::GetFrameHeight()));

    //// Add menu bar flag and disable everything else
    //ImGuiWindowFlags flags =
    //    ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs |
    //    ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollWithMouse |
    //    ImGuiWindowFlags_NoSavedSettings |
    //    ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoBackground |
    //    ImGuiWindowFlags_MenuBar;


    //if (ImGui::Begin("StatusBar", nullptr, flags)) {
    //    if (ImGui::BeginMenuBar()) {
    //        ImGui::Text("%s", "Haiiii :3");
    //        ImGui::EndMenuBar();
    //    }
    //    ImGui::End();
    //}
}
