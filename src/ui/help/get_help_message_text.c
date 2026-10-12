#include "types.h"

extern void *help_msgs __asm__("D_0015F6A0");
extern u8 s_paradox_this_message_does_not[] __asm__("D_00199968");
extern s32 func_001fdca0();

s32 get_help_message_text(void) __asm__("FUN_001fdd10");

s32 get_help_message_text(void) {
    s32 index;
    index = func_001fdca0();
    if (index >= 0) {
        return *(s32 *)((u8 *)help_msgs + (index * 16));
    }
    return (s32)s_paradox_this_message_does_not;
}

extern s32 func_001FDD10(void) __attribute__((alias("FUN_001fdd10")));
