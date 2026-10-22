#include "types.h"

#include "rnc/ui/menus/menu_system.h"
#include "rnc/ui/menus/menu_screen.h"
#include "rnc/input/pad_state.h"
#include "rnc/data_labels.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/gameplay/state/game_globals.h"
extern void allocate_voice_for_target_entry(s32, s32, s32) __asm__("func_0022DA68");
extern void mode_freeze_init(s32, struct MenuPage *) __asm__("func_001FBAB8");
extern void fade_to_black(s32) __asm__("func_001F4A58");
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");

s32 update_menu_option_list(struct MenuScreen *m) __asm__("FUN_00220e28");

s32 update_menu_option_list(struct MenuScreen *menu_screen) {
    s32 active;
    s32 old;
    s32 remaining_fade_frames;
    s32 capped_fade_frames;

    active = menu_system.current->focus == menu_screen;
    if (menu_screen->data.options.fade_timer != 0) {
        remaining_fade_frames = menu_screen->data.options.fade_timer - 1;
        capped_fade_frames = remaining_fade_frames < 5 ? remaining_fade_frames : 4;
        menu_screen->data.options.fade_timer = remaining_fade_frames;
        sequence = (f32)capped_fade_frames * 0.25f;
        return 0;
    }
    if (!active) {
        return 0;
    }
    if ((controller_state.pressed_unmasked & (PAD_SELECT | PAD_R3 | PAD_START)) && menu_system.close_locked == 0) {
        return 1;
    }
    if (controller_state.pressed_unmasked & PAD_TRIANGLE) {
        if (menu_system.current->back != 0) {
            menu_system.next = menu_system.current->back;
        } else if (menu_system.close_locked == 0) {
            return -1;
        }
    }
    old = menu_screen->data.options.selection;
    if ((controller_state.pressed_unmasked & PAD_UP) && old != 0) {
        menu_screen->data.options.selection = old - 1;
    }
    if ((controller_state.pressed_unmasked & PAD_DOWN) && menu_screen->data.options.list[menu_screen->data.options.selection + 1].unk0 != 0) {
        menu_screen->data.options.selection = menu_screen->data.options.selection + 1;
    }
    if (controller_state.pressed_unmasked & PAD_CROSS) {
        allocate_voice_for_target_entry(0, 0x11, menu_screen->moby);
        if (menu_screen->data.options.list[menu_screen->data.options.selection].flags & 1) {
            if (requested_video_mode != 0) {
                mode_freeze_init(6, menu_system.current);
            } else {
                fade_to_black(4);
                menu_screen->data.options.fade_timer = scale_game_frames(0x10);
                requested_video_mode = requested_video_mode == 0;
            }
        } else if (menu_screen->data.options.list[menu_screen->data.options.selection].flag != 0) {
            *menu_screen->data.options.list[menu_screen->data.options.selection].flag = *menu_screen->data.options.list[menu_screen->data.options.selection].flag == 0;
        }
    }
    if (menu_screen->data.options.selection != old) {
        allocate_voice_for_target_entry(1, 0x11, menu_screen->moby);
    }
    return 0;
}

extern __typeof__(update_menu_option_list) func_00220E28 __attribute__((alias("FUN_00220e28")));
