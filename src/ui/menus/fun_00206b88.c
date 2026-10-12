/* Ported from rac1-decomp (src/game/menu.c, FUN_002073b8). */
#include "sda.h"
extern unsigned char menu_item_enabled_16 __asm__("D_0013D3A4") NOT_SDA;
int FUN_00206b88(void) {
    return menu_item_enabled_16 != 0;
}

extern __typeof__(FUN_00206b88) func_00206B88 __attribute__((alias("FUN_00206b88")));
