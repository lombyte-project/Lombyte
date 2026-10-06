#include "types.h"
#include "sda.h"

typedef struct {
    u8 pad_0[4];
    s16 unk4;
} SpriteFile;

typedef struct {
    u8 pad_0[8];
    s32 unk8;
    s32 pad_C;
    s32 unk10;
    s32 pad_14;
    s32 unk18;
    s32 pad_1C;
    s32 unk20;
    s32 pad_24;
    s32 unk28;
    s32 pad_2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
} DrawFlags;

extern u8 D_00100AE0[];
extern u16 D_0010FA90[];
extern u8 D_0010FAA0[];
extern s32 D_0015ED80 MACRO_ADDR;
extern u8 D_0015EE40;
extern s32 D_0015F34C __attribute__((sda));
extern s32 D_0015F350 __attribute__((sda));
extern s32 D_0015F370[2] __attribute__((sda));
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
extern s32 D_0015F434 MACRO_ADDR;
extern f32 D_0015F43C MACRO_ADDR;
extern f32 D_0015F440 MACRO_ADDR;
extern s32 D_0015F464 __attribute__((sda));
extern s32 D_0015F468 __attribute__((sda));
extern s32 D_0015F46C __attribute__((sda));
extern s32 D_0015F470 __attribute__((sda));
extern s32 D_0015F604 MACRO_ADDR;
extern f32 D_0015F614 MACRO_ADDR;
extern s32 D_0015F620 MACRO_ADDR;
extern s32 D_0015F648 MACRO_ADDR;
extern SpriteFile *D_0016045C;
extern u8 D_001610C0 MACRO_ADDR;
extern u8 D_001610C1 MACRO_ADDR;
extern u8 D_001610C2 MACRO_ADDR;
extern u8 D_001610C3 MACRO_ADDR;
extern s32 D_001872D4[];
extern DrawFlags D_0018A2B0;
extern s32 D_0018C34C[];
extern u8 D_001D8EB0[];
extern u8 D_001E1300[];
extern u8 D_001E3200[];
extern u8 D_001E78A0[];
extern u8 D_001E78B8[];

extern void AppendDmaTag(u32);
extern void FlushCache(s32);
extern void transition_draw_sky(void) __asm__("func_001E9AB8");
extern void func_001EDC50(void);
extern void render_queued_rotated_sprites(void) __asm__("FUN_001ee338");
extern void func_001F21B0(void *, s32);
extern void func_001F21B8(void *, s32);
extern void func_001F2260(void);
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
extern f32 func_001FA6C0(s32);
extern s32 truncate_float_to_s32(f32) __asm__("func_001FA6D0");
extern void append_gif_transfer_packet(void) __asm__("func_001FB368");
extern void aa_blur_pass(void) __asm__("func_001FB680");
extern void draw_help(void) __asm__("func_001FE980");
extern void update_hud(void) __asm__("func_001FF780");
extern void prune_moby_references(void) __asm__("FUN_0020cc60");
extern void patch_moby_gifs(void) __asm__("func_0020CEF8");
extern void draw_mobys(void) __asm__("func_0020D460");
extern void func_00217C18(void);
extern void func_00228A30(void);
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
extern void FUN_00234f98(void *);
extern void patch_tie_texture_fields(void) __asm__("FUN_00235780");
extern void copy_render_buffer_pair(void) __asm__("FUN_00235840");
extern void register_entity_render_resources(void) __asm__("FUN_00235898");
extern void draw_ties_1(void) __asm__("func_002358C8");
extern void draw_ties_2(void) __asm__("func_00235990");
extern void FUN_00237370(void *);
extern void func_00237A70(void);

void draw_debug_profiler(void) __asm__("FUN_001f39d0");

