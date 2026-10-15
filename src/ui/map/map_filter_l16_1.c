/* Ported from rac1-decomp (src/game/menu.c, func_002082F8). */
#include "sda.h"
extern unsigned char menu_item_enabled_103 __asm__("D_0013D3FB") NOT_SDA;
int map_filter_l16_1(void) __asm__("FUN_00207ac8");

int map_filter_l16_1(void) {
    return menu_item_enabled_103 != 0;
}

extern __typeof__(map_filter_l16_1) func_00207AC8 __attribute__((alias("FUN_00207ac8")));
