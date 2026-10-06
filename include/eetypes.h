#ifndef LOMBYTE_EETYPES_H
#define LOMBYTE_EETYPES_H

#include "types.h"

/* EE SIMD/qword storage types used by Sony SDK code and game data. */
typedef u32 qword[4] __attribute__((aligned(16)));
typedef s64 dword[2] __attribute__((aligned(16)));
typedef s32 s128 __attribute__((mode(TI), aligned(16)));
typedef u32 u128 __attribute__((mode(TI), aligned(16)));

typedef s32 sceVu0IVECTOR[4] __attribute__((aligned(16)));
typedef f32 sceVu0FVECTOR[4] __attribute__((aligned(16)));
typedef f32 sceVu0FMATRIX[4][4] __attribute__((aligned(16)));

typedef union {
    u128 ul128;
    u64 ul64[2];
    u32 ui32[4];
    f32 fl32[4];
    u16 us16[8];
    u8 uc8[16];
    sceVu0FVECTOR fv;
    sceVu0IVECTOR iv;
} Q_WORDDATA;

#endif /* LOMBYTE_EETYPES_H */
