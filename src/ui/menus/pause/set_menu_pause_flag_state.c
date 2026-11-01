/* Ported from rac1-decomp (src/game/pause.c, func_0021EDD8). */
#include "sda.h"
#include "rnc/ui/menus/menu_screen.h"

extern unsigned char D_001413F4 NOT_SDA;

static inline short pauseFlagState(void) {
    if (D_001413F4 != 1) {
        return MENU_ACTION_SET_NEXT_PAGE;
    }
    return MENU_ACTION_DISABLED;
}

/* The select sits in a `static inline short` helper so that it stays a
   branch: jump.c turns an if into movz/movn only when its arm sets a
   full register, and the inline's short return value is a subreg. */
int set_menu_pause_flag_state(struct MenuScreen *menu) __asm__("FUN_0021ddd0");

int set_menu_pause_flag_state(struct MenuScreen *menu) {
    struct MenuTextItem *first_item = menu->data.list.items;
    first_item->action = pauseFlagState();
    return 0;
}

extern __typeof__(set_menu_pause_flag_state) func_0021DDD0 __attribute__((alias("FUN_0021ddd0")));
