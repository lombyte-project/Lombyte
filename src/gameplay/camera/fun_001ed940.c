#include "rnc/assembly_textbin_fun_001ed940_types.h"
#include "types.h"
extern struct M2c_D_0013F350 D_0013F350;
extern s32 D_0015EF98;
extern s32 D_0015EF9C;
extern s32 D_0015EFA0;
extern struct M2c_D_00187080 D_00187080;
extern struct M2c_D_001870D0 D_001870D0;
/* retail small-data globals, declared to GAS before the body */

void FUN_001ed940(void) {
    struct M2c_D_001870D0 *q;

    q = &D_001870D0;
    D_0015EF9C = 0x14;
    if ((u32)(D_0013F350.unk208C - 0x11) < 2U || D_0013F350.unk2084 == 0x73) {
        D_0015EF9C = 0x34;
    }
    if (D_0013F350.unk208C != 0x11 && D_0013F350.unk2F0 < D_00187080.unk8) {
        D_0015EF9C = 0x14;
    }
    D_0015EFA0 = D_0015EF9C;
    D_0015EF9C |= 0x80;
    D_0015EF98 = 0xB4;
    if (D_0013F350.unk12E5 != 0) {
        q->unkC0 = 0x100;
        D_0015EF98 = 0x1B4;
    } else if (D_0013F350.unk12EB != 0) {
        q->unkC0 = 0xB00;
        D_0015EF98 = 0xBB4;
    } else if (D_0013F350.unk12E6 != 0) {
        q->unkC0 = 0x300;
        D_0015EF98 = 0x3B4;
    } else if (D_0013F350.unk12EC != 0) {
        q->unkC0 = 0xD00;
        D_0015EF98 = 0xDB4;
    } else if (D_0013F350.unk12E4 != 0) {
        q->unkC0 = 0;
        D_0015EF98 = 0xB4;
    } else {
        D_0015EF98 = q->unkC0 | 0xB4;
    }
}

extern __typeof__(FUN_001ed940) func_001ED940 __attribute__((alias("FUN_001ed940")));
