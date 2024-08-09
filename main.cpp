#include <ifaddrs.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unordered_map>

#define BROADCAST_PORT 10000u
using namespace std;



int main(int argc, char* argv[])
{    
    int fd;
    fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if( fd == -1 )
    {
        printf( "Encountered error: %s\n", strerror(errno));
        exit(-1);
    }

    sockaddr_un addr;
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, "/tmp/network_sim", sizeof(addr.sun_path) - 1);
    
    if(connect(fd, (sockaddr*)&addr, sizeof(sockaddr_un)) == -1 )
    {
        printf( "Encountered error: %s\n", strerror(errno));
        exit(-1);
    }

    while (true)
    {
    }
    return 0;
}

