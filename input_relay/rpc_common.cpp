#include "rpc_common.hpp"

size_t messageMaxSize = sizeof(message_t);
size_t messageIdSize = sizeof(ID_TYPE);

size_t getMessageSize(int id){

    switch(id){

        case KEY_PRESS_MSG_ID:
        return sizeof(keyPressMsg_t);

        case BUTTON_PRESS_MSG_ID:
        return sizeof(buttonPressMsg_t);

        case REL_MOTION_MSG_ID:
        return sizeof(relMotionMsg_t);

        case ABS_MOTION_MSG_ID:
        return sizeof(absMotionMsg_t);

        case DUMMY_MSG_ID:
        return sizeof(dummyMsg_t);

        // if the msg id is not recognized, do not read more than the id
        case NULL_MSG_ID:
        default:
            return sizeof(nullMsg_t); 

    }
}