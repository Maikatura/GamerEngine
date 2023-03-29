// Server.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <TurNet/Server/ServerUDP.h>
#include <TurNet/Shared/Utility/TurCompare.h>
#include "../Editor/src/Layers/Network/CreateObjectMessage.h"
#include "../Editor/src/Layers/Network/MoveObjectMessage.h"
#include "Math/Vector3.hpp"

int main()
{
	TurNet::ServerUDP myServer;
	std::vector<sockaddr_in> myClientsConnected;

	myServer.Start();

	auto function = [&myServer, &myClientsConnected](TurNet::TurMessage& someData)
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

				myClientsConnected.push_back(someData.Header.Connection);

				break;
			}
			case TurNet::NetworkDataTypes::Disconnect:
			{
				std::cout << "Client Disconnected" << std::endl;

				for(size_t i = 0; i < myClientsConnected.size(); i++)
				{
					if (TurNet::CompareClients(someData.Header.Connection, myClientsConnected[i]))
					{
						myClientsConnected.erase(myClientsConnected.begin() + i);
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
							if(!TurNet::CompareClients(someData.Header.Connection, myClientsConnected[i]))
							{
								myServer.SendToClient(myClientsConnected[i], outMsg);
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
							if(!TurNet::CompareClients(someData.Header.Connection, myClientsConnected[i]))
							{
								myServer.SendToClient(myClientsConnected[i], outMsg);
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
