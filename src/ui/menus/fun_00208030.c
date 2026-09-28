#include "types.h"

struct Table16 {
    s32 v[16];
};

extern u8 *D_001A00FC[];
extern struct Table16 D_001E8080;
extern void FillTransferWords(void *dst, s32 value, s32 size);

void FUN_00208030(u8 *out) {
    s32 acc[128];
    struct Table16 tbl;
    u8 *src;
    u8 *dst;
    s32 i;
    s32 j;
    s32 k;
    s32 m;
    u32 b;

    dst = out;
    src = D_001A00FC[0];
    tbl = D_001E8080;
    for (i = 0; i < 0x200; i++) {
        if ((i & 3) == 0) {
            FillTransferWords(acc, 0, 0x200);
        }
        for (j = 0; j < 128; j += 2) {
            b = *src++;
            acc[j] += tbl.v[b & 0xF];
            acc[j + 1] += tbl.v[b >> 4];
        }
        if (i % 4 == 3) {
            for (k = 0; k < 128; k++) {
                if (acc[k] < 8) {
                    acc[k] = 0;
                } else {
                    acc[k] = 1 << (k % 8);
                }
            }
            for (m = 0; m < 128; m += 8) {
                *dst++ = acc[m] | acc[m + 1] | acc[m + 2] | acc[m + 3] |
                         acc[m + 4] | acc[m + 5] | acc[m + 6] | acc[m + 7];
            }
        }
    }
    out[0] = (out[0] & 0xFE) | 2;
}

extern __typeof__(FUN_00208030) func_00208030 __attribute__((alias("FUN_00208030")));
