#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_0021abf8/FUN_0021abf8.s", FUN_0021abf8);
#else
#include "types.h"
#include "rnc/ui/menus/menu_system.h"

#include "sda.h"

#include "rnc/ui/menus/menu_screen.h"

typedef struct {
    u8 pad0[0x1B4];
    s32 held;
    u8 pad1B8[0xC];
    s32 pressed;
} MenuControllerState;

extern MenuControllerState controller_state __asm__("D_0013C940");
extern s32 current_level_index __asm__("D_0015ED84") __attribute__((sda));
extern s32 requested_level_index __asm__("D_0015ED88") __attribute__((sda));
extern s32 mode_freeze_state __asm__("D_0015EEB0");
extern s32 mode_freeze_flags __asm__("D_0015EEB4");
extern s32 menu_fade_duration __asm__("D_001601B4") __attribute__((sda));
extern s32 *menu_level_indices __asm__("D_001601E0") __attribute__((sda));
extern s32 menu_action_messages[] __asm__("D_00199478");
extern s32 selected_level_index[] __asm__("D_001A0314");

extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern void mode_freeze_init(s32, s32) __asm__("func_001FBAB8");
extern s32 allocate_voice_for_target_entry(s32, s32, s32) __asm__("func_0022DA68");

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
    struct MenuTextItem *items;
    struct MenuTextItem *entry;
    s32 selected_entry;

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
    if (controller_state.pressed & 0xD00) {
        if (menu->data.list.flags & 0x20) {
            selected_level_index[0] = current_level_index;
        }
        return -1;
    }
    if (controller_state.pressed & 0x10) {
        if (menu->data.list.flags & 0x20) {
            selected_level_index[0] = current_level_index;
        }
        if (menu_system.current->back != 0) {
            menu_system.next = menu_system.current->back;
        } else if (menu_system.unk124 == 0) {
            return -1;
        }
    }
    if (controller_state.pressed & 0x40) {
        items = menu->data.list.items;
        selected_entry = menu->data.list.selected;
        switch (items[selected_entry].action) {
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
            requested_level_index = items[selected_entry].param.value;
            return 0;
        case 2:
            allocate_voice_for_target_entry(2, 0x11, menu->moby);
            break;
        }
    }
    entry_count = 0;
    previous_selection = menu->data.list.selected;
    flags = menu->data.list.flags;
    if (menu->data.list.items[0].text != 0) {
        do {
            entry = &menu->data.list.items[entry_count];
            entry_count++;
        } while (entry[1].text != 0);
    }
    if (flags & 1) {
        buttons = controller_state.held;
    } else {
        buttons = controller_state.pressed;
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

#endif /* NON_MATCHING */
