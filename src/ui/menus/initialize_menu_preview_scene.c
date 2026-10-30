#include "types.h"
#include "rnc/ui/menus/panel_slots.h"
#include "sda.h"
#include "qcopy.h"
#include "qzero.h"
#include "rnc/globals.h"
#include "rnc/data_labels.h"
#include "rnc/game/game_globals.h"
#include "rnc/rendering/render_globals.h"

struct P {
    u8 pad_0[0x10];
    u8 f10;
};

struct T {
    u8        pad_0[0x48];
    struct P *tbl[1];
};

struct O2 {
    u8        pad_0[0x10];
    float     f10, f14, f18;
    u8        pad_1C[8];
    struct T *f24;
    u8        pad_28[0xC];
    u16       f34;
    u8        pad_36[0xA];
    s32       f40, f44, f48;
    u8        pad_4C[0x28];
    s32      *f74;
};

#include "rnc/ui/menus/menu_system.h"

extern struct MenuPage
          menu_preview_initial_page __asm__("D_001D45C8"); /* page made current and next at init */
extern u8 D_0019C150[];
extern u8 D_0019C160[];
extern void quad_moby_update() __asm__("FUN_0023a2c0");

#include "rnc/ui/menus/menu_globals.h"
#include "rnc/ui/menus/initialize_menu_preview_scene_globals.h"
#include "rnc/math/fast_math.h"

extern u8         menu_preview_lighting_scale_source_gp __asm__("D_001601C0") __attribute__((sda));
extern u8         menu_preview_lighting_copy_source_gp __asm__("D_001601D0") __attribute__((sda));
extern void       vu1_sync_chain(s32) __asm__("func_002337B0");
extern s32        sceGsSyncV(s32);
extern s32        initialize_menu_stream_buffers() __asm__("FUN_00225ac0");
extern void       vu1_init_chain(void) __asm__("func_002335D0");
extern struct O2 *create_menu_preview_moby() __asm__("func_00225490");
extern s32        set_moby_animation() __asm__("func_00212ED8");

void func_00225AC0__void() __asm__("FUN_00225ac0");
void initialize_menu_preview_scene(void) __asm__("FUN_00218f98");

void initialize_menu_preview_scene(void) {
    s32 i;
    s32 preview_workspace_start, graphics_workspace_base, help_text_buffer_start,
        graphics_buffer_start;
    menu_system.state = 2;
    menu_system.current = &menu_preview_initial_page;
    menu_system.next = &menu_preview_initial_page;
    menu_system.close_locked = 0;
    fast_vec_scale((s32)D_0019C150, (s32)&menu_preview_lighting_scale_source_gp, 1.0f);
    qcopy(D_0019C150 - 0x10, &menu_preview_lighting_copy_source_gp);
    qzero(D_0019C150 + 0x20);
    qzero(D_0019C160);
    vu1_sync_chain(1);
    sceGsSyncV(0);
    game_frame_counter++;
    preview_workspace_start = D_001940C0[1] + 0xA0000;
    graphics_workspace_base = D_001940C0[2] + 0xA0000;
    help_text_buffer_start = preview_workspace_start + 0x3C000;
    graphics_buffer_start = graphics_workspace_base + 0xE0000;
    menu_system.unkFC = help_text_buffer_start + 0xC1000;
    menu_system.unk100 = graphics_buffer_start + 0x11800;
    current_vu_chain = 0xA0000;
    menu_system.unk104 = preview_workspace_start;
    menu_system.unk10 = graphics_workspace_base;
    menu_system.help_text_buffer = help_text_buffer_start;
    menu_system.unk10C = graphics_buffer_start;
    func_00225AC0__void(1);
    vu1_init_chain();
    menu_system.saved_texture_start = gs_texture_allocation_start;
    if (menu_system.current != 0) {
        for (i = 0; i < 14; i++) {
            struct O2 *preview_moby = (struct O2 *)create_menu_preview_moby(0x472);
            panel_slots[i] = preview_moby;
            if (preview_moby != 0) {
                s32 k;
                preview_moby->f34 &= 0xFFFD;
                ((struct O2 *)panel_slots[i])->f74 = quad_moby_update;
                ((struct O2 *)panel_slots[i])->f10 = preview_camera.position.f[0];
                ((struct O2 *)panel_slots[i])->f14 = preview_camera.position.f[1];
                ((struct O2 *)panel_slots[i])->f18 = preview_camera.position.f[2];
                ((struct O2 *)panel_slots[i])->f40 = 0;
                ((struct O2 *)panel_slots[i])->f44 = 0;
                ((struct O2 *)panel_slots[i])->f48 = 0;
                k = menu_system.current->moby_anims[i];
                set_moby_animation(
                    panel_slots[i], k, ((struct O2 *)panel_slots[i])->f24->tbl[k]->f10 - 1);
            }
        }
    }
}

extern __typeof__(initialize_menu_preview_scene) func_00218F98
    __attribute__((alias("FUN_00218f98")));
