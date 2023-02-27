#include "Editor.pch.h"
#include <Layers/NetworkingLayer.h>

#include "Components/Components.hpp"
#include "Renderer/Model/Entity.h"
#include "Network/CreateObjectMessage.h"
#include "Renderer/Scene/SceneManager.h"
#include "TurNet/Shared/Utility/TurCompare.h"

#include "Network/NetOverloads.h"
#include "Network/MoveObjectMessage.h"
#include "Network/PlayerConnectMessage.h"
#include "Network/PlayerDisconnectMessage.h"
#include "Network/PlayerMoveMessage.h"
#include "Network/SkinChangerMessage.h"
#include "Renderer/AssetHandlers/ModelAssetHandler.h"
#include "Renderer/Render/Renderer.h"

using namespace std::chrono_literals;

NetworkingLayer::NetworkingLayer() : mySendTime(1.0f), myMoveSendTime((1.0f / 30.0f) * 10.0f)
{

}

void NetworkingLayer::OnAttach()
{
	Layer::OnAttach();
	myHost = "127.0.0.1";
}

bool NetworkingLayer::OnDetach()
{
	return true;
}

bool NetworkingLayer::OnImGuiRender()
{

	ImGui::Begin("Networking");

    if (ImGui::Button("Host"))
    {
        StartNetworkingServer();

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		myHost = "127.0.0.1";
		StartNetworkingClient();

		myIsHost = true;
    }

	if (ImGui::Button("Connect"))
	{
    	StartNetworkingClient();
	}

	ImGui::InputText("Host IP", &myHost);
	//ImGui::InputText("Port", &myPort);

	if (ImGui::Button("Test"))
	{
		
	}


	ImGui::NewLine();
	ImGui::NewLine();

	static int currentItem = 0;

	std::vector<const char*> skins;
	skins.push_back("Flushed");
	skins.push_back("Pensive");

	ImGui::ListBox("Skins", &currentItem, skins.data(), static_cast<int>(skins.size()));

	if (ImGui::Button("Change Skin"))
	{
		TurNet::TurMessage outMsg;
		PlayerSkinChangeMessage skinMsg;
		skinMsg.ClientID = myClientID;
		skinMsg.SkinID = currentItem;

		outMsg << skinMsg;

		myClient.SendToServer(outMsg);
	}

	ImGui::End();


	return true;
}

void NetworkingLayer::OnUpdate()
{
	Layer::OnUpdate();

	


	if (mySendTime <= 0.0f)
	{
		if (Renderer::GetCamera()->HasMoved())
		{
			mySendTime += myMoveSendTime;

			Vector3f position = Renderer::GetCamera()->ViewProjection.GetPosition();
			Vector3f rotation = Renderer::GetCamera()->ViewProjection.GetRotation();


			TurNet::TurMessage outMsg;
			PlayerMoveMessage moveMsg;
			moveMsg.ClientID = myClientID;
			moveMsg.Time = std::chrono::high_resolution_clock::now();
			moveMsg.MoveSpeed = Renderer::GetCamera()->GetCameraSpeed();;
			moveMsg.Translation = position;
			moveMsg.Rotation = rotation;

			outMsg << moveMsg;
			myClient.SendToServer(outMsg);

			Renderer::GetCamera()->SetHasMoved(false);
		}
	}
	else
	{
		mySendTime -= Time::GetDeltaTime();
	}

	for(int i = 0; i < myPlayers.size(); i++)
	{
		if(!SceneManager::GetScene()->GetRegistry().valid(myPlayers[i].myEntity))
		{
			continue;
		}

		auto& transComp = myPlayers[i].myEntity.GetComponent<TransformComponent>();

		float distance = (myPlayers[i].Translation - transComp.Translation).Length();

		float distanceRot = (myPlayers[i].Rotation - transComp.Rotation).Length();

		Renderer::GetCamera()->GetCameraSpeed();

		std::chrono::steady_clock::time_point clockNow = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> time = clockNow - myPlayers[i].Time;

		float lerpTime = static_cast<float>(time.count()) / 1000.0f;
		//float time_to_reach_targetTan = CommonUtilities::Max(0.0f, (distance / myPlayers[i].MoveSpeed) - lerpTime);
		transComp.Translation = Vector3f::Lerp(transComp.Translation, myPlayers[i].Translation, lerpTime);
		transComp.Rotation = myPlayers[i].Rotation;
	}

}

void NetworkingLayer::Listen()
{
}

