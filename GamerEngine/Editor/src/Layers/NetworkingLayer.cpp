#include "Editor.pch.h"
#include <Layers/NetworkingLayer.h>

#include "Time.hpp"
#include "Components/Components.hpp"
#include "Components/Network/NetworkComponent.h"
#include "Core/Model/Entity.h"
#include "Network/CreateObjectMessage.h"
#include "Scene/SceneManager.h"
#include "TurNet/Shared/Utility/TurCompare.h"

#include "Network/NetOverloads.h"
#include "Network/MoveObjectMessage.h"
#include "Network/NetworkSettings.h"
#include "Network/PlayerConnectMessage.h"
#include "Network/PlayerDisconnectMessage.h"
#include "Network/PlayerMoveMessage.h"
#include "Network/SkinChangerMessage.h"
#include "AssetHandlers/ModelAssetHandler.h"
#include "Core/Rendering/Renderer.h"

using namespace std::chrono_literals;

NetworkingLayer::NetworkingLayer() : Layer("Networking"), mySendTime(1.0f), myMoveSendTime((1.0f / 30.0f) * 10.0f)
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

void NetworkingLayer::OnImGuiRender()
{
	NetworkSettings::IsClient = true;

	BeginMenu();
	

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

	ImGui::SameLine();

	if(ImGui::Button("Disconnect"))
	{
		myClient.Stop();
	}

	ImGui::InputText("Host IP", &myHost);
	//ImGui::InputText("Port", &myPort);



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
		skinMsg.SkinID = static_cast<short>(currentItem);

		outMsg << skinMsg;

		myClient.SendToServer(outMsg);
	}
	

	if (myRecvResetTime <= 0.0f)
	{
		myRecvBytes2 = myRecvBytes;
		myRecvBytes = NetworkingLayer::GetClient().GetRecvBytes();
		myRecvResetTime += 1.0f;

		if (myRecvBytes != myRecvBytes2)
		{
			myRealRecvSize = myRecvBytes - myRecvBytes2;
		}
	}
	else
	{
		myRecvResetTime -= Time::GetDeltaTime();
	}

	ImGui::Text("Bytes Per Seconds: ");
	ImGui::SameLine();

	const char* bytes = std::to_string(myRealRecvSize).c_str();
	ImGui::Text(bytes);

	EndMenu();
}

void NetworkingLayer::OnUpdate()
{
	Layer::OnUpdate();

	UpdateCamera();

	for(int i = 0; i < myPlayers.size(); i++)
	{
		if(!SceneManager::Get().GetScene()->GetRegistry().valid(myPlayers[i].myEntity))
		{
			continue;
		}

		auto& transComp = myPlayers[i].myEntity.GetComponent<GamerEngine::TransformComponent>();

		float distance = (myPlayers[i].Translation - transComp.GetPosition()).Length();
		
		//float distanceRot = (myPlayers[i].Rotation - transComp.Rotation).Length();

		if (!Renderer::GetCamera())
		{
			return;
		}

		Renderer::GetCamera()->GetCameraSpeed();

		std::chrono::steady_clock::time_point clockNow = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> time = clockNow - myPlayers[i].Time;

		Vector3f oldPosition = transComp.GetPosition();
		//Vector3f newPosition = oldPosition + (myPlayers[i].MoveSpeed * Time::GetDeltaTime());

		float lerpTime = static_cast<float>(time.count()) / 1000.0f;
		float time_to_reach_targetTan = CommonUtilities::Max(0.0f, (distance / myPlayers[i].MoveSpeed) - lerpTime);
		transComp.SetPosition(Vector3f::Lerp(transComp.GetPosition(), myPlayers[i].Translation, Time::GetDeltaTime()));
		transComp.SetPosition(myPlayers[i].Rotation);
	}

}

void NetworkingLayer::Listen()
{
}

