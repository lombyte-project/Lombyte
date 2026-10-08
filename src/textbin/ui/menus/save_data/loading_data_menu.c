#include "types.h"
#include "rnc/ui/menus/menu_system.h"
#include "asm.h"

#include "types.h"
#include "rnc/globals.h"

struct LoadMenuControllerState {
    u8 pad_0[0x1B4];
    s32 held_buttons;
    u8 pad_1B8[0xC];
    s32 pressed_buttons;
};
struct LoadMenuMixerState {
    u8 pad_0[0x48];
    s32 group_0_volume;
    s32 group_1_volume;
    s32 group_2_volume;
    s32 group_3_volume;
    s32 group_4_volume;
    s32 group_5_volume;
};
struct LoadMenuDescriptor {
    u8 pad_0[0x14];
    s32 sound_owner;
    u8 pad_18[0x18];
    s32 flags;
    u8 pad_34[0xC];
    s32 selected_save_slot;
};
extern struct LoadMenuControllerState controller_state __asm__("D_0013C940");
#include "rnc/storage/memory_card/memory_card_state.h"
extern s16 D_0013E05A[];
extern struct LoadMenuMixerState mixer_state __asm__("D_0013E550");
extern s32 music_volume __asm__("D_0015EDEC");
extern s32 sound_volume __asm__("D_0015EDF0");
extern s32 selected_save_slot __asm__("D_0015EE34");
extern void InitializeGlobalStateEntry(s32);
extern s32 mode_freeze_init() __asm__("func_001FBAB8");
extern s32 allocate_voice_for_target_entry() __asm__("func_0022DA68");
s32 loading_data_menu(struct LoadMenuDescriptor *menu) __asm__("FUN_002232d8");

s32 loading_data_menu(struct LoadMenuDescriptor *menu) {
    s32 previous_save_slot;
    struct MenuPage *back_page;
    s32 next_save_slot;
    s32 buttons;
    s32 scaled_volume_80;
    s32 scaled_volume_70;
    previous_save_slot = menu->selected_save_slot;
    if (menu_system.card_op_pending != 0) {
        if ((memory_card_state.state < 3) &&
            (memory_card_state.pending_state < 0)) {
            menu_system.card_op_pending = 0;
            if (memory_card_state.err != 0) {
                mode_freeze_flags |= 0x100;
                mode_freeze_init(3, menu_system.current);
                return 0;
            }
            memory_card_state.unkF4 = 1;
            mixer_state.group_0_volume = ((s32)(sound_volume * 8)) / 10;
            mixer_state.group_1_volume = music_volume;
            scaled_volume_70 = ((s32)(sound_volume * 7)) / 10;
            scaled_volume_80 = mixer_state.group_0_volume;
            mixer_state.group_2_volume = scaled_volume_80;
            mixer_state.group_3_volume = ((s32)(sound_volume * 7)) / 10;
            scaled_volume_70 = ((s32)(sound_volume * 7)) / 10;
            mixer_state.group_4_volume = scaled_volume_70;
            mixer_state.group_5_volume = sound_volume;
            InitializeGlobalStateEntry(current_level_index);
            D_0013E05A[0] = 0;
        } else {
            return 0;
        }
    }
    if (controller_state.pressed_buttons & 0xD00) {
        if (menu_system.close_locked == 0) {
            return 1;
        }
    }
    if (controller_state.pressed_buttons & 0x10) {
        back_page = menu_system.current->back;
        if (back_page != 0) {
            menu_system.next = back_page;
        } else if (menu_system.close_locked == 0) {
            return -1;
        }
    }
    if ((mode_freeze_state != 0x10) && (mode_freeze_state != 1)) {
        menu_system.next = menu_system.current->back;
        return 0;
    }
    if (((memory_card_state.state < 3) &&
         (memory_card_state.pending_state < 0)) &&
        (memory_card_state.card[0].type == 2)) {
        if (menu->flags & 1) {
            buttons = controller_state.held_buttons;
        } else {
            buttons = controller_state.pressed_buttons;
        }
        menu->selected_save_slot = (s32)selected_save_slot;
        if ((buttons & 0x1000) && (selected_save_slot != 0)) {
            menu->selected_save_slot = (s32)(selected_save_slot - 1);
        }
        next_save_slot = menu->selected_save_slot;
        if ((buttons & 0x4000) && (next_save_slot < 4)) {
            menu->selected_save_slot = (s32)(next_save_slot + 1);
            next_save_slot = ((volatile struct LoadMenuDescriptor *)menu)->selected_save_slot;
        }
        selected_save_slot = next_save_slot;
        if (((buttons & 0x40) && (memory_card_state.card[0].type == 2)) &&
            ((*((s32 *)((((u8 *)&memory_card_state) + (next_save_slot * 0x1C)) + 0x20))) >= 0)) {
            allocate_voice_for_target_entry(0, 0x11, menu->sound_owner);
            memory_card_state.active_card = 0;
            memory_card_state.card[0].save_index = (s32)menu->selected_save_slot;
            if (memory_card_state.pending_state < 0) {
                memory_card_state.pending_card = 0;
                memory_card_state.pending_state = 0xD;
            }
            menu_system.card_op_pending = 1;
            menu_system.card_op_text = 0x4FB6;
        }
        if (menu->selected_save_slot != previous_save_slot) {
            allocate_voice_for_target_entry(1, 0x11, menu->sound_owner);
        }
    }
    return 0;
}
