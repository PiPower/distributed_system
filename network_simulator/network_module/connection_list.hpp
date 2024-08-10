#ifndef CONNECTION_LIST
#define CONNECTION_LIST

#include <thread>

#define MAX_CLIENT_COUNT 254
#define BROADCAST_ID 255
#define SLOT_COUNT 254

#define THREAD_UNSUED 0x00
#define THREAD_STARTING 0x01
#define THREAD_RUNNING 0x02


struct ConnenctionDescription
{
    int fd;
    std::thread channelExecutor;
};

struct ConnectionList
{
    uint8_t isConnectionActive[SLOT_COUNT];
    ConnenctionDescription descriptors[SLOT_COUNT];
};

#endif