/* Ported from rac1-decomp (src/game/menu.c, func_002082E8). */
#include "sda.h"
extern unsigned char menu_item_enabled_102 __asm__("D_0013D3FA") NOT_SDA;
int map_filter_l16_0(void) __asm__("FUN_00207ab8");

int map_filter_l16_0(void) {
    return menu_item_enabled_102 != 0;
}

extern __typeof__(map_filter_l16_0) func_00207AB8 __attribute__((alias("FUN_00207ab8")));
