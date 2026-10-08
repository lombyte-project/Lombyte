#ifndef LOMBYTE_RNC_SDK_LIBRARY_FP_NUMBER_H
#define LOMBYTE_RNC_SDK_LIBRARY_FP_NUMBER_H

#include "types.h"

/* Unpacked double used by the soft-float routines (libgcc fp-bit). */
typedef struct FpNumber {
    s32 class;
    u32 sign;
    s32 normal_exp;
    s32 alignment_padding;
    u64 fraction;
} FpNumber;

#endif /* LOMBYTE_RNC_SDK_LIBRARY_FP_NUMBER_H */
