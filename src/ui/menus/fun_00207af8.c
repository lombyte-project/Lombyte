/* Ported from rac1-decomp (src/game/menu.c, func_00208328). */
#include "sda.h"
extern unsigned char menu_item_enabled_115 __asm__("D_0013D407") NOT_SDA;
int FUN_00207af8(void) {
    return menu_item_enabled_115 != 0;
}

extern __typeof__(FUN_00207af8) func_00207AF8 __attribute__((alias("FUN_00207af8")));
