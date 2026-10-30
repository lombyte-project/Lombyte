#include "types.h"
#include "rnc/ui/hud/hud_state.h"

extern s32 decompress_wad() __asm__("func_0020B618");
void       load_compressed_hud_bank(s32 bank, s32 raw_size) __asm__("FUN_00202d10");

void load_compressed_hud_bank(s32 bank, s32 raw_size) {
    s32 size;
    u32 base1;

    size = (raw_size + 0xF) & 0xFFFFFFF0;
    if (size != 0) {
        base1 = *(u32 *)0x15EE4C;
        decompress_wad(*(s32 *)((u8 *)(base1 - (-(bank * 8))) + 0x28) + base1, size);
    }
    {
        register u32 b2;
        register s32 i2;
        i2 = bank * 4;
        b2 = (u32)hud_state.header.counts;
        *(s32 *)((u8 *)b2 + i2 + 0x74) = 0;
    }
}
