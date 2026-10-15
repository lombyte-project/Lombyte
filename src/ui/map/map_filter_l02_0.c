/* Ported from rac1-decomp (src/game/menu.c, func_002071F0). */
#include "sda.h"
#include "rnc/data_labels.h"
#include "rnc/ui/menus/menu_globals.h"
int map_filter_l02_0(void) __asm__("FUN_002069c0");

int map_filter_l02_0(void) {
    return menu_item_enabled_9 != 0;
}

extern __typeof__(map_filter_l02_0) func_002069C0 __attribute__((alias("FUN_002069c0")));
