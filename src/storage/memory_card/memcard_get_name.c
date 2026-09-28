#include "types.h"
extern char D_0013D1D0[];
extern char D_0013D1E8[];
extern char D_0013D200[];
extern char D_0013D220[];
extern char D_0013D240[];
extern char D_0013D270[];
extern char *strncpy(char *, const char *, u32);
void memcard_get_name(u8 *src) __asm__("FUN_00209030");

void memcard_get_name(u8 *src) {
    s32 i;

    if (src[0x12] == 'E') {
        D_0013D1D0[2] = src[0x12];
    }
    for (i = 3; i < 7; i++) {
        D_0013D1D0[i] = src[i + 0xD];
    }
    for (i = 8; i < 11; i++) {
        D_0013D1D0[i] = src[i + 0xD];
    }
    for (i = 11; i < 13; i++) {
        D_0013D1D0[i] = src[i + 0xE];
    }
    strncpy(D_0013D1E8, D_0013D1D0, 13);
    strncpy(D_0013D200, D_0013D1D0, 13);
    strncpy(D_0013D220, D_0013D1D0, 13);
    strncpy(D_0013D240, D_0013D1D0, 13);
    strncpy(D_0013D240 + 0x14, D_0013D1D0, 13);
    strncpy(D_0013D270, D_0013D1D0, 13);
}

extern __typeof__(memcard_get_name) func_00209030 __attribute__((alias("FUN_00209030")));
