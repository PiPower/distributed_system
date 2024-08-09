#ifndef CONNECTION_CHANNEL
#define CONNECTION_CHANNEL
#include "connection_list.hpp"

void handleChannel(int client_fd, const volatile ConnectionList* connection);

#endif