#include "types.h"

extern s32 draw_moby_list() __asm__("func_0020D330");
s32 draw_moby_entries_from_object(s32 arg0) __asm__("FUN_00225a68");

s32 draw_moby_entries_from_object(s32 arg0) {
    s32 *var_16_10;
    s32 temp_4_12;
    s32 var_17_8;
    var_17_8 = 0x17;
    var_16_10 = (s32 *)(arg0 + 0x44);
    do {
        temp_4_12 = *var_16_10;
        if (temp_4_12 != 0) {
            draw_moby_list(temp_4_12, 1);
        }
        var_17_8 -= 1;
        var_16_10 += 1;
    } while (var_17_8 >= 0);
    return 4;
}
