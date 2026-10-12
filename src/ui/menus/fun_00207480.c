/* Ported from rac1-decomp (src/game/menu.c, func_00207CB0). */
#include "sda.h"
extern unsigned char menu_item_enabled_41 __asm__("D_0013D3BD") NOT_SDA;
extern int D_001413DC NOT_SDA;
int FUN_00207480(int arg0, int arg1) {
    if (arg1 >= 0x101) {
        return D_001413DC == 0xF;
    }
    return menu_item_enabled_41 != 0;
}

extern __typeof__(FUN_00207480) func_00207480 __attribute__((alias("FUN_00207480")));
