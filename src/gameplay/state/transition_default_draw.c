#include "rnc/gameplay/state/transition_default_draw.h"
extern u8 D_00100AE0[];
extern s32 D_0013E504[];
extern s32 D_0015ED88;
extern s64 D_0015EF48;
extern s32 D_0015EF50;
extern s32 D_0015EF54;
extern f32 D_0015F43C;
extern s32 D_0015F464;
extern s32 D_0015F604;
extern s32 D_0015F620;
extern struct Globals_0016045C *D_0016045C;
extern s32 D_0018A2E8[];
extern u8 D_00193FC0[];
extern u8 D_001D8EB0[];
extern u8 D_001E1300[];
extern u8 D_001E3200[];
extern s32 AppendDmaTag();
extern s32 FillTransferWords();
extern s32 FlushCache();
extern s32 transition_draw_sky() __asm__("func_001E9AB8");
extern s32 transition_update_movie_camera() __asm__("func_001EAF88");
extern s32 func_001F2260();
extern s32 update_fog() __asm__("func_001F2588");
extern void reset_gs_registers() __asm__("func_001F3868");
extern s32 setup_gif_paging() __asm__("func_001F4280");
extern s32 do_gif_paging() __asm__("func_001F4398");
extern u64 get_effect_texture() __asm__("func_001F44B8");
extern s32 dispatch_callback_list_1() __asm__("func_001F4650");
extern s32 emit_rgba_draw_packet() __asm__("func_001F5210");
extern s32 draw_textured_quad() __asm__("func_001F5450");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern s32 append_gif_transfer_packet() __asm__("func_001FB368");
extern s32 aa_blur_pass() __asm__("func_001FB680");
extern s32 draw_dialog_text() __asm__("func_001FBC50");
extern s32 prune_moby_references() __asm__("FUN_0020cc60");
extern s32 patch_moby_gifs() __asm__("func_0020CEF8");
extern s32 draw_mobys() __asm__("func_0020D460");
extern void func_00217C18();
extern s32 render_level_effects_and_screen_sprites() __asm__("func_002196B8");
extern s32 func_00228A30();
extern s32 draw_shrubs() __asm__("func_00228B38");
extern s32 func_0022A5E0();
extern s32 patch_tfrag_gifs() __asm__("func_00233308");
extern s32 draw_tfrag() __asm__("func_002333A8");
extern s32 vu0_load_micro_program() __asm__("func_002334D8");
extern s32 vu1_sync_chain() __asm__("func_002337B0");
extern s32 vu1_add_g_sregister() __asm__("func_00233980");
extern void vu1_gs_regs_alt() __asm__("func_00233C28");
extern s32 func_00234F98();
extern s32 patch_tie_texture_fields() __asm__("func_00235780");
extern s32 draw_ties_1() __asm__("func_002358C8");
extern s32 func_00237370();
void transition_default_draw(s32 *arg0) __asm__("FUN_001eb410");

void transition_default_draw(s32 *arg0) {
    s32 n;

    if (D_0016045C == 0 || D_0016045C->unk4 != 0) {
        append_gif_transfer_packet();
    }
    FillTransferWords(D_00193FC0, -1, 0x80);
    transition_update_movie_camera();
    func_001F2260();
    prune_moby_references();
    reset_gs_registers();
    D_0015F620 = -1;
    if (D_0016045C != 0) {
        transition_draw_sky();
    }
    draw_tfrag();
    AppendDmaTag(0x02010000);
    draw_ties_1();
    AppendDmaTag(0x02020000);
    draw_shrubs();
    AppendDmaTag(0x02040000);
    if (D_0015F604 == 3) {
        render_level_effects_and_screen_sprites();
    } else {
        draw_mobys();
    }
    AppendDmaTag(0x02080000);
    setup_gif_paging(0);
    vu1_gs_regs_alt();
    if (D_0015F464 != 0) {
        dispatch_callback_list_1();
    }
    vu1_gs_regs_alt();
    if (D_0018A2E8[0] != 0) {
        vu1_add_g_sregister(8, 5);
        vu1_gs_regs_alt();
        FlushCache(0);
        func_00217C18();
        D_0015F620 = 8;
    }
    aa_blur_pass();
    reset_gs_registers();
    if (D_0015EF50 != 0) {
        draw_textured_quad(0xEC, 0x10, 0x100, 0x80, 0, 0, 0x100, 0x80,
                           (long)(D_0015EF50 << 24 | 0x808080), D_0015EF48);
    }
    if (D_0015EF54 != 0) {
        n = D_0015ED88 - 1;
        if (n < 0) {
            n = 0;
        }
        draw_textured_quad(0xA0, D_0013E504[0] - 0x50, 0xC0, 0x60, 0, 0, 0x100, 0x80,
                           (long)(D_0015EF54 << 24 | 0x808080), get_effect_texture(n + 4));
    }
    do_gif_paging();
    if (D_0015F43C > 0.0f) {
        if (D_0015F43C > 1.0f) {
            D_0015F43C = 1.0f;
        }
        emit_rgba_draw_packet(0, 0, 0, truncate_float_to_s32(D_0015F43C * 128.0f));
    }
    vu0_load_micro_program(D_00100AE0);
    FlushCache(0);
    if (D_0015F604 == 4) {
        draw_dialog_text();
    }
    vu1_sync_chain(2);
    func_00234F98(D_001E1300);
    patch_tfrag_gifs();
    vu1_sync_chain(4);
    func_00237370(D_001E3200);
    patch_tie_texture_fields();
    vu1_sync_chain(8);
    func_0022A5E0(D_001D8EB0);
    func_00228A30();
    vu1_sync_chain(0x10);
    patch_moby_gifs();
    update_fog();
}

extern __typeof__(transition_default_draw) func_001EB410 __attribute__((alias("FUN_001eb410")));
