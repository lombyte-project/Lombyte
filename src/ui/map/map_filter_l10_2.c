#include "sda.h"
extern unsigned char menu_item_enabled_65 __asm__("D_0013D3D5") NOT_SDA;

int map_filter_l10_2(void) __asm__("FUN_002076e0");

int map_filter_l10_2(void) {
    return menu_item_enabled_65 != 0;
}

extern __typeof__(map_filter_l10_2) func_002076E0 __attribute__((alias("FUN_002076e0")));
