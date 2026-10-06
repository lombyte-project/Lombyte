#include "types.h"

struct Globals_001D5BF0 {
    u8 pad_0[0x4];
    struct ModeEntry *unk4;
    s32 unk8;
    s32 unkC;
    u8 pad_10[0xD4];
    s32 unkE4;
    u8 pad_E8[0x8];
    s32 unkF0;
    s32 unkF4;
    u8 pad_F8[0x2C];
    s32 unk124;
};
struct ModeEntry {
    u8 pad_0[0x38];
    s32 unk38;
};
struct MenuScreen {
    u8 pad_0[0x14];
    s32 unk14;
};
extern s32 D_0013CB04[];
extern s32 D_0015ED84;
extern s32 D_001A0314[];
extern struct Globals_001D5BF0 D_001D5BF0;
extern s32 allocate_voice_for_target_entry() __asm__("func_0022DA68");
s32 FUN_00221968(struct MenuScreen *arg0) {
    if (D_0013CB04[0] & 0x10) {
        if (D_001D5BF0.unk4->unk38 != 0) {
            D_001D5BF0.unk8 = D_001D5BF0.unk4->unk38;
        } else if (D_001D5BF0.unk124 == 0) {
            return -1;
        }
    } else if (D_0013CB04[0] & 0x800) {
        D_001A0314[0] = D_0015ED84;
        return 1;
    } else if (D_0013CB04[0] & 0x40) {
        allocate_voice_for_target_entry(0, 0x11, arg0->unk14);
        return 1;
    } else if (D_0013CB04[0] & 0x20) {
        D_001D5BF0.unkE4 = D_001A0314[0];
        D_001D5BF0.unkF0 = (s32)D_001D5BF0.unk4;
        D_001D5BF0.unkC = 3;
        D_001D5BF0.unkF4 = 0xF;
        allocate_voice_for_target_entry(0, 0x11, arg0->unk14);
    }
    return 0;
}

extern __typeof__(FUN_00221968) func_00221968 __attribute__((alias("FUN_00221968")));
