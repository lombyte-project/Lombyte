/* Ported from rac1-decomp (src/game/menu.c, func_00207F00). */
#include "sda.h"
extern unsigned char menu_item_enabled_55 __asm__("D_0013D3CB") NOT_SDA;
int FUN_00207660(void) {
    return menu_item_enabled_55 != 0;
}

extern __typeof__(FUN_00207660) func_00207660 __attribute__((alias("FUN_00207660")));
