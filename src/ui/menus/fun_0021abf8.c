#include "types.h"
#include "sda.h"
#include "rnc/input/pad_state.h"
#include "rnc/ui/menus/menu_screen.h"
#include "rnc/ui/menus/menu_system.h"

extern struct PadState D_0013C940;
/* gp-relative here, so not the plain rnc/globals.h spellings. */
extern s32 current_level_index __asm__("D_0015ED84") __attribute__((sda));
extern s32 game_language __asm__("D_0015ED88") __attribute__((sda));
extern s32 mode_freeze_state __asm__("D_0015EEB0");
extern s32 mode_freeze_flags __asm__("D_0015EEB4");
extern s32 menu_fade_duration __asm__("D_001601B4") __attribute__((sda));
extern s32 *menu_level_indices __asm__("D_001601E0") __attribute__((sda));
extern s32 menu_action_messages[] __asm__("D_00199478");
extern s32 selected_level_index[] __asm__("D_001A0314");

extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
extern void mode_freeze_init(s32, s32) __asm__("func_001FBAB8");
extern s32 allocate_voice_for_target_entry(s32, s32, s32) __asm__("func_0022DA68");

/*
 * Per-frame update of a menu list: fades the entry highlights, handles
 * cancel/back, runs the selected entry's action on accept, and moves the
 * selection up/down (with wrap or hand-off to the neighbouring list).
 */
s32 update_menu_entry_actions(struct MenuScreen *menu) __asm__("FUN_0021abf8");

