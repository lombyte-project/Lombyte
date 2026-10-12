#include "sda.h"
extern int pause_action_list_mode __asm__("D_0015EE90") MACRO_ADDR;
extern int pause_action_list_a[] __asm__("D_001D4810");
extern int pause_action_list_b[] __asm__("D_001D4840");

int set_pause_action_list(int *p) __asm__("FUN_0021a1b0");

int set_pause_action_list(int *p) {
    if (pause_action_list_mode != 0) {
        p[0xD] = (int)pause_action_list_a;
    } else {
        p[0xD] = (int)pause_action_list_b;
    }
    return 0;
}

extern __typeof__(set_pause_action_list) func_0021A1B0 __attribute__((alias("FUN_0021a1b0")));
/* Recovered original symbol name. */
extern __typeof__(set_pause_action_list) SetPauseActionList __attribute__((alias("FUN_0021a1b0")));
