#include "types.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/ui/menus/menu_screen.h"
#include "rnc/input/pad_state.h"

extern s32 allocate_voice_for_target_entry() __asm__("func_0022DA68");

s32 update_menu_choice_list(struct MenuScreen *menu) __asm__("FUN_002212b8");

s32 update_menu_choice_list(struct MenuScreen *menu) {
    struct MenuChoice *selected_choice;
    s32               *selected_choice_options;
    s32               *opt;
    s32                previous_choice_index;
    s32                selected_choice_index;
    s32                option_count;

    if (menu_system.current->focus != menu) {
        return 0;
    }
    if ((controller_state.pressed_unmasked & (PAD_SELECT | PAD_R3 | PAD_START)) &&
        menu_system.close_locked == 0) {
        return 1;
    }
    if (controller_state.pressed_unmasked & PAD_TRIANGLE) {
        if (menu_system.current->back != 0) {
            menu_system.next = menu_system.current->back;
        } else if (menu_system.close_locked == 0) {
            return -1;
        }
    }
    previous_choice_index = menu->data.choices.selection;
    if ((controller_state.pressed_unmasked & PAD_UP) && previous_choice_index != 0) {
        menu->data.choices.selection = previous_choice_index - 1;
    }
    if (controller_state.pressed_unmasked & PAD_DOWN) {
        selected_choice_index = menu->data.choices.selection;
        if (menu->data.choices.list[selected_choice_index + 1].unk0 != 0) {
            menu->data.choices.selection = selected_choice_index + 1;
        }
    }
    if (previous_choice_index != menu->data.choices.selection) {
        allocate_voice_for_target_entry(1, 0x11, menu->moby);
    }
    selected_choice = &menu->data.choices.list[menu->data.choices.selection];
    selected_choice_options = selected_choice->option;
    option_count = 0;
    if (selected_choice_options[0] != 0) {
        opt = &selected_choice->option[0];
        do {
            option_count++;
            opt++;
            if (*opt == 0) {
                break;
            }
        } while (option_count < 4);
    }
    if (controller_state.pressed_unmasked & PAD_CROSS) {
        if (selected_choice->value != NULL) {
            *selected_choice->value = (*selected_choice->value + 1) % option_count;
            allocate_voice_for_target_entry(0, 0x11, menu->moby);
        }
    }
    return 0;
}
