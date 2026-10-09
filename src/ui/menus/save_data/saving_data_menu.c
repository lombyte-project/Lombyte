#include "types.h"
typedef struct __attribute__((packed)) {
    s64 v;
} Unaligned64;
extern Unaligned64 D_0015EE98[1];
#include "rnc/storage/memory_card/memory_card_state.h"
extern s32 D_0015ED84;
extern s32 D_0015ED98;
extern s32 D_0015EE20;
extern s32 D_0015EE24;
extern s32 D_0015EE34;
extern s32 mode_freeze_state __asm__("D_0015EEB0");
extern s32 mode_freeze_flags __asm__("D_0015EEB4");
extern u8 D_001D2578[];
extern s32 D_001D2640[];
#include "rnc/ui/menus/menu_system.h"
#include "rnc/input/pad_state.h"
#include "rnc/ui/menus/menu_screen.h"
extern void mode_freeze_init() __asm__("func_001FBAB8");
extern s32 prepare_save_game() __asm__("FUN_002269c0");
extern s32 allocate_voice_for_target_entry() __asm__("func_0022DA68");
s32 saving_data_menu(struct MenuScreen *menu) __asm__("FUN_00222f88");

s32 saving_data_menu(struct MenuScreen *menu) {
    u8 *new_var;
    struct MenuPage *temp_2_126;
    s32 prev_slot;
    s32 save_state;
    s32 slot;
    s32 selected_slot;
    s32 pad_buttons;
    if (menu_system.current->focus == menu) {
        if (((menu->data.save.step == 0) && (menu_system.previous == D_001D2578)) &&
            (menu_system.previous->confirmed != 0)) {
            menu->data.save.step = 1;
        }
        save_state = menu->data.save.step;
        if (save_state == 1) {
            prepare_save_game(menu->data.save.save_data, menu->data.save.slot);
            menu_system.card_op_pending = save_state;
            menu_system.card_op_text = 0x4FB5;
        }
        menu->data.save.step = 2;
        prev_slot = menu->data.save.slot;
        if (menu_system.card_op_pending != 0) {
            if ((memory_card_state.state < 3) && (memory_card_state.pending_state < 0)) {
                menu_system.card_op_pending = 0;
                if (memory_card_state.err != 0) {
                    mode_freeze_flags |= 0x80;
                    mode_freeze_init(3, menu_system.current);
                    goto block_46;
                }
                *((s32 *)((((u8 *)&memory_card_state) + (memory_card_state.card[0].save_index * 0x1C)) + 0x24)) =
                    (s32)D_0015ED98;
                *((s32 *)((((u8 *)&memory_card_state) + (memory_card_state.card[0].save_index * 0x1C)) + 0x20)) =
                    (s32)D_0015ED84;
                *((s32 *)((((u8 *)&memory_card_state) + (memory_card_state.card[0].save_index * 0x1C)) + 0x2C)) =
                    (s32)D_0015EE24;
                *((Unaligned64 *)((((u8 *)&memory_card_state) + (memory_card_state.card[0].save_index * 0x1C)) + 0x30)) =
                    D_0015EE98[0];
                *((s32 *)((((u8 *)&memory_card_state) + (memory_card_state.card[0].save_index * 0x1C)) + 0x28)) =
                    (s32)D_0015EE20;
                goto block_13;
            }
            return 0;
        }
    block_13:
        if (0xD00 & controller_state.pressed_unmasked) {
            if (menu_system.close_locked == 0) {
                return 1;
            }
            goto block_17;
        }

    block_17:
        if (controller_state.pressed_unmasked & 0x10) {
            temp_2_126 = menu_system.current->back;
            if (temp_2_126 != 0) {
                menu_system.next = temp_2_126;
                goto block_23;
            }
            if (menu_system.close_locked == 0) {
                return -1;
            }
            goto block_24;
        }

    block_23:
    block_24:
        if ((mode_freeze_state != 0x10) && (mode_freeze_state != 1)) {
            menu_system.next = (void *)menu_system.current->back;
            return 0;
        }

        if (((memory_card_state.state < 3) && (memory_card_state.pending_state < 0)) && (memory_card_state.card[0].type == 2)) {
            if (menu->data.save.flags & 1) {
                pad_buttons = controller_state.raw_pressed;
            } else {
                pad_buttons = controller_state.pressed;
            }
            menu->data.save.slot = (s32)D_0015EE34;
            if ((pad_buttons & 0x1000) && (D_0015EE34 != 0)) {
                menu->data.save.slot = (s32)(D_0015EE34 - 1);
            }
            if (pad_buttons & 0x4000) {
                slot = menu->data.save.slot;
                if (slot < 4) {
                    menu->data.save.slot = (s32)(slot + 1);
                }
            }
            selected_slot = menu->data.save.slot;
            D_0015EE34 = selected_slot;
            if ((pad_buttons & 0x40) && (memory_card_state.card[0].type == 2)) {
                new_var = (((u8 *)&memory_card_state) + (selected_slot * 0x1C)) + 0x20;
                if ((*((s32 *)new_var)) != (-1)) {
                    menu_system.unkD4 = 0;
                    menu_system.next = (new_var = D_001D2578);
                    D_001D2640[0] = menu->data.save.slot;
                } else {
                    menu->data.save.step = 1;
                }
            }
            if (menu->data.save.slot != prev_slot) {
                allocate_voice_for_target_entry(1, 0x11, menu->moby);
            }
        block_46:;

            ;
        }
        return 0;
    }
    return 0;
}

extern __typeof__(saving_data_menu) func_00222F88 __attribute__((alias("FUN_00222f88")));
