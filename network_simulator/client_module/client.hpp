#ifndef CLIENT_API
#define CLIENT_API

#include <inttypes.h>

struct ConnectedChannel
{
    int fd;
    uint8_t id;
};


ConnectedChannel establishConnection();
int sendTo(const ConnectedChannel& channel, const uint8_t reciever, const void* msg, const uint32_t msgLen);
int recvFrom(ConnectedChannel& channel, char *msgBuffer, unsigned int buffSize, uint8_t* src = nullptr, uint8_t* protocol = nullptr);
#endif