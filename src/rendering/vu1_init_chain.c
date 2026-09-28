#include "types.h"
#include "sda.h"

extern s32 D_001940C0[];
extern s32 D_00160F0C;
extern s32 D_0015F5B8;
extern s32 D_00160EF8[];
extern s32 D_00160F10 MACRO_ADDR;
extern s32 D_0015F63C MACRO_ADDR;
extern s32 D_00160F00 MACRO_ADDR;
extern s32 D_0015F638 __attribute__((sda));

void vu1_init_chain(void) __asm__("FUN_002335d0");

void vu1_init_chain(void) {
    s32 start;
    s32 end;
    s32 limit;

    start = D_001940C0[1];
    end = D_001940C0[2];
    limit = start + D_00160F0C - D_0015F5B8;
    D_00160EF8[1] = end;
    D_00160F10 = 0;
    D_00160F00 = start;
    D_00160EF8[0] = start;
    D_0015F638 = limit;
    D_0015F63C = limit - 0x2000;
}

extern __typeof__(vu1_init_chain) func_002335D0 __attribute__((alias("FUN_002335d0")));
