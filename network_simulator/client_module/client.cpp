#include "client.hpp"
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <cstdlib>
#include <sys/uio.h>
#include "../network_module/connection_channel.hpp"

ConnectedChannel establishConnection()
{
    ConnectedChannel channel;

    channel.fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if( channel.fd == -1 )
    {
        printf( "Encountered error: %s\n", strerror(errno));
        exit(-1);
    }

    sockaddr_un addr;
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, "/tmp/network_sim", sizeof(addr.sun_path) - 1);
    
    if(connect(channel.fd, (sockaddr*)&addr, sizeof(sockaddr_un)) == -1 )
    {
        printf( "Encountered error: %s\n", strerror(errno));
        exit(-1);
    }

    //read only one byte, rest will be handled later
    int n = recv( channel.fd, &channel.id, 1, 0);
    if(n == -1 )
    {
        printf( "Encountered error: %s\n", strerror(errno));
        exit(-1);
    }

    return channel;
}

int sendTo(const ConnectedChannel &channel, const uint8_t reciever, const void *msg, const uint32_t msgLen)
{
    PacketHeader header;
    header.protocol = PROTOCOL_NONE;
    header.dest = reciever;
    header.src = channel.id;
    header.payloadSize = msgLen;

    iovec buffers[2] ;
    buffers[0].iov_base = &header;
    buffers[0].iov_len = sizeof(PacketHeader);
    buffers[1].iov_base = const_cast<void *>( msg ) ;
    buffers[1].iov_len = msgLen;

    int n = writev(channel.fd,  buffers, 2);

    if(n = -1)
    {
        return -1;
    }
    if( n != sizeof(PacketHeader) + msgLen)
    {
        return -2;
    }
    return n;
}

int recvFrom(ConnectedChannel& channel, char *msgBuffer, unsigned int buffSize, uint8_t* src, uint8_t* protocol)
{
    PacketHeader header;
    // read EXACTLY header size of bytes
    int readSizeHeader = 0;
    int n = recv( channel.fd, msgBuffer, sizeof(PacketHeader), 0);
    readSizeHeader += n;
    while (readSizeHeader< sizeof(PacketHeader))
    {
        n = recv( channel.fd, msgBuffer, sizeof(PacketHeader) - readSizeHeader, 0);
        readSizeHeader += n;
    }
    memcpy(&header, msgBuffer, sizeof(PacketHeader));
    
    if(src)
    {
        *src = header.src;
    }

    if(protocol)
    {
        *protocol = header.protocol;
    }

    if(header.payloadSize > buffSize - sizeof(PacketHeader))
    {
        printf("payload size too large for buffer of size %d", buffSize);
        std::terminate();
    }

    //read EXACTLY payload size of bytes
    int readSizeBody = 0;
    while (readSizeBody < header.payloadSize )
    {
        int n = recv(channel.fd, msgBuffer + readSizeBody, header.payloadSize - readSizeBody, 0);
        readSizeBody += n;
    }

    return readSizeBody ;
}
