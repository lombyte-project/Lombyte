/* Ported from rac1-decomp (src/game/menu.c, func_00208328). */
#include "sda.h"
extern unsigned char menu_item_enabled_115 __asm__("D_0013D407") NOT_SDA;
int map_filter_l18_0(void) __asm__("FUN_00207af8");

int map_filter_l18_0(void) {
    return menu_item_enabled_115 != 0;
}

extern __typeof__(map_filter_l18_0) func_00207AF8 __attribute__((alias("FUN_00207af8")));
