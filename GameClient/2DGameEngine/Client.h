#pragma once

#include <iostream>
#include "include/enet/enet.h"

class Client {

	ENetHost* client;
	ENetAddress address;
	ENetEvent event;
	ENetPeer* peer;

public:
	int Init()
	{
		if (enet_initialize() != 0)
		{
			fprintf(stderr, "An error occurred while initializing Enet!\n");
			return EXIT_FAILURE;
		}
		atexit(enet_deinitialize);


		client = enet_host_create(NULL, 1, 1, 0, 0);

		if (client == NULL)
		{
			fprintf(stderr, "An error occurred while trying to create ENet client!\n");
			return EXIT_FAILURE;
		}

		enet_address_set_host(&address, "127.0.0.1");
		address.port = 5555;

		peer = enet_host_connect(client, &address, 1, 0);
		if (peer == NULL)
		{
			fprintf(stderr, "Can't Connect to server!\n");
			return EXIT_FAILURE;
		}

		if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
		{
			puts("Connection to Server succeeded.");
		}
		else
		{
			enet_peer_reset(peer);
			puts("Connection to server failed");
		}



		while (enet_host_service(client, &event, 1000) > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_RECEIVE:
				printf("A packet of length %u containing %s was received from %s on channel %u.\n",
					event.packet->dataLength,
					event.peer->address.host,
					event.peer->address.port,
					event.channelID);
				enet_packet_destroy(event.packet);
				break;
			}
		}




		/*enet_peer_disconnect(peer, 0);

		while (enet_host_service(client, &event, 3000) > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_RECEIVE:
				enet_packet_destroy(event.packet);
				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				puts("Disconnect Succesful");
				break;
			}
		}*/
	}

	void Update() 
	{
		
	}

};