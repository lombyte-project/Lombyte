#include "sda.h"
extern unsigned char menu_item_enabled_64 __asm__("D_0013D3D4") NOT_SDA;

int FUN_002076d0(void) {
    return menu_item_enabled_64 != 0;
}

extern __typeof__(FUN_002076d0) func_002076D0 __attribute__((alias("FUN_002076d0")));
