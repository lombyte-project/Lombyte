/* Ported from rac1-decomp (src/game/menu.c, func_00207F30). */
#include "sda.h"
extern unsigned char menu_item_enabled_57 __asm__("D_0013D3CD") NOT_SDA;
int FUN_00207680(void) {
    return menu_item_enabled_57 != 0;
}

extern __typeof__(FUN_00207680) func_00207680 __attribute__((alias("FUN_00207680")));
