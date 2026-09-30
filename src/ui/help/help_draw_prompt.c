#include "types.h"
struct M2c_D_001996D0 {
    s32 unk0;
    s32 unk4;
    u8 pad_8[0x8];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
};

extern struct M2c_D_001996D0 D_001996D0;
extern u64 func_001F44B8();
extern s32 func_001F5450();
extern s32 func_001F5F18();
void help_draw_prompt(void) __asm__("FUN_001fe898");

void help_draw_prompt(void) {
    s32 alpha;

    D_001996D0.unk18 = 0x20;
    D_001996D0.unk1C = 0x20;
    func_001F5F18(D_001996D0.unk14 - 0x20, D_001996D0.unk14 + 0x20, D_001996D0.unk10 - 0x20, D_001996D0.unk10 + 0x20, 0x60);
    if (D_001996D0.unk0 == 1 || D_001996D0.unk0 == 7) {
        alpha = D_001996D0.unk4 * 21;
    } else {
        alpha = 0x7E;
    }
    if (alpha > 0x80) {
        alpha = 0x80;
    }
    func_001F5450(D_001996D0.unk10 - 0x20, D_001996D0.unk14 - 0x20, 0x40, 0x40, 0, 0, 0x40, 0x40,
                  (u64)(alpha << 24 | 0x808080), func_001F44B8(4));
}
