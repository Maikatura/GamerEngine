#pragma once

#include "include/enet/enet.h"
#include "NetworkPacket.h"

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

#define NETWORK_EVENT_CONNECT     0x0
#define NETWORK_EVENT_DISCONNECT  0x1

#define MAX_NETWORK_MESSAGES 1000



class NetworkServer {

    typedef struct {
        int id;
        bool isLocalPlayer, hasAuthority;
    } network_identity;

    typedef struct {
        void* enet_peer;
    } network_peer;

    typedef struct {
        network_peer sender;
        size_t size;
        char* data;
    } network_data;

    typedef struct {
        int num_messages;
        network_data messages[MAX_NETWORK_MESSAGES];
    } network_events;

    
    ENetPeer* peer;
    ENetEvent event;
    ENetAddress address;

    network_events eventlist;
    network_events * hgeNetworkEventList() { return &eventlist; }

    bool hosting = false;
    bool connected = false;

    bool IsHost() { return hosting; }
    bool IsConnected() { return connected; }

    int num_connections = 0;
    


public:

    ENetHost* host;

    int Init() {
        printf("initializing networking engine");
        if (enet_initialize() != 0)
        {
            printf("an error occurred while initializing ENet");
            return EXIT_FAILURE;
        }
        atexit(enet_deinitialize);
        printf("initialized networking engine");
    }


    int StartServer(unsigned int port, unsigned int max_connections) {

        address.host = ENET_HOST_ANY;
        address.port = port;

        host = enet_host_create(&address	/* the address to bind the server host to */,
            max_connections	/* allow up to max_connections clients and/or outgoing connections */,
            1	/* allow up to 1 channel to be used. */,
            0	/* assume any amount of incoming bandwidth */,
            0	/* assume any amount of outgoing bandwidth */);

        if (host == NULL)
        {
            printf("an error occurred while trying to create an ENet server host");
            return EXIT_FAILURE;
        }

        hosting = true;
        connected = true;
        return EXIT_SUCCESS;
    }

    void Update() 
    {
        if (!connected) return;
        
        /* Wait up to 0 milliseconds for an event. */
        while (enet_host_service(host, &event, 0) > 0)
        {
            switch (event.type)
            {
            case ENET_EVENT_TYPE_CONNECT:
                printf("A new client connected from %x:%u.\n",
                    event.peer->address.host,
                    event.peer->address.port);

                num_connections++;
                
                // Allocates storage
                event.peer->data = (char*)malloc(255 * sizeof(char));
                printf((char*)event.peer->data, "%x:%u", event.peer->address.host, event.peer->address.port);
                break;
            case ENET_EVENT_TYPE_RECEIVE:
                eventlist.messages[eventlist.num_messages].sender.enet_peer = event.peer;
                eventlist.messages[eventlist.num_messages].size = event.packet->dataLength;
                eventlist.messages[eventlist.num_messages].data = (char*)malloc(event.packet->dataLength * sizeof(char));
                // Copy Data Into Buffer
                for (int i = 0; i < event.packet->dataLength; i++) {
                    eventlist.messages[eventlist.num_messages].data[i] = event.packet->data[i];
                }
                eventlist.num_messages++;

                /* Clean up the packet now that we're done using it. */
                enet_packet_destroy(event.packet);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                printf("%s disconnected.\n", event.peer->data);
                num_connections--;

                /* Reset the peer's client information. */
                free(event.peer->data);
                event.peer->data = NULL;
                break;
            }
        }

    }

    

    void NetworkBroadcastPacket(const char* data, size_t size) {
        ENetPacket* packet = enet_packet_create(data, size, ENET_PACKET_FLAG_RELIABLE);
        // Send the packet to all connected peers on channel 0
        enet_host_broadcast(host, 0, packet);
    }

    void NetworkSendPacket(/*ENetPeer* peer, */const char* data, size_t size) {
        ENetPacket* packet = enet_packet_create(data, size, ENET_PACKET_FLAG_RELIABLE);
        // Send the packet to the peer on channel 0
        enet_peer_send(peer, 0, packet);
    }

    void NetworkSendPacketTo(network_peer network_peer, const char* data, size_t size) {
        ENetPacket* packet = enet_packet_create(data, size, ENET_PACKET_FLAG_RELIABLE);
        // Send the packet to the peer on channel 0
        ENetPeer* n_peer = (ENetPeer*)network_peer.enet_peer;
        printf("Sending Packet To %s!\n", n_peer->data);
        enet_peer_send(n_peer, 0, packet);
    }

    



private:


};

