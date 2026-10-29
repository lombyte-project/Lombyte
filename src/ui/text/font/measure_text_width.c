#include "types.h"

s32 measure_text_width(u8 *text, s32 max_chars, s32 glyph_table) __asm__("FUN_001f6200");

s32 measure_text_width(u8 *text, s32 max_chars, s32 glyph_table) {
    s32 total;
    s32 count;
    s8  glyph_advance;
    u8 *text_cursor;
    u8  index;

    total = 0;
    count = 0;
    if (max_chars == 0)
        goto done;
    if (*text == 0)
        goto done;
    text_cursor = text;
    do {
        index = *text_cursor;
        text_cursor++;
        count++;
        glyph_advance = *(s8 *)(glyph_table + index * 4 + 3);
        if (glyph_advance != 0)
            total += glyph_advance;
    } while (count != max_chars && *text_cursor != 0);
done:
    return total;
}

extern __typeof__(measure_text_width) func_001F6200 __attribute__((alias("FUN_001f6200")));
