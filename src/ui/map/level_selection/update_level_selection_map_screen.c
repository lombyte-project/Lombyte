#include "sda.h"
#include "rnc/input/pad_state.h"
#include "types.h"
#include "rnc/ui/menus/menu_system.h"

#include "rnc/ui/menus/menu_pages.h"

int update_level_selection_map_screen(void) __asm__("FUN_00221908");

int update_level_selection_map_screen(void) {
    if (controller_state.pressed_unmasked & PAD_CROSS) {
        menu_system.next = &planet_confirm_page;
    }
    return 0;
}

extern __typeof__(update_level_selection_map_screen) func_00221908
    __attribute__((alias("FUN_00221908")));
