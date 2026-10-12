/* Ported from rac1-decomp (src/game/menu.c, func_00208318). */
#include "sda.h"
extern unsigned char menu_item_enabled_105 __asm__("D_0013D3FD") NOT_SDA;
int FUN_00207ae8(void) {
    return menu_item_enabled_105 != 0;
}

extern __typeof__(FUN_00207ae8) func_00207AE8 __attribute__((alias("FUN_00207ae8")));
