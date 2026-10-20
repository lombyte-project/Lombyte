
#include "types.h"
#include "rnc/input/pad_state.h"
struct Menu {
    u8 pad0[0x34];
    s32 str;
    u8 pad38[4];
    s32 unk3C;
    s32 unk40;
    u8 pad44[0xC];
    u32 unk50;
    s32 unk54;
};
#include "rnc/ui/menus/menu_system.h"
#include "sda.h"
extern s32 D_001D2AF4[];
extern u8 D_001D5098[];
extern u8 D_001D5148[];
extern u8 D_001D5208[];
u8 D_001D5298[] NOT_SDA = "9Q";
extern u8 D_001D52B0[];
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
s32 update_timed_menu_sequence(struct Menu *menu) __asm__("FUN_002223f0");

s32 update_timed_menu_sequence(struct Menu *menu) {
    enum {
        INITIALIZE_MENU_SEQUENCE = 0,
        WAIT_FOR_INITIAL_DELAY = 1,
        ADVANCE_FIRST_PROGRESS_PHASE = 2,
        BEGIN_DELAY_AFTER_FIRST_PROGRESS_PHASE = 3,
        PREPARE_SECOND_PROGRESS_PHASE = 4,
        WAIT_BEFORE_SECOND_PROGRESS_PHASE = 5,
        ADVANCE_SECOND_PROGRESS_PHASE = 6,
        BEGIN_DELAY_AFTER_SECOND_PROGRESS_PHASE = 7,
        WAIT_BEFORE_THIRD_PROGRESS_PHASE = 8,
        ADVANCE_THIRD_PROGRESS_PHASE = 9,
        BEGIN_DELAY_AFTER_THIRD_PROGRESS_PHASE = 10,
        PREPARE_FOURTH_PROGRESS_PHASE = 11,
        WAIT_BEFORE_FOURTH_PROGRESS_PHASE = 12,
        ADVANCE_FOURTH_PROGRESS_PHASE = 13,
        BEGIN_DELAY_AFTER_FOURTH_PROGRESS_PHASE = 14,
        PREPARE_FIFTH_PROGRESS_PHASE = 15,
        WAIT_BEFORE_FIFTH_PROGRESS_PHASE = 16,
        ADVANCE_FIFTH_PROGRESS_PHASE = 17,
        BEGIN_FINAL_DELAY = 18,
        WAIT_FOR_FINAL_DELAY = 19
    };
    int flag;
    s32 previous_counter_value;

    flag = (((struct PadStateWords *)&controller_state)->buttons.held_pressed & 0xF) == 0xF;
    if ((menu_system.unkDC == 0) || (flag && (((s32)((struct PadStateWords *)&controller_state)->buttons.held_pressed) & 0x10))) {
        if ((controller_state.pressed_unmasked & (PAD_SELECT | PAD_R3 | PAD_START)) && (menu_system.close_locked == 0)) {
            return 1;
        }
        if (controller_state.pressed_unmasked & PAD_TRIANGLE) {
            if (menu_system.current->back != 0) {
                menu_system.next = menu_system.current->back;
            } else if (menu_system.close_locked == 0) {
                return -1;
            }
        }
    }
    previous_counter_value = menu->unk40;
    menu->unk40 = previous_counter_value - 1;
    if (flag) {
        menu->unk40 = previous_counter_value - 2;
    }
    if (menu->unk40 < 0) {
        menu->unk40 = 0;
    }
    switch (menu->unk50) {
    case INITIALIZE_MENU_SEQUENCE:
        menu->unk50 = menu->unk50 + 1;
        menu->unk40 = scale_game_frames(0xB4);
        menu->unk3C = 0;
        D_001D2AF4[0] = 0x50A9;
        menu->str = (s32)D_001D5098;
        break;

    case WAIT_FOR_INITIAL_DELAY:

    case WAIT_BEFORE_SECOND_PROGRESS_PHASE:

    case WAIT_BEFORE_THIRD_PROGRESS_PHASE:

    case WAIT_BEFORE_FOURTH_PROGRESS_PHASE:

    case WAIT_BEFORE_FIFTH_PROGRESS_PHASE:
        if (menu->unk40 == 0) {
            menu->unk54 = 0;
            menu->unk50 = *(volatile u32 *)(&menu->unk50) + 1;
        }
        break;

    case ADVANCE_FIRST_PROGRESS_PHASE:

    case ADVANCE_SECOND_PROGRESS_PHASE:

    case ADVANCE_THIRD_PROGRESS_PHASE:

    case ADVANCE_FOURTH_PROGRESS_PHASE:

    case ADVANCE_FIFTH_PROGRESS_PHASE:
        previous_counter_value = menu->unk3C;
        menu->unk3C = previous_counter_value + 0xA;
        if (flag) {
            menu->unk3C = previous_counter_value + 0x14;
        }
        if (menu->unk54 != 0) {
            menu->unk40 = scale_game_frames(0xB4);
            menu->unk50 = menu->unk50 + 1;
        }
        break;

    case PREPARE_SECOND_PROGRESS_PHASE:
        if (menu->unk40 == 0) {
            menu->unk50 = 5;
            menu->unk40 = scale_game_frames(0xB4);
            menu->unk3C = 0;
            D_001D2AF4[0] = 0x50D6;
            menu->str = (s32)D_001D5148;
        }
        break;

    case BEGIN_DELAY_AFTER_FIRST_PROGRESS_PHASE:
        if (menu->unk40 == 0) {
            D_001D2AF4[0] = 0x50D4;
            menu->str = 0x50D5;
            menu->unk40 = scale_game_frames(0xF0);
            menu->unk3C = 0;
            menu->unk50 = menu->unk50 + 1;
        }
        break;

    case BEGIN_DELAY_AFTER_SECOND_PROGRESS_PHASE:
        if (menu->unk40 == 0) {
            D_001D2AF4[0] = 0x5106;
            menu->str = (s32)D_001D5208;
            menu->unk40 = scale_game_frames(0xF0);
            menu->unk3C = 0;
            menu->unk50 = menu->unk50 + 1;
        }
        break;

    case BEGIN_DELAY_AFTER_THIRD_PROGRESS_PHASE:
        if (menu->unk40 == 0) {
            D_001D2AF4[0] = 0x5136;
            menu->str = 0x5137;
            menu->unk40 = scale_game_frames(0xF0);
            menu->unk3C = 0;
            menu->unk50 = menu->unk50 + 1;
        }
        break;

    case PREPARE_FOURTH_PROGRESS_PHASE:
        if (menu->unk40 == 0) {
            menu->unk40 = scale_game_frames(0xB4);
            menu->unk3C = 0;
            D_001D2AF4[0] = 0x5138;
            menu->str = (s32)D_001D5298;
            menu->unk50 = 0xC;
        }
        break;

    case BEGIN_DELAY_AFTER_FOURTH_PROGRESS_PHASE:
        if (menu->unk40 == 0) {
            D_001D2AF4[0] = 0;
            menu->str = 0x5143;
            menu->unk40 = scale_game_frames(0xF0);
            menu->unk3C = 0;
            menu->unk50 = menu->unk50 + 1;
        }
        break;

    case PREPARE_FIFTH_PROGRESS_PHASE:
        if (menu->unk40 == 0) {
            menu->unk40 = scale_game_frames(0xB4);
            menu->unk3C = 0;
            D_001D2AF4[0] = 0x5144;
            menu->str = (s32)D_001D52B0;
            menu->unk50 = 0x10;
        }
        break;

    case BEGIN_FINAL_DELAY:
        if (menu->unk40 == 0) {
            D_001D2AF4[0] = 0;
            menu->str = 0x5175;
            menu->unk50 = menu->unk50 + 1;
            menu->unk40 = scale_game_frames(0x12C);
        }
        break;

    case WAIT_FOR_FINAL_DELAY:
        if (menu->unk40 == 0) {
            if (menu_system.unkDC != 0) {
                return 1;
            }
        }
        break;
    }

    return 0;
}

extern __typeof__(update_timed_menu_sequence) func_002223F0 __attribute__((alias("FUN_002223f0")));
