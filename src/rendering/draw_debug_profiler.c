#include "types.h"
#include "rnc/rendering/draw_config.h"
#include "sda.h"

typedef struct {
    u8 pad_0[4];
    s16 unk4;
} SpriteFile;

extern u8 D_00100AE0[];
extern u16 D_0010FA90[];
extern u8 D_0010FAA0[];
extern u8 subtitles_enabled __asm__("D_0015EE40") MACRO_ADDR;
extern s32 screen_overlay_enabled __asm__("D_0015F34C") __attribute__((sda));
extern s32 screen_color_effect_now __asm__("D_0015F350") __attribute__((sda));
extern s32 occl_debug_overlay_enabled[2] __asm__("D_0015F370") __attribute__((sda));
extern u8 D_0015F380[];
extern u8 D_0015F390[];
extern u8 D_0015F3A0[];
extern u8 D_0015F3B0[];
extern u8 D_0015F3C0[];
extern u8 D_0015F3D0[];
extern u8 D_0015F3E0[];
extern u8 D_0015F3F0[];
extern u8 D_0015F3F8[];
extern u8 D_0015F400[];
extern u8 D_0015F410[];
extern u8 D_0015F418[];
extern u8 D_0015F428[];
extern s32 draw_enable_mask __asm__("D_0015F434") MACRO_ADDR;
extern f32 D_0015F440 MACRO_ADDR;
extern s32 draw_callback_count __asm__("D_0015F464") __attribute__((sda));
extern s32 draw_callback2_count __asm__("D_0015F468") __attribute__((sda));
extern s32 draw_callback3_count __asm__("D_0015F46C") __attribute__((sda));
extern s32 draw_callback4_count __asm__("D_0015F470") __attribute__((sda));
extern f32 D_0015F614 MACRO_ADDR;
extern s32 font_state __asm__("D_0015F620") MACRO_ADDR;
extern s32 D_0015F648 MACRO_ADDR;
extern SpriteFile *D_0016045C;
extern u8 D_001610C0 MACRO_ADDR;
extern u8 D_001610C1 MACRO_ADDR;
extern u8 D_001610C2 MACRO_ADDR;
extern u8 D_001610C3 MACRO_ADDR;
extern s32 use_alt_fog[] __asm__("D_001872D4");
extern s32 D_0018C34C[];
extern u8 D_001D8EB0[];
extern u8 D_001E1300[];
extern u8 tie_light_data[] __asm__("D_001E3200");
extern u8 D_001E78A0[];
extern u8 D_001E78B8[];

extern void AppendDmaTag(u32);
extern void FlushCache(s32);
extern void transition_draw_sky(void) __asm__("func_001E9AB8");
extern void draw_lens_flare(void) __asm__("func_001EDC50");
extern void render_queued_rotated_sprites(void) __asm__("FUN_001ee338");
extern void noop_core_callback(void *, s32) __asm__("func_001F21B0");
extern void noop_core_callback_secondary(void *, s32) __asm__("func_001F21B8");
extern void update_camera_view_matrices(void) __asm__("func_001F2260");
extern void update_fog(void) __asm__("func_001F2588");
extern void update_occlusion(void) __asm__("FUN_001f2c10");
extern void reset_gs_registers(void) __asm__("func_001F3868");
extern void setup_gif_paging(s32) __asm__("func_001F4280");
extern void do_gif_paging(void) __asm__("func_001F4398");
extern void dispatch_callback_list_1(void) __asm__("func_001F4650");
extern void dispatch_callback_list_2(void) __asm__("func_001F46C8");
extern void dispatch_callback_list_3(void) __asm__("func_001F4740");
extern void dispatch_callback_list_4(void) __asm__("func_001F4808");
extern void draw_light_quads(void) __asm__("func_001F4880");
extern void draw_subtitles(void) __asm__("func_001F4BE0");
extern void draw_letterbox_bars(void) __asm__("func_001F4D98");
extern void draw_screen_effect(void) __asm__("func_001F4FB8");
extern void draw_fogged_fullscreen_sprite(void *) __asm__("func_001F5138");
extern void emit_rgba_draw_packet(s32, s32, s32, s32) __asm__("func_001F5210");
extern void draw_debug_font(void) __asm__("func_001F79A8");
extern void append_billboard_batch(void) __asm__("FUN_001f92b0");
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern s32 truncate_float_to_s32(f32) __asm__("func_001FA6D0");
extern void append_gif_transfer_packet(void) __asm__("func_001FB368");
extern void aa_blur_pass(void) __asm__("func_001FB680");
extern void draw_help(void) __asm__("func_001FE980");
extern void update_hud(void) __asm__("func_001FF780");
extern void prune_moby_references(void) __asm__("FUN_0020cc60");
extern void patch_moby_gifs(void) __asm__("func_0020CEF8");
extern void draw_mobys(void) __asm__("func_0020D460");
extern void part_proc(void) __asm__("func_00217C18");
extern void patch_shrub_gifs(void) __asm__("func_00228A30");
extern void draw_shrubs(void) __asm__("func_00228B38");
extern void FUN_0022a5e0(void *);
extern void patch_tfrag_gifs(void) __asm__("func_00233308");
extern void draw_tfrag(void) __asm__("func_002333A8");
extern void vu0_load_micro_program(void *) __asm__("func_002334D8");
extern void vu1_sync_chain(s32) __asm__("func_002337B0");
extern void vu1_add_data_ref(void *, s32) __asm__("func_00233830");
extern void vu1_add_g_sregister(s32, u64) __asm__("func_00233980");
extern void vu1_gs_regs_normal(void) __asm__("func_00233BC8");
extern void vu1_gs_regs_alt(void) __asm__("func_00233C28");
extern void light_tfrags(void *) __asm__("FUN_00234f98");
extern void patch_tie_texture_fields(void) __asm__("FUN_00235780");
extern void copy_render_buffer_pair(void) __asm__("FUN_00235840");
extern void register_entity_render_resources(void) __asm__("FUN_00235898");
extern void draw_ties_1(void) __asm__("func_002358C8");
extern void draw_ties_2(void) __asm__("func_00235990");
extern void light_ties(void *) __asm__("FUN_00237370");
extern void noop_graphics_callback(void) __asm__("func_00237A70");

