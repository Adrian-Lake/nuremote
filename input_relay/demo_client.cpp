#include <iostream>

#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <unistd.h>

#include "rpc_client.hpp"

int main(){

    struct sockaddr_un address; // unix socket struct
    memset(&address, 0, sizeof(address)); // init struct
    address.sun_family = AF_UNIX; // socket type must always be defined
    strncpy(address.sun_path, HOST_PATH, sizeof(HOST_PATH)); // set host location

    int sockFd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(sockFd < 0)
    {
        std::cout << "Could not create socket. errno: " << errno << std::endl;
        return ERR_STARTUP;
    }


    int rcConnect = connect(sockFd, (struct sockaddr *) &address, sizeof(address));
    if(rcConnect < 0)
    {
        std::cout << "Could not connect socket. errno: " << errno << std::endl;
        return ERR_STARTUP;
    }


    sendDummyMsg(sockFd);

    int rcClose = close(sockFd);
    if(rcClose < 0)
    {
        std::cout << "Could not close socket. errno: " << errno << std::endl;
        return ERR_STARTUP;
    }

}