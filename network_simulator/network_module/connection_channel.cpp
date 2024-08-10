#include "connection_channel.hpp"
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <inttypes.h>
#include "connection_binder.hpp"
#define MSG_BUFFER_LEN 10000
// role of a channel thread is to take output from client and redirect it 
// to correct clients
void handleChannel(int client_fd, uint8_t id, const volatile ConnectionList *connection, volatile uint8_t *connectionStatus)
{
    int n = send(client_fd, &id, 1, 0);
    if(n == -1 )
    {
        printf( "Encountered error: %s\n", strerror(errno));
        *connectionStatus = CONNECTION_ERROR;
        removeChannel(id);
        return;
    }

    *connectionStatus = CONNECTION_ESTABLISHED;
    while (*connectionStatus != CONNECTION_ACK)
    {
        /*  Wait until binder acknowledges connection, by setting
            it alive, this way binder sets connection status to true
            this prevent the case where recv from channel is called before
            binder. In this case if recv returns error message removeChannel 
            might be called before binder setts apprioriate flag, which means
            that binder setting connection as alive will be called after binder 
            killing connection what is ERROR.  
        */
    }
    
    char msgBuffer[MSG_BUFFER_LEN];
    while (true)
    {
        int readSize = prepareMsg(client_fd, id, msgBuffer, MSG_BUFFER_LEN);
        if(readSize == 0)
        {
            removeChannel(id);
            return;
        }
        routeMsg(msgBuffer, connection );
    }
}

int prepareMsg(int client_fd, int index, char *msgBuffer, unsigned int buffSize)
{
    PacketHeader header;
    // read EXACTLY header size of bytes
    int readSizeHeader = 0;
    int n = recv( client_fd, msgBuffer, sizeof(PacketHeader), 0);
    if(n <= 0)
    {
        return n;
    }
    readSizeHeader += n;
    while (readSizeHeader < sizeof(PacketHeader))
    {
        n = recv( client_fd, msgBuffer, sizeof(PacketHeader) - readSizeHeader, 0);
        if(n <= 0)
        {
            return n;
        }
        readSizeHeader += n;
    }
    memcpy(&header, msgBuffer, sizeof(PacketHeader));
    

    if(header.payloadSize > buffSize - sizeof(PacketHeader))
    {
        printf("payload size too large for client %d", index);
        std::terminate();
    }

    //read EXACTLY payload size of bytes
    int readSizeBody = 0;
    while (readSizeBody < header.payloadSize )
    {
        int n = recv(client_fd, msgBuffer + readSizeBody + sizeof(PacketHeader), header.payloadSize - readSizeBody, 0);
        if(n <= 0)
        {
            return n;
        }
        readSizeBody += n;
    }

    return readSizeHeader + readSizeBody;
}

int routeMsg(char *msgBuffer, const volatile ConnectionList *connection)
{
    uint8_t dest = *(uint8_t*)(msgBuffer + 2);
    uint32_t payloadSize =  *(uint32_t*)(msgBuffer + 4);
    ssize_t n;
    if(dest == BROADCAST_ID)
    {
        uint8_t source = *(uint8_t*)(msgBuffer + 3);
        for(int i=0; i < SLOT_COUNT; i++)
        {
            if( connection->isConnectionActive[i] == THREAD_RUNNING && i != source)
            {
                int n = send(connection->descriptors[i].fd, msgBuffer,  payloadSize + sizeof(PacketHeader), 0);
            }
        }
    }
    else if (connection->isConnectionActive[dest] == THREAD_RUNNING)
    {
        return send(connection->descriptors[dest].fd, msgBuffer,  payloadSize + sizeof(PacketHeader), 0);
    }
    

    return ENOTSOCK;
}

