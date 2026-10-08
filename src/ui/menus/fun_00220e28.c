#include "types.h"

struct PadState {
    u8 pad0[0x1C0];
    u32 held;
    u32 pressed;
};

struct MenuOpt {
    void *name;
    u8 *flag;
    u8 pad8[8];
    u32 type;
};

struct Menu {
    u8 pad0[0x14];
    s32 unk14;
    u8 pad18[0x1C];
    struct MenuOpt *opts;
    s32 sel;
    s32 timer;
};

struct ModeEntry {
    u8 pad0[0x38];
    s32 unk38;
    u8 pad3C[4];
    struct Menu *owner;
};


extern struct PadState D_0013C940;
#include "rnc/ui/menus/menu_system.h"
extern f32 D_0015F43C;
extern u8 D_0016034C;
extern void allocate_voice_for_target_entry(s32, s32, s32) __asm__("func_0022DA68");
extern void mode_freeze_init(s32, struct ModeEntry *) __asm__("func_001FBAB8");
extern void fade_to_black(s32) __asm__("func_001F4A58");
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");

s32 FUN_00220e28(struct Menu *m) {
    s32 active;
    s32 old;
    s32 t;
    s32 v;

    active = menu_system.current->owner == m;
    if (m->timer != 0) {
        t = m->timer - 1;
        v = t < 5 ? t : 4;
        m->timer = t;
        D_0015F43C = (f32)v * 0.25f;
        return 0;
    }
    if (!active) {
        return 0;
    }
    if ((D_0013C940.pressed & 0xD00) && menu_system.unk124 == 0) {
        return 1;
    }
    if (D_0013C940.pressed & 0x10) {
        if (menu_system.current->unk38 != 0) {
            menu_system.next = menu_system.current->unk38;
        } else if (menu_system.unk124 == 0) {
            return -1;
        }
    }
    old = m->sel;
    if ((D_0013C940.pressed & 0x1000) && old != 0) {
        m->sel = old - 1;
    }
    if ((D_0013C940.pressed & 0x4000) && m->opts[m->sel + 1].name != 0) {
        m->sel = m->sel + 1;
    }
    if (D_0013C940.pressed & 0x40) {
        allocate_voice_for_target_entry(0, 0x11, m->unk14);
        if (m->opts[m->sel].type & 1) {
            if (D_0016034C != 0) {
                mode_freeze_init(6, menu_system.current);
            } else {
                fade_to_black(4);
                m->timer = scale_game_frames(0x10);
                D_0016034C = D_0016034C == 0;
            }
        } else if (m->opts[m->sel].flag != 0) {
            *m->opts[m->sel].flag = *m->opts[m->sel].flag == 0;
        }
    }
    if (m->sel != old) {
        allocate_voice_for_target_entry(1, 0x11, m->unk14);
    }
    return 0;
}

extern __typeof__(FUN_00220e28) func_00220E28 __attribute__((alias("FUN_00220e28")));
