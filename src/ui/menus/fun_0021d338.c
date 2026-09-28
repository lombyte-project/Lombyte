#include "types.h"

extern s16 D_001516D8[];
extern s32 D_00137B80[];
extern s32 D_001D5CF8[];
extern u8 D_001D5BF0[];
extern s32 D_0015ED88;
extern u8 D_001996D0[];

extern s32 D_0015F6A0;

extern s32 start_audio_stream_read(s32 arg0, s32 arg1, s32 arg2) __asm__("FUN_00216788");
extern void FUN_001f9838(void *, void *, s32);

static inline int pauseSlotCount(void) {
    char *b = D_001996D0;
    return *(int *)(b + 0x2C);
}

int FUN_0021d338(char *arg0) {
    switch (*(int *)(arg0 + 0x50)) {
    case 0:
        if (D_001516D8[0] == 0) {
            if (start_audio_stream_read(D_001D5CF8[0], D_00137B80[0x1528 / 4],
                              D_00137B80[0x152C / 4]) != 0) {
                *(int *)(arg0 + 0x50) = 1;
            } else {
                *(int *)(arg0 + 0x50) = 3;
            }
        }
        break;
    case 1:
        if (D_001516D8[0] == 0) {
            char *g = D_001D5BF0;
            int *tbl = *(int **)(g + 0x108);
            int *p = (int *)((char *)tbl + tbl[D_0015ED88]);
            int n = *p++;
            int sz = *p++;
            char *b;
            int *t;
            int i;

            FUN_001f9838(tbl, p, ((sz + 3) & ~3) - 8);
            b = D_001996D0;
            *(int *)(arg0 + 0x54) = D_0015F6A0;
            *(int *)(arg0 + 0x38) = *(int *)(b + 0x2C);
            *(int *)(b + 0x2C) = n;
            t = *(int **)(g + 0x108);
            D_0015F6A0 = (int)t;
            for (i = 0; i < pauseSlotCount(); i++) {
                int d = (int)t - 8;
                *(int *)((char *)t + i * 0x10) += d;
            }
            *(int *)(arg0 + 0x10) &= ~4;
            *(int *)(arg0 + 0x50) = 2;
        }
        break;
    case 2:
    case 3:
        break;
    }
    return 0;
}

extern __typeof__(FUN_0021d338) func_0021D338 __attribute__((alias("FUN_0021d338")));
