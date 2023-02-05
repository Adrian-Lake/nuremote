#ifndef IR_ERROR_CODES_H
#define IR_ERROR_CODES_H

// general errors, N
#define OK 0
#define ERR 1

// host operation errors, 1N
#define ERR_STARTUP 10
#define ERR_MAIN 11
#define ERR_EXIT 12
#define ERR_CONN_DISPLAY 13

// communication errors, 2N
#define ERR_RECV 20
#define ERR_SEND 21
#define ERR_UNKNOWN_ID 22

#endif