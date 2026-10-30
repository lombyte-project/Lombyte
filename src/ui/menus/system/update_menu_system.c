#include "types.h"
#include "rnc/ui/menus/panel_slots.h"
#include "rnc/globals.h"

#include "rnc/ui/menus/menu_system.h"
#include "rnc/ui/menus/menu_screen.h"
#include "rnc/data_labels.h"
#include "rnc/game/game_globals.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/mobys/animation/set_moby_animation.h"
#include "rnc/rendering/effects/update_fog.h"
#include "rnc/rendering/vu1/select_level_vu_chain.h"

struct Ent3 {
    u8 pad0[0x10];
    u8 unk10;
};

struct Ent2 {
    u8           pad0[0x48];
    struct Ent3 *unk48[1];
};

struct Ent {
    u8           pad0[0x24];
    struct Ent2 *unk24;
    u8           pad28[0x30];
    f32          unk58;
};

extern void reset_active_camera_position_and_basis(void) __asm__("func_00218D10");
extern void initialize_menu_preview_scene(void) __asm__("FUN_00218f98");
extern void update_visible_mobys() __asm__("FUN_00212e28");
extern void mode_freeze_init(s32, struct MenuPage *) __asm__("func_001FBAB8");
extern void close_menu_system(void) __asm__("FUN_002191b8");

void update_menu_system(void) __asm__("FUN_002192a8");

void update_menu_system(void) {
    s32                 i;
    s32                 j;
    s32                 k;
    s32                 flag;
    struct MenuScreen  *current_page_screen;
    struct MenuScreen **objs;
    struct MenuScreen  *focused_screen;

    menu_system.update_count = menu_system.update_count + 1;
    reset_active_camera_position_and_basis();
    if (menu_system.state == 0x14) {
        if (menu_system.timer != 0) {
            menu_system.timer = menu_system.timer - 1;
            if (menu_system.timer != 0) {
                return;
            }
        }
        if (cd_read_active[0] != 0) {
            return;
        }
        D_0015F5B8 = 0x1E000;
        select_level_vu_chain();
        update_fog();
        space_load_in_progress = 1;
        menu_system.help_text_buffer = 0;
        menu_system.unk10C = 0;
        menu_system.unk104 = 0;
        menu_system.unk10 = 0;
        game_mode = 0;
        return;
    }
    if (menu_system.state == 0 || menu_system.state == 0x2D) {
        initialize_menu_preview_scene();
    }
    if (mode_freeze_flags & 1) {
        mode_freeze_init(3, menu_system.current);
        return;
    }
    if (menu_system.state == 1) {
        menu_system.timer = (menu_system.timer < 1) ? 0 : menu_system.timer - 1;
        if (menu_system.timer == 0) {
            struct MenuPage *incoming_page = menu_system.next;
            menu_system.next = 0;
            menu_system.current = incoming_page;
            menu_system.state = incoming_page->state;
            for (i = 0; i < 14; i++) {
                current_page_screen = menu_system.current->screens[i];
                if (current_page_screen != 0 && current_page_screen->enter != 0) {
                    current_page_screen->enter(current_page_screen, 0);
                }
            }
        }
    } else if (menu_system.next != 0) {
        if (menu_system.current == menu_system.next) {
            allocate_voice_for_target_entry(3, 0x11, panel_slots[0]);
        } else {
            allocate_voice_for_target_entry(4, 0x11, panel_slots[0]);
        }
        for (i = 0; i < 14; i++) {
            current_page_screen = menu_system.current->screens[i];
            if (current_page_screen != 0 && current_page_screen->leave != 0) {
                current_page_screen->leave(current_page_screen, 0);
            }
        }
        flag = (menu_system.next == menu_system.current->back);
        if (menu_system.current == menu_system.next) {
            flag = flag ^ 1;
        }
        for (j = 0; j < 14; j++) {
            if (menu_system.next->screens[j] != 0) {
                menu_system.next->screens[j]->moby = (s32)panel_slots[j];
            }
            if (flag) {
                s32 current_animation_index = menu_system.current->moby_anims[j];
                set_moby_animation(
                    panel_slots[j],
                    current_animation_index,
                    ((struct Ent *)panel_slots[j])->unk24->unk48[current_animation_index]->unk10 -
                        1);
                ((struct Ent *)panel_slots[j])->unk58 = -1.0f;
            } else {
                set_moby_animation(panel_slots[j], menu_system.next->moby_anims[j], 0);
                ((struct Ent *)panel_slots[j])->unk58 = 1.0f;
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
            focused_screen = menu_system.current->focus;
            if (focused_screen->leave != 0) {
                focused_screen->leave(focused_screen, 1);
            }
            menu_system.current->focus = menu_system.current->pending_focus;
            menu_system.current->pending_focus = 0;
            focused_screen = menu_system.current->focus;
            if (focused_screen->enter != 0) {
                focused_screen->enter(focused_screen, 1);
            }
        }
    }
    update_visible_mobys();
    if (menu_system.close_request != 0) {
        close_menu_system();
    }
}

extern __typeof__(update_menu_system) func_002192A8 __attribute__((alias("FUN_002192a8")));

/* Zero at load; initialize_menu_preview_scene fills it when the menu opens. */
struct MenuSystem menu_system = {0};
