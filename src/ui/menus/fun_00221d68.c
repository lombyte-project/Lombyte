#include "types.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/input/pad_state.h"
#include "rnc/ui/menus/menu_screen.h"

extern s32 allocate_voice_for_target_entry() __asm__("func_0022DA68");
s32 FUN_00221d68(struct MenuScreen *menu) {
    s32 prev;

    if ((controller_state.pressed_unmasked & (PAD_SELECT | PAD_R3 | PAD_START)) && menu_system.close_locked == 0) {
        return 1;
    }
    if (controller_state.pressed_unmasked & PAD_TRIANGLE) {
        if (menu_system.current->back != 0) {
            menu_system.next = menu_system.current->back;
        } else if (menu_system.close_locked == 0) {
            return -1;
        }
    }
    prev = menu->data.list.selected;
    if (controller_state.pressed & PAD_CROSS) {
        menu->data.list.selected = (prev + 1) % 30;
    } else if (controller_state.pressed & PAD_CIRCLE) {
        menu->data.list.selected = (prev + 29) % 30;
    }
    if (menu->data.list.selected != prev) {
        allocate_voice_for_target_entry(1, 0x11, menu->moby);
    }
    return 0;
}
