#include <iostream>
#include "include/enet/enet.h"
#include "NetworkServer.h"
#include <map>


NetworkServer server;

int main(int argc, char** argv)
{
    server.Init();
    server.StartServer(5555, 32);

    // GAME LOOP START
    while (1) {
        server.Update();
    }
    // GAME LOOP END

    
    
}

