#ifndef IR_RPC_COMMON_HPP
#define IR_RPC_COMMON_HPP

#include <iostream>

#include "configurable.hpp"
#include "utilities.hpp"
#include "error_codes.hpp"

#define ACTION_PRESS 0
#define ACTION_UP 1
#define ACTION_DOWN 2

#define ID_TYPE int

// procedure parameter definition 
#define NULL_MSG_ID 0
struct nullMsg_t
{
    ID_TYPE id {NULL_MSG_ID}; // id must allways be first element
};

#define KEY_PRESS_MSG_ID 1
struct keyPressMsg_t
{
    ID_TYPE id {KEY_PRESS_MSG_ID}; 
    int keyCode;
    int action {ACTION_PRESS};
};

#define BUTTON_PRESS_MSG_ID 2
struct buttonPressMsg_t
{
    ID_TYPE id {BUTTON_PRESS_MSG_ID};
    int buttonCode;
    int action {ACTION_PRESS};
};


#define REL_MOTION_MSG_ID 3
struct relMotionMsg_t
{
    ID_TYPE id {REL_MOTION_MSG_ID}; 
    int x;
    int y;
};

#define ABS_MOTION_MSG_ID 4
struct absMotionMsg_t
{
    ID_TYPE id {ABS_MOTION_MSG_ID}; 
    int x;
    int y;
};

#define DUMMY_MSG_ID -1
struct dummyMsg_t
{
    ID_TYPE id {DUMMY_MSG_ID}; 
    int param {42};
};

// msg layout and utilities
union message_t
{
    ID_TYPE id; // convenience 
    nullMsg_t null;
    keyPressMsg_t keyPress;
    buttonPressMsg_t buttonPress;
    relMotionMsg_t relMotion;
    absMotionMsg_t absMotion;
    dummyMsg_t dummy;
};

// message statistics
extern size_t messageMaxSize;
extern size_t messageIdSize;

size_t getMessageSize(int id);

#endif