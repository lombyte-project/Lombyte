#include "types.h"

struct Globals_00137B80 {
    u8 pad_0[0x8];
    s32 unk8;
    s32 unkC;
};

extern struct Globals_00137B80 D_00137B80;
extern u8 D_001AABC0[];
extern void load_pif_as_psmt8_h() __asm__("func_001E9168");
extern s32 load() __asm__("func_00216828");

void load_debug_font(void) __asm__("FUN_001e9338");

void load_debug_font(void) {
    u8 sp_slot[32];

    load(D_001AABC0, D_00137B80.unk8, D_00137B80.unkC);
    load_pif_as_psmt8_h(D_001AABC0, sp_slot, *(s32 *)0x15EE88 + 0xC0000, 0x3FFC00);
    *(u64 *)0x15EEC8 = *(u64 *)(void *)sp_slot;
}
