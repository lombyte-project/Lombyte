#include "types.h"
#include "rnc/input/pad_state.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/globals.h"
#include "rnc/ui/menus/menu_screen.h"
#include "rnc/data_labels.h"
#include "rnc/ui/menus/menu_globals.h"

extern s32 allocate_voice_for_target_entry() __asm__("func_0022DA68");
s32        handle_menu_action_buttons(struct MenuScreen *menu_screen) __asm__("FUN_00221968");

s32 handle_menu_action_buttons(struct MenuScreen *menu_screen) {
    if (controller_state.pressed_unmasked & PAD_TRIANGLE) {
        if (menu_system.current->back != 0) {
            menu_system.next = menu_system.current->back;
        } else if (menu_system.close_locked == 0) {
            return -1;
        }
    } else if (controller_state.pressed_unmasked & PAD_START) {
        selected_level_index[0] = current_level_index;
        return 1;
    } else if (controller_state.pressed_unmasked & PAD_CROSS) {
        allocate_voice_for_target_entry(0, 0x11, menu_screen->moby);
        return 1;
    } else if (controller_state.pressed_unmasked & PAD_CIRCLE) {
        menu_system.unkE4 = selected_level_index[0];
        menu_system.unkF0 = menu_system.current;
        menu_system.close_request = 3;
        menu_system.unkF4 = 0xF;
        allocate_voice_for_target_entry(0, 0x11, menu_screen->moby);
    }
    return 0;
}

extern __typeof__(handle_menu_action_buttons) func_00221968 __attribute__((alias("FUN_00221968")));