void draw_debug_profiler(void) __asm__("FUN_001f39d0");

extern s32 pal_mode __asm__("D_0015ED80") MACRO_ADDR;
extern float sequence_macro __asm__("D_0015F43C") MACRO_ADDR;
extern s32 game_mode_macro __asm__("D_0015F604") MACRO_ADDR;
void draw_debug_profiler(void) {
    f32 rcnt1_count_float;
    f32 div;

    if (D_0016045C == NULL || D_0016045C->unk4 != 0 || ((draw_enable_mask ^ 1) & 1) ||
        draw_config.unk8 == 0 || D_0018C34C[0] != 0) {
        append_gif_transfer_packet();
    }
    update_camera_view_matrices();
    prune_moby_references();
    update_occlusion();
    reset_gs_registers();
    font_state = -1;
    noop_core_callback(D_0015F380, 0xF);
    noop_core_callback_secondary(D_0015F380, 0xF);
    if (D_0016045C != NULL && (draw_enable_mask & 1)) {
        if (draw_config.unk8 != 0) {
            transition_draw_sky();
        }
        noop_core_callback_secondary(D_0015F390, 0xE);
        noop_core_callback(D_0015F390, 0xE);
    }
    if (draw_enable_mask & 2) {
        draw_tfrag();
    }
    AppendDmaTag(0x02010000);
    if (draw_enable_mask & 4) {
        if (pal_mode != 0) {
            draw_ties_2();
        } else {
            draw_ties_1();
        }
    }
    AppendDmaTag(0x02020000);
    if (draw_config.unk34 != 0 && draw_callback3_count != 0) {
        vu1_gs_regs_alt();
        setup_gif_paging(1);
        dispatch_callback_list_2();
        do_gif_paging();
        vu1_gs_regs_normal();
    }
    noop_core_callback(D_0015F3A0, 6);
    noop_core_callback_secondary(D_0015F3A0, 6);
    if (draw_enable_mask & 8) {
        draw_shrubs();
    }
    AppendDmaTag(0x02040000);
    if (draw_config.unk44 != 0 && occl_debug_overlay_enabled[0] != 0) {
        draw_fogged_fullscreen_sprite(occl_debug_overlay_enabled);
    }
    if (draw_enable_mask & 0x20) {
        setup_gif_paging(1);
        draw_debug_font();
        do_gif_paging();
    }
    if (draw_config.unk34 != 0 && draw_callback4_count != 0) {
        vu1_gs_regs_alt();
        setup_gif_paging(1);
        dispatch_callback_list_3();
        do_gif_paging();
        vu1_gs_regs_normal();
    }
    if (draw_enable_mask & 0x10) {
        draw_mobys();
    }
    AppendDmaTag(0x02080000);
    setup_gif_paging(0);
    if ((draw_enable_mask & 0x20) && draw_config.unk30 != 0 && font_state != 6) {
        vu1_add_data_ref(D_0010FAA0, D_0010FA90[0]);
        font_state = 6;
    }
    noop_core_callback_secondary(D_0015F3B0, 4);
    noop_core_callback(D_0015F3B0, 4);
    if (draw_enable_mask & 0x20) {
        vu1_gs_regs_alt();
        if (draw_config.unk34 != 0) {
            if (draw_callback_count != 0) {
                dispatch_callback_list_1();
            }
            vu1_add_g_sregister(0x42, 0x8000000048);
            draw_lens_flare();
            vu1_gs_regs_alt();
            draw_light_quads();
        }
        noop_core_callback(D_0015F3C0, 6);
        noop_core_callback_secondary(D_0015F3C0, 6);
        if (draw_config.unk38 != 0) {
            vu1_add_g_sregister(8, 5);
            vu1_gs_regs_alt();
            FlushCache(0);
            part_proc();
            font_state = 8;
        }
        noop_core_callback(D_0015F3D0, 8);
        noop_core_callback_secondary(D_0015F3D0, 8);
        if (draw_config.unk3C != 0) {
            if (draw_callback2_count != 0) {
                vu1_gs_regs_alt();
                dispatch_callback_list_4();
            }
            if (game_mode_macro == 0) {
                append_billboard_batch();
            }
            vu1_add_g_sregister(0x42, 0x8000000044);
            render_queued_rotated_sprites();
        }
        noop_core_callback(D_0015F3E0, 6);
        noop_core_callback_secondary(D_0015F3E0, 6);
    }
    if (draw_config.unk48 != 0) {
        aa_blur_pass();
    }
    noop_core_callback_secondary(D_0015F3F0, 0xF);
    vu1_gs_regs_alt();
    if (draw_enable_mask & 0x10000) {
        noop_graphics_callback();
    }
    if ((draw_enable_mask & 0x80) && draw_config.unk40 != 0) {
        update_hud();
        draw_help();
        draw_letterbox_bars();
    }
    if (game_mode_macro == 2 && subtitles_enabled != 0) {
        draw_subtitles();
    }
    noop_core_callback_secondary(D_0015F3F8, 0xE);
    noop_core_callback(D_0015F3F8, 0xE);
    do_gif_paging();
    if (draw_enable_mask & 0x40) {
        if (draw_config.unk44 != 0) {
            vu1_add_g_sregister(0x42, 0x8000000044);
            if (use_alt_fog[0] != 0) {
                emit_rgba_draw_packet(D_001610C0, D_001610C1, D_001610C2, D_001610C3);
            }
            if (sequence_macro > 0.0f) {
                if (sequence_macro > 1.0f) {
                    sequence_macro = 1.0f;
                }
                emit_rgba_draw_packet(0, 0, 0, truncate_float_to_s32(sequence_macro * 128.0f));
            }
            if (D_0015F440 > 0.0f) {
                if (D_0015F440 > 1.0f) {
                    D_0015F440 = 1.0f;
                }
                emit_rgba_draw_packet(0xFF, 0xFF, 0xFF, truncate_float_to_s32(D_0015F440 * 128.0f));
            }
            if (screen_overlay_enabled != 0 && screen_color_effect_now != 0) {
                draw_screen_effect();
            }
        }
        noop_core_callback_secondary(D_0015F400, 0xA);
    }
    vu0_load_micro_program(D_00100AE0);
    FlushCache(0);
    rcnt1_count_float = convert_integer_to_float(*(volatile s32 *)0x10000800);
    D_0015F614 = rcnt1_count_float / (pal_mode != 0 ? 11520.0f : 9600.0f);
    vu1_sync_chain(2);
    noop_core_callback(D_0015F410, 0x11);
    if (draw_enable_mask & 2) {
        if (draw_config.tfrag.enabled != 0) {
            light_tfrags(D_001E1300);
            patch_tfrag_gifs();
        }
        noop_core_callback(D_001E78A0, 2);
    }
    vu1_sync_chain(4);
    noop_core_callback(D_0015F410, 0x11);
    if (draw_enable_mask & 4) {
        if (draw_config.tie.enabled != 0) {
            if (pal_mode != 0) {
                register_entity_render_resources();
                copy_render_buffer_pair();
            } else {
                light_ties(tie_light_data);
                patch_tie_texture_fields();
            }
        }
        noop_core_callback(D_0015F418, 5);
    }
    vu1_sync_chain(8);
    noop_core_callback(D_0015F410, 0x11);
    if (draw_enable_mask & 8) {
        if (draw_config.shrub.enabled != 0) {
            FUN_0022a5e0(D_001D8EB0);
            patch_shrub_gifs();
        }
        noop_core_callback(D_001E78B8, 7);
    }
    vu1_sync_chain(0x10);
    noop_core_callback(D_0015F410, 0x11);
    if (draw_enable_mask & 0x10) {
        if (draw_config.moby.enabled != 0) {
            patch_moby_gifs();
        }
        noop_core_callback(D_0015F428, 3);
    }
    update_fog();
    D_0015F648 = 0;
}

extern __typeof__(draw_debug_profiler) func_001F39D0 __attribute__((alias("FUN_001f39d0")));

/* Defined below its only user, so retail reaches it with lui. */
u8 subtitles_enabled __asm__("D_0015EE40") MACRO_ADDR = 0;
