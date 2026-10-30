#include "types.h"

extern u8  thousands_grouped_integer_format[] __asm__("D_00161078");
extern u8  plain_integer_format[] __asm__("D_00161080");
extern s32 sprintf();
void       format_scaled_display_value(u8 *buf, s32 value) __asm__("FUN_00238688");

void format_scaled_display_value(u8 *destination, s32 value) {
    if (value >= 0x3E8) {
        sprintf(destination, thousands_grouped_integer_format, value / 1000, value % 1000);
        return;
    }
    sprintf(destination, plain_integer_format, value);
}

extern __typeof__(format_scaled_display_value) func_00238688 __attribute__((alias("FUN_00238688")));
