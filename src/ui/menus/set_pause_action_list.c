#include "sda.h"
#include "types.h"
extern int pause_action_list_mode __asm__("D_0015EE90") MACRO_ADDR;

int set_pause_action_list(int *p) __asm__("FUN_0021a1b0");

extern int D_001D4810[];
extern int D_001D4840[];
int set_pause_action_list(int *destination) {
    if (pause_action_list_mode != 0) {
        destination[0xD] = (int)D_001D4810;
    } else {
        destination[0xD] = (int)D_001D4840;
    }
    return 0;
}

extern __typeof__(set_pause_action_list) func_0021A1B0 __attribute__((alias("FUN_0021a1b0")));
/* Recovered original symbol name. */
extern __typeof__(set_pause_action_list) SetPauseActionList __attribute__((alias("FUN_0021a1b0")));
