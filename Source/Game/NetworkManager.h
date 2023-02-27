#pragma once
#include "TurNet/Client/ClientUDP.h"
class NetworkClient
{
	static TurNet::ClientUDP myClient;
	inline static std::string myHost = "127.0.0.1";

public:
	static void Start();

	inline static bool Started = false;

};
