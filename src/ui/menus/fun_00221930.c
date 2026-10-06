#include "types.h"
struct M2c_arg0 {
    u8 pad_0[0x18];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
};

extern s32 draw_level_selection_map() __asm__("FUN_001fd748");
s32 FUN_00221930(struct M2c_arg0 *menu) {
    s32 x;
    s32 y;

    x = menu->unk18;
    y = menu->unk1C;
    draw_level_selection_map(x, x + menu->unk20, y, y + menu->unk24);
    return 2;
}
