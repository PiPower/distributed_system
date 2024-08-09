#include <string>
#include <unordered_map>
#include "network_simulator/client_module/client.hpp"
#define BROADCAST_PORT 10000u
using namespace std;



int main(int argc, char* argv[])
{       
    ConnectedChannel channel = establishConnection();
    const char* buffer = "HEHEHEHHE";
    sendTo(channel, 255, buffer, 10);
    char recvBuffer[10000];
    while (true)
    {
        recvFrom(channel, recvBuffer, 10000);
        printf("Recieved Message: %s\n", recvBuffer);
        fflush(stdout);
    }
    return 0;
}

