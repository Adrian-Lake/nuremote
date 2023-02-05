#include <iostream>
#include <cstring>
#include <cstdlib>

#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <unistd.h>


#include "configurable.hpp"
#include "rpc_host.hpp"



int main(int argv, char **argc)
{
    initInput();

    struct sockaddr_un address; // unix socket struct
    memset(&address, 0, sizeof(address)); // init struct
    address.sun_family = AF_UNIX; // socket type must allways be deffined
    strncpy(address.sun_path, HOST_PATH, sizeof(HOST_PATH)); // set host location

    // create socket
    int srvSockFd = socket(AF_UNIX, SOCK_STREAM, 0);
    HANDLE_ERROR_ERRNO(srvSockFd < 0, "socket", std::exit(ERR_STARTUP))

    // if the address already exists, delete it
    int rcUnlink =  unlink(HOST_PATH);
    HANDLE_ERROR_ERRNO(rcUnlink < 0 && errno != ENOENT, "unlink", std::exit(ERR_STARTUP))

    int rcBind =  bind(srvSockFd, (struct sockaddr *) &address, sizeof(address));
    HANDLE_ERROR_ERRNO(rcBind < 0, "bind", std::exit(ERR_STARTUP))

    // give R/W access to other users/groups
    int rcChmod =  chmod(HOST_PATH, 666); 
    HANDLE_ERROR_ERRNO(rcChmod < 0, "chmod", std::exit(ERR_STARTUP))

    int rcListen =  listen(srvSockFd, HOST_BACKLOG);
    HANDLE_ERROR_ERRNO(rcListen < 0, "listen", std::exit(ERR_STARTUP))

    message_t message{};
    

    // start accepting msgs
    while(true)
    {
        memset(&message, 0, messageMaxSize); // init struct

        int cliSockFd =  accept(srvSockFd, NULL, NULL);
        HANDLE_ERROR_ERRNO(cliSockFd < 0, "accept", continue)

        int rcRecvMsg = recvMsg(cliSockFd, &message);
        
        // handle msg, create input events on host
        int rcHandleMsg = handleMsg(&message);
        
        // note, handle this in the future https://stackoverflow.com/questions/19172804/crash-when-sending-data-without-connection-via-socket-in-linux
        int nBytesSent = send(cliSockFd, &rcHandleMsg, sizeof(rcHandleMsg), 0);
        HANDLE_ERROR_ERRNO(nBytesSent < 0, "send", MSG_NOSIGNAL)

        int rcCliSockClose = close(cliSockFd);
        HANDLE_ERROR_ERRNO(rcCliSockClose < 0, "close", NULL)

    }

    int rcSrvSockClose = close(srvSockFd);
    HANDLE_ERROR_ERRNO(rcSrvSockClose < 0, "close", NULL)

    deinitInput();

    std::cout << "Exiting without error." << std::endl;
    return OK;

}