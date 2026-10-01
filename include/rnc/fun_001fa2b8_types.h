#ifndef RNC_FUN_001FA2B8_TYPES_H
#define RNC_FUN_001FA2B8_TYPES_H

#include "eetypes.h"

/* Three 16-byte matrix columns. */
struct Matrix3x4 {
    u128 columns[3];
};

#endif /* RNC_FUN_001FA2B8_TYPES_H */
