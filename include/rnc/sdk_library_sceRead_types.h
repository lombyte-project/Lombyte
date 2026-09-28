#ifndef RNC_SDK_LIBRARY_SCEREAD_TYPES_H
#define RNC_SDK_LIBRARY_SCEREAD_TYPES_H

#include "types.h"

typedef struct Rep {
    size_t len;
    size_t res;
    size_t ref;
    int selfish;
} Rep;

typedef struct String {
    char *dat;
    unsigned char flag;
} String;

#endif /* RNC_SDK_LIBRARY_SCEREAD_TYPES_H */
