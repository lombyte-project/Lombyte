#include "sda.h"
extern unsigned char menu_item_enabled_65 __asm__("D_0013D3D5") NOT_SDA;

int FUN_002076e0(void) {
    return menu_item_enabled_65 != 0;
}

extern __typeof__(FUN_002076e0) func_002076E0 __attribute__((alias("FUN_002076e0")));
