#include "types.h"
#include "rnc/ui/map/map_state.h"
#include "rnc/globals.h"

#include "rnc/ui/menus/menu_system.h"
#include "rnc/ui/menus/menu_screen.h"
#include "rnc/input/pad_state.h"
#include "rnc/gameplay/state/level_state.h"
extern s32 D_001CF874[];
extern s32 D_001CF758[];
extern void allocate_voice_for_target_entry(s32, s32, s32) __asm__("func_0022DA68");
extern void update_mission_list(void) __asm__("func_0020B950");
extern s32 collect_mission_ids(void *, s32, void *, s32) __asm__("func_0020BC00");

s32 update_menu_mission_list(struct MenuScreen *m) __asm__("FUN_0021c4c0");

s32 update_menu_mission_list(struct MenuScreen *menu_screen) {
    s32 old;
    s32 i;
    s32 prev;
    u32 pad;
    s32 *mission_choice_ptr;
    s32 *ch;

    {
        struct MenuScreen *owner = menu_system.current->focus;
        old = level_map_selection.level;
        if (owner != menu_screen) {
            if (old < 20) {
                menu_screen->data.missions.choice[old] = -1;
            }
            return 0;
        }
    }
    if ((controller_state.pressed_unmasked & (PAD_SELECT | PAD_R3 | PAD_START)) && menu_system.close_locked == 0) {
        return 1;
    }
    if (controller_state.pressed_unmasked & 8) {
        for (i = level_map_selection.level + 1; i < 20; i++) {
            if (level_available[i] != 0 || current_level_index == i) {
                level_map_selection.level = i;
                break;
            }
        }
    }
    if (controller_state.pressed_unmasked & 4) {
        for (i = level_map_selection.level - 1; i >= 0; i--) {
            if (level_available[i] != 0 || current_level_index == i) {
                level_map_selection.level = i;
                break;
            }
        }
    }
    if (level_map_selection.level != old) {
        allocate_voice_for_target_entry(1, 0x11, menu_screen->moby);
        update_mission_list();
    }
    if (menu_screen->data.missions.count != 0) {
        pad = controller_state.pressed_unmasked;
        ch = menu_screen->data.missions.choice;
        prev = menu_screen->data.missions.choice[level_map_selection.level];
        if (pad & 0x1000) {
            menu_screen->data.missions.choice[level_map_selection.level] = (prev + menu_screen->data.missions.count - 1) % menu_screen->data.missions.count;
        }
        if (pad & 0x4000) {
            menu_screen->data.missions.choice[level_map_selection.level] = (menu_screen->data.missions.choice[level_map_selection.level] + 1) % menu_screen->data.missions.count;
        }
        if (menu_screen->data.missions.choice[level_map_selection.level] != prev) {
            allocate_voice_for_target_entry(1, 0x11, menu_screen->moby);
        }
        if ((pad & 0x5000) || level_map_selection.level != old) {
            menu_screen->data.missions.count = collect_mission_ids((void *)0x70000000, 0, (void *)0x70000100, 1);
            mission_choice_ptr = &ch[level_map_selection.level];
            D_001CF874[0] = ((u32 *)0x70000000)[*mission_choice_ptr];
            D_001CF758[0] = ((u32 *)0x70000100)[*mission_choice_ptr];
        }
    }
    if (controller_state.pressed_unmasked & PAD_TRIANGLE) {
        if (menu_system.current->back != 0) {
            menu_system.next = menu_system.current->back;
        } else if (menu_system.close_locked == 0) {
            return -1;
        }
    }
    return 0;
}

extern __typeof__(update_menu_mission_list) func_0021C4C0 __attribute__((alias("FUN_0021c4c0")));
