#include "types.h"

struct PadState {
    u8 pad_0[0x1A4];
    s32 unk1A4;
    u8 pad_1A8[0x1C];
    s32 unk1C4;
};
struct Globals_001D5BF0 {
    u8 pad_0[0x4];
    struct ModeEntry *unk4;
    s32 unk8;
    u8 pad_C[0x118];
    s32 unk124;
};
struct ModeEntry {
    u8 pad_0[0x38];
    s32 unk38;
};
struct MenuScreen {
    u8 pad_0[0x14];
    s32 unk14;
    u8 pad_18[0x28];
    s32 unk40;
};
extern struct PadState D_0013C940;
extern struct Globals_001D5BF0 D_001D5BF0;
__attribute__((section(".data"))) extern s32 D_001D5D14;
extern s32 allocate_voice_for_target_entry() __asm__("func_0022DA68");
s32 FUN_00221d68(struct MenuScreen *menu) {
    s32 prev;

    if ((D_0013C940.unk1C4 & 0xD00) && D_001D5D14 == 0) {
        return 1;
    }
    if (D_0013C940.unk1C4 & 0x10) {
        if (D_001D5BF0.unk4->unk38 != 0) {
            D_001D5BF0.unk8 = D_001D5BF0.unk4->unk38;
        } else if (D_001D5BF0.unk124 == 0) {
            return -1;
        }
    }
    prev = menu->unk40;
    if (D_0013C940.unk1A4 & 0x40) {
        menu->unk40 = (prev + 1) % 30;
    } else if (D_0013C940.unk1A4 & 0x20) {
        menu->unk40 = (prev + 29) % 30;
    }
    if (menu->unk40 != prev) {
        allocate_voice_for_target_entry(1, 0x11, menu->unk14);
    }
    return 0;
}
