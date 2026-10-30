/* Text: draws the dialog background and text, and the page contents. */
#include "types.h"
#include "asm.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/rendering/texture/do_gif_paging.h"
#include "rnc/rendering/texture/setup_gif_paging.h"
#include "rnc/rendering/debug/draw_debug_profiler.h"
#include "rnc/rendering/packets/append_gif_transfer_packet.h"
#include "rnc/rendering/packets/emit_rgba_draw_packet.h"
#include "rnc/rendering/frame/reset_gs_registers.h"
#include "rnc/overlay/shared/ui/menus/initialize_pause_menu.h"

extern int D_L00_0015F5D8;
int        FUN_L00_00201720(void);
/* The level overlay's own copy of draw_dialog_text: same code, own data. */
void       level_draw_dialog_text(void) __asm__("FUN_L00_002018c8");
extern s32 D_L00_0015F3F4_gp __asm__("D_L00_0015F3F4") __attribute__((sda));
void       draw_dialog_background_and_text(void) __asm__("FUN_L00_001f8ec8");

void draw_dialog_background_and_text(void) {
    if (D_L00_0015F5D8 == 0) {
        if (FUN_L00_00201720() == 0) {
            append_gif_transfer_packet();
            D_L00_0015F3F4_gp = 0x100ff;
            draw_debug_profiler();
        } else if (FUN_L00_00201720() == 3)
            draw_pause_menu_pages(1);
        emit_rgba_draw_packet(0, 0, 0, 0x40);
        level_draw_dialog_text();
    }
}

extern s32 D_L00_0015F5D8;
/* Calls FUN_L00_002772c0(0) while D_L00_0015F5D8 is clear. */
void draw_page_contents(void) __asm__("FUN_L00_001f8f50");

void draw_page_contents(void) {
    if (D_L00_0015F5D8 == 0)
        draw_pause_menu_pages(0);
}

extern volatile s32 D_L00_0015F628;

#include "rnc/overlay/shared/ui/text/draw_dialog_background_and_text_globals.h"

extern u8 timed_text_entries[] __asm__("D_L00_00173080");
void      queue_framebuffer_upload(s32) __asm__("FUN_L00_001fc558");
void      draw_timed_text(void *, s32) __asm__("FUN_L00_001fcca0");
void      draw_framebuffer_with_timed_text(void) __asm__("FUN_L00_001f8f78");

void draw_framebuffer_with_timed_text(void) {
    s32 i;
    u8 *timed_text_entry;
    if (D_L00_0015F5D8)
        return;
    queue_framebuffer_upload(D_L00_0015F628);
    if (0.0f < fade_level) {
        if (1.0f < fade_level)
            fade_level = 1.0f;
        emit_rgba_draw_packet(0, 0, 0, truncate_float_to_s32(fade_level * 128.0f));
    }
    reset_gs_registers();
    setup_gif_paging(0);
    timed_text_entry = timed_text_entries;
    for (i = 0xB6; i >= 0; i--) {
        draw_timed_text(timed_text_entry, timed_text_frame);
        timed_text_entry += 16;
    }
    do_gif_paging();
}

s32 D_L01_0015F628 = 0;
s32 D_L02_0015F628 = 0;
s32 D_L03_0015F628 = 0;
s32 D_L04_0015F628 = 0;
s32 D_L05_0015F628 = 0;
s32 D_L06_0015F628 = 0;
s32 D_L07_0015F628 = 0;
s32 D_L08_0015F628 = 0;
s32 D_L09_0015F628 = 0;
s32 D_L10_0015F628 = 0;
s32 D_L11_0015F628 = 0;
s32 D_L12_0015F628 = 0;
s32 D_L13_0015F628 = 0;
s32 D_L14_0015F628 = 0;
s32 D_L15_0015F628 = 0;
s32 D_L16_0015F628 = 0;
s32 D_L17_0015F628 = 0;
s32 D_L18_0015F628 = 0;
