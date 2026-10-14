/* Ported from rac1-decomp (src/game/menu.c, func_002073C8). */
#include "sda.h"
#include "rnc/data_labels.h"
#include "rnc/ui/menus/menu_globals.h"
int FUN_00206b98(void) {
    return menu_item_enabled_17 != 0;
}

extern __typeof__(FUN_00206b98) func_00206B98 __attribute__((alias("FUN_00206b98")));
