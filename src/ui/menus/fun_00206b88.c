/* Ported from rac1-decomp (src/game/menu.c, FUN_002073b8). */
#include "sda.h"
#include "rnc/data_labels.h"
#include "rnc/ui/menus/menu_globals.h"
int FUN_00206b88(void) {
    return menu_item_enabled_16 != 0;
}

extern __typeof__(FUN_00206b88) func_00206B88 __attribute__((alias("FUN_00206b88")));
