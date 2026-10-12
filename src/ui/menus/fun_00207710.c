#include "sda.h"
extern unsigned char menu_item_enabled_69 __asm__("D_0013D3D9") NOT_SDA;

int FUN_00207710(void) {
    return menu_item_enabled_69 != 0;
}

extern __typeof__(FUN_00207710) func_00207710 __attribute__((alias("FUN_00207710")));
