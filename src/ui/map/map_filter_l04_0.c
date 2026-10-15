/* Ported from rac1-decomp (src/game/menu.c, map_filter_l07_7). */
#include "sda.h"
#include "rnc/data_labels.h"
#include "rnc/ui/menus/menu_globals.h"
int map_filter_l04_0(void) __asm__("FUN_00206b88");

int map_filter_l04_0(void) {
    return menu_item_enabled_16 != 0;
}

extern __typeof__(map_filter_l04_0) func_00206B88 __attribute__((alias("FUN_00206b88")));
