#include "types.h"
struct MenuScreen {
    u8 pad_0[0x44];
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    s32 unk54;
};

extern s32 complete_stream_buffer_transfer() __asm__("func_00225CD8");
s32 FUN_0021fd78(struct MenuScreen *menu) {
    menu->unk48 = complete_stream_buffer_transfer(menu->unk48);
    menu->unk4C = complete_stream_buffer_transfer(menu->unk4C);
    menu->unk50 = -1;
    menu->unk54 = -1;
    menu->unk44 = -1;
    return 0;
}
