#include "types.h"
#include "rnc/input/pad_state.h"

struct Menu {
    u8  pad0[0x34];
    s32 str;
    u8  pad38[4];
    s32 unk3C;
    s32 unk40;
    u8  pad44[0xC];
    u32 unk50;
    s32 unk54;
};

#include "rnc/ui/menus/menu_system.h"
#include "sda.h"
#include "rnc/math/fast_math.h"

extern s32 D_001D2AF4[];

/* The text ids each progress phase shows (menu->str), each list ending in -1. */
s32 timed_menu_first_progress_string_ids[43] __asm__("D_001D5098") = {
    0x50AA, 0x50AB, 0x50AC, 0x50AD, 0x50AE, 0x50AF, 0x50B0, 0x50B1, 0x50B2, 0x50B3, 0x50B4,
    0x50B5, 0x50B6, 0x50B7, 0x50B8, 0x50B9, 0x50BA, 0x50BB, 0x50BC, 0x50BD, 0x50BE, 0x50BF,
    0x50C0, 0x50C1, 0x50C2, 0x50C3, 0x50C4, 0x50C5, 0x50C6, 0x50C7, 0x50C8, 0x50C9, 0x50CA,
    0x50CB, 0x50CC, 0x50CD, 0x50CE, 0x50CF, 0x50D0, 0x50D1, 0x50D2, 0x50D3, -1,
};

s32 timed_menu_second_progress_string_ids[48] __asm__("D_001D5148") = {
    0x50D7, 0x50D8, 0x50D9, 0x50DA, 0x50DB, 0x50DC, 0x50DD, 0x50DE, 0x50DF, 0x50E0, 0x50E1, 0x50E2,
    0x50E3, 0x50E4, 0x50E5, 0x50E6, 0x50E7, 0x50E8, 0x50E9, 0x50EA, 0x50EB, 0x50EC, 0x50ED, 0x50EE,
    0x50EF, 0x50F0, 0x50F1, 0x50F2, 0x50F3, 0x50F4, 0x50F5, 0x50F6, 0x50F7, 0x50F8, 0x50F9, 0x50FA,
    0x50FB, 0x50FC, 0x50FD, 0x50FE, 0x50FF, 0x5100, 0x5101, 0x5102, 0x5103, 0x5104, 0x5105, -1,
};

s32 timed_menu_third_progress_string_ids[36] __asm__("D_001D5208") = {
    0x5107, 0x5108, 0x5109, 0x510A, 0x510B, 0x510C, 0x510D, 0x510E, 0x510F, 0x5110, 0x5111, 0x5112,
    0x5113, 0x5114, 0x5115, 0x5116, 0x5117, 0x5118, 0x5119, 0x511A, 0x511B, 0x511C, 0x511D, 0x511E,
    0x511F, 0x5120, 0x5121, 0x5122, 0x5123, 0x5124, 0x5125, 0x5126, 0x5127, 0x5128, 0x5129, -1,
};

s32 timed_menu_fourth_progress_string_ids[6] __asm__("D_001D5298") = {
    0x5139,
    0x513A,
    0x513B,
    0x513C,
    0x513D,
    -1,
};

s32 timed_menu_fifth_progress_string_ids[13] __asm__("D_001D52B0") = {
    0x5145,
    0x5146,
    0x5147,
    0x5148,
    0x5149,
    0x514A,
    0x514B,
    0x514C,
    0x514D,
    0x514E,
    0x514F,
    0x5150,
    -1,
};

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
    if ((menu_system.unkDC == 0) ||
        (flag &&
         (((s32)((struct PadStateWords *)&controller_state)->buttons.held_pressed) & 0x10))) {
        if ((controller_state.pressed_unmasked & (PAD_SELECT | PAD_R3 | PAD_START)) &&
            (menu_system.close_locked == 0)) {
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
        menu->str = (s32)timed_menu_first_progress_string_ids;
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
            menu->str = (s32)timed_menu_second_progress_string_ids;
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
            menu->str = (s32)timed_menu_third_progress_string_ids;
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
            menu->str = (s32)timed_menu_fourth_progress_string_ids;
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
            menu->str = (s32)timed_menu_fifth_progress_string_ids;
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
