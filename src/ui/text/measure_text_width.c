#include "types.h"

s32 measure_text_width(u8 *text, s32 max_chars, s32 glyph_table) __asm__("FUN_001f6200");

s32 measure_text_width(u8 *text, s32 max_chars, s32 glyph_table) {
    s32 total;
    s32 count;
    s8 value;
    u8 *p;
    u8 index;

    total = 0;
    count = 0;
    if (max_chars == 0)
        goto done;
    if (*text == 0)
        goto done;
    p = text;
    do {
        index = *p;
        p++;
        count++;
        value = *(s8 *)(glyph_table + index * 4 + 3);
        if (value != 0)
            total += value;
    } while (count != max_chars && *p != 0);
done:
    return total;
}

extern __typeof__(measure_text_width) func_001F6200 __attribute__((alias("FUN_001f6200")));
