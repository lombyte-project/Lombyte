#include "types.h"
#include "asm.h"

#include "types.h"

struct LevelOverlayState {
    u8 pad_0[0xD4];
    s32 stage;
    u8 pad_D8[0x4];
    s32 selection;
};
struct LevelRenderState {
    u8 pad_0[0x58];
    s32 mode;
};
struct LevelDisplayState {
    u8 pad_0[0x4];
    s32 screen_height;
};
struct LevelProjectionState {
    u8 pad_0[0xB0];
    f32 projection_scale;
};
extern struct LevelOverlayState D_0013D290;
extern u8 D_0013DD43[];
extern struct LevelRenderState level_render_state __asm__("D_0013E030");
extern struct LevelDisplayState screen_offsets __asm__("D_0013E500");
extern s32 current_level_index __asm__("D_0015ED84");
extern s32 game_frame_counter __asm__("D_0015F438");
extern f32 sequence_fade __asm__("D_0015F43C");
extern s32 D_0015F620;
extern s32 D_0018CB54[];
extern s32 D_0018CC98[];
extern struct LevelProjectionState view_context __asm__("D_0018CD00");
extern void AppendDmaTag(u32);
extern void func_001F2260();
extern void update_view_context() __asm__("func_001F2D98");
extern void reset_gs_registers() __asm__("func_001F3868");
extern void setup_gif_paging(s32) __asm__("func_001F4280");
extern void do_gif_paging() __asm__("func_001F4398");
extern s64 get_effect_texture(s32) __asm__("func_001F44B8");
extern void emit_rgba_draw_packet(s32, s32, s32, s32) __asm__("func_001F5210");
extern void draw_textured_quad(s32, s32, s32, s32, s32, s32, s32, s32, s64,
                               s64) __asm__("func_001F5450");
extern s32 truncate_float_to_s32(f32) __asm__("func_001FA6D0");
extern void append_gif_transfer_packet() __asm__("func_001FB368");
extern void draw_rotated_sprite(f32, f32, f32, f32, s32, s32, s64, f32) __asm__("FUN_00200600");
extern void prune_moby_references(void) __asm__("FUN_0020cc60");
extern void patch_moby_gifs() __asm__("func_0020CEF8");
extern void draw_mobys(void) __asm__("func_0020D460");
extern void draw_sky_shells() __asm__("func_0022B288");
extern void build_resident_indexed_texture_warp_meshes(s32) __asm__("func_0022E420");
extern void draw_resident_textured_quad() __asm__("func_0022E8C8");
extern void draw_resident_textured_banner(s32) __asm__("FUN_0022ea08");
extern void render_environment_mapped_object(s32) __asm__("func_002327A0");
extern void vu1_sync_chain(s32) __asm__("func_002337B0");
extern void vu1_add_g_sregister(s32, s64) __asm__("func_00233980");

extern struct LevelDisplayState D_0013E500_far __asm__("D_0013E500")
    __attribute__((section(".data")));
void render_level_frame(void) __asm__("FUN_0022f288");

void render_level_frame(void) {
    f32 rotation_angle;
    f32 screen_y;
    f32 quad_extent;
    s32 overlay_alpha;
    f32 saved_projection_scale;
    struct LevelDisplayState *display_state;
    s64 texture;
    append_gif_transfer_packet();
    func_001F2260();
    prune_moby_references();
    reset_gs_registers();
    saved_projection_scale = view_context.projection_scale;
    D_0015F620 = -1;
    if (saved_projection_scale < 0.63f) {
        view_context.projection_scale = 0.63f;
    }
    update_view_context();
    func_001F2260();
    draw_sky_shells();
    view_context.projection_scale = saved_projection_scale;
    update_view_context();
    func_001F2260();
    if (level_render_state.mode == 4) {
        setup_gif_paging(1);
        draw_resident_textured_quad();
        do_gif_paging();
    }
    draw_mobys();
    AppendDmaTag(0x02080000);
    setup_gif_paging(1);
    if ((current_level_index != 0) && ((current_level_index != 1) || (D_0013DD43[0] != 0))) {
        build_resident_indexed_texture_warp_meshes(D_0018CC98[0]);
    }
    if ((level_render_state.mode == 4) && (D_0018CB54[0] >= 0x3D)) {
        overlay_alpha = (D_0018CB54[0] - 0x3C) * 2;
        if (overlay_alpha >= 0x81) {
            overlay_alpha = 0x80;
        }
        draw_resident_textured_banner(overlay_alpha);
    }
    if ((current_level_index != 0) && ((current_level_index != 1) || (D_0013DD43[0] != 0))) {
        render_environment_mapped_object(D_0018CC98[0]);
    }
    if ((D_0013D290.stage >= 3) || (D_0013D290.selection >= 0)) {
        vu1_add_g_sregister(0x47, 0x3004B);
        quad_extent = 272.0f;
        texture = get_effect_texture(2);
        display_state = &D_0013E500_far;
        draw_textured_quad(0x2C, display_state->screen_height - 0x60, 0x40, 0x40, 0, 0, 0x40, 0x40,
                           0x80808080, texture);
        screen_y = (f32)((display_state->screen_height - 0x40) * 16);
        rotation_angle = ((game_frame_counter % 55) * (-6.2831855f)) / 55.0f;
        /* The frame counter contributes only the sprite angle modulo 55. */
        /* Retail passes the full texture value in a2 and the five floats in f12-f16. */
        draw_rotated_sprite(1216.0f, screen_y, quad_extent, 272.0f, 0x40, 0x40,
                            get_effect_texture(3), rotation_angle);
    }
    do_gif_paging();
    if (sequence_fade > 0.0f) {
        if (sequence_fade > 1.0f) {
            sequence_fade = 1.0f;
        }
        emit_rgba_draw_packet(0, 0, 0, truncate_float_to_s32(sequence_fade * 128.0f));
    }
    vu1_sync_chain(0x10);
    patch_moby_gifs();
}
extern __typeof__(render_level_frame) func_0022F288 __attribute__((alias("FUN_0022f288")));
