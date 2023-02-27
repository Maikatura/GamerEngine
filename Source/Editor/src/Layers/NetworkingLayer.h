#pragma once
#include <Layers/Layer.h>
#include <TurNet/Client/ClientUDP.h>

#include "Components/Components.hpp"
#include "Renderer/Model/Entity.h"
#include "TurNet/Server/ServerUDP.h"


struct ClientData
{
	int ClientID;
	float MoveSpeed;
	std::chrono::steady_clock::time_point Time;
	Entity myEntity;
	Vector3f Translation;
	Vector3f Rotation;


	void LoadSkin(short aSkin)
	{
		std::string skin = "Editor\\Models\\network\\";

		switch(aSkin)
		{
		case 0:
			skin += "NetworkCube.dds";
			break;
		case 1:
			skin += "NetworkCube1.dds";
			break;
		}



		myEntity.GetComponent<ModelComponent>().GetModel()->GetMaterial()->SetAlbedoTexture(TextureAssetHandler::GetTexture(Helpers::string_cast<std::wstring>(skin)));
	}
};


class NetworkingLayer : public Layer
{
public:


	NetworkingLayer();

	void OnAttach() override;
	bool OnDetach() override;
	bool OnImGuiRender() override;
	void OnUpdate() override;
	void Listen();

	static TurNet::ClientUDP& GetClient() { return myClient; }

private:
	void StartNetworkingClient();
	void StartNetworkingServer();

	bool myIsHost = false;
	std::string myHost;
	std::string myPort;

	
	const float myMoveSendTime = 0.0f;
	float mySendTime = 0.0f;

	inline static int myClientID;

	inline static std::vector<ClientData> myPlayers;
	inline static std::vector<sockaddr_in> myClientsConnected;

	inline static TurNet::ClientUDP myClient;
	inline static TurNet::ServerUDP myServer;
};
