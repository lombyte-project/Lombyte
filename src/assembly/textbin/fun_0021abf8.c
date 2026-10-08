#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_0021abf8/FUN_0021abf8.s", FUN_0021abf8);
#else
#include "types.h"
#include "rnc/ui/menus/menu_system.h"

#include "sda.h"

typedef struct {
    s16 type;
    s16 action;
    union {
        s32 entry_index;
        struct {
            u16 lo;
            s16 hi;
        } h;
    } param;
    s16 reserved8;
    u16 timer;
} MenuItem;

typedef struct {
    u8 pad0[0x14];
    s32 sound;
    u8 pad18[0x18];
    s32 flags;
    MenuItem *items;
    s32 prev;
    s32 next;
    s32 selected_entry;
} MenuDescriptor;

typedef struct {
    u8 pad0[0x38];
    s32 back;
    u8 pad3C[4];
    MenuDescriptor *focus;
    u8 pad44[0x3C];
    s32 link;
} MenuPage;

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

s32 update_menu_entry_actions(MenuDescriptor *menu) __asm__("FUN_0021abf8");

s32 update_menu_entry_actions(MenuDescriptor *menu) {
    s32 focused;
    s32 entry_index;
    s32 entry_count;
    s32 previous_selection;
    s32 flags;
    s32 buttons;
    s16 fade_timer;
    s16 message_index;
    MenuItem *items;
    MenuItem *entry;
    s32 selected_entry;

    focused = ((MenuPage *)menu_system.current)->focus == menu;
    for (entry_index = 0; menu->items[entry_index].type != 0; entry_index++) {
        if (focused && menu->selected_entry == entry_index) {
            menu->items[entry_index].timer = menu->items[entry_index].timer + 1;
        } else {
            fade_timer = (s16)menu->items[entry_index].timer;
            if (scale_game_frames(menu_fade_duration) < fade_timer) {
                menu->items[entry_index].timer = scale_game_frames(menu_fade_duration);
            }
            menu->items[entry_index].timer =
                (s16)menu->items[entry_index].timer > 0 ? menu->items[entry_index].timer - 1 : 0;
        }
    }
    if (!focused) {
        return 0;
    }
    if (controller_state.pressed & 0xD00) {
        if (menu->flags & 0x20) {
            selected_level_index[0] = current_level_index;
        }
        return -1;
    }
    if (controller_state.pressed & 0x10) {
        if (menu->flags & 0x20) {
            selected_level_index[0] = current_level_index;
        }
        if (menu_system.current->back != 0) {
            menu_system.next = menu_system.current->back;
        } else if (menu_system.close_blocked == 0) {
            return -1;
        }
    }
    if (controller_state.pressed & 0x40) {
        items = menu->items;
        selected_entry = menu->selected_entry;
        switch (items[selected_entry].action) {
        case 0:
            break;
        case 1:
        case 3:
            menu_system.next = (struct MenuPage *)menu->items[menu->selected_entry].param.entry_index;
            break;
        case 4:
            allocate_voice_for_target_entry(0, 0x11, menu->sound);
            if (mode_freeze_state == 1 || mode_freeze_state == 0x10) {
                menu_system.next = (struct MenuPage *)menu->items[menu->selected_entry].param.entry_index;
            } else {
                mode_freeze_flags |= 2;
                mode_freeze_init(3, menu->items[menu->selected_entry].param.entry_index);
            }
            break;
        case 5:
            allocate_voice_for_target_entry(0, 0x11, menu->sound);
            if (mode_freeze_state == 1 || mode_freeze_state == 0x10) {
                menu_system.next = (struct MenuPage *)menu->items[menu->selected_entry].param.entry_index;
            } else {
                mode_freeze_flags |= 4;
                mode_freeze_init(3, menu->items[menu->selected_entry].param.entry_index);
            }
            break;
        case 6:
            /* This action passes the low halfword; the other indexed actions pass the full word. */
            message_index = menu->items[menu->selected_entry].param.h.hi;
            if (message_index != 0) {
                menu_system.action_message = menu_action_messages[message_index];
            }
            menu_system.close_request = 5;
            menu_system.return_page = menu_system.current;
            menu_system.action_mode = 0;
            menu_system.action_value = menu->items[menu->selected_entry].param.h.lo;
            allocate_voice_for_target_entry(0, 0x11, menu->sound);
            return 0;
        case 7:
            menu_system.action_mode = 2;
            menu_system.return_page = menu_system.current;
            menu_system.close_request = 3;
            menu_system.action_value = menu->items[menu->selected_entry].param.entry_index;
            allocate_voice_for_target_entry(0, 0x11, menu->sound);
            return 0;
        case 8:
            menu_system.action_mode = 2;
            menu_system.return_page = menu_system.current;
            menu_system.close_request = 4;
            menu_system.action_value = menu->items[menu->selected_entry].param.entry_index;
            allocate_voice_for_target_entry(0, 0x11, menu->sound);
            return 0;
        case 10:
            menu_system.action_mode = 2;
            menu_system.return_page = menu_system.current;
            menu_system.close_request = 6;
            menu_system.action_value = menu->items[menu->selected_entry].param.entry_index;
            allocate_voice_for_target_entry(0, 0x11, menu->sound);
            return 0;
        case 11:
            menu_system.return_page = menu_system.current;
            menu_system.action_mode = 2;
            menu_system.close_request = 7;
            allocate_voice_for_target_entry(0, 0x11, menu->sound);
            return 0;
        case 9:
            requested_level_index = items[selected_entry].param.entry_index;
            return 0;
        case 2:
            allocate_voice_for_target_entry(2, 0x11, menu->sound);
            break;
        }
    }
    entry_count = 0;
    previous_selection = menu->selected_entry;
    flags = menu->flags;
    if (menu->items[0].type != 0) {
        do {
            entry = &menu->items[entry_count];
            entry_count++;
        } while (entry[1].type != 0);
    }
    if (flags & 1) {
        buttons = controller_state.held;
    } else {
        buttons = controller_state.pressed;
    }
    if ((buttons & 0x1000) || ((flags & 0x100) && (buttons & 4))) {
        if (menu->selected_entry != 0) {
            menu->selected_entry--;
        } else if (flags & 0x1000) {
            menu->selected_entry = entry_count - 1;
        } else {
            ((MenuPage *)menu_system.current)->link = menu->prev;
        }
    }
    if ((buttons & 0x4000) || ((menu->flags & 0x100) && (buttons & 8))) {
        if (menu->items[menu->selected_entry + 1].type != 0 &&
            menu->items[menu->selected_entry + 1].action != 0) {
            menu->selected_entry++;
        } else if (menu->flags & 0x1000) {
            menu->selected_entry = 0;
        } else {
            ((MenuPage *)menu_system.current)->link = menu->next;
        }
    }
    if (menu->selected_entry != previous_selection || ((MenuPage *)menu_system.current)->link != 0) {
        allocate_voice_for_target_entry(1, 0x11, menu->sound);
        if (menu->flags & 0x20) {
            selected_level_index[0] = menu_level_indices[menu->selected_entry];
        }
    }
    return 0;
}

extern __typeof__(update_menu_entry_actions) func_0021ABF8 __attribute__((alias("FUN_0021abf8")));

#endif /* NON_MATCHING */
