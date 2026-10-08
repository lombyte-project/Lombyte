/* Ported from rac1-decomp (src/game/pause.c, func_0021C790). */
#include "sda.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/ui/menus/menu_screen.h"
/* Pause page link walk: with reverse clear, follow arg0's +0x4C chain for
   as long as the current page is skippable (bit 8 of its +0x30 flags
   while menu_system.unk134 is set, or bit 4 while +0x138 is set), and if
   it moved at all, make the page it stopped on current->pending_focus.
   Each block reads menu_system through its own pointer local: the loop's
   own copy is why GCSE leaves its +0x138 load in the loop (only +0x134,
   read first in the body, is hoisted by loop.c), and the last block's
   copy is retail's kept %hi. */
int find_next_pause_page(char *arg0, int reverse) __asm__("FUN_0021b7a8");

int find_next_pause_page(char *arg0, int reverse) {
    int ok = 0;
    int found = 0;

    if (reverse != 0) {
        return 0;
    }
    {
        struct MenuSystem *g = &menu_system;
        if (g->unk134 != 0 && (*(int *)(arg0 + 0x30) & 8)) {
            ok = 1;
        }
        if (g->unk138 != 0 && (*(int *)(arg0 + 0x30) & 4)) {
            ok = 1;
        }
    }
    while (ok) {
        struct MenuSystem *g = &menu_system;
        found = 1;
        arg0 = *(char **)(arg0 + 0x4C);
        ok = 0;
        if (g->unk134 != 0 && (*(int *)(arg0 + 0x30) & 8)) {
            ok = 1;
        }
        if (g->unk138 != 0 && (*(int *)(arg0 + 0x30) & 4)) {
            ok = 1;
        }
    }
    if (found) {
        struct MenuSystem *g2 = &menu_system;
        g2->current->pending_focus = (struct MenuScreen *)arg0;
    }
    return 0;
}

extern __typeof__(find_next_pause_page) func_0021B7A8 __attribute__((alias("FUN_0021b7a8")));
