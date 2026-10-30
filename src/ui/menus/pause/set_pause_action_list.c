#include "sda.h"
#include "types.h"
#include "rnc/ui/menus/menu_globals.h"

int set_pause_action_list(int *p) __asm__("FUN_0021a1b0");

extern int pause_action_list_nonzero_mode[] __asm__("D_001D4810");
extern int pause_action_list_zero_mode[] __asm__("D_001D4840");

int set_pause_action_list(int *destination) {
    if (pause_action_list_mode != 0) {
        destination[0xD] = (int)pause_action_list_nonzero_mode;
    } else {
        destination[0xD] = (int)pause_action_list_zero_mode;
    }
    return 0;
}

extern __typeof__(set_pause_action_list) func_0021A1B0 __attribute__((alias("FUN_0021a1b0")));
/* Recovered original symbol name. */
extern __typeof__(set_pause_action_list) SetPauseActionList __attribute__((alias("FUN_0021a1b0")));
