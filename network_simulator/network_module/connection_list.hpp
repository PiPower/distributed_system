#ifndef CONNECTION_LIST
#define CONNECTION_LIST

#include <thread>

#define MAX_CLIENT_COUNT 254
#define BROADCAST_ID 255
#define SLOT_COUNT 254


struct ConnenctionDescription
{
    int fd;
    std::thread channelExecutor;
};

struct ConnectionList
{
    bool isConnectionActive[SLOT_COUNT];
    ConnenctionDescription descriptors[SLOT_COUNT];
};

#endif