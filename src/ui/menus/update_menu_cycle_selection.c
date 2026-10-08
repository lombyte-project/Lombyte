#include "types.h"

#include "rnc/input/pad_state.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/ui/menus/menu_screen.h"

extern struct PadState controller_state __asm__("D_0013C940");
extern s32 allocate_voice_for_target_entry(s32, s32, s32) __asm__("func_0022DA68");

s32 update_menu_cycle_selection(struct MenuScreen *menu) __asm__("FUN_00221e50");

/* The twelve choices wrap in either direction; back-page changes are deferred. */
s32 update_menu_cycle_selection(struct MenuScreen *menu) {
    struct MenuPage *back_page;

    if ((controller_state.pressed_unmasked & 0xD00) && menu_system.close_locked == 0) {
        return 1;
    }
    if (controller_state.pressed_unmasked & 0x10) {
        back_page = menu_system.current->back;
        if (back_page != 0) {
            menu_system.next = back_page;
        } else if (menu_system.close_locked == 0) {
            return -1;
        }
    }
    if (controller_state.pressed & 0x2040) {
        menu->data.cycle.selection = (menu->data.cycle.selection + 1) % 12;
        allocate_voice_for_target_entry(1, 0x11, menu->moby);
    } else if (controller_state.pressed & 0x8020) {
        menu->data.cycle.selection = (menu->data.cycle.selection + 11) % 12;
        allocate_voice_for_target_entry(1, 0x11, menu->moby);
    }
    return 0;
}

extern __typeof__(update_menu_cycle_selection) func_00221E50 __attribute__((alias("FUN_00221e50")));
