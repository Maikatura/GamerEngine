#pragma once
#include <Layers/Layer.h>
#include <TurNet/Client/ClientUDP.h>

#include "Components/Components.hpp"
#include "Renderer/Model/Entity.h"
#include "TurNet/Server/ServerUDP.h"


struct ClientData
{
	uint64_t ClientID;
	float MoveSpeed;
	std::chrono::steady_clock::time_point Time;
	Entity myEntity;
	Vector3f Translation;
	Vector3f Rotation;
	bool IsServer;


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


		//(*myEntity.GetComponent<ModelComponent>().GetModel()->GetMaterial())[0].SetAlbedoTexture(TextureAssetHandler::GetTexture(Helpers::string_cast<std::wstring>(skin)));
	}
};


class NetworkingLayer : public Layer
{
public:


	NetworkingLayer();

	void OnAttach() override;
	bool OnDetach() override;
	void OnImGuiRender() override;
	void OnUpdate() override;
	void Listen();

	static TurNet::ClientUDP& GetClient() { return myClient; }

private:
	void StartNetworkingClient();
	void StartNetworkingServer();

	bool myIsHost = false;
	std::string myHost;
	std::string myPort;

	float myRecvResetTime;
	int myRecvBytes;
	int myRecvBytes2;
	int myRealRecvSize;
	
	const float myMoveSendTime = 0.0f;
	float mySendTime = 0.0f;

	inline static uint64_t myClientID;

	inline static std::vector<ClientData> myPlayers;
	inline static std::vector<sockaddr_in> myClientsConnected;

	inline static TurNet::ClientUDP myClient;
	inline static TurNet::ServerUDP myServer;
};
