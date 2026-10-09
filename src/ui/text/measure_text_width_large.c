#include "types.h"
#include "rnc/ui/text/font_metrics.h"

extern void measure_text_width(s32 text, s32 max_chars, void *arg2) __asm__("func_001F6200");

void measure_text_width_large(s32 text, s32 max_chars) __asm__("FUN_001f6290");

void measure_text_width_large(s32 text, s32 max_chars) {
    measure_text_width(text, max_chars, large_font_metrics);
}

extern __typeof__(measure_text_width_large) func_001F6290 __attribute__((alias("FUN_001f6290")));
