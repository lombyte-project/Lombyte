#include "types.h"
extern u8 D_001D63D0[];
extern u8 D_001D64D0[];
extern u8 D_001D7130[];
extern s32 fill_transfer_words() __asm__("FUN_001f97e8");
void clear_scene_state_buffers(void) __asm__("FUN_00226f50");

void clear_scene_state_buffers(void) {
    fill_transfer_words(D_001D64D0, 0, 0xC60);
    fill_transfer_words(D_001D7130, 0, 0xC60);
    fill_transfer_words(D_001D63D0, 0, 0x100);
}

extern void func_00226F50(void) __attribute__((alias("FUN_00226f50")));
