/* Ported from rac1-decomp (src/game/menu.c, func_002073E8). */
#include "sda.h"
#include "rnc/data_labels.h"
#include "rnc/ui/menus/menu_globals.h"
int FUN_00206bb8(void) {
    return menu_item_enabled_19 != 0;
}

extern __typeof__(FUN_00206bb8) func_00206BB8 __attribute__((alias("FUN_00206bb8")));
