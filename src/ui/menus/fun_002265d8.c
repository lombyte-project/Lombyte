#include "types.h"
struct Light { s32 active; s32 a[8]; s32 b[4]; s32 c; };
extern s32 D_00160350;
extern struct Light D_001D5EC0[];

s32 FUN_002265d8(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7,
                 s32 s0, s32 s1, s32 s2, s32 s3, s32 s4) {
    struct Light *l;
    s32 n;

    n = D_00160350;
    if (n >= 8) {
        return -1;
    }
    D_00160350 = n + 1;
    l = &D_001D5EC0[n];
    l->active = 0;
    l->a[0] = a0;
    l->a[1] = a1;
    l->a[2] = a2;
    l->a[3] = a3;
    l->a[4] = a4;
    l->a[5] = a5;
    l->a[6] = a6;
    l->a[7] = a7;
    l->b[0] = s0;
    l->b[1] = s1;
    l->b[2] = s2;
    l->b[3] = s3;
    l->c = s4;
    return 0;
}

extern __typeof__(FUN_002265d8) func_002265D8 __attribute__((alias("FUN_002265d8")));
