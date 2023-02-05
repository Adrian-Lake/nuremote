
#ifndef IR_RPC_CLIENT_HPP
#define IR_RPC_CLIENT_HPP

#include <iostream>
#include <cstring>

#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <unistd.h>

#include "utilities.hpp"
#include "rpc_common.hpp"


extern "C" {

    int connectHost();

    int disconnectHost();

    int null();

    int keyPress(int keyCode, int action);

    int buttonPress(int keyCode, int action);

    int relMotion(int x, int y);

    int absMotion(int x, int y);

    int dummy(int param);

}

#endif
