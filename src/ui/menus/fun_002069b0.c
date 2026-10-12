/* Ported from rac1-decomp (src/game/menu.c, func_002071E0). */
#include "sda.h"
extern unsigned char menu_item_enabled_1 __asm__("D_0013D395") NOT_SDA;
int FUN_002069b0(void) {
    return menu_item_enabled_1 != 0;
}

extern __typeof__(FUN_002069b0) func_002069B0 __attribute__((alias("FUN_002069b0")));
