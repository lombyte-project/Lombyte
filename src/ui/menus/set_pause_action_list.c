#include "sda.h"
extern int D_0015EE90 MACRO_ADDR;
extern int D_001D4810[];
extern int D_001D4840[];

int set_pause_action_list(int *p) __asm__("FUN_0021a1b0");

int set_pause_action_list(int *p) {
    if (D_0015EE90 != 0) {
        p[0xD] = (int)D_001D4810;
    } else {
        p[0xD] = (int)D_001D4840;
    }
    return 0;
}

extern __typeof__(set_pause_action_list) func_0021A1B0 __attribute__((alias("FUN_0021a1b0")));
/* Recovered original symbol name. */
extern __typeof__(set_pause_action_list) SetPauseActionList __attribute__((alias("FUN_0021a1b0")));
