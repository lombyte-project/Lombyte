#include "sda.h"
extern unsigned char menu_item_enabled_37 __asm__("D_0013D3B9") NOT_SDA;

int FUN_00207640(void) {
    return menu_item_enabled_37 != 0;
}

extern __typeof__(FUN_00207640) func_00207640 __attribute__((alias("FUN_00207640")));
