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
#include "Math/Vector3.hpp"


struct ClientData
{
	int ClientID;
	sockaddr_in ClientAddress;
	Vector3f Translation;
};

int main()
{
	TurNet::ServerUDP myServer;
	std::vector<ClientData> myClientsConnected;

	myServer.Start();

	int ClientID = 0;

	auto function = [&myServer, &myClientsConnected,&ClientID](TurNet::TurMessage& someData)
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
				myClientsConnected.push_back(data);

				TurNet::TurMessage msgOut;
				PlayerConnectSelfMessage playerMsg;
				playerMsg.ClientID = ClientID;
				msgOut << playerMsg;

				TurNet::TurMessage msgOutNewPlayer;
				PlayerConnectMessage playerMsgNewPlayer;
				playerMsgNewPlayer.ClientID = ClientID;
				msgOutNewPlayer << playerMsgNewPlayer;
				

				for(int i = 0; i < myClientsConnected.size(); i++)
				{
					if(!TurNet::CompareClients(someData.Header.Connection, myClientsConnected[i].ClientAddress))
					{

						TurNet::TurMessage msgOut2;
						PlayerConnectMessage playerMsg2;
						playerMsg2.ClientID = myClientsConnected[i].ClientID;
						msgOut2 << playerMsg2;

						myServer.SendToClient(myClientsConnected[i].ClientAddress, msgOutNewPlayer);
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
					if(TurNet::CompareClients(someData.Header.Connection, myClientsConnected[i].ClientAddress))
					{
						auto it = myClientsConnected.begin() + i;
						disMsg.ClientID = it->ClientID;
						myClientsConnected.erase(it);
					}
				}

				outMsg << disMsg;

				for(int i = 0; i < myClientsConnected.size(); i++)
				{
					myServer.SendToClient(myClientsConnected[i].ClientAddress, outMsg);
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
							if(!TurNet::CompareClients(someData.Header.Connection, myClientsConnected[i].ClientAddress))
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
							if(!TurNet::CompareClients(someData.Header.Connection, myClientsConnected[i].ClientAddress))
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


						
						/*for(int i = 0; i < myClientsConnected.size(); i++)
						{
							if (TurNet::CompareClients(myClientsConnected[i].ClientAddress, someData.Header.Connection))
							{
								if (myClientsConnected[i].ClientID != mvInMsg.ClientID)
								{
									return;
								}
							}
						}*/


						TurNet::TurMessage outMsg;
						PlayerMoveMessage mvMsg;

						mvMsg.ClientID = mvInMsg.ClientID;
						mvMsg.Time = mvInMsg.Time;
						mvMsg.MoveSpeed = mvInMsg.MoveSpeed;
						mvMsg.Translation = mvInMsg.Translation;
						mvMsg.Rotation = mvInMsg.Rotation;

						outMsg << mvMsg;

						for(int i = 0; i < myClientsConnected.size(); i++)
						{
							if(myClientsConnected[i].ClientID != mvInMsg.ClientID)
							{
								myServer.SendToClient(myClientsConnected[i].ClientAddress, outMsg);
							}
						}
						break;
					}
					case NetworkMessages::PlayerChangeSkin:
					{
						PlayerSkinChangeMessage skinMsg;
						someData >> skinMsg;

						for(int i = 0; i < myClientsConnected.size(); i++)
						{
							if(myClientsConnected[i].ClientID != skinMsg.ClientID)
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

	while(true)
	{
		using namespace std::literals::chrono_literals;
		std::this_thread::sleep_for(1s);
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
