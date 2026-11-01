#include "types.h"
#include "rnc/input/pad_state.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/ui/menus/menu_screen.h"

/* The re-reads of cursor and first_shown are how retail's reloads come out;
   the page size is formed after the clamp, and each arm stores first_y
   itself. The icon pitch is a literal 0x260, so gcc keeps the
   divide-by-zero trap retail has. The back-button branch has no early
   return: "else if" gives retail's bnel with the store in the delay slot. */
extern void allocate_voice_for_target_entry(s32, s32, s32) __asm__("func_0022DA68");

int update_menu_icon_list(struct MenuScreen *menu) __asm__("FUN_00219e10");

int update_menu_icon_list(struct MenuScreen *menu) {
    {
        if (controller_state.pressed_unmasked & (PAD_SELECT | PAD_R3 | PAD_START)) {
            if (menu_system.close_locked == 0) {
                return 1;
            }
        }
    }
    {
        if (controller_state.pressed_unmasked & PAD_TRIANGLE) {
            struct MenuSystem *menu_system_state = &menu_system;
            struct MenuPage   *back = menu_system_state->current->back;
            if (back != 0) {
                menu_system_state->next = back;
            } else if (menu_system_state->close_locked == 0) {
                return -1;
            }
        }
    }
    {
        int pressed_buttons = controller_state.pressed_unmasked;
        int previous_cursor_index = menu->data.icons.cursor;

        if ((pressed_buttons & PAD_UP) && previous_cursor_index != 0) {
            menu->data.icons.cursor = previous_cursor_index - 1;
        }
        if (pressed_buttons & PAD_DOWN) {
            int next_cursor = menu->data.icons.cursor + 1;
            if (next_cursor < menu->data.icons.count) {
                menu->data.icons.cursor = next_cursor;
            }
        }
        if (menu->data.icons.cursor != previous_cursor_index) {
            allocate_voice_for_target_entry(1, 0x11, menu->moby);
        }
    }
    {
        int list_height = menu->height * 16;

        if (list_height / 0x260 >= menu->data.icons.count) {
            menu->data.icons.first_y = 0x60;
        } else {
            int cursor_scroll_offset_base = (list_height - 0x28) / 0x260;
            int max_cursor_scroll_offset;

            int cursor = *(volatile s32 *)&menu->data.icons.cursor;
            if (menu->data.icons.first_shown >= cursor) {
                menu->data.icons.first_shown = cursor - 1;
                if (menu->data.icons.first_shown < 0) {
                    menu->data.icons.first_shown = 0;
                }
            }
            max_cursor_scroll_offset = cursor_scroll_offset_base - 2;
            if (menu->data.icons.first_shown < menu->data.icons.cursor - max_cursor_scroll_offset) {
                menu->data.icons.first_shown = menu->data.icons.cursor - max_cursor_scroll_offset;
            }
            menu->data.icons.first_y = 0x190 - menu->data.icons.first_shown * 0x260;
        }
    }
    return 0;
}

extern __typeof__(update_menu_icon_list) func_00219E10 __attribute__((alias("FUN_00219e10")));
