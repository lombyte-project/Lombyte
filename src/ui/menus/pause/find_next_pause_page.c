/* Ported from rac1-decomp (src/game/pause.c, func_0021C790). */
#include "sda.h"
extern char D_001D5BF0[] NOT_SDA;
/* Pause page link walk: with reverse clear, follow arg0's +0x4C chain for
   as long as the current page is skippable (bit 8 of its +0x30 flags
   while D_001D5BF0+0x134 is set, or bit 4 while +0x138 is set), and if
   it moved at all, make the page it stopped on the current one's +0x80.
   Each block reads D_001D5BF0 through its own `char *` local: the loop's
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
        char *g = D_001D5BF0;
        if (*(int *)(g + 0x134) != 0 && (*(int *)(arg0 + 0x30) & 8)) {
            ok = 1;
        }
        if (*(int *)(g + 0x138) != 0 && (*(int *)(arg0 + 0x30) & 4)) {
            ok = 1;
        }
    }
    while (ok) {
        char *g = D_001D5BF0;
        found = 1;
        arg0 = *(char **)(arg0 + 0x4C);
        ok = 0;
        if (*(int *)(g + 0x134) != 0 && (*(int *)(arg0 + 0x30) & 8)) {
            ok = 1;
        }
        if (*(int *)(g + 0x138) != 0 && (*(int *)(arg0 + 0x30) & 4)) {
            ok = 1;
        }
    }
    if (found) {
        char *g2 = D_001D5BF0;
        *(char **)(*(char **)(g2 + 4) + 0x80) = arg0;
    }
    return 0;
}

extern __typeof__(find_next_pause_page) func_0021B7A8 __attribute__((alias("FUN_0021b7a8")));
