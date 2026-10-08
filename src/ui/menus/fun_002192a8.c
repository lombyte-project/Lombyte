#include "types.h"
#include "rnc/globals.h"

#include "rnc/ui/menus/menu_system.h"
#include "rnc/ui/menus/menu_screen.h"

struct Ent3 {
    u8 pad0[0x10];
    u8 unk10;
};

struct Ent2 {
    u8 pad0[0x48];
    struct Ent3 *unk48[1];
};

struct Ent {
    u8 pad0[0x24];
    struct Ent2 *unk24;
    u8 pad28[0x30];
    f32 unk58;
};

extern s16 D_001516D8[];
extern s32 D_0015F5B8;
extern s32 D_0015F618;
extern struct Ent *D_001D5D90[];

extern void func_00218D10(void);
extern void func_00218F98(void) __asm__("FUN_00218f98");
extern void update_visible_resident_objects() __asm__("FUN_00212e28");
extern s32 allocate_voice_for_target_entry(s32, s32, struct Ent *) __asm__("func_0022DA68");
extern void mode_freeze_init(s32, struct MenuPage *) __asm__("func_001FBAB8");
extern void CalculateDmaTransferAddress(void);
extern void update_fog(void) __asm__("FUN_001f2588");
extern void set_moby_animation(struct Ent *, s32, s32) __asm__("FUN_00212ed8");
extern void func_002191B8(void) __asm__("FUN_002191b8");

void FUN_002192a8(void) {
    s32 i;
    s32 j;
    s32 k;
    s32 flag;
    struct MenuScreen *obj;
    struct MenuScreen **objs;
    struct MenuScreen *m;

    menu_system.update_count = menu_system.update_count + 1;
    func_00218D10();
    if (menu_system.state == 0x14) {
        if (menu_system.timer != 0) {
            menu_system.timer = menu_system.timer - 1;
            if (menu_system.timer != 0) {
                return;
            }
        }
        if (D_001516D8[0] != 0) {
            return;
        }
        D_0015F5B8 = 0x1E000;
        CalculateDmaTransferAddress();
        update_fog();
        D_0015F618 = 1;
        menu_system.help_text_buffer = 0;
        menu_system.unk10C = 0;
        menu_system.unk104 = 0;
        menu_system.unk10 = 0;
        game_mode = 0;
        return;
    }
    if (menu_system.state == 0 || menu_system.state == 0x2D) {
        func_00218F98();
    }
    if (mode_freeze_flags & 1) {
        mode_freeze_init(3, menu_system.current);
        return;
    }
    if (menu_system.state == 1) {
        menu_system.timer = (menu_system.timer < 1) ? 0 : menu_system.timer - 1;
        if (menu_system.timer == 0) {
            struct MenuPage *o8 = menu_system.next;
            menu_system.next = 0;
            menu_system.current = o8;
            menu_system.state = o8->state;
            for (i = 0; i < 14; i++) {
                obj = menu_system.current->screens[i];
                if (obj != 0 && obj->enter != 0) {
                    obj->enter(obj, 0);
                }
            }
        }
    } else if (menu_system.next != 0) {
        if (menu_system.current == menu_system.next) {
            allocate_voice_for_target_entry(3, 0x11, D_001D5D90[0]);
        } else {
            allocate_voice_for_target_entry(4, 0x11, D_001D5D90[0]);
        }
        for (i = 0; i < 14; i++) {
            obj = menu_system.current->screens[i];
            if (obj != 0 && obj->leave != 0) {
                obj->leave(obj, 0);
            }
        }
        flag = (menu_system.next == menu_system.current->back);
        if (menu_system.current == menu_system.next) {
            flag = flag ^ 1;
        }
        for (j = 0; j < 14; j++) {
            if (menu_system.next->screens[j] != 0) {
                menu_system.next->screens[j]->moby = (s32)D_001D5D90[j];
            }
            if (flag) {
                s32 n = menu_system.current->moby_anims[j];
                set_moby_animation(D_001D5D90[j], n, D_001D5D90[j]->unk24->unk48[n]->unk10 - 1);
                D_001D5D90[j]->unk58 = -1.0f;
            } else {
                set_moby_animation(D_001D5D90[j], menu_system.next->moby_anims[j], 0);
                D_001D5D90[j]->unk58 = 1.0f;
            }
        }
        menu_system.state = 1;
        menu_system.timer = 12;
        menu_system.previous = menu_system.current;
        menu_system.current = 0;
    }
    if (menu_system.current != 0) {
        objs = menu_system.current->screens;
        for (k = 0; k < 14; k++) {
            if (objs != 0 && objs[k] != 0 && objs[k]->update != 0) {
                if (objs[k]->update(objs[k]) != 0) {
                    menu_system.close_request = 1;
                }
            }
        }
        if (menu_system.current != 0 && menu_system.current->pending_focus != 0) {
            m = menu_system.current->focus;
            if (m->leave != 0) {
                m->leave(m, 1);
            }
            menu_system.current->focus = menu_system.current->pending_focus;
            menu_system.current->pending_focus = 0;
            m = menu_system.current->focus;
            if (m->enter != 0) {
                m->enter(m, 1);
            }
        }
    }
    update_visible_resident_objects();
    if (menu_system.close_request != 0) {
        func_002191B8();
    }
}

extern __typeof__(FUN_002192a8) func_002192A8 __attribute__((alias("FUN_002192a8")));
