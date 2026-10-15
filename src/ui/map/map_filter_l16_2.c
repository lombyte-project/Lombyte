/* Ported from rac1-decomp (src/game/menu.c, func_00208308). */
#include "sda.h"
extern unsigned char menu_item_enabled_104 __asm__("D_0013D3FC") NOT_SDA;
int map_filter_l16_2(void) __asm__("FUN_00207ad8");

int map_filter_l16_2(void) {
    return menu_item_enabled_104 != 0;
}

extern __typeof__(map_filter_l16_2) func_00207AD8 __attribute__((alias("FUN_00207ad8")));
