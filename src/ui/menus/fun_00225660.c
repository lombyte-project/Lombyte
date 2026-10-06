#include "types.h"
struct MusicStreamState {
    u8 pad_0[0x5A];
    u16 unk5A;
};

struct MenuScreen {
    u8 pad_0[0x3C];
    s32 unk3C;
};

extern struct MusicStreamState D_001516D0;
extern s32 delete_moby() __asm__("FUN_00225530");
extern s32 complete_stream_buffer_transfer() __asm__("func_00225CD8");
s32 FUN_00225660(struct MenuScreen *menu) {
    s32 *slot;
    s32 remaining;

    remaining = 0x17;
    slot = ((u8 *)menu + (0x44));
    do {
        remaining -= 1;
        *slot = delete_moby(*slot);
        slot += 1;
    } while (remaining >= 0);
    menu->unk3C = complete_stream_buffer_transfer(menu->unk3C);
    if ((u32)(D_001516D0.unk5A - 6) >= 2U) {
        D_001516D0.unk5A = 5U;
    }
    return 0;
}
