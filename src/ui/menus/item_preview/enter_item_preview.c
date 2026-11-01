/* Ported from rac1-decomp (src/game/pause.c, func_0021EF38). */
#include "rnc/ui/menus/menu_screen.h"

int enter_item_preview(struct MenuScreen *menu) __asm__("FUN_0021df30");

int enter_item_preview(struct MenuScreen *menu) {
    menu->data.preview.rotation_angle = 3.14159274f;
    menu->data.preview.state = 0;
    menu->data.preview.moby = 0;
    menu->data.preview.second_moby = 0;
    return 0;
}

extern __typeof__(enter_item_preview) func_0021DF30 __attribute__((alias("FUN_0021df30")));
