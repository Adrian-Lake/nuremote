#include "rpc_client.hpp"


struct sockaddr_un address; // unix socket struct
int sockFd;

extern "C" {
    
    int connectHost(){

        memset(&address, 0, sizeof(address)); // init struct
        address.sun_family = AF_UNIX; // socket type must allways be deffined
        strncpy(address.sun_path, HOST_PATH, sizeof(HOST_PATH)); // set host location

        sockFd = socket(AF_UNIX, SOCK_STREAM, 0);
        HANDLE_ERROR_ERRNO(sockFd < 0, "socket", return ERR_STARTUP;)

        int rcConnect = connect(sockFd, (struct sockaddr *) &address, sizeof(address));
        HANDLE_ERROR_ERRNO(rcConnect < 0, "connect", return ERR_STARTUP)

        return OK;
    }


    int disconnectHost(){

        int rcClose = close(sockFd);
        HANDLE_ERROR_ERRNO(rcClose < 0, "close", return ERR_EXIT)

        return rcClose;
    }


    int null(){

        connectHost();

        struct nullMsg_t msg{};    

        int rcSend = send(sockFd, &msg, sizeof(msg), 0);
        HANDLE_ERROR_ERRNO(rcSend < 0, "send", return ERR_SEND)

        int rcRecV = recv(sockFd, &msg, sizeof(int), 0);
        HANDLE_ERROR_ERRNO(rcRecV < 0, "recv", return ERR_RECV)
        
        disconnectHost();

        return rcRecV;
    }


    int keyPress(int keyCode, int action){

        connectHost();

        struct keyPressMsg_t msg{};
        msg.keyCode = keyCode;
        msg.action = action;

        int rcSend = send(sockFd, &msg, sizeof(msg), 0);
        HANDLE_ERROR_ERRNO(rcSend < 0, "send", return ERR_SEND)

        int rcRecV = recv(sockFd, &msg, sizeof(int), 0);
        HANDLE_ERROR_ERRNO(rcRecV < 0, "recv", return ERR_RECV)

        disconnectHost();

        return rcRecV;
    }

    int buttonPress(int buttonCode, int action){

        connectHost();

        struct buttonPressMsg_t msg{};
        msg.buttonCode = buttonCode;
        msg.action = action;

        // for(int i = 0; i < 16; i++){
        //     printf("%i, ", *((uint8_t*)&msg + i));
        // }
        // std::cout << sizeof(msg) << std::endl;

        int rcSend = send(sockFd, &msg, sizeof(msg), 0);
        HANDLE_ERROR_ERRNO(rcSend < 0, "send", return ERR_SEND)

        int rcRecV = recv(sockFd, &msg, sizeof(int), 0);
        HANDLE_ERROR_ERRNO(rcRecV < 0, "recv", return ERR_RECV)

        disconnectHost();

        return rcRecV;
    }

    int relMotion(int x, int y){

        connectHost();

        struct relMotionMsg_t msg{};
        msg.x = x;
        msg.y = y;

        int rcSend = send(sockFd, &msg, sizeof(msg), 0);
        HANDLE_ERROR_ERRNO(rcSend < 0, "send", return ERR_SEND)

        int rcRecV = recv(sockFd, &msg, sizeof(int), 0);
        HANDLE_ERROR_ERRNO(rcRecV < 0, "recv", return ERR_RECV)

        disconnectHost();

        return rcRecV;
    }

    int absMotion(int x, int y){

        connectHost();

        struct absMotionMsg_t msg{};
        msg.x = x;
        msg.y = y;

        int rcSend = send(sockFd, &msg, sizeof(msg), 0);
        HANDLE_ERROR_ERRNO(rcSend < 0, "send", return ERR_SEND)

        int rcRecV = recv(sockFd, &msg, sizeof(int), 0);
        HANDLE_ERROR_ERRNO(rcRecV < 0, "recv", return ERR_RECV)

        disconnectHost();

        return rcRecV;
    }

    int dummy(int param){

        connectHost();

        struct dummyMsg_t msg{};
        msg.param = param;

        int rcSend = send(sockFd, &msg, sizeof(msg), 0);
        HANDLE_ERROR_ERRNO(rcSend < 0, "send", return ERR_SEND)

        int rcRecV = recv(sockFd, &msg, sizeof(int), 0);
        HANDLE_ERROR_ERRNO(rcRecV < 0, "recv", return ERR_RECV)

        disconnectHost();

        return rcRecV;
    }
}
