/* Ported from rac1-decomp (src/game/menu.c, func_002071D0). */
#include "sda.h"
#include "rnc/data_labels.h"
#include "rnc/ui/menus/menu_globals.h"
int FUN_002069a0(void) {
    return menu_item_enabled_0 != 0;
}

extern __typeof__(FUN_002069a0) func_002069A0 __attribute__((alias("FUN_002069a0")));
