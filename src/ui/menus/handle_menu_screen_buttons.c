#include "types.h"
#include "rnc/input/pad_state.h"
#include "rnc/ui/menus/menu_system.h"

/* Accept/back handling shared by the menu screens: only the screen that has
   focus on the current page reacts. Accept (0xD00) returns 1 unless closing
   is locked; back (0x10) requests the page's back page, or returns -1 when
   there is none and closing is not locked. */
extern struct PadState D_0013C940;

s32 handle_menu_screen_buttons(struct MenuScreen *screen) {
    if (menu_system.current->focus != screen) {
        return 0;
    }
    if ((D_0013C940.pressed_unmasked & 0xD00) && menu_system.close_locked == 0) {
        return 1;
    }
    if (D_0013C940.pressed_unmasked & 0x10) {
        struct MenuPage *back = menu_system.current->back;
        if (back != 0) {
            menu_system.next = back;
        } else if (menu_system.close_locked == 0) {
            return -1;
        }
    }
    return 0;
}
