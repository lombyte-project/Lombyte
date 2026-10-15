/* Ported from rac1-decomp (src/game/menu.c, func_002073F8). */
#include "sda.h"
extern unsigned char menu_item_enabled_25 __asm__("D_0013D3AD") NOT_SDA;
int map_filter_l06_0(void) __asm__("FUN_00206bc8");

int map_filter_l06_0(void) {
    return menu_item_enabled_25 != 0;
}

extern __typeof__(map_filter_l06_0) func_00206BC8 __attribute__((alias("FUN_00206bc8")));
