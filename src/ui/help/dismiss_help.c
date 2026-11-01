/* Ported from rac1-decomp (src/game/help.c, func_001FE438). */
#include "rnc/ui/help/help_state.h"

/* Every access goes
   through the global, and cases 6 and 7 are spelled out: the jump table
   has eight entries. */
void dismiss_help(void) __asm__("FUN_001fdc08");

void dismiss_help(void) {
    switch (help_state.state) {
    case 0:
        help_state.pending = -1;
        break;
    case 1:
    case 2:
        help_state.state = 7;
        help_state.timer = 0;
        break;
    case 3:
        help_state.state = 7;
        help_state.timer = 0;
        break;
    case 4:
        help_state.state = 6;
        help_state.timer = 4 - help_state.timer;
        break;
    case 5:
        help_state.state = 6;
        help_state.timer = 0;
        break;
    case 6:
    case 7:
        break;
    }
}

extern __typeof__(dismiss_help) func_001FDC08 __attribute__((alias("FUN_001fdc08")));
