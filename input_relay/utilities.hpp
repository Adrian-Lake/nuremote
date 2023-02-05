#ifndef IR_UTILITIES_HPP
#define IR_UTILITIES_HPP

#include <iostream>
#include <cstring>

// general formatter for displaying error info
#define HANDLE_ERROR(TEST, FUNC_NAME, VALUE, DESC, ACTION) if(TEST)                      \
{        \
    if(LOG_INPUT)                                                                                 \
    std::cout << "ERROR (" << VALUE << ", " << FUNC_NAME << "): " << DESC << std::endl;  \
    ACTION;                                                                              \
}

// general formatter for displaying error info for function utilising errno
#define HANDLE_ERROR_ERRNO(TEST, FUNC_NAME, ACTION) if(TEST)                                             \
{         \
    if(LOG_INPUT)                                                                                               \
        std::cout << "ERROR (" << errno << ", " << FUNC_NAME << "): " << std::strerror(errno) << std::endl;  \
    ACTION;                                                                                              \
}

#endif