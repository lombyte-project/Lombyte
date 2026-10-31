/* Ported from rac1-decomp (src/game/pause.c, func_0021C790). */
#include "sda.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/ui/menus/menu_screen.h"

/* Each block reads menu_system through its own pointer local: the loop's
   own copy is why GCSE leaves its unk138 load in the loop (only unk134,
   read first in the body, is hoisted by loop.c), and the last block's
   copy is retail's kept %hi. */
int find_next_pause_page(struct MenuScreen *grid, int reverse) __asm__("FUN_0021b7a8");

int find_next_pause_page(struct MenuScreen *grid, int reverse) {
    int skip = 0;
    int moved = 0;

    if (reverse != 0) {
        return 0;
    }
    {
        struct MenuSystem *g = &menu_system;
        if (g->unk134 != 0 && (grid->data.grid.flags & MENU_GRID_HIDDEN_WITH_UNK134)) {
            skip = 1;
        }
        if (g->unk138 != 0 && (grid->data.grid.flags & MENU_GRID_HIDDEN_WITH_UNK138)) {
            skip = 1;
        }
    }
    while (skip) {
        struct MenuSystem *g = &menu_system;
        moved = 1;
        grid = grid->data.grid.up;
        skip = 0;
        if (g->unk134 != 0 && (grid->data.grid.flags & MENU_GRID_HIDDEN_WITH_UNK134)) {
            skip = 1;
        }
        if (g->unk138 != 0 && (grid->data.grid.flags & MENU_GRID_HIDDEN_WITH_UNK138)) {
            skip = 1;
        }
    }
    if (moved) {
        struct MenuSystem *focus_menu_system = &menu_system;
        focus_menu_system->current->pending_focus = grid;
    }
    return 0;
}

extern __typeof__(find_next_pause_page) func_0021B7A8 __attribute__((alias("FUN_0021b7a8")));
