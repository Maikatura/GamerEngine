#include "ClientUDP.h"
#include <cassert>
#include <cstdio>
#include <iostream>
#include <ostream>
#include <stdio.h>

#include "TurNet/Security/Crypt.hpp"
#include "TurNet/Shared/DataHandling/NetworkDataTypes.h"
#include "TurNet/Shared/DataHandling/MessageData.h"
#include "TurNet/Shared/Utility/TurCompare.h"
#include "TurNet/Shared/Utility/TurTimer.h"


TurNet::ClientUDP::ClientUDP(): myConnectSocket(0), myServer(), myServerPort(0)
{
	myStatus.Status = Status::Disconnected;
}

TurNet::ClientUDP::~ClientUDP()
{
	Stop();
}

bool TurNet::ClientUDP::Connect(const std::string& aAddress, u_short aPort)
{
	TIMEVAL Timeout;
	Timeout.tv_sec = 0;
	Timeout.tv_usec = 100;


	myServerIP = aAddress;
	myServerPort = aPort;

	memset((char*)&myServer, 0, sizeof(myServer));
	myServer.sin_family = AF_INET;
#pragma warning(suppress : 4996)
	myServer.sin_addr.S_un.S_addr = inet_addr(myServerIP.c_str());
	myServer.sin_port = htons(myServerPort);

	if(myServer.sin_addr.s_addr == INADDR_NONE)
		return false;

	if(connect(myConnectSocket, (SOCKADDR*)&myServer, sizeof(myServer)) == SOCKET_ERROR)
	{
		int err = WSAGetLastError();
		printf("Failed to connect:  Error code: %d.\n", err);
		WSACleanup();
		return false;
	}


	myStatus.Status = Status::Connected;
	StartWorker();
	return true;
}

