#include <string>
#include <unordered_map>
#include "network_simulator/client_module/client.hpp"
#include<unistd.h>
#define BROADCAST_PORT 10000u
using namespace std;



int main(int argc, char* argv[])
{       
    ConnectedChannel channel = establishConnection();
    const char* buffer = "HEHEHEHHE";
    char recvBuffer[10000];
    while (true)
    {
        sendTo(channel, 255, buffer, 10);
        recvFrom(channel, recvBuffer, 10000);
        printf("Recieved Message: %s\n", recvBuffer);
        fflush(stdout);
        sleep(1);
    }
    return 0;
}

