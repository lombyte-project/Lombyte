#include "sda.h"
extern unsigned char menu_item_enabled_66 __asm__("D_0013D3D6") NOT_SDA;

int map_filter_l10_3(void) __asm__("FUN_002076f0");

int map_filter_l10_3(void) {
    return menu_item_enabled_66 != 0;
}

extern __typeof__(map_filter_l10_3) func_002076F0 __attribute__((alias("FUN_002076f0")));
