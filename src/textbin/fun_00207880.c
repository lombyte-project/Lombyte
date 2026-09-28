/* Ported from rac1-decomp, the PAL decompilation (src/game/menu.c, func_002080B0). */
#include "sda.h"
typedef struct {
    char _pad0[0x12E4];
    unsigned char unk12E4;
    char _pad12E5[0x208C - 0x12E5];
    int unk208C;
} Menu13F450;
extern Menu13F450 D_0013F350;
extern unsigned char D_0013D3E2 NOT_SDA;
/* The Menu13F450 state test of func_00207340, computed up front (retail
   evaluates it before the arg0 branch), gates the arg0 < 0x15F arm; the
   other arm is func_00208160's second test with D_0013D3E2. */
int FUN_00207880(int arg0, float unused1, float unused2, float arg1) {
    Menu13F450 *s = &D_0013F350;
    int a = s->unk208C == 17 || s->unk208C == 18 || s->unk12E4 == 1;

    if (arg0 < 0x15F) {
        return (arg1 >= 200.0f) ? a : 0;
    }
    return arg1 >= 233.0f && arg1 <= 235.0f && D_0013D3E2 != 0;
}

extern __typeof__(FUN_00207880) func_00207880 __attribute__((alias("FUN_00207880")));
