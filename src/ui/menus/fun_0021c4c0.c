#include "types.h"
#include "rnc/ui/map/map_state.h"
#include "rnc/globals.h"

extern struct PadState D_0013C940;
#include "rnc/ui/menus/menu_system.h"
#include "rnc/ui/menus/menu_screen.h"
#include "rnc/input/pad_state.h"
extern u8 D_0013DD40[];
extern u32 D_001CF874[];
extern u32 D_001CF758[];
extern void allocate_voice_for_target_entry(s32, s32, s32) __asm__("func_0022DA68");
extern void update_mission_list(void) __asm__("func_0020B950");
extern s32 collect_mission_ids(void *, s32, void *, s32) __asm__("func_0020BC00");

s32 FUN_0021c4c0(struct MenuScreen *m) {
    s32 old;
    s32 i;
    s32 prev;
    u32 pad;
    s32 *p;
    s32 *ch;

    {
        struct MenuScreen *owner = menu_system.current->focus;
        old = D_001A00F0.level;
        if (owner != m) {
            if (old < 20) {
                m->data.missions.choice[old] = -1;
            }
            return 0;
        }
    }
    if ((D_0013C940.pressed_unmasked & 0xD00) && menu_system.close_locked == 0) {
        return 1;
    }
    if (D_0013C940.pressed_unmasked & 8) {
        for (i = D_001A00F0.level + 1; i < 20; i++) {
            if (D_0013DD40[i] != 0 || current_level_index == i) {
                D_001A00F0.level = i;
                break;
            }
        }
    }
    if (D_0013C940.pressed_unmasked & 4) {
        for (i = D_001A00F0.level - 1; i >= 0; i--) {
            if (D_0013DD40[i] != 0 || current_level_index == i) {
                D_001A00F0.level = i;
                break;
            }
        }
    }
    if (D_001A00F0.level != old) {
        allocate_voice_for_target_entry(1, 0x11, m->moby);
        update_mission_list();
    }
    if (m->data.missions.count != 0) {
        pad = D_0013C940.pressed_unmasked;
        ch = m->data.missions.choice;
        prev = m->data.missions.choice[D_001A00F0.level];
        if (pad & 0x1000) {
            m->data.missions.choice[D_001A00F0.level] = (prev + m->data.missions.count - 1) % m->data.missions.count;
        }
        if (pad & 0x4000) {
            m->data.missions.choice[D_001A00F0.level] = (m->data.missions.choice[D_001A00F0.level] + 1) % m->data.missions.count;
        }
        if (m->data.missions.choice[D_001A00F0.level] != prev) {
            allocate_voice_for_target_entry(1, 0x11, m->moby);
        }
        if ((pad & 0x5000) || D_001A00F0.level != old) {
            m->data.missions.count = collect_mission_ids((void *)0x70000000, 0, (void *)0x70000100, 1);
            p = &ch[D_001A00F0.level];
            D_001CF874[0] = ((u32 *)0x70000000)[*p];
            D_001CF758[0] = ((u32 *)0x70000100)[*p];
        }
    }
    if (D_0013C940.pressed_unmasked & 0x10) {
        if (menu_system.current->back != 0) {
            menu_system.next = menu_system.current->back;
        } else if (menu_system.close_locked == 0) {
            return -1;
        }
    }
    return 0;
}

extern __typeof__(FUN_0021c4c0) func_0021C4C0 __attribute__((alias("FUN_0021c4c0")));
