#include "types.h"
#include "rnc/input/pad_state.h"
#include "rnc/ui/menus/menu_system.h"

/* Pause list handler (allocate_voice_for_target_entry's family): one `char *` pad local per
   block. The cursor at menu+0x3C moves on 0x1000/0x4000 within
   menu+0x40, with a notify on change; then the scroll (menu+0x60) is
   clamped around the cursor and the bar position at menu+0x5C derived
   from it, or pinned to 0x60. The re-reads of 0x3C and 0x60 are how
   retail's reloads come out; the page size q - 2 is formed after the
   clamp, and each arm stores the bar position itself. The page size is a
   literal 0x260, so gcc keeps the divide-by-zero trap retail has. The
   switch target has no early return: "else if" instead, which is what
   gives retail's bnel with the store in the delay slot. */
extern void allocate_voice_for_target_entry(s32, s32, s32) __asm__("func_0022DA68");

int update_menu_icon_list(char *menu) __asm__("FUN_00219e10");

int update_menu_icon_list(char *menu) {
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
            struct MenuPage   *t = menu_system_state->current->back;
            if (t != 0) {
                menu_system_state->next = t;
            } else if (menu_system_state->close_locked == 0) {
                return -1;
            }
        }
    }
    {
        int pressed_buttons = controller_state.pressed_unmasked;
        int old = *(int *)(menu + 0x3C);

        if ((pressed_buttons & 0x1000) && old != 0) {
            *(int *)(menu + 0x3C) = old - 1;
        }
        if (pressed_buttons & 0x4000) {
            int c = *(int *)(menu + 0x3C) + 1;
            if (c < *(int *)(menu + 0x40)) {
                *(int *)(menu + 0x3C) = c;
            }
        }
        if (*(int *)(menu + 0x3C) != old) {
            allocate_voice_for_target_entry(1, 0x11, *(int *)(menu + 0x14));
        }
    }
    {
        int t = *(int *)(menu + 0x24) * 16;

        if (t / 0x260 >= *(int *)(menu + 0x40)) {
            *(int *)(menu + 0x5C) = 0x60;
        } else {
            int cursor_scroll_offset_base = (t - 0x28) / 0x260;
            int max_cursor_scroll_offset;

            int c = *(volatile int *)(menu + 0x3C);
            if (*(int *)(menu + 0x60) >= c) {
                *(int *)(menu + 0x60) = c - 1;
                if (*(int *)(menu + 0x60) < 0) {
                    *(int *)(menu + 0x60) = 0;
                }
            }
            max_cursor_scroll_offset = cursor_scroll_offset_base - 2;
            if (*(int *)(menu + 0x60) < *(int *)(menu + 0x3C) - max_cursor_scroll_offset) {
                *(int *)(menu + 0x60) = *(int *)(menu + 0x3C) - max_cursor_scroll_offset;
            }
            *(int *)(menu + 0x5C) = 0x190 - *(int *)(menu + 0x60) * 0x260;
        }
    }
    return 0;
}

extern __typeof__(update_menu_icon_list) func_00219E10 __attribute__((alias("FUN_00219e10")));
