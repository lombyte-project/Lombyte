/* Ported from rac1-decomp (src/game/menu.c, func_002073F8). */
#include "sda.h"
extern unsigned char menu_item_enabled_25 __asm__("D_0013D3AD") NOT_SDA;
int FUN_00206bc8(void) {
    return menu_item_enabled_25 != 0;
}

extern __typeof__(FUN_00206bc8) func_00206BC8 __attribute__((alias("FUN_00206bc8")));
