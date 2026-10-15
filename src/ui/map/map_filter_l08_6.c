#include "sda.h"
extern unsigned char menu_item_enabled_37 __asm__("D_0013D3B9") NOT_SDA;

int map_filter_l08_6(void) __asm__("FUN_00207640");

int map_filter_l08_6(void) {
    return menu_item_enabled_37 != 0;
}

extern __typeof__(map_filter_l08_6) func_00207640 __attribute__((alias("FUN_00207640")));