s32 update_menu_entry_actions(struct MenuScreen *menu) {
    s32 focused;
    s32 entry_index;
    s32 entry_count;
    s32 previous_selection;
    s32 flags;
    s32 buttons;
    s16 fade_timer;
    s16 message_index;

    focused = menu_system.current->focus == menu;
    for (entry_index = 0; menu->data.list.items[entry_index].text != 0; entry_index++) {
        if (focused && menu->data.list.selected == entry_index) {
            menu->data.list.items[entry_index].fade_timer =
                menu->data.list.items[entry_index].fade_timer + 1;
        } else {
            fade_timer = menu->data.list.items[entry_index].fade_timer;
            if (scale_game_frames(menu_fade_duration) < fade_timer) {
                menu->data.list.items[entry_index].fade_timer =
                    scale_game_frames(menu_fade_duration);
            }
            menu->data.list.items[entry_index].fade_timer =
                menu->data.list.items[entry_index].fade_timer > 0
                    ? menu->data.list.items[entry_index].fade_timer - 1
                    : 0;
        }
    }
    if (!focused) {
        return 0;
    }
    if (D_0013C940.pressed_unmasked & 0xD00) {
        if (menu->data.list.flags & 0x20) {
            selected_level_index[0] = current_level_index;
        }
        return -1;
    }
    if (D_0013C940.pressed_unmasked & 0x10) {
        if (menu->data.list.flags & 0x20) {
            selected_level_index[0] = current_level_index;
        }
        if (menu_system.current->back != 0) {
            menu_system.next = menu_system.current->back;
        } else if (menu_system.close_locked == 0) {
            return -1;
        }
    }
    if (D_0013C940.pressed_unmasked & 0x40) {
        /* Index through menu-> directly: locals for items/selected change
           which register keeps the copy of selected that actions 6 and 9 reuse. */
        switch (menu->data.list.items[menu->data.list.selected].action) {
        case 0:
            break;
        case 1:
        case 3:
            menu_system.next =
                (struct MenuPage *)menu->data.list.items[menu->data.list.selected].param.value;
            break;
        case 4:
            allocate_voice_for_target_entry(0, 0x11, menu->moby);
            if (mode_freeze_state == 1 || mode_freeze_state == 0x10) {
                menu_system.next =
                    (struct MenuPage *)menu->data.list.items[menu->data.list.selected].param.value;
            } else {
                mode_freeze_flags |= 2;
                mode_freeze_init(3, menu->data.list.items[menu->data.list.selected].param.value);
            }
            break;
        case 5:
            allocate_voice_for_target_entry(0, 0x11, menu->moby);
            if (mode_freeze_state == 1 || mode_freeze_state == 0x10) {
                menu_system.next =
                    (struct MenuPage *)menu->data.list.items[menu->data.list.selected].param.value;
            } else {
                mode_freeze_flags |= 4;
                mode_freeze_init(3, menu->data.list.items[menu->data.list.selected].param.value);
            }
            break;
        case 6:
            /* This action passes the low halfword; the other indexed actions pass the full word. */
            message_index = menu->data.list.items[menu->data.list.selected].param.half.hi;
            if (message_index != 0) {
                menu_system.unkEC = menu_action_messages[message_index];
            }
            menu_system.close_request = 5;
            menu_system.unkF0 = menu_system.current;
            menu_system.unkF4 = 0;
            menu_system.unkE4 =
                menu->data.list.items[menu->data.list.selected].param.half.lo;
            allocate_voice_for_target_entry(0, 0x11, menu->moby);
            return 0;
        case 7:
            menu_system.unkF4 = 2;
            menu_system.unkF0 = menu_system.current;
            menu_system.close_request = 3;
            menu_system.unkE4 = menu->data.list.items[menu->data.list.selected].param.value;
            allocate_voice_for_target_entry(0, 0x11, menu->moby);
            return 0;
        case 8:
            menu_system.unkF4 = 2;
            menu_system.unkF0 = menu_system.current;
            menu_system.close_request = 4;
            menu_system.unkE4 = menu->data.list.items[menu->data.list.selected].param.value;
            allocate_voice_for_target_entry(0, 0x11, menu->moby);
            return 0;
        case 10:
            menu_system.unkF4 = 2;
            menu_system.unkF0 = menu_system.current;
            menu_system.close_request = 6;
            menu_system.unkE4 = menu->data.list.items[menu->data.list.selected].param.value;
            allocate_voice_for_target_entry(0, 0x11, menu->moby);
            return 0;
        case 11:
            menu_system.unkF0 = menu_system.current;
            menu_system.unkF4 = 2;
            menu_system.close_request = 7;
            allocate_voice_for_target_entry(0, 0x11, menu->moby);
            return 0;
        case 9:
            game_language = menu->data.list.items[menu->data.list.selected].param.value;
            return 0;
        case 2:
            allocate_voice_for_target_entry(2, 0x11, menu->moby);
            break;
        }
    }
    /* Count first, then read selected/flags: in the other order (or as a
       do-while) the allocator shifts every register of the tail. */
    for (entry_count = 0; menu->data.list.items[entry_count].text != 0; entry_count++) {
    }
    previous_selection = menu->data.list.selected;
    flags = menu->data.list.flags;
    if (flags & 1) {
        buttons = D_0013C940.raw_pressed;
    } else {
        buttons = D_0013C940.pressed_unmasked;
    }
    if ((buttons & 0x1000) || ((flags & 0x100) && (buttons & 4))) {
        if (menu->data.list.selected != 0) {
            menu->data.list.selected--;
        } else if (flags & 0x1000) {
            menu->data.list.selected = entry_count - 1;
        } else {
            menu_system.current->pending_focus = menu->data.list.up;
        }
    }
    if ((buttons & 0x4000) || ((menu->data.list.flags & 0x100) && (buttons & 8))) {
        if (menu->data.list.items[menu->data.list.selected + 1].text != 0 &&
            menu->data.list.items[menu->data.list.selected + 1].action != 0) {
            menu->data.list.selected++;
        } else if (menu->data.list.flags & 0x1000) {
            menu->data.list.selected = 0;
        } else {
            menu_system.current->pending_focus = menu->data.list.down;
        }
    }
    if (menu->data.list.selected != previous_selection || menu_system.current->pending_focus != 0) {
        allocate_voice_for_target_entry(1, 0x11, menu->moby);
        if (menu->data.list.flags & 0x20) {
            selected_level_index[0] = menu_level_indices[menu->data.list.selected];
        }
    }
    return 0;
}

extern __typeof__(update_menu_entry_actions) func_0021ABF8 __attribute__((alias("FUN_0021abf8")));

