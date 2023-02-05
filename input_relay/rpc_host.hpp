#ifndef IR_RPC_HOST_HPP
#define IR_RPC_HOST_HPP

#include <cstring>
#include <cstdlib>

#include <sys/socket.h>

#include <X11/Xlib.h> // libx11-dev
#include <X11/Xutil.h> // same as above
#include <X11/Intrinsic.h> // libxt-dev
#include <X11/extensions/XTest.h> // libxtst-dev
// sudo apt install libx11-dev libxt-dev libxtst-dev

#include "rpc_common.hpp"

int initInput();

int deinitInput();

int recvMsg(int cliSockFd, message_t *message_p);

int handleMsg(message_t *message_p);


#endif
