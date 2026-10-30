#include "sda.h"
#include "types.h"
#include "rnc/ui/menus/menu_globals.h"

int set_pause_action_list(int *p) __asm__("FUN_0021a1b0");

#include "rnc/ui/menus/menu_pages.h"

int set_pause_action_list(int *destination) {
    if (pause_action_list_mode != 0) {
        destination[0xD] = (int)title_options_items_with_video;
    } else {
        destination[0xD] = (int)title_options_items_no_video;
    }
    return 0;
}

extern __typeof__(set_pause_action_list) func_0021A1B0 __attribute__((alias("FUN_0021a1b0")));
/* Recovered original symbol name. */
extern __typeof__(set_pause_action_list) SetPauseActionList __attribute__((alias("FUN_0021a1b0")));
