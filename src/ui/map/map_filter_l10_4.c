#include "sda.h"
extern unsigned char menu_item_enabled_67 __asm__("D_0013D3D7") NOT_SDA;

int map_filter_l10_4(void) __asm__("FUN_00207700");

int map_filter_l10_4(void) {
    return menu_item_enabled_67 != 0;
}

extern __typeof__(map_filter_l10_4) func_00207700 __attribute__((alias("FUN_00207700")));
