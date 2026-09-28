#ifndef RNC_SDK_LIBRARY_D2B_TYPES_H
#define RNC_SDK_LIBRARY_D2B_TYPES_H

#include "types.h"

typedef struct _Bigint {
    struct _Bigint *_next;
    int _k;
    int _maxwds;
    int _sign;
    int _wds;
    unsigned int _x[1];
} _Bigint;

#endif /* RNC_SDK_LIBRARY_D2B_TYPES_H */
