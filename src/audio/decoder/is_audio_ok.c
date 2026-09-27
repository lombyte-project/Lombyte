#include "types.h"

extern s32 D_0016120C;
extern s32 func_0023AEE0();

s32 is_audio_ok(void) __asm__("FUN_0023a790");

s32 is_audio_ok(void) {
    s32 result;
    result = func_0023AEE0(D_0016120C + 0xD9100);
    return result;
}

extern s32 func_0023A790(void) __attribute__((alias("FUN_0023a790")));