void draw_debug_profiler(void) {
    f32 t;
    f32 div;

    if (D_0016045C == NULL || D_0016045C->unk4 != 0 || ((D_0015F434 ^ 1) & 1) ||
        D_0018A2B0.unk8 == 0 || D_0018C34C[0] != 0) {
        append_gif_transfer_packet();
    }
    func_001F2260();
    prune_moby_references();
    update_occlusion();
    reset_gs_registers();
    D_0015F620 = -1;
    func_001F21B0(D_0015F380, 0xF);
    func_001F21B8(D_0015F380, 0xF);
    if (D_0016045C != NULL && (D_0015F434 & 1)) {
        if (D_0018A2B0.unk8 != 0) {
            transition_draw_sky();
        }
        func_001F21B8(D_0015F390, 0xE);
        func_001F21B0(D_0015F390, 0xE);
    }
    if (D_0015F434 & 2) {
        draw_tfrag();
    }
    AppendDmaTag(0x02010000);
    if (D_0015F434 & 4) {
        if (D_0015ED80 != 0) {
            draw_ties_2();
        } else {
            draw_ties_1();
        }
    }
    AppendDmaTag(0x02020000);
    if (D_0018A2B0.unk34 != 0 && D_0015F46C != 0) {
        vu1_gs_regs_alt();
        setup_gif_paging(1);
        dispatch_callback_list_2();
        do_gif_paging();
        vu1_gs_regs_normal();
    }
    func_001F21B0(D_0015F3A0, 6);
    func_001F21B8(D_0015F3A0, 6);
    if (D_0015F434 & 8) {
        draw_shrubs();
    }
    AppendDmaTag(0x02040000);
    if (D_0018A2B0.unk44 != 0 && D_0015F370[0] != 0) {
        draw_fogged_fullscreen_sprite(D_0015F370);
    }
    if (D_0015F434 & 0x20) {
        setup_gif_paging(1);
        draw_debug_font();
        do_gif_paging();
    }
    if (D_0018A2B0.unk34 != 0 && D_0015F470 != 0) {
        vu1_gs_regs_alt();
        setup_gif_paging(1);
        dispatch_callback_list_3();
        do_gif_paging();
        vu1_gs_regs_normal();
    }
    if (D_0015F434 & 0x10) {
        draw_mobys();
    }
    AppendDmaTag(0x02080000);
    setup_gif_paging(0);
    if ((D_0015F434 & 0x20) && D_0018A2B0.unk30 != 0 && D_0015F620 != 6) {
        vu1_add_data_ref(D_0010FAA0, D_0010FA90[0]);
        D_0015F620 = 6;
    }
    func_001F21B8(D_0015F3B0, 4);
    func_001F21B0(D_0015F3B0, 4);
    if (D_0015F434 & 0x20) {
        vu1_gs_regs_alt();
        if (D_0018A2B0.unk34 != 0) {
            if (D_0015F464 != 0) {
                dispatch_callback_list_1();
            }
            vu1_add_g_sregister(0x42, 0x8000000048);
            func_001EDC50();
            vu1_gs_regs_alt();
            draw_light_quads();
        }
        func_001F21B0(D_0015F3C0, 6);
        func_001F21B8(D_0015F3C0, 6);
        if (D_0018A2B0.unk38 != 0) {
            vu1_add_g_sregister(8, 5);
            vu1_gs_regs_alt();
            FlushCache(0);
            func_00217C18();
            D_0015F620 = 8;
        }
        func_001F21B0(D_0015F3D0, 8);
        func_001F21B8(D_0015F3D0, 8);
        if (D_0018A2B0.unk3C != 0) {
            if (D_0015F468 != 0) {
                vu1_gs_regs_alt();
                dispatch_callback_list_4();
            }
            if (D_0015F604 == 0) {
                append_billboard_batch();
            }
            vu1_add_g_sregister(0x42, 0x8000000044);
            render_queued_rotated_sprites();
        }
        func_001F21B0(D_0015F3E0, 6);
        func_001F21B8(D_0015F3E0, 6);
    }
    if (D_0018A2B0.unk48 != 0) {
        aa_blur_pass();
    }
    func_001F21B8(D_0015F3F0, 0xF);
    vu1_gs_regs_alt();
    if (D_0015F434 & 0x10000) {
        func_00237A70();
    }
    if ((D_0015F434 & 0x80) && D_0018A2B0.unk40 != 0) {
        update_hud();
        draw_help();
        draw_letterbox_bars();
    }
    if (D_0015F604 == 2 && D_0015EE40 != 0) {
        draw_subtitles();
    }
    func_001F21B8(D_0015F3F8, 0xE);
    func_001F21B0(D_0015F3F8, 0xE);
    do_gif_paging();
    if (D_0015F434 & 0x40) {
        if (D_0018A2B0.unk44 != 0) {
            vu1_add_g_sregister(0x42, 0x8000000044);
            if (D_001872D4[0] != 0) {
                emit_rgba_draw_packet(D_001610C0, D_001610C1, D_001610C2, D_001610C3);
            }
            if (D_0015F43C > 0.0f) {
                if (D_0015F43C > 1.0f) {
                    D_0015F43C = 1.0f;
                }
                emit_rgba_draw_packet(0, 0, 0, truncate_float_to_s32(D_0015F43C * 128.0f));
            }
            if (D_0015F440 > 0.0f) {
                if (D_0015F440 > 1.0f) {
                    D_0015F440 = 1.0f;
                }
                emit_rgba_draw_packet(0xFF, 0xFF, 0xFF, truncate_float_to_s32(D_0015F440 * 128.0f));
            }
            if (D_0015F34C != 0 && D_0015F350 != 0) {
                draw_screen_effect();
            }
        }
        func_001F21B8(D_0015F400, 0xA);
    }
    vu0_load_micro_program(D_00100AE0);
    FlushCache(0);
    t = func_001FA6C0(*(volatile s32 *)0x10000800);
    D_0015F614 = t / (D_0015ED80 != 0 ? 11520.0f : 9600.0f);
    vu1_sync_chain(2);
    func_001F21B0(D_0015F410, 0x11);
    if (D_0015F434 & 2) {
        if (D_0018A2B0.unk10 != 0) {
            FUN_00234f98(D_001E1300);
            patch_tfrag_gifs();
        }
        func_001F21B0(D_001E78A0, 2);
    }
    vu1_sync_chain(4);
    func_001F21B0(D_0015F410, 0x11);
    if (D_0015F434 & 4) {
        if (D_0018A2B0.unk18 != 0) {
            if (D_0015ED80 != 0) {
                register_entity_render_resources();
                copy_render_buffer_pair();
            } else {
                FUN_00237370(D_001E3200);
                patch_tie_texture_fields();
            }
        }
        func_001F21B0(D_0015F418, 5);
    }
    vu1_sync_chain(8);
    func_001F21B0(D_0015F410, 0x11);
    if (D_0015F434 & 8) {
        if (D_0018A2B0.unk20 != 0) {
            FUN_0022a5e0(D_001D8EB0);
            func_00228A30();
        }
        func_001F21B0(D_001E78B8, 7);
    }
    vu1_sync_chain(0x10);
    func_001F21B0(D_0015F410, 0x11);
    if (D_0015F434 & 0x10) {
        if (D_0018A2B0.unk28 != 0) {
            patch_moby_gifs();
        }
        func_001F21B0(D_0015F428, 3);
    }
    update_fog();
    D_0015F648 = 0;
}

extern __typeof__(draw_debug_profiler) func_001F39D0 __attribute__((alias("FUN_001f39d0")));
