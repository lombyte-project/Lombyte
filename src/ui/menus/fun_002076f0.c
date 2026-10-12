#include "sda.h"
extern unsigned char menu_item_enabled_66 __asm__("D_0013D3D6") NOT_SDA;

int FUN_002076f0(void) {
    return menu_item_enabled_66 != 0;
}

extern __typeof__(FUN_002076f0) func_002076F0 __attribute__((alias("FUN_002076f0")));
