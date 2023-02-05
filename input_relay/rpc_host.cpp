#include "rpc_host.hpp"

// TODO: Wayland compatibility, Remove x11 dependancies using uinput events.

Display *display; // x11 display

int initInput(){
    display = XOpenDisplay(NULL); // use display listed in enviroment
    HANDLE_ERROR(display == NULL, "XOpenDisplay", display, "Could not open X11 Display", std::exit(ERR_CONN_DISPLAY))

    return OK;
}

int deinitInput(){
    XCloseDisplay(display);

    return OK;
}

 // key press
int sendKeyDown(int keycode){
    XTestFakeKeyEvent(display, keycode, true, 0);
    XFlush(display);

    return OK;
}

int sendKeyUp(int keycode){
    XTestFakeKeyEvent(display, keycode, false, 0);
    XFlush(display);

    return OK;
}

int sendKeyPress(int keycode){
    XTestFakeKeyEvent(display, keycode, true, 0);
    XFlush(display);
    XTestFakeKeyEvent(display, keycode, false, 50);
    XFlush(display);

    return OK;
}

// button press
int sendButtonDown(int buttonCode){
    XTestFakeButtonEvent(display, buttonCode, true, 0);
    XFlush(display);

    return OK;
}

int sendButtonUp(int buttonCode){
    XTestFakeButtonEvent(display, buttonCode, false, 0);
    XFlush(display);

    return OK;
}

int sendButtonPress(int buttonCode){
    XTestFakeButtonEvent(display, buttonCode, true, 0);
    XFlush(display);
    XTestFakeButtonEvent(display, buttonCode, false, 50);
    XFlush(display);

    return OK;
}

int sendRelMotion(int x, int y){
    XTestFakeRelativeMotionEvent(display, x, y, 0);
    XFlush(display);

    return OK;
}

int sendAbsMotion(int x, int y){
    XTestFakeMotionEvent(display, -1, x, y, 0);
    XFlush(display);
    
    return OK;
}



int recvMsg(int cliSockFd, message_t *message_p){

        int nBytesRecvd =  recv(cliSockFd, message_p, messageIdSize, 0);
        HANDLE_ERROR_ERRNO(nBytesRecvd < 0, "recv", return ERR_RECV)

        nBytesRecvd =  recv(cliSockFd, (char*)message_p + nBytesRecvd, getMessageSize(message_p->id) - messageIdSize, 0);
        HANDLE_ERROR_ERRNO(nBytesRecvd < 0, "recv", return ERR_RECV)

        // std::cout << "get msg size " << getMessageSize(message_p->id) << std::endl;

        // for(int i = 0; i < 16; i++){
        //     printf("%i, ", *((uint8_t*)message_p + i));
        // }
        // std::cout << std::endl;

    return OK;
}

int handleNullMsg(nullMsg_t message){
    if(LOG_INPUT)
        std::cout << "Recived dummy message" << std::endl;
    return OK;
}

int handleKeyPressMsg(keyPressMsg_t message){
    if(LOG_INPUT){
        std::cout << "Recived key press message" << std::endl;
        std::cout << "Key Code: " << message.keyCode << std::endl;
        std::cout << "Key Action: " << message.action << std::endl;
    }

    if(message.action == ACTION_PRESS){
        return sendKeyPress(message.keyCode);
    }

    if(message.action == ACTION_UP){
        return sendKeyUp(message.keyCode);
    }

    if(message.action == ACTION_DOWN){
        return sendKeyDown(message.keyCode);
    }

    return OK;
}

int handleButtonPressMsg(buttonPressMsg_t message){
    if(LOG_INPUT){
        std::cout << "Recived button press message" << std::endl;
        std::cout << "Button Code: " << message.buttonCode << std::endl;
        std::cout << "Button Action: " << message.action << std::endl;
    }

    if(message.action == ACTION_PRESS){
        return sendButtonPress(message.buttonCode);
    }

    if(message.action == ACTION_UP){
        return sendButtonUp(message.buttonCode);
    }

    if(message.action == ACTION_DOWN){
        return sendButtonDown(message.buttonCode);
    }

    return OK;
    
}

int handleRelMotion(relMotionMsg_t message){
    if(LOG_INPUT){
        std::cout << "Recived relative motion message" << std::endl;
        std::cout << "x: " << message.x << ", y: " << message.y << std::endl;
    }

    return sendRelMotion(message.x, message.y);
}

int handleAbsMotion(absMotionMsg_t message){
    if(LOG_INPUT){
        std::cout << "Recived absolute motion message" << std::endl;
        std::cout << "x: " << message.x << ", y: " << message.y << std::endl;
    }

    return sendAbsMotion(message.x, message.y);
}

int handleDummyMsg(dummyMsg_t message){
    if(LOG_INPUT){
        std::cout << "Recived dummy message" << std::endl;
        std::cout << "ID: " << message.id << std::endl;
        std::cout << "Param: " << message.param << std::endl;
    }

    return OK;
}

int handleMsg(message_t *message_p){

    switch(message_p->id){

        case KEY_PRESS_MSG_ID:
            return handleKeyPressMsg(message_p->keyPress);

        case BUTTON_PRESS_MSG_ID:
            return handleButtonPressMsg(message_p->buttonPress);

        case REL_MOTION_MSG_ID:
            return handleRelMotion(message_p->relMotion);

        case ABS_MOTION_MSG_ID:
            return handleAbsMotion(message_p->absMotion);

        case DUMMY_MSG_ID:
            return handleDummyMsg(message_p->dummy);

        case NULL_MSG_ID:
            return handleNullMsg(message_p->null);

        default:
            if(LOG_INPUT)
                std::cout << "ERROR: No handler available for " << message_p->id << std::endl;
            return ERR_UNKNOWN_ID;
    }

}