#include "types.h"

s32 measure_text_width(u8 *arg0, s32 arg1, s32 arg2) __asm__("FUN_001f6200");

s32 measure_text_width(u8 *arg0, s32 arg1, s32 arg2) {
    s32 total;
    s32 count;
    s8 value;
    u8 *p;
    u8 index;

    total = 0;
    count = 0;
    if (arg1 == 0)
        goto done;
    if (*arg0 == 0)
        goto done;
    p = arg0;
    do {
        index = *p;
        p++;
        count++;
        value = *(s8 *)(arg2 + index * 4 + 3);
        if (value != 0)
            total += value;
    } while (count != arg1 && *p != 0);
done:
    return total;
}

extern __typeof__(measure_text_width) func_001F6200 __attribute__((alias("FUN_001f6200")));
