#include "types.h"

extern u32 menu_selection_count __asm__("D_0015FD64") __attribute__((sda));

s32 map_filter_l03_0(void) __asm__("FUN_00206b78");

s32 map_filter_l03_0(void) {
    return menu_selection_count < 1;
}

extern __typeof__(map_filter_l03_0) func_00206B78 __attribute__((alias("FUN_00206b78")));
