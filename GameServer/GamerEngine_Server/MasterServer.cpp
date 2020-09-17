#include <iostream>
#include "include/enet/enet.h"

#include <map>

class ClientData {
public:
    ClientData(int id) : m_id(id) {}

    void SetUsername(std::string username) {
        m_username = username;
    }

    int GetID() { return m_id; }
    std::string GetUsername() { return m_username; }


private:
    int m_id;
    std::string m_username;
};

std::map<int, ClientData*> clients;

void ParseData(ENetHost* server, int id, char* data) 
{
    int data_type;
    sscanf_s(data, "%d|", &data_type);

    switch (data_type) 
    {
        case 1:
            break;
        case 2:
            char username[80];
            sscanf_s(data, "2|%[^\n]", &username);

            char send_data[1024] = { '\0' };
            sprintf_s(send_data, "2|%d|%s", id, username);
            std::cout << "SEND: " << send_data << "\n";

            clients[id]->SetUsername(username);

            break;

            break;
    }
}


int main(int argc, char** argv)
{
    if (enet_initialize() != 0) 
    {
        fprintf(stderr, "An error occurred while initializing Enet!\n");
        return EXIT_FAILURE;
    }
    atexit(enet_deinitialize);

    ENetAddress address;
    ENetHost* server;
    ENetEvent event;

    address.host = ENET_HOST_ANY;
    address.port = 5555;

    
    server = enet_host_create(&address, 32, 1, 0, 0);

    if (server == NULL) 
    {
        fprintf(stderr, "An error occurred while trying to create ENet Server!\n");
        return EXIT_FAILURE;
    }


    // GAME LOOP START
    int new_player_id = 0;
    while (true) 
    {
        while (enet_host_service(server, &event, 1000) > 0) 
        {
            switch (event.type) 
            {
                case ENET_EVENT_TYPE_CONNECT:
                    printf("A new client connected form %x:%u.\n", event.peer->address.host, event.peer->address.port);
                    new_player_id++;
                    break;
                case ENET_EVENT_TYPE_RECEIVE:
                    printf("A packet of length %u containing %s was received from %s:%u on channel %s.\n",
                        event.packet->dataLength,
                        event.peer->address.host,
                        event.peer->address.port,
                        event.channelID);
                    enet_packet_destroy(event.packet);
                    
                    break;

                case ENET_EVENT_TYPE_DISCONNECT:
                    printf("%x:%u disconnected.\n", event.peer->address.host, event.peer->address.port);
                    
                    break;
            }
        }
    }
    // GAME LOOP END

    enet_host_destroy(server);
    return EXIT_SUCCESS;
}

