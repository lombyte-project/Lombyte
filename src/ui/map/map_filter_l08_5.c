#include "sda.h"
extern unsigned char menu_item_enabled_36 __asm__("D_0013D3B8") NOT_SDA;

int map_filter_l08_5(void) __asm__("FUN_00207630");

int map_filter_l08_5(void) {
    return menu_item_enabled_36 != 0;
}

extern __typeof__(map_filter_l08_5) func_00207630 __attribute__((alias("FUN_00207630")));
