#include "ServerUDP.h"

#include <cassert>
#include <cstdio>
#include <iostream>
#include <ostream>
#include <stdio.h>

#include "ServerMessageQueue.h"
#include "TurNet/Shared/DataHandling/NetworkDataTypes.h"
#include "TurNet/Shared/DataHandling/MessageData.h"
#include "TurNet/Shared/Utility/TurCompare.h"
#include "TurNet/Shared/Utility/TurTimer.h"
#include "TurNet/Security/Crypt.hpp"

TurNet::ServerUDP::ServerUDP()
{

}

TurNet::ServerUDP::~ServerUDP()
{
	Stop();
}

bool TurNet::ServerUDP::Start()
{
	struct sockaddr_in server {};

	if(!StartWinSock())
	{
		return false;
	}

	if((myListenSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
	{
		std::cout << "socket() failed with error code : " << WSAGetLastError() << std::endl;;
		closesocket(myListenSocket);
		WSACleanup();
		return false;
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(DEFAULT_PORT);

	u_long mode = 1;  // 1 to enable non-blocking socket
	if(ioctlsocket(myListenSocket, FIONBIO, &mode) == SOCKET_ERROR)
	{
		return false;
	}

	myReturnResult = bind(myListenSocket, (struct sockaddr*)&server, sizeof(server));
	if(myReturnResult == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
		closesocket(myListenSocket);
		WSACleanup();
		return false;
	}

	/*struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 100;
	if(setsockopt(myListenSocket, SOL_SOCKET, SO_SNDTIMEO, (char*)&tv, sizeof(tv)) < 0)
	{
		std::cout << "Error setting send timeout" << std::endl;
		return false;
	}*/


	std::cout << "Winsock started up without problems!\n";
	myStatus.Status = Status::Started;

	return true;
}

void TurNet::ServerUDP::StartWorker()
{
	myWorkerShouldRun = true;


	myWorkerThread.reset(new std::thread([this]()
	{
		WorkerThread();
	}));
	myHeartbeatThread.reset(new std::thread([this]()
	{
		HeartbeatThread();
	}));

	myStatus.Status = Status::Connected;
}


int TurNet::ServerUDP::SendToClient(sockaddr_in aAddress, TurNet::TurMessage& aMessage)
{
	std::string messageToClient = aMessage.BufferRaw();

	std::string crypted = TurNet::Encrypt("Test", messageToClient);

	if(messageToClient.size() > DEFAULT_BUFFER_SIZE)
	{
		std::cout << "Can't send a message over the size of: " << DEFAULT_BUFFER_SIZE << "\n";
		return 0;
	}

	char aDataBuffer[DEFAULT_BUFFER_SIZE];
	std::memcpy(aDataBuffer, crypted.c_str(), crypted.size());


	return SendToClientRaw(aAddress, aDataBuffer, static_cast<int>(crypted.size()));
}



void TurNet::ServerUDP::Stop()
{

	if((myStatus.Status == Status::Connected) || (myStatus.Status == Status::Started))
	{
		// Do something here
	}

	myWorkerShouldRun = false;
	if(myWorkerThread)
	{
		myWorkerThread->join();
		myWorkerThread.reset();
	}

	if(myHeartbeatThread)
	{
		myHeartbeatThread->join();
		myHeartbeatThread.reset();
	}

}

int TurNet::ServerUDP::SendToClientRaw(sockaddr_in aAddress, char aDataBuffer[DEFAULT_BUFFER_SIZE], int aSize)
{

	int length = sizeof(struct sockaddr_in);
	int result = sendto(myListenSocket, aDataBuffer, aSize, 0, reinterpret_cast<const struct sockaddr*>(&aAddress), length);
	if(result == SOCKET_ERROR)
	{
		std::cout << "send failed with error: " << WSAGetLastError() << "\n";
		closesocket(myListenSocket);
		myWorkerShouldRun = false;
		WSACleanup();
	}

	return result;
}

void TurNet::ServerUDP::WorkerThread()
{


	while(myWorkerShouldRun)
	{

		if (!ServerMessageQueue::Empty())
		{
			auto message = ServerMessageQueue::Top();

			for(int i = 0; i < myClients.size(); i++)
			{
				SendToClient(myClients[i].Address, message);
			}
		}

		try
		{
			MessageData messageData;
			std::memset(messageData.Buffer, 0, DEFAULT_BUFFER_SIZE);



			int nRet = 0;


			
				sockaddr connection;
				int slen = sizeof(sockaddr);
				nRet = recvfrom(myListenSocket, messageData.Buffer, messageData.BufferLength, 0, (struct sockaddr*)&connection, &slen);
				if(nRet == 0)
				{
					std::cout << "Disconnect" << std::endl;
					continue;
				}
				else if(nRet == SOCKET_ERROR)
				{
					int ierr = WSAGetLastError();
					if(ierr != WSAEWOULDBLOCK)
					{
						//// currently no data available
						using namespace std::literals::chrono_literals;
						std::this_thread::sleep_for(50ms);  // wait and try again
						continue;
					}
					else if(ierr == WSAEWOULDBLOCK && nRet <= 0)
					{
						using namespace std::literals::chrono_literals;
						std::this_thread::sleep_for(50ms);  // wait and try again
						continue;
					}
					else
					{
						sockaddr_in* sin = reinterpret_cast<sockaddr_in*>(&connection);
#pragma warning(suppress : 4996)
						const char* ipAddress = inet_ntoa(sin->sin_addr);
						std::cout << "Packet from " << ipAddress << ":" << sin->sin_port << "\n";
						std::cout << "Packet Size: " << messageData.BufferReceiveType << "\n";

						TurNet::TurMessage data;
						TurNet::MessageDecoder(data, messageData);

						messageData.Connection = *sin;

						for(int i = 0; i < myClients.size(); i++)
						{
							if(TurNet::CompareClients(myClients[i].Address, messageData.Connection))
							{
								myClients.erase(myClients.begin() + i);
							}
						}
					}


				}
				else
				{

					messageData.BufferReceiveType = nRet;


					std::string unhashedData = TurNet::Decrypt("Test", messageData.Buffer);
					memcpy(messageData.Buffer, unhashedData.data(), unhashedData.size());
					messageData.BufferLength = unhashedData.size();

					sockaddr_in* sin = reinterpret_cast<sockaddr_in*>(&connection);
#pragma warning(suppress : 4996)
					const char* ipAddress = inet_ntoa(sin->sin_addr);
					std::cout << "Packet from " << ipAddress << ":" << sin->sin_port << "\n";
					std::cout << "Packet Size: " << messageData.BufferReceiveType << "\n";

					TurNet::TurMessage data;
					TurNet::MessageDecoder(data, messageData);

					messageData.Connection = *sin;

					data.Header.Connection = messageData.Connection;

					if(messageData.BufferReceiveType > DEFAULT_BUFFER_SIZE)
					{
						std::cout << "Buffer was too big" << std::endl;
					}



					switch(data.Header.ID)
					{
						case TurNet::NetworkDataTypes::Connect:
						{
							TurNet::TurMessage message;
							message.Header.ID = TurNet::NetworkDataTypes::Connect;
							std::string connectMessage = "Client connected to server.";
							message << connectMessage;
							SendToClient(data.Header.Connection, message);
							ClientConnection client;



							client.Address = messageData.Connection;
							client.ResetTimers(myServerConfig.TimerValue, myServerConfig.TimeoutValue);
							myClients.push_back(std::move(client));

							break;
						}
						case TurNet::NetworkDataTypes::Disconnect:
						{
							std::cout << "Disconnect Client" << std::endl;
							for(int i = 0; i < myClients.size(); i++)
							{
								if(TurNet::CompareClients(myClients[i].Address, messageData.Connection))
								{
									myClients.erase(myClients.begin() + i);
								}
							}

							break;
						}
						case TurNet::NetworkDataTypes::Ping:
						{

							ClientConnection client;
							client.Address = messageData.Connection;

							client.ResetTimers(myServerConfig.TimerValue, myServerConfig.TimeoutValue);

							myClients.push_back(std::move(client));

							TurNet::TurMessage pingMsg;
							pingMsg.Header.ID = NetworkDataTypes::Ping;

							SendToClient(messageData.Connection, pingMsg);

							break;
						}
						case TurNet::NetworkDataTypes::Heartbeat:
						{
							for(int i = 0; i < myClients.size(); i++)
							{
								if(TurNet::CompareClients(myClients[i].Address, messageData.Connection))
								{
									myClients[i].ResetTimers(myServerConfig.TimerValue, myServerConfig.TimeoutValue);
								}
							}
							break;
						}
						case TurNet::NetworkDataTypes::Message:
						{
							bool foundClient = false;
							for(int i = 0; i < myClients.size(); i++)
							{
								if(TurNet::CompareClients(myClients[i].Address, messageData.Connection))
								{
									foundClient = true;
									myClients[i].ResetTimers(myServerConfig.TimerValue, myServerConfig.TimeoutValue);
								}
							}

							if(!foundClient)
							{
								TurNet::TurMessage message;
								message.Header.ID = TurNet::NetworkDataTypes::Connect;
								std::string connectMessage = "Client reconnected to server.";
								message << connectMessage;

								SendToClient(data.Header.Connection, message);

								ClientConnection client;
								client.Address = messageData.Connection;

								client.ResetTimers(myServerConfig.TimerValue, myServerConfig.TimeoutValue);

								myClients.push_back(std::move(client));
								data.Header.ID = NetworkDataTypes::ClientReconnect;
							}
							break;
						}
					}
					myMessageLoop(data);
				}
		}
		catch(const std::overflow_error& e)
		{
			// this executes if f() throws std::overflow_error (same type rule)
			std::cout << "Got error code: " << e.what() << " - please contact developer" << std::endl;
		}
		catch(const std::runtime_error& e)
		{
			// this executes if f() throws std::underflow_error (base class rule)
			std::cout << "Got error code: " << e.what() << " - please contact developer" << std::endl;
		}
		catch(const std::exception& e)
		{
			// this executes if f() throws std::logic_error (base class rule)
			std::cout << "Got error code: " << e.what() << " - please contact developer" << std::endl;
		}
		catch(...)
		{
			std::cout << "Unknown error" << std::endl;
		}
	}

	{
		using namespace std::literals::chrono_literals;
		std::this_thread::sleep_for(100ms);
	}


}

void TurNet::ServerUDP::HeartbeatThread()
{
	TurNet::TurTimer timer;

	while(myWorkerShouldRun)
	{
		timer.Update();

		for(int i = 0; i < myClients.size(); i++)
		{
			if(myClients[i].WaitingTimeout)
			{
				myClients[i].TimeoutTimer -= timer.GetDeltaTime();

				if(myClients[i].TimeoutTimer <= 0.0f)
				{
					std::cout << "Remove client in heartbeat loop" << std::endl;
					myClients.erase(myClients.begin() + i);
				}
			}
			else if(myClients[i].Timer <= 0.0f)
			{
				myClients[i].WaitingTimeout = true;
				myClients[i].HasSendMessage = true;
				TurNet::TurMessage outMsg;
				TurHeartbeatMessage heartMsg;
				outMsg << heartMsg;

				SendToClient(myClients[i].Address, outMsg);
			}
			else
			{
				myClients[i].Timer -= timer.GetDeltaTime();
			}
		}

		{
			using namespace std::literals::chrono_literals;
			std::this_thread::sleep_for(100ms);
		}
	}
}
