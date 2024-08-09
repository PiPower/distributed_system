#ifndef CONNECTION_CHANNEL
#define CONNECTION_CHANNEL
#include "connection_list.hpp"

struct PacketHeader
{
    // to keep data aligned 
    uint8_t reserved[2];
    uint8_t dest;
    uint8_t src;
    uint32_t payloadSize;
};

void handleChannel(int client_fd, uint8_t id, const volatile ConnectionList* connection);
int prepareMsg(int client_fd, int index, char* msgBuffer, unsigned int buffSize);
int routeMsg(char* msgBuffer, const volatile ConnectionList* connection);
#endif