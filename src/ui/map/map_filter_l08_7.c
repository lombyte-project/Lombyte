/* Ported from rac1-decomp (src/game/menu.c, func_00207F00). */
#define NOT_SDA __attribute__((section(".data")))
extern unsigned char menu_item_enabled_38 __asm__("D_0013D3BA") NOT_SDA;
int map_filter_l08_7(void) __asm__("FUN_00207650");

int map_filter_l08_7(void) {
    return menu_item_enabled_38 != 0;
}

extern __typeof__(map_filter_l08_7) func_00207650 __attribute__((alias("FUN_00207650")));