void NetworkingLayer::StartNetworkingClient()
{
	if(!myClient.Start())
	{
		std::cout << "Failed to create socket" << std::endl;
	}


	std::function<void(TurNet::TurMessage&)> messageLoop = [&](TurNet::TurMessage& someData)
	{
		char typeBase;
		someData >> typeBase;
		NetworkMessages messageType = static_cast<NetworkMessages>(typeBase);

		switch(messageType)
		{

			

			case NetworkMessages::ObjectMove:
			{
				ObjectMoveMessage moveMsg;
				someData >> moveMsg;

				std::cout << "Got Something from server";

				auto view = SceneManager::GetScene()->GetRegistry().view<IDComponent, TransformComponent>();
				for(auto entity : view)
				{
					if (!SceneManager::GetScene()->GetRegistry().valid(entity))
					{
						return;
					}

					auto& ID = view.get<IDComponent>(entity);

					if(ID.ID == moveMsg.EntityID)
					{
						auto& transform = view.get<TransformComponent>(entity);
						transform = moveMsg.Transform;
						std::cout << moveMsg << std::endl;
					}
				}
				break;
			}
			case NetworkMessages::ObjectSpawn:
			{
				ObjectCreateMessage createMsg;
				someData >> createMsg;

				std::cout << "Got Something from server" << std::endl;

				SceneManager::CreateEntityType(createMsg.EntityType, createMsg.EntityID);

				break;
			}
			case NetworkMessages::PlayerConnectSelf:
			{
				std::cout << "Connect self" << std::endl;

				PlayerConnectSelfMessage selfConnectMsg;
				someData >> selfConnectMsg;

				myClientID = selfConnectMsg.ClientID;
				std::cout << "Connected and got ID: " << myClientID << std::endl;

				break;
			}
			case NetworkMessages::PlayerConnect:
			{
				std::cout << "Connect Other" << std::endl;

				PlayerConnectMessage playerConnectMsg;
				someData >> playerConnectMsg;

				ClientData data;
				data.ClientID = playerConnectMsg.ClientID;
				data.myEntity = SceneManager::CreateEntityType(0);
				data.myEntity.GetComponent<TransformComponent>().Translation = playerConnectMsg.Translation;
				float scale = 20.0f;
				data.myEntity.GetComponent<TransformComponent>().Scale = { scale, scale ,scale };
				auto model = data.myEntity.AddComponent<ModelComponent>(ModelAssetHandler::GetModelInstance(L"Editor\\Models\\network\\NetworkCube.fbx"));
				model.GetModel()->GetMaterial()->SetAlbedoTexture(TextureAssetHandler::GetTexture(L"Editor\\Models\\network\\NetworkCube.dds"));
				model.GetModel()->GetMaterial()->SetNormalTexture(TextureAssetHandler::GetTexture(L"Editor\\Textures\\T_Default_N.dds"));
				model.GetModel()->GetMaterial()->SetMaterialTexture(TextureAssetHandler::GetTexture(L"Editor\\Textures\\T_Default_M.dds"));
				myPlayers.push_back(data);

				break;
			}
			case NetworkMessages::PlayerMove:
			{
				std::cout << "Player Moved" << std::endl;

				PlayerMoveMessage moveMsg;
				someData >> moveMsg;

				for (int i = 0; i < myPlayers.size(); i++)
				{
					if (myPlayers[i].ClientID == moveMsg.ClientID)
					{
						myPlayers[i].Translation = moveMsg.Translation;
						myPlayers[i].Rotation = moveMsg.Rotation;
						myPlayers[i].Time = moveMsg.Time;
						myPlayers[i].MoveSpeed = moveMsg.MoveSpeed;
					}
				}

				break;
			}
			case NetworkMessages::PlayerDisconnect:
			{

				PlayerDisconnectMessage disMsg;
				someData >> disMsg;

				for(int i = 0; i < myPlayers.size(); i++)
				{
					if(myPlayers[i].ClientID == disMsg.ClientID)
					{
						SceneManager::GetScene()->DeleteEntity(myPlayers[i].myEntity);
					}
				}

				std::cout << "Player Disconnect" << std::endl;
				break;
			}

			case NetworkMessages::PlayerChangeSkin:
			{
				PlayerSkinChangeMessage skinMsg;
				someData >> skinMsg;

				for(int i = 0; i < myPlayers.size(); i++)
				{
					if(myPlayers[i].ClientID == skinMsg.ClientID)
					{
						myPlayers[i].LoadSkin(skinMsg.SkinID);
					}
				}

				break;
			}
		}
	};

	myClient.SetReceiveMessageLoop(messageLoop);
	myClient.Connect(myHost, DEFAULT_PORT);

	TurNet::TurMessage connectMessage;
	connectMessage.Header.ID = TurNet::NetworkDataTypes::Connect;

	myClient.SendToServer(connectMessage);
}

void NetworkingLayer::StartNetworkingServer()
{
	
}
