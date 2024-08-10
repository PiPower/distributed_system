#ifndef CONNECTION_BINDER
#define CONNECTION_BINDER

#include <inttypes.h>

int bindConnection(int connectionFd);
void removeChannel(uint8_t id);

#endif