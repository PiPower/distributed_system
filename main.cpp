#include <string>
#include <unordered_map>
#include "network_simulator/client_module/client.hpp"
#include<unistd.h>
#include <string.h>
#define BROADCAST_PORT 10000u
using namespace std;



int main(int argc, char* argv[])
{       
    ConnectedChannel channel = establishConnection();
    char sendBuffer[1000];
    char recvBuffer[10000];
    int i =0;

    while (true)
    {
        sprintf(sendBuffer, "Client with index %d sends message number %d", channel.id, i);
        sendTo(channel, 255, sendBuffer, strlen(sendBuffer) + 1);
        int n = recvFrom(channel, recvBuffer, 10000);
        printf("Recieved Message: %s, recieved msg size is %d\n", recvBuffer, n);
        memset(recvBuffer, 0, strlen(recvBuffer ) + 1);
        fflush(stdout);
        sleep(1);
        i+= 1;
    
    }
    return 0;
}

