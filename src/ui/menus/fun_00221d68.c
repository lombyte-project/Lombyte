#include "types.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/input/pad_state.h"
#include "rnc/ui/menus/menu_screen.h"

extern struct PadState D_0013C940;
extern s32 allocate_voice_for_target_entry() __asm__("func_0022DA68");
s32 FUN_00221d68(struct MenuScreen *menu) {
    s32 prev;

    if ((D_0013C940.pressed_unmasked & 0xD00) && menu_system.close_locked == 0) {
        return 1;
    }
    if (D_0013C940.pressed_unmasked & 0x10) {
        if (menu_system.current->back != 0) {
            menu_system.next = menu_system.current->back;
        } else if (menu_system.close_locked == 0) {
            return -1;
        }
    }
    prev = menu->data.list.selected;
    if (D_0013C940.pressed & 0x40) {
        menu->data.list.selected = (prev + 1) % 30;
    } else if (D_0013C940.pressed & 0x20) {
        menu->data.list.selected = (prev + 29) % 30;
    }
    if (menu->data.list.selected != prev) {
        allocate_voice_for_target_entry(1, 0x11, menu->moby);
    }
    return 0;
}
