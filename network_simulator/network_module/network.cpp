#include <string>
#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <thread>
#include <signal.h>
#include "connection_binder.hpp"

using namespace std;

constexpr const char* server_path = "/tmp/network_sim";
void handleConnection(int fd);


int main()
{
    signal(SIGPIPE, SIG_IGN);

    int fd;
    fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if( fd == -1 )
    {
        printf( "Encountered error: %s\n", strerror(errno));
        exit(-1);
    }
    /* Construct server socket address, bind socket to it,
    and make this a listening socket */
    if (remove(server_path) == -1 && errno != ENOENT)
    {
        printf( "Encountered error: %s\n", strerror(errno));
        exit(-1);;
    }

    sockaddr_un addr;
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, server_path, sizeof(addr.sun_path) - 1);
    
    if (bind(fd, (struct sockaddr *) &addr, sizeof(sockaddr_un)) == -1)
    {
        printf( "Encountered error: %s\n", strerror(errno));
        exit(-1);
    }

    if(listen(fd, 10) == -1)
    {
        printf( "Encountered error: %s\n", strerror(errno));
        exit(-1);;
    }


    thread handler(handleConnection, fd);
    while (true)
    {
        string command;
        getline(cin, command);
    }

}

void handleConnection(int fd)
{
    while (true)
    {
        sockaddr_un addr;
        socklen_t len = sizeof(sockaddr_un);
        int clientfd = accept(fd, (sockaddr*)&addr, &len);

        bindConnection(clientfd);

    }
}
