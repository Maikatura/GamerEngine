// Server.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <TurNet/Server/ServerUDP.h>
#include <TurNet/Shared/Utility/TurCompare.h>
#include "../Editor/src/Layers/Network/CreateObjectMessage.h"
#include "../Editor/src/Layers/Network/MoveObjectMessage.h"
#include "../Editor/src/Layers/Network/PlayerMoveMessage.h"
#include "../Editor/src/Layers/Network/PlayerConnectMessage.h"
#include "../Editor/src/Layers/Network/PlayerDisconnectMessage.h"
#include "../Editor/src/Layers/Network/SkinChangerMessage.h"
#include "Renderer/Scene/UUID.h"
#include "Math/Vector3.hpp"
#include <random>

#include "Network/NetworkSettings.h"
#include "Renderer/Scene/SceneManager.h"


#include <windows.h>

int PressAnyKey(const char* prompt)
{
	DWORD        mode;
	HANDLE       hstdin;
	INPUT_RECORD inrec;
	DWORD        count;
	char         default_prompt[] = "Press a key to continue...";

	/* Set the console mode to no-echo, raw input, */
	/* and no window or mouse events.              */
	hstdin = GetStdHandle(STD_INPUT_HANDLE);
	if(hstdin == INVALID_HANDLE_VALUE
		|| !GetConsoleMode(hstdin, &mode)
		|| !SetConsoleMode(hstdin, 0))
		return 0;

	if(!prompt) prompt = default_prompt;

	/* Instruct the user */
	WriteConsole(
		GetStdHandle(STD_OUTPUT_HANDLE),
		prompt,
		lstrlen((LPCWSTR)prompt),
		&count,
		NULL
	);

	FlushConsoleInputBuffer(hstdin);

	/* Get a single key RELEASE */
	do ReadConsoleInput(hstdin, &inrec, 1, &count);
	while((inrec.EventType != KEY_EVENT) || inrec.Event.KeyEvent.bKeyDown);

	/* Restore the original console mode */
	SetConsoleMode(hstdin, mode);

	return inrec.Event.KeyEvent.wVirtualKeyCode;
}

struct ClientData
{
	bool IsBot = false;
	uint64_t ClientID;
	sockaddr_in ClientAddress;
	Vector3f Translation;
};

std::random_device rd; // obtain a random number from hardware
std::mt19937 gen(rd());
std::uniform_int_distribution<> distr(-250, 250);


void LoadScene()
{
	SceneManager::LoadScene("Assets\\Scenes\\NetworkRandomMoverTester.csf");
}

HANDLE hStdin;

