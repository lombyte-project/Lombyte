#include "sda.h"
extern unsigned char menu_item_enabled_36 __asm__("D_0013D3B8") NOT_SDA;

int FUN_00207630(void) {
    return menu_item_enabled_36 != 0;
}

extern __typeof__(FUN_00207630) func_00207630 __attribute__((alias("FUN_00207630")));
