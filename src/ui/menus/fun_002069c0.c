/* Ported from rac1-decomp (src/game/menu.c, func_002071F0). */
#include "sda.h"
#include "rnc/data_labels.h"
#include "rnc/ui/menus/menu_globals.h"
int FUN_002069c0(void) {
    return menu_item_enabled_9 != 0;
}

extern __typeof__(FUN_002069c0) func_002069C0 __attribute__((alias("FUN_002069c0")));