int main()
{
	NetworkSettings::IsClient = false;

	TurNet::ServerUDP myServer;
	std::vector<ClientData> myClientsConnected;
	myServer.Start();

	uint64_t ClientID = 0;

	/*for(int i = 0; i < 10; i++)
	{
		ClientData data;
		data.ClientID = UUID2();
		data.IsBot = true;
		float xPos = static_cast<float>(distr(gen));
		float yPos = static_cast<float>(distr(gen));
		float zPos = static_cast<float>(distr(gen));

		data.Translation = Vector3f(xPos, yPos, zPos);
		myClientsConnected.push_back(data);
	}*/


	SceneManager::Initialize();
	SceneManager::SetHeadless(true);
	LoadScene();


	auto function = [&myServer, &myClientsConnected, &ClientID](TurNet::TurMessage& someData)
	{
		TurNet::NetworkDataTypes type = someData.Header.ID;
		switch(type)
		{
			case TurNet::NetworkDataTypes::Connect:
			{
				TurNet::TurMessage message;
				message.Header.ID = TurNet::NetworkDataTypes::Connect;
				std::string connectMessage = "Welcome to this server!";

				message << connectMessage;
				myServer.SendToClient(someData.Header.Connection, message);

				ClientData data;
				data.ClientAddress = someData.Header.Connection;
				data.ClientID = ClientID;
				data.IsBot = false;
				myClientsConnected.push_back(data);

				TurNet::TurMessage msgOut;
				PlayerConnectSelfMessage playerMsg;
				playerMsg.ClientID = ClientID;
				msgOut << playerMsg;

				TurNet::TurMessage msgOutNewPlayer;
				PlayerConnectMessage playerMsgNewPlayer;
				playerMsgNewPlayer.ClientID = ClientID;
				playerMsgNewPlayer.Translation = myClientsConnected.back().Translation;
				playerMsgNewPlayer.IsServer = (myClientsConnected.back().IsBot ? true : false);

				msgOutNewPlayer << playerMsgNewPlayer;

				for(int i = 0; i < myClientsConnected.size(); i++)
				{
					if(!TurNet::CompareClients(someData.Header.Connection, myClientsConnected[i].ClientAddress))
					{

						TurNet::TurMessage msgOut2;
						PlayerConnectMessage playerMsg2;
						playerMsg2.ClientID = myClientsConnected[i].ClientID;
						playerMsg2.Translation = myClientsConnected[i].Translation;
						playerMsg2.IsServer = (myClientsConnected[i].IsBot ? true : false);
						msgOut2 << playerMsg2;

						if(!myClientsConnected[i].IsBot)
						{
							myServer.SendToClient(myClientsConnected[i].ClientAddress, msgOutNewPlayer);
						}

						myServer.SendToClient(someData.Header.Connection, msgOut2);
					}
					else
					{
						myServer.SendToClient(someData.Header.Connection, msgOut);
					}
				}

				ClientID++;
				break;
			}
			case TurNet::NetworkDataTypes::Disconnect:
			{
				std::cout << "Client Disconnected" << std::endl;

				TurNet::TurMessage outMsg;
				PlayerDisconnectMessage disMsg;

				for(int i = 0; i < myClientsConnected.size(); i++)
				{
					if(TurNet::CompareClients(someData.Header.Connection, myClientsConnected[i].ClientAddress) && !myClientsConnected[i].IsBot)
					{
						auto it = myClientsConnected.begin() + i;
						disMsg.ClientID = it->ClientID;
						myClientsConnected.erase(it);
					}
				}

				outMsg << disMsg;

				for(int i = 0; i < myClientsConnected.size(); i++)
				{
					if(!myClientsConnected[i].IsBot)
					{
						myServer.SendToClient(myClientsConnected[i].ClientAddress, outMsg);
					}
				}
				break;
			}
			case TurNet::NetworkDataTypes::Message:
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

						TurNet::TurMessage outMsg;
						outMsg << moveMsg;

						for(int i = 0; i < myClientsConnected.size(); i++)
						{
							if(!TurNet::CompareClients(someData.Header.Connection, myClientsConnected[i].ClientAddress) && !myClientsConnected[i].IsBot)
							{
								myServer.SendToClient(myClientsConnected[i].ClientAddress, outMsg);
							}
						}
						break;
					}
					case NetworkMessages::ObjectSpawn:
					{
						ObjectCreateMessage moveMsg;
						someData >> moveMsg;

						TurNet::TurMessage outMsg;
						outMsg << moveMsg;

						std::cout << "Got Something from server";

						for(int i = 0; i < myClientsConnected.size(); i++)
						{
							if(!TurNet::CompareClients(someData.Header.Connection, myClientsConnected[i].ClientAddress) && !myClientsConnected[i].IsBot)
							{
								myServer.SendToClient(myClientsConnected[i].ClientAddress, outMsg);
							}
						}

						break;
					}
					case NetworkMessages::PlayerMove:
					{
						PlayerMoveMessage mvInMsg;
						someData >> mvInMsg;

						TurNet::TurMessage outMsg;
						PlayerMoveMessage mvMsg;

						mvMsg.ClientID = mvInMsg.ClientID;
						mvMsg.Time = mvInMsg.Time;
						mvMsg.MoveSpeed = mvInMsg.MoveSpeed;
						mvMsg.Translation = mvInMsg.Translation;
						//mvMsg.Rotation = mvInMsg.Rotation;

						outMsg << mvMsg;

						for(int i = 0; i < myClientsConnected.size(); i++)
						{
							if(myClientsConnected[i].ClientID != mvInMsg.ClientID && !myClientsConnected[i].IsBot)
							{
								myServer.SendToClient(myClientsConnected[i].ClientAddress, outMsg);
							}
						}
						break;
					}
					case NetworkMessages::PlayerChangeSkin:
					{
						PlayerSkinChangeMessage skinMsg{};
						someData >> skinMsg;

						for(int i = 0; i < myClientsConnected.size(); i++)
						{
							if(myClientsConnected[i].ClientID != skinMsg.ClientID && !myClientsConnected[i].IsBot)
							{
								TurNet::TurMessage outMsg;
								outMsg << skinMsg;

								myServer.SendToClient(myClientsConnected[i].ClientAddress, outMsg);
							}
						}

						break;
					}
				}
				break;
			}

		}
	};

	myServer.SetReceiveMessageLoop(function);
	myServer.StartWorker();

	bool shouldRemoveNow = false;

	bool reloadedScene = false;

	HWND name;
	name = GetForegroundWindow();

	while(true)
	{
		if (GetAsyncKeyState('R') && name == GetForegroundWindow() && !reloadedScene)
		{
			reloadedScene = true;
			std::cout << "Reloaded Scene" << std::endl;
			LoadScene();

			using namespace std::literals::chrono_literals;
			std::this_thread::sleep_for(500ms);  // wait and try again
		}
		else
		{
			Time::Update();
			if(SceneManager::GetStatus() == SceneStatus::NeedSwap)
			{
				SceneManager::SwapScene();
			}

			if(SceneManager::GetStatus() == SceneStatus::Complete && SceneManager::GetScene())
			{
				reloadedScene = false;
				SceneManager::GetScene()->OnUpdate(true);
			}
		}
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
