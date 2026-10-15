/* Ported from rac1-decomp (src/game/menu.c, func_002073C8). */
#include "sda.h"
#include "rnc/data_labels.h"
#include "rnc/ui/menus/menu_globals.h"
int map_filter_l04_1(void) __asm__("FUN_00206b98");

int map_filter_l04_1(void) {
    return menu_item_enabled_17 != 0;
}

extern __typeof__(map_filter_l04_1) func_00206B98 __attribute__((alias("FUN_00206b98")));
