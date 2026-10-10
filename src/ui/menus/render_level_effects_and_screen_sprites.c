#include "types.h"
#include "eetypes.h"
#include "qcopy.h"
#include "rnc/math/vector.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/ui/menus/panel_slots.h"

/* Retail 0x002196b8..0x00219d77: draws the level-effect moby lists, then
   projects each enabled menu panel slot to the screen, renders panel
   contents into a power-of-two texture page and blits it back, and finally
   draws the flashing panel overlays.

   Slot 6 is skipped in every loop while menu_system.unkD8 is clear. */

typedef struct {
    u8 pad0[0x50];
    s32 screen_x;         /* 0x50 */
    s32 screen_y;         /* 0x54 */
    s32 projected_width;  /* 0x58 */
    s32 projected_height; /* 0x5C */
} ProjectedPanelFrame;

typedef struct {
    u8 pad0[0x78];
    ProjectedPanelFrame *frame; /* 0x78 */
} PanelRenderSlot;

typedef struct RenderPanel RenderPanel;
struct RenderPanel {
    u8 pad0[4];
    s32 (*draw)(RenderPanel *); /* 0x04: bit 0 = skip blit, 2/8/4/0x10 = fit mode */
    u8 pad8[8];
    s32 flags; /* 0x10: 1 = draw directly, 2 = first pass, 4 = hidden */
    u8 pad14[4];
    s32 screen_x;         /* 0x18 */
    s32 screen_y;         /* 0x1C */
    s32 projected_width;  /* 0x20 */
    s32 projected_height; /* 0x24 */
};

extern s64 capture_texture_tex0 __asm__("D_0015EED0");
extern void *resident_object_pool __asm__("D_0015FF18");
extern s32 panel_clear_color __asm__("D_001601B0") __attribute__((sda));

extern void vu1_add_g_sregister(s32, s64) __asm__("FUN_00233980");
extern void draw_mobys_setup(void) __asm__("FUN_0020d278");
extern void init_moby_class_dists(void) __asm__("FUN_0020d1f0");
extern void stash_moby_class_dists(void) __asm__("FUN_0020d218");
extern void draw_moby_list(void *, s32) __asm__("FUN_0020d330");
extern void func_00218D10(void) __asm__("FUN_00218d10");
extern void func_001F2260(void) __asm__("FUN_001f2260");
extern void project_graphics_bounds(Vec4 *, Vec4 *, s32 *, s32 *, s32 *, s32 *)
    __asm__("FUN_00237a78");
extern void append_screen_rect_packet(s32, s32, s32, s32, u64, s32) __asm__("FUN_00200e08");
extern void func_001F7888(s32, s32, s32, f32) __asm__("FUN_001f7888");
extern void draw_hud_rect_depth(s32, s32, s32, s32, u64, u32, s32) __asm__("FUN_00200f90");
extern void begin_draw_frame(void) __asm__("FUN_001f7978");
extern void draw_textured_quad(s32, s32, s32, s32, s32, s32, s32, s32, s64, s64)
    __asm__("FUN_001f5450");
extern void restore_moby_class_dists(void) __asm__("FUN_0020d248");
extern void draw_mobys_clean_up(void) __asm__("FUN_0020d3b0");
extern void setup_gif_paging(s32) __asm__("FUN_001f4280");
extern void draw_menu_flashing_panel(PanelRenderSlot *) __asm__("FUN_00223e28");
extern void do_gif_paging(void) __asm__("FUN_001f4398");

void render_level_effects_and_screen_sprites(void) __asm__("FUN_002196b8");

