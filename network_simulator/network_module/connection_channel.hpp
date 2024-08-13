#ifndef CONNECTION_CHANNEL
#define CONNECTION_CHANNEL
#include "connection_list.hpp"

#define CONNECTION_NOT_ESTABLISHED 0x00
#define CONNECTION_ESTABLISHED 0x01
#define CONNECTION_ERROR 0x02
#define CONNECTION_ACK 0x03

#define PROTOCOL_NONE 0x00
#define PROTOCOL_RAFT 0x01

struct PacketHeader
{
    // to keep data aligned 
    uint8_t reserved;
    //----------------
    uint8_t protocol;
    uint8_t dest;
    uint8_t src;
    uint32_t payloadSize;
};

void handleChannel(int client_fd, uint8_t id, const volatile ConnectionList* connection, volatile uint8_t *connectionStatus);
int prepareMsg(int client_fd, int index, char* msgBuffer, unsigned int buffSize);
int routeMsg(char* msgBuffer, const volatile ConnectionList* connection);

#endif