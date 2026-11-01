#include "rnc/ui/menus/menu_screen.h"

int enter_timed_menu_sequence(struct MenuScreen *menu) __asm__("FUN_002223d8");

int enter_timed_menu_sequence(struct MenuScreen *menu) {
    menu->data.timed.timer = 0;
    menu->data.timed.phase = 0;
    menu->data.timed.scroll = 0;
    return 0;
}

extern __typeof__(enter_timed_menu_sequence) func_002223D8 __attribute__((alias("FUN_002223d8")));
