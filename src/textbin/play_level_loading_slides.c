#include "types.h"
#include "sda.h"



#include "rnc/rendering/fs_aa_packets.h"
#include "rnc/storage/memory_card/memory_card_state.h"
#define RENDER_PACKET_CURSOR_ATTR MACRO_ADDR
#include "rnc/rendering/dma_tag.h"
#include "rnc/rendering/screen.h"
extern s32 D_0015ED84 __attribute__((sda));
extern s16 D_0015EE48 MACRO_ADDR;
extern s16 D_0015EE4A MACRO_ADDR;
extern void read_file_entry_with_retry(s32) __asm__("func_0012F368");
extern void reset_gs_registers(void) __asm__("func_001F3868");
extern void reset_gs_registers_pr(void) __asm__("func_001F3958");
extern void fade_to_black(s32) __asm__("func_001F4A58");
extern void draw_textured_quad(s32, s32, s32, s32, s32, s32, s32, s32, u64,
                               u64) __asm__("func_001F5450");
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern void put_draw_buffer_small(void) __asm__("func_001FB3D0");
extern s32 service_level_archive_load(void) __asm__("func_00204428");
extern void run_state_handler(void) __asm__("func_00208840");
extern void memcard_update_state(void) __asm__("func_002093D8");
extern void append_scrolling_textured_quad(s32, s32, s32, s32, u64, u64, f32, f32, f32,
                                           f32) __asm__("func_002316E8");
extern void prepare_loading_slide_textures(s32, s32, s32, u64 *, u64 *,
                                           u64 *) __asm__("func_00231878");
extern void vu1_init_chain(void) __asm__("func_002335D0");
extern void swap_render_buffer_chain(void) __asm__("func_00233630");
extern void vu1_send_chain(void) __asm__("func_002336A0");
extern void vu1_sync_chain(s32) __asm__("func_002337B0");
extern void vu1_add_g_sregister(s32, u64) __asm__("func_00233980");
extern void sceGsSyncV(s32);

void play_level_loading_slides(s32 language_index, s32 first_slide, s32 second_slide,
                               s32 duration_ticks, s32 preload_level) __asm__("FUN_00231bd8");

void play_level_loading_slides(s32 language_index, s32 first_slide, s32 second_slide,
                               s32 duration_ticks, s32 preload_level) {
    u64 shared_texture;
    u64 first_texture;
    u64 second_texture;
    s32 frame;
    s32 alpha;
    s32 fade_in_alpha;
    f32 scroll_phase;
    f32 scroll_start;
    f32 scroll_end;

    prepare_loading_slide_textures(language_index, first_slide, second_slide, &shared_texture,
                                   &first_texture, &second_texture);
    if (preload_level != 0) {
        read_file_entry_with_retry(D_0015ED84);
        D_0015EE48 = 0;
        D_0015EE4A = 0;
    }
    sceGsSyncV(0);
    vu1_init_chain();
    for (frame = 0; frame < duration_ticks && memory_card_state.state < 3 && memory_card_state.pending_state < 0;
         frame++) {
        alpha = 0x80;
        reset_gs_registers();
        put_draw_buffer_small();
        vu1_add_g_sregister(1, (u64)0x8000 << 16);
        vu1_add_g_sregister(8, 0);
        render_packet_cursor.words[0] = 0x30000014;
        fade_in_alpha = frame * 4;
        if (frame <= 0x1F) {
            alpha = fade_in_alpha;
        }
        render_packet_cursor.words[1] = (s32)second_clear_packet;
        render_packet_cursor.words[2] = 0;
        render_packet_cursor.words[3] = 0x50000014;
        render_packet_cursor.words += 4;
        if (duration_ticks - 0x10 < frame) {
            alpha = (duration_ticks - frame) * 8;
        }
        scroll_phase = convert_integer_to_float(frame % 600) * 0.0016666667f;
        if (first_slide == second_slide) {
            append_scrolling_textured_quad(0, screen_extent.half_height - 0x20, 0x200, 0x40,
                                           (alpha << 24) | 0x808080, shared_texture, 0.0f, 4.0f,
                                           scroll_phase + 0.0f, scroll_phase + 0.4f);
            draw_textured_quad(0, screen_extent.half_height - 0x20, 0x200, 0x40, 0, 0, 0x200, 0x40,
                               0x80808080, first_texture);
        } else {
            scroll_start = scroll_phase + 0.0f;
            scroll_end = scroll_phase + 0.4f;
            append_scrolling_textured_quad(0, screen_extent.half_height - 0x2E, 0x200, 0x40,
                                           (alpha << 24) | 0x808080, shared_texture, 0.0f, 4.0f,
                                           scroll_start, scroll_end);
            draw_textured_quad(0, screen_extent.half_height - 0x2E, 0x200, 0x40, 0, 0, 0x200, 0x40,
                               0x80808080, first_texture);
            if (frame > 0x40) {
                if (frame < 0x60) {
                    alpha = (frame - 0x40) * 4;
                }
                append_scrolling_textured_quad(0, screen_extent.half_height, 0x200, 0x40,
                                               (alpha << 24) | 0x808080, shared_texture, 0.0f, 4.0f,
                                               scroll_start, scroll_end);
                draw_textured_quad(0, screen_extent.half_height, 0x200, 0x40, 0, 0, 0x200, 0x40,
                                   0x80808080, second_texture);
            }
        }
        memcard_update_state();
        run_state_handler();
        vu1_sync_chain(1);
        sceGsSyncV(0);
        reset_gs_registers_pr();
        vu1_send_chain();
        swap_render_buffer_chain();
        if (preload_level != 0) {
            if (service_level_archive_load() == 0) {
                if (duration_ticks < frame + 0x14) {
                    duration_ticks = frame + 0x14;
                }
            } else {
                preload_level = 0;
            }
        }
    }
    fade_to_black(2);
}

extern __typeof__(play_level_loading_slides) func_00231BD8 __attribute__((alias("FUN_00231bd8")));
