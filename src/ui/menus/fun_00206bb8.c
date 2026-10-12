/* Ported from rac1-decomp (src/game/menu.c, func_002073E8). */
#include "sda.h"
extern unsigned char menu_item_enabled_19 __asm__("D_0013D3A7") NOT_SDA;
int FUN_00206bb8(void) {
    return menu_item_enabled_19 != 0;
}

extern __typeof__(FUN_00206bb8) func_00206BB8 __attribute__((alias("FUN_00206bb8")));
