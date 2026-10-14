/* Ported from rac1-decomp (src/game/menu.c, func_002073D8). */
#include "sda.h"
#include "rnc/data_labels.h"
#include "rnc/ui/menus/menu_globals.h"
int FUN_00206ba8(void) {
    return menu_item_enabled_18 != 0;
}

extern __typeof__(FUN_00206ba8) func_00206BA8 __attribute__((alias("FUN_00206ba8")));
