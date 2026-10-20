/* Ported from rac1-decomp (src/game/framebuf.c,
   func_001FB608). */
#include "sda.h"
#include "rnc/rendering/dma_tag.h"
#include "rnc/rendering/fs_aa_buffer.h"
#include "types.h"

/* Sets up a (1 << a) x (1 << b) render target at GS address c: records
   TEX0 for it in D_0015EED0 (buffer width 1 << max(a - 6, 1) pages,
   PSMCT32, TCC), its size and FBP (c >> 13) in D_00151780, then appends
   a packet to render_packet_cursor.words: a default draw environment (sceGsSetDefDrawEnv
   with ztest 3) patched with the FBP and the configured Z buffer,
   followed by TEST_1 0x30003 and a black sprite over the whole target.
   FBP is set through the sceGsFrame bitfield, the form that
   sign-extends the halfword before masking as retail does. */
typedef struct {
    unsigned long FBP : 9;
    unsigned long pad09 : 7;
    unsigned long FBW : 6;
    unsigned long pad22 : 2;
    unsigned long PSM : 6;
    unsigned long pad30 : 2;
    unsigned long FBMSK : 32;
} GsFrame; /* sceGsFrame */

extern void fast_mem_zero16(void *, int) __asm__("FUN_001f9810");
extern int sceGsSetDefDrawEnv(void *, short, short, short, short, short);

extern long capture_texture_tex0 __asm__("D_0015EED0") MACRO_ADDR;
void setup_render_target(int a, int b, int c) __asm__("FUN_001fb440");

void setup_render_target(int width_log2, int b, int framebuffer_address) {
    int x;
    long *draw_environment_gif_tag;
    unsigned long *draw_environment;
    long *r;

    x = width_log2 - 6;
    if (x <= 0) {
        x = 1;
    }
    fs_aa_buffer.target_fbp = framebuffer_address >> 13;
    fs_aa_buffer.target_width = 1 << width_log2;
    fs_aa_buffer.target_height = 1 << b;
    capture_texture_tex0 = (long)(framebuffer_address >> 8) | ((long)(1 << x) << 14) | ((long)width_log2 << 26) |
                 ((unsigned long)b << 30) | ((unsigned long)1 << 34);
    fast_mem_zero16(render_packet_cursor.words, 0xF0);
    render_packet_cursor.words[0] = 0x1000000E;
    render_packet_cursor.words[1] = 0;
    render_packet_cursor.words[2] = 0;
    render_packet_cursor.words[3] = 0x5000000E;
    render_packet_cursor.words += 4;
    draw_environment_gif_tag = (long *)render_packet_cursor.words;
    draw_environment_gif_tag[0] = 0x1000000000000008L;
    draw_environment_gif_tag[1] = 0xE;
    render_packet_cursor.words += 4;
    draw_environment = (unsigned long *)render_packet_cursor.words;
    sceGsSetDefDrawEnv(draw_environment, fs_aa_buffer.target_psm, fs_aa_buffer.target_width, fs_aa_buffer.target_height, 3, 0);
    ((GsFrame *)draw_environment)->FBP = fs_aa_buffer.target_fbp;
    draw_environment[2] = fs_aa_buffer.zbp | ((unsigned long)(fs_aa_buffer.zpsm & 0xF) << 24);
    render_packet_cursor.words += 0x20;
    r = (long *)render_packet_cursor.words;
    r[0] = 0x1000000000000001L;
    r[1] = 0xE;
    r[2] = 0x30003;
    r[3] = 0x47;
    r[4] = 0x4400000000008001L;
    r[5] = 0x4410;
    r[6] = 0x106;
    r[7] = 0;
    r[8] = (long)(0x8000 - fs_aa_buffer.target_width * 8) | ((long)(0x8000 - fs_aa_buffer.target_height * 8) << 16);
    r[9] = (long)(fs_aa_buffer.target_width * 8 + 0x8000) | ((long)(fs_aa_buffer.target_height * 8 + 0x7FF0) << 16);
    render_packet_cursor.words += 0x14;
}

extern __typeof__(setup_render_target) func_001FB440 __attribute__((alias("FUN_001fb440")));