void NetworkingLayer::UpdateCamera()
{
	if(mySendTime <= 0.0f)
	{
		if(!Renderer::GetCamera())
		{
			return;
		}

		if(Renderer::GetCamera()->HasMoved())
		{
			mySendTime += myMoveSendTime;



			const Vector3f position = Renderer::GetCamera()->ViewProjection.GetPosition();
			const Vector3f rotation = Renderer::GetCamera()->ViewProjection.GetRotation();


			TurNet::TurMessage outMsg;
			PlayerMoveMessage moveMsg;
			moveMsg.ClientID = myClientID;
			moveMsg.Time = std::chrono::high_resolution_clock::now();
			moveMsg.MoveSpeed = Renderer::GetCamera()->GetCameraSpeed();
			moveMsg.Translation = position;

			outMsg << moveMsg;
			myClient.SendToServer(outMsg);

			Renderer::GetCamera()->SetHasMoved(false);
		}
	}
	else
	{
		mySendTime -= Time::GetDeltaTime();
	}
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

		switch(NetworkMessages messageType = static_cast<NetworkMessages>(typeBase))
		{
			case NetworkMessages::ObjectMove:
			{
				ObjectMoveMessage moveMsg;
				someData >> moveMsg;

				std::cout << "Got Something from server";

				auto view = SceneManager::Get().GetScene()->GetRegistry().view<IDComponent, GamerEngine::TransformComponent, Network::NetworkComponent>();
				for(auto entity : view)
				{
					if (!SceneManager::Get().GetScene()->GetRegistry().valid(entity))
					{
						return;
					}

					auto& ID = view.get<IDComponent>(entity);

					if(ID.ID == moveMsg.EntityID)
					{
						auto& netComp = view.get<Network::NetworkComponent>(entity);
						netComp.SetNewPosition(moveMsg.Transform.GetPosition());
						std::cout << moveMsg << std::endl;
					}
				}
				break;
			}
			case NetworkMessages::ServerObjectMove:
			{
				NetObjectMoveMessage moveMsg;
				someData >> moveMsg;

				std::cout << "Server Moved A Object" << std::endl;

				auto view = SceneManager::Get().GetScene()->GetRegistry().view<IDComponent, GamerEngine::TransformComponent, Network::NetworkComponent>();
				for(auto& entity : view)
				{
					if(!SceneManager::Get().GetScene()->GetRegistry().valid(entity))
					{
						return;
					}

					auto& netComp = view.get< Network::NetworkComponent>(entity);
					if(netComp.GetID() == moveMsg.EntityID)
					{
						netComp.SetNewPosition(moveMsg.Transform.GetPosition());
					}
				}
				break;
			}
			case NetworkMessages::ObjectSpawn:
			{
				ObjectCreateMessage createMsg;
				someData >> createMsg;

				std::cout << "Got Something from server" << std::endl;

				SceneManager::Get().CreateEntityType(createMsg.EntityType, createMsg.EntityID);

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
				float scale = 20.0f;

				GamerEngine::UUID id = GamerEngine::UUID(playerConnectMsg.ClientID);
				data.ClientID = id;
				data.IsServer = playerConnectMsg.IsServer;
				data.Translation = playerConnectMsg.Translation;
				data.MoveSpeed = 0.0f;
				data.Time = std::chrono::high_resolution_clock::now();
				data.Rotation = { 0.0f,0.0f,0.0f };

				data.myEntity = SceneManager::Get().CreateEntityType(0, id);
				data.myEntity.GetComponent<GamerEngine::TransformComponent>().SetPosition(playerConnectMsg.Translation);
				data.myEntity.GetComponent<GamerEngine::TransformComponent>().SetScale({ scale, scale ,scale });
				auto& networkComp = data.myEntity.AddComponent<Network::NetworkComponent>();
				networkComp.SetID(id);
				networkComp.SetServer(playerConnectMsg.IsServer);

				auto model = data.myEntity.AddComponent<ModelComponent>(ModelAssetHandler::Get().GetModelInstance(L"Editor\\Models\\network\\NetworkCube.fbx"));
				model.GetModel()->GetMeshData(0).MaterialData.SetAlbedoTexture(TextureAssetHandler::GetTexture(L"Editor\\Models\\network\\NetworkCube.dds"));
				model.GetModel()->GetMeshData(0).MaterialData.SetNormalTexture(TextureAssetHandler::GetTexture(L"Editor\\Textures\\T_Default_N.dds"));
				model.GetModel()->GetMeshData(0).MaterialData.SetMaterialTexture(TextureAssetHandler::GetTexture(L"Editor\\Textures\\T_Default_M.dds"));
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
						//myPlayers[i].Rotation = moveMsg.Rotation;
						myPlayers[i].Time = moveMsg.Time;
						myPlayers[i].MoveSpeed = moveMsg.MoveSpeed;
					}
				}

				break;
			}
			case NetworkMessages::PlayerDisconnect:
			{

				PlayerDisconnectMessage disMsg{};
				someData >> disMsg;

				for(int i = 0; i < myPlayers.size(); i++)
				{
					if(myPlayers[i].ClientID == disMsg.ClientID)
					{
						SceneManager::Get().GetScene()->DeleteEntity(myPlayers[i].myEntity);
					}
				}

				std::cout << "Player Disconnect" << std::endl;
				break;
			}

			case NetworkMessages::PlayerChangeSkin:
			{
				PlayerSkinChangeMessage skinMsg{};
				someData >> skinMsg;

				for(int i = 0; i < myPlayers.size(); i++)
				{
					if(myPlayers[i].ClientID == skinMsg.ClientID && !myPlayers[i].IsServer)
					{
						myPlayers[i].LoadSkin(skinMsg.SkinID);
					}
				}

				break;
			}
		default: 
			break;
		}
	};

	myClient.SetReceiveMessageLoop(messageLoop);
	myClient.Connect(myHost, DEFAULT_PORT);

	TurNet::TurMessage connectMessage;
	connectMessage.Header.ID = TurNet::NetworkDataTypes::Connect;

	myClient.SendToServer(connectMessage, true);
}

void NetworkingLayer::StartNetworkingServer()
{
	
}
