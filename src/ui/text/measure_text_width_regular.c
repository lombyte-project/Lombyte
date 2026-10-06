#include "types.h"

extern u8 D_001DF050[];
extern void measure_text_width(s32 text, s32 max_chars, void *arg2) __asm__("func_001F6200");

void measure_text_width_regular(s32 text, s32 max_chars) __asm__("FUN_001f6250");

void measure_text_width_regular(s32 text, s32 max_chars) {
    measure_text_width(text, max_chars, D_001DF050);
}

extern __typeof__(measure_text_width_regular) func_001F6250 __attribute__((alias("FUN_001f6250")));
