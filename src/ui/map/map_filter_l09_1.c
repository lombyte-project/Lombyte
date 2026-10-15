/* Ported from rac1-decomp (src/game/menu.c, func_00207F20). */
#include "sda.h"
extern unsigned char menu_item_enabled_56 __asm__("D_0013D3CC") NOT_SDA;
int map_filter_l09_1(void) __asm__("FUN_00207670");

int map_filter_l09_1(void) {
    return menu_item_enabled_56 != 0;
}

extern __typeof__(map_filter_l09_1) func_00207670 __attribute__((alias("FUN_00207670")));
