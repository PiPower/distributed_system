#include "connection_channel.hpp"
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#define MSG_BUFFER_LEN 10000
// role of a channel thread is to take output from client and redirect it 
// to correct clients
void handleChannel(int client_fd, const volatile ConnectionList *connection)
{
    uint8_t id = connection->descriptors->fd;
    int n = send(client_fd, &id, 1, 0);
    if(n == -1 )
    {
        printf( "Encountered error: %s\n", strerror(errno));
        exit(-1);
    }

    char msgBuffer[MSG_BUFFER_LEN];
    while (true)
    {
        int readSize = prepareMsg(client_fd, id, msgBuffer, MSG_BUFFER_LEN);
        routeMsg(msgBuffer, connection );
    }
    

}

int prepareMsg(int client_fd, int index, char *msgBuffer, unsigned int buffSize)
{
    PacketHeader header;
    int zdsz = sizeof(PacketHeader);
    // read EXACTLY header size of bytes
    int readSizeHeader = 0;
    int n = recv( client_fd, msgBuffer, sizeof(PacketHeader), 0);
    readSizeHeader += n;
    while (readSizeHeader< sizeof(PacketHeader))
    {
        n = recv( client_fd, msgBuffer, sizeof(PacketHeader) - readSizeHeader, 0);
        readSizeHeader += n;
    }
    memcpy(&header, msgBuffer, sizeof(PacketHeader));
    

    if(header.payloadSize > MSG_BUFFER_LEN - sizeof(PacketHeader))
    {
        printf("payload size too large for client %d", index);
        std::terminate();
    }

    //read EXACTLY payload size of bytes
    int readSizeBody = 0;
    while (readSizeBody < header.payloadSize )
    {
        int n = recv(client_fd, msgBuffer + readSizeBody + sizeof(PacketHeader), header.payloadSize - readSizeBody, 0);
        readSizeBody += n;
    }

    return readSizeHeader + readSizeBody;
}

int routeMsg(char *msgBuffer, const volatile ConnectionList *connection)
{
    uint8_t dest = *(uint8_t*)msgBuffer;
    uint32_t payloadSize =  *(uint32_t*)(msgBuffer + 2);
    ssize_t n;
    if(dest == BROADCAST_ID)
    {
        uint source = *(uint32_t*)(msgBuffer + 1);
        for(int i=0; i < SLOT_COUNT; i++)
        {
            if( connection->isConnectionActive[i])
            {
                send(connection->descriptors[i].fd, msgBuffer,  payloadSize + sizeof(PacketHeader), 0);
            }
        }
    }
    else if (connection->isConnectionActive[dest])
    {
        return send(connection->descriptors[dest].fd, msgBuffer,  payloadSize + sizeof(PacketHeader), 0);
    }
    

    return ENOTSOCK;
}
