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

void removeChannel(uint8_t id)
{
    // IMPORTANT: function called from channel thread
    // join here is unallowed
    connectionList.isConnectionActive[id] = THREAD_UNSUED;
    close(connectionList.descriptors[id].fd);

}

void addEntryIntoList(int connectionFd, uint8_t index)
{
    connectionList.descriptors[index].fd = connectionFd;
    connectionList.isConnectionActive[index] = THREAD_STARTING;
    // thread must start last as it needs all the data to be ready
    if(connectionList.descriptors[index].channelExecutor.joinable() )
    {
        connectionList.descriptors[index].channelExecutor.join();
    }

    /* 
        By setting thread as alive at the end we make sure that
        first possible message sent to client is by channel handler(as it is required by protocol)
        connectionStatus variables states what exactly happend inside of handler, there two possible outcomes
        1. Connection if established means connectionStatus == CONNECTION_ESTABLISHED 
        2. If error occured during hello message, binder returns from this thread yet it is called
        on channel thread to remove this connection
        This synchronization schema removes possibility of connectionList.isConnectionActive having
        uncorrect value
    */
    volatile uint8_t connectionStatus = CONNECTION_NOT_ESTABLISHED;
    connectionList.descriptors[index].channelExecutor = thread{handleChannel, connectionFd, index, &connectionList, &connectionStatus};

    while (connectionStatus == CONNECTION_NOT_ESTABLISHED)
    {
        // bussy waiting
    }
    
    if(connectionStatus == CONNECTION_ESTABLISHED)
    {
        connectionList.isConnectionActive[index] = THREAD_RUNNING;
    }
    connectionStatus = CONNECTION_ACK;
}

