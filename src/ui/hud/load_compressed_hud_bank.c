#include "types.h"
extern u32 D_0015EE4C[];
extern u32 D_0019A400[];
extern s32 func_0020B618();
void load_compressed_hud_bank(s32 arg0, s32 arg1) __asm__("FUN_00202d10");

void load_compressed_hud_bank(s32 arg0, s32 arg1) {
    s32 size;
    u32 base1;

    size = (arg1 + 0xF) & 0xFFFFFFF0;
    if (size != 0) {
        base1 = *(u32 *)0x15EE4C;
        func_0020B618(*(s32 *)((u8 *)(base1 - (-(arg0 * 8))) + 0x28) + base1, size);
    }
    {
        register u32 b2;
        register s32 i2;
        i2 = arg0 * 4;
        b2 = D_0019A400[0];
        *(s32 *)((u8 *)b2 + i2 + 0x74) = 0;
    }
}
