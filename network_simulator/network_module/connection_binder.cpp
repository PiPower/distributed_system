#include "connection_binder.hpp"
#include "connection_list.hpp"
#include "connection_channel.hpp"
#include <sys/socket.h>
#include <thread>
using namespace std;

static ConnectionList connectionList;

static void addEntryIntoList(int connectionFd, uint8_t index);


int bindConnection(int connectionFd)
{
    for(int i=0; i < SLOT_COUNT; i++)
    {
        if(!connectionList.isConnectionActive[i])
        {
            addEntryIntoList(connectionFd, i);
            return 1;
        }
    }
    return 0;
}


void addEntryIntoList(int connectionFd, uint8_t index)
{
    connectionList.isConnectionActive[index] = true;
    connectionList.descriptors[index].fd = connectionFd;
    // thread must start last as it needs all the data to be ready
    connectionList.descriptors[index].channelExecutor = thread{handleChannel, connectionFd, index, &connectionList};
}

