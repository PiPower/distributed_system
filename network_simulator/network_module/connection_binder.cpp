#include "connection_binder.hpp"
#include "connection_list.hpp"
#include "connection_channel.hpp"
#include <thread>
using namespace std;

static ConnectionList connectionList;

static void addEntryIntoList(int connectionFd, int index);


int bindConnection(int connectionFd)
{
    for(int i=0 ; i < SLOT_COUNT; i++)
    {
        if(!connectionList.isConnectionActive)
        {
            addEntryIntoList(connectionFd, i);
            return 1;
        }
    }
    return 0;
}


void addEntryIntoList(int connectionFd, int index)
{
    connectionList.isConnectionActive[index] = true;
    connectionList.descriptors[index].fd = connectionFd;
    // thread must start last as it needs all the data to be ready
    connectionList.descriptors[index].channelExecutor = thread{handleChannel, connectionFd, &connectionList};
}

