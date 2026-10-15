#include "sda.h"
extern unsigned char menu_item_enabled_69 __asm__("D_0013D3D9") NOT_SDA;

int map_filter_l10_5(void) __asm__("FUN_00207710");

int map_filter_l10_5(void) {
    return menu_item_enabled_69 != 0;
}

extern __typeof__(map_filter_l10_5) func_00207710 __attribute__((alias("FUN_00207710")));