void render_level_effects_and_screen_sprites(void) {
    Vec4 frame_vectors[4];
    /* Hypothesis, not evidence of a real variable: retail reserves
       sp+0x50..0x9F, which nothing in the function reads or writes. This
       array only reproduces that gap so the corner copies land at
       sp+0xA0/0xB0; what the original local was is unknown. */
    Vec4 unused[5];
    Vec4 first_corner;
    Vec4 opposite_corner;
    s32 projected_width;
    s32 projected_height;
    s32 screen_x;
    s32 screen_y;
    RenderPanel **panels;
    PanelRenderSlot *slot;
    PanelRenderSlot *projected_slot;
    RenderPanel *panel;
    ProjectedPanelFrame *projected_frame;
    ProjectedPanelFrame *panel_frame;
    s32 effect_slot_index;
    s32 projection_slot_index;
    s32 pass;
    s32 draw_slot_index;
    s32 final_slot_index;
    /* One local serves as the projected width in the projection loop and
       as the panel flags in the draw loop; retail allocates both through
       the same register, which separate locals do not reproduce. */
    s32 value;
    s32 frame_width;
    s32 slot_count;
    s32 frame_height;
    s32 texture_width_log2;
    s32 texture_height_log2;
    s32 texture_width;
    s32 texture_height;
    s32 draw_result;
    s32 texture_left;
    s32 texture_top;
    s32 texture_right;
    s32 texture_bottom;
    s32 frame_x;
    s32 frame_y;

    slot_count = 14;
    vu1_add_g_sregister(0x47, 0x5360B);
    draw_mobys_setup();
    init_moby_class_dists();
    stash_moby_class_dists();
    draw_moby_list(resident_object_pool, 4);
    func_00218D10();
    func_001F2260();
    for (effect_slot_index = 0; effect_slot_index < slot_count; effect_slot_index++) {
        if (panel_slot_enabled[effect_slot_index] != 0 && panel_slots[effect_slot_index] != 0 &&
            (effect_slot_index != 6 || menu_system.unkD8 != 0)) {
            draw_moby_list(panel_slots[effect_slot_index], 1);
        }
    }

    panels = menu_system.current != 0 ? (RenderPanel **)menu_system.current->screens : 0;
    for (projection_slot_index = 0; projection_slot_index < slot_count;
         projection_slot_index++) {
        projected_slot = panel_slots[projection_slot_index];
        if (projected_slot == 0 || panel_slot_enabled[projection_slot_index] == 0 ||
            (projection_slot_index == 6 && menu_system.unkD8 == 0)) {
            continue;
        }
        projected_frame = projected_slot->frame;
        qcopy(&frame_vectors[0], (Vec4 *)projected_frame + 0);
        qcopy(&frame_vectors[1], (Vec4 *)projected_frame + 1);
        qcopy(&frame_vectors[2], (Vec4 *)projected_frame + 2);
        qcopy(&frame_vectors[3], (Vec4 *)projected_frame + 3);
        first_corner.q = frame_vectors[0].q;
        opposite_corner.q = frame_vectors[3].q;
        project_graphics_bounds(&first_corner, &opposite_corner, &projected_width,
                                &projected_height, &screen_x, &screen_y);
        screen_x++;
        screen_y++;
        value = projected_width;
        if (panels != 0 && panels[projection_slot_index] != 0) {
            panels[projection_slot_index]->projected_width = value;
            panels[projection_slot_index]->projected_height = projected_height;
            panels[projection_slot_index]->screen_x = screen_x;
            panels[projection_slot_index]->screen_y = screen_y;
        }
        projected_frame->screen_x = screen_x;
        projected_frame->screen_y = screen_y;
        projected_frame->projected_width = projected_width;
        projected_frame->projected_height = projected_height;
        append_screen_rect_packet(screen_x + 1, screen_y + 1, screen_x + projected_width - 1,
                                  screen_y + projected_height - 1, panel_clear_color, 0);
    }

    for (pass = 0; pass < 2; pass++) {
        for (draw_slot_index = 0; draw_slot_index < slot_count; draw_slot_index++) {
            slot = panel_slots[draw_slot_index];
            if (slot == 0 || panels == 0) {
                continue;
            }
            panel = panels[draw_slot_index];
            if (panel == 0) {
                continue;
            }
            value = panel->flags;
            if (value & 4) {
                continue;
            }
            if (panel_slot_enabled[draw_slot_index] == 0 || panel->draw == 0) {
                continue;
            }
            if (draw_slot_index == 6 && menu_system.unkD8 == 0) {
                continue;
            }
            if (pass == 0 && !(value & 2)) {
                continue;
            }
            if (pass == 1 && (value & 2)) {
                continue;
            }
            if (value & 1) {
                panel->draw(panel);
                continue;
            }
            panel_frame = slot->frame;
            frame_x = panel_frame->screen_x;
            frame_y = panel_frame->screen_y;
            frame_width = panel_frame->projected_width;
            frame_height = panel_frame->projected_height;
            texture_width_log2 = 7;
            while ((1 << texture_width_log2) < frame_width) {
                texture_width_log2++;
            }
            texture_height_log2 = 7;
            while ((1 << texture_height_log2) < frame_height) {
                texture_height_log2++;
            }
            while (texture_width_log2 + texture_height_log2 >= 18) {
                texture_height_log2--;
            }
            func_001F7888(texture_width_log2, texture_height_log2, pass != 0, 1.0f);
            texture_width = 1 << texture_width_log2;
            texture_height = 1 << texture_height_log2;
            draw_hud_rect_depth(0, 0, texture_width, texture_height, panel_clear_color, 0, 0);
            draw_result = panels[draw_slot_index]->draw(panels[draw_slot_index]);
            begin_draw_frame();
            if (draw_result & 1) {
                continue;
            }
            texture_left = 0;
            texture_right = texture_width;
            texture_top = 0;
            texture_bottom = texture_height;
            if (draw_result & 2) {
                texture_right = frame_width;
                texture_bottom = frame_height;
            } else if (draw_result & 8) {
                /* Centre the frame in the texture page, clamping each axis. */
                texture_left = (texture_right - frame_width) / 2;
                texture_top = (texture_bottom - frame_height) / 2;
                texture_right -= texture_left;
                texture_left = texture_left > -1 ? texture_left : 0;
                texture_bottom -= texture_top;
                texture_top = texture_top > -1 ? texture_top : 0;
                if (texture_width < texture_right) {
                    texture_right = texture_width;
                }
                if (texture_height < texture_bottom) {
                    texture_bottom = texture_height;
                }
            } else if (draw_result & 4) {
                if (frame_width < frame_height) {
                    texture_left =
                        texture_right / 2 - frame_width * texture_right / (frame_height * 2);
                    texture_right -= texture_left;
                } else {
                    texture_top =
                        texture_bottom / 2 - frame_height * texture_bottom / (frame_width * 2);
                    texture_bottom -= texture_top;
                }
            } else if (!(draw_result & 0x10)) {
                continue;
            }
            vu1_add_g_sregister(0x42, 0x8000000064L);
            vu1_add_g_sregister(0x47, 0x43);
            draw_textured_quad(frame_x, frame_y, frame_width, frame_height, texture_left,
                               texture_top, texture_right - texture_left,
                               texture_bottom - texture_top, 0x80808080L, capture_texture_tex0);
        }
        if (pass == 0) {
            restore_moby_class_dists();
            draw_mobys_clean_up();
        }
    }

    setup_gif_paging(0);
    for (final_slot_index = 0; final_slot_index < slot_count; final_slot_index++) {
        if (panel_slot_enabled[final_slot_index] != 0 &&
            (final_slot_index != 6 || menu_system.unkD8 != 0)) {
            draw_menu_flashing_panel(panel_slots[final_slot_index]);
        }
    }
    do_gif_paging();
}

extern __typeof__(render_level_effects_and_screen_sprites) func_002196B8
    __attribute__((alias("FUN_002196b8")));

s32 panel_slot_enabled[14] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
