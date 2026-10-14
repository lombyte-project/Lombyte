#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/ui/menus/menu_globals.h"
s32 find_id_in_terminated_table(s32 id) __asm__("FUN_00205220");

s32 find_id_in_terminated_table(s32 id) {
    s32 i;

    for (i = 0; menu_level_indices[i] != 0 && i < 20; i++) {
        if (menu_level_indices[i] == id) {
            return i;
        }
    }
    return -1;
}

extern __typeof__(find_id_in_terminated_table) func_00205220 __attribute__((alias("FUN_00205220")));
