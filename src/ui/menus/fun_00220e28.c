#include "types.h"

extern struct PadState D_0013C940;
#include "rnc/ui/menus/menu_system.h"
#include "rnc/ui/menus/menu_screen.h"
#include "rnc/input/pad_state.h"
extern f32 D_0015F43C;
extern u8 D_0016034C;
extern void allocate_voice_for_target_entry(s32, s32, s32) __asm__("func_0022DA68");
extern void mode_freeze_init(s32, struct MenuPage *) __asm__("func_001FBAB8");
extern void fade_to_black(s32) __asm__("func_001F4A58");
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");

s32 FUN_00220e28(struct MenuScreen *m) {
    s32 active;
    s32 old;
    s32 t;
    s32 v;

    active = menu_system.current->focus == m;
    if (m->data.options.fade_timer != 0) {
        t = m->data.options.fade_timer - 1;
        v = t < 5 ? t : 4;
        m->data.options.fade_timer = t;
        D_0015F43C = (f32)v * 0.25f;
        return 0;
    }
    if (!active) {
        return 0;
    }
    if ((D_0013C940.pressed_unmasked & 0xD00) && menu_system.close_locked == 0) {
        return 1;
    }
    if (D_0013C940.pressed_unmasked & 0x10) {
        if (menu_system.current->back != 0) {
            menu_system.next = menu_system.current->back;
        } else if (menu_system.close_locked == 0) {
            return -1;
        }
    }
    old = m->data.options.selection;
    if ((D_0013C940.pressed_unmasked & 0x1000) && old != 0) {
        m->data.options.selection = old - 1;
    }
    if ((D_0013C940.pressed_unmasked & 0x4000) && m->data.options.list[m->data.options.selection + 1].unk0 != 0) {
        m->data.options.selection = m->data.options.selection + 1;
    }
    if (D_0013C940.pressed_unmasked & 0x40) {
        allocate_voice_for_target_entry(0, 0x11, m->moby);
        if (m->data.options.list[m->data.options.selection].flags & 1) {
            if (D_0016034C != 0) {
                mode_freeze_init(6, menu_system.current);
            } else {
                fade_to_black(4);
                m->data.options.fade_timer = scale_game_frames(0x10);
                D_0016034C = D_0016034C == 0;
            }
        } else if (m->data.options.list[m->data.options.selection].flag != 0) {
            *m->data.options.list[m->data.options.selection].flag = *m->data.options.list[m->data.options.selection].flag == 0;
        }
    }
    if (m->data.options.selection != old) {
        allocate_voice_for_target_entry(1, 0x11, m->moby);
    }
    return 0;
}

extern __typeof__(FUN_00220e28) func_00220E28 __attribute__((alias("FUN_00220e28")));
