#include "types.h"
#include "rnc/ui/menus/menu_screen.h"

extern s32 draw_level_selection_map() __asm__("FUN_001fd748");
s32        draw_level_selection_map_screen(struct MenuScreen *menu) __asm__("FUN_00221930");

s32 draw_level_selection_map_screen(struct MenuScreen *menu) {
    s32 x;
    s32 y;

    x = menu->x;
    y = menu->y;
    draw_level_selection_map(x, x + menu->width, y, y + menu->height);
    return 2;
}
