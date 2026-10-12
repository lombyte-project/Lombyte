#include "sda.h"
extern unsigned char menu_item_enabled_67 __asm__("D_0013D3D7") NOT_SDA;

int FUN_00207700(void) {
    return menu_item_enabled_67 != 0;
}

extern __typeof__(FUN_00207700) func_00207700 __attribute__((alias("FUN_00207700")));