bool TurNet::ClientUDP::Start()
{
	if(!StartWinSock())
	{
		return false;
	}

	if((myConnectSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
	{
		std::cout << "socket() failed with error code : " << WSAGetLastError() << std::endl;
		Stop();
		return false;
	}

	u_long mode = 1;  // 1 to enable non-blocking socket
	if(ioctlsocket(myConnectSocket, FIONBIO, &mode) == SOCKET_ERROR)
	{
		return false;
	}

	//int timeout = 750; //in milliseconds. this is 0.3 seconds

	//setsockopt(myConnectSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(int)); //setting the receive timeout

	myStatus.Status = Status::Started;

	std::cout << "Winsock started up without problems!\n";
	return true;
}

void TurNet::ClientUDP::Stop()
{
	if((myStatus.Status == Status::Connected))
	{
		TurNet::TurMessage outMsg;
		outMsg.Header.ID = NetworkDataTypes::Disconnect;
		SendToServer(outMsg);

		myReturnResult = shutdown(myConnectSocket, SD_SEND);
		if(myReturnResult == SOCKET_ERROR)
		{
			printf("shutdown failed with error: %d\n", WSAGetLastError());
			closesocket(myConnectSocket);
			WSACleanup();
		}

		myStatus.Status = Status::Disconnected;
	}

	myWorkerShouldRun = false;
	if(myWorkerThread)
	{
		myWorkerThread->join();
		myWorkerThread.reset();
	}
}

void TurNet::ClientUDP::StartWorker()
{
	myWorkerShouldRun = false;

	if(myWorkerThread)
	{
		myWorkerThread->join();
		myWorkerThread.reset();
	}

	myWorkerShouldRun = true;
	myWorkerThread.reset(new std::thread([&]() { WorkerThread(); }));
}

int TurNet::ClientUDP::SendToServer(TurNet::TurMessage& aMessage)
{
	if(myStatus.Status != Status::Connected && aMessage.Header.ID != NetworkDataTypes::Ping)
	{
		return -1;
	}

	std::string messageToServer = aMessage.BufferRaw();

	std::string crypted = TurNet::Encrypt("Test", messageToServer);

	if (crypted.size() > DEFAULT_BUFFER_SIZE)
	{
		std::cout << "Can't send a message over the size of: " << DEFAULT_BUFFER_SIZE << "\n";
		return 0;
	}

	char aDataBuffer[DEFAULT_BUFFER_SIZE];
	std::memcpy(aDataBuffer, crypted.c_str(), crypted.size());

	return SendToServerRaw(aDataBuffer, static_cast<int>(crypted.size()));
}

int TurNet::ClientUDP::SendToServerRaw(char aDataBuffer[DEFAULT_BUFFER_SIZE], int aSize)
{
	int length = sizeof(myServer);
	int result = sendto(myConnectSocket, aDataBuffer, aSize, 0, (struct sockaddr*)&myServer, length);
	if(result == SOCKET_ERROR)
	{
		std::cout << "send failed with error: " << WSAGetLastError() << "\n";
		closesocket(myConnectSocket);
		WSACleanup();
		myWorkerShouldRun = false;
	}

	mySentBytes += aSize;
	return result;
}

void TurNet::ClientUDP::WorkerThread()
{
	while(myWorkerShouldRun)
	{
		if (!myWorkerShouldRun)
		{
			break;
		}


		try
		{
			MessageData messageData;
			std::memset(messageData.Buffer, 0, DEFAULT_BUFFER_SIZE);


			int nRet = 0;

			

			
				sockaddr connection;
				int slen = sizeof(sockaddr);
				nRet = recvfrom(myConnectSocket, messageData.Buffer, messageData.BufferLength, 0, (struct sockaddr*)&connection, &slen);
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
						//// currently no data available
						using namespace std::literals::chrono_literals;
						std::this_thread::sleep_for(50ms);  // wait and try again
						continue;
					}
					else
					{
						printf("recvfrom error: %d\n", ierr);
					}
				}
				else
				{

					messageData.BufferReceiveType = nRet;

					std::string unhashedData = TurNet::Decrypt("Test", messageData.Buffer);
					memcpy(messageData.Buffer, unhashedData.data(), unhashedData.size());
					messageData.BufferLength = unhashedData.size();

					TurNet::TurMessage data;
					TurNet::MessageDecoder(data, messageData);




					sockaddr_in* sin = reinterpret_cast<sockaddr_in*>(&connection);
					const char* ipAddress = inet_ntoa(sin->sin_addr);
					std::cout << "Packet from " << ipAddress << ":" << sin->sin_port << "\n";
					std::cout << "Packet Size: " << messageData.BufferReceiveType << "\n";

					messageData.Connection = *sin;

					data.Header.Connection = messageData.Connection;

					if(messageData.BufferReceiveType == -1)
					{
						myStatus.Status = Status::Reconnecting;
						data.Header.ID = NetworkDataTypes::ClientDisconnected;
					}

					TurNet::NetworkDataTypes type = data.Header.ID;

					if(type == NetworkDataTypes::Ping)
					{
						myStatus.Status = Status::Connected;
						return;
					}
					else if(myStatus.Status == Status::Reconnecting)
					{

						using namespace std::literals::chrono_literals;
						std::this_thread::sleep_for(2s);

						TurNet::TurMessage outMsg;
						outMsg.Header.ID = NetworkDataTypes::Ping;
						SendToServer(outMsg);
						return;
					}

					bool runMessage = true;

					switch(type)
					{
						case TurNet::NetworkDataTypes::Ping:
						{
							runMessage = false;
							break;
						}
						case TurNet::NetworkDataTypes::Connect:
						{
							std::cout << "You are now connected to the server!" << "\n";
							myStatus.Status = TurNet::Status::Connected;
							break;
						}
						case TurNet::NetworkDataTypes::ClientConnect:
						{
							std::cout << "A client connected" << std::endl;
							break;
						}
						case TurNet::NetworkDataTypes::ClientDisconnect:
						{
							std::cout << "A Client disconnected" << std::endl;
							break;
						}
						case TurNet::NetworkDataTypes::Heartbeat:
						{
							TurMessage outMsg;
							TurNet::TurHeartbeatMessage heartbeatMsg;
							outMsg << heartbeatMsg;

							SendToServer(outMsg);
							break;
						}
					}

					if(runMessage)
					{
						myMessageLoop(data);
					}
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
}
