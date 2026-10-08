#include "types.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/ui/menus/menu_screen.h"

extern u8 available_preview_items[] __asm__("D_0013D4C0");
extern s32 draw_moby_list() __asm__("func_0020D330");
s32 draw_available_item_preview_mobys(struct MenuScreen *preview) __asm__("FUN_0021e608");

s32 draw_available_item_preview_mobys(struct MenuScreen *preview) {
    char *primary_moby;
    char *secondary_moby;
    struct MenuScreen *grid;

    grid = menu_system.current->focus;
    if (available_preview_items[grid->data.grid.cells[grid->data.grid.selected_cell].id] == 0) {
        return 0;
    }
    primary_moby = preview->data.preview.moby;
    if (primary_moby != 0) {
        draw_moby_list(primary_moby, 1);
    }
    secondary_moby = preview->data.preview.second_moby;
    if (secondary_moby != 0) {
        draw_moby_list(secondary_moby, 1);
    }
    return 8;
}

/* draw_moby_list reads only the Moby address and selection arguments. */
