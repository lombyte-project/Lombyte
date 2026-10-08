/* Ported from rac1-decomp (src/game/framebuf.c,
   func_001FB608). */
#include "sda.h"
#include "rnc/rendering/dma_tag.h"

/* Sets up a (1 << a) x (1 << b) render target at GS address c: records
   TEX0 for it in D_0015EED0 (buffer width 1 << max(a - 6, 1) pages,
   PSMCT32, TCC), its size and FBP (c >> 13) in D_00151780, then appends
   a packet to render_packet_cursor.words: a default draw environment (sceGsSetDefDrawEnv
   with ztest 3) patched with the FBP and the configured Z buffer,
   followed by TEST_1 0x30003 and a black sprite over the whole target.
   FBP is set through the sceGsFrame bitfield, the form that
   sign-extends the halfword before masking as retail does. */
typedef struct {
    char unk_000[0x160];
    short w;   /* 0x160 */
    short h;   /* 0x162 */
    short psm; /* 0x164 */
    short fbp; /* 0x166 */
    short unk_168[2];
    short zpsm; /* 0x16C */
    short zbp;  /* 0x16E */
} FrameCfg;

typedef struct {
    unsigned long FBP : 9;
    unsigned long pad09 : 7;
    unsigned long FBW : 6;
    unsigned long pad22 : 2;
    unsigned long PSM : 6;
    unsigned long pad30 : 2;
    unsigned long FBMSK : 32;
} GsFrame; /* sceGsFrame */

extern FrameCfg D_00151780;
extern long D_0015EED0 MACRO_ADDR;
extern void FUN_001f9810(void *, int);
extern int sceGsSetDefDrawEnv(void *, short, short, short, short, short);

void FUN_001fb440(int a, int b, int c) {
    int x;
    long *q;
    unsigned long *d;
    long *r;

    x = a - 6;
    if (x <= 0) {
        x = 1;
    }
    D_00151780.fbp = c >> 13;
    D_00151780.w = 1 << a;
    D_00151780.h = 1 << b;
    D_0015EED0 = (long)(c >> 8) | ((long)(1 << x) << 14) | ((long)a << 26) |
                 ((unsigned long)b << 30) | ((unsigned long)1 << 34);
    FUN_001f9810(render_packet_cursor.words, 0xF0);
    render_packet_cursor.words[0] = 0x1000000E;
    render_packet_cursor.words[1] = 0;
    render_packet_cursor.words[2] = 0;
    render_packet_cursor.words[3] = 0x5000000E;
    render_packet_cursor.words += 4;
    q = (long *)render_packet_cursor.words;
    q[0] = 0x1000000000000008L;
    q[1] = 0xE;
    render_packet_cursor.words += 4;
    d = (unsigned long *)render_packet_cursor.words;
    sceGsSetDefDrawEnv(d, D_00151780.psm, D_00151780.w, D_00151780.h, 3, 0);
    ((GsFrame *)d)->FBP = D_00151780.fbp;
    d[2] = D_00151780.zbp | ((unsigned long)(D_00151780.zpsm & 0xF) << 24);
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
    r[8] = (long)(0x8000 - D_00151780.w * 8) | ((long)(0x8000 - D_00151780.h * 8) << 16);
    r[9] = (long)(D_00151780.w * 8 + 0x8000) | ((long)(D_00151780.h * 8 + 0x7FF0) << 16);
    render_packet_cursor.words += 0x14;
}

extern __typeof__(FUN_001fb440) func_001FB440 __attribute__((alias("FUN_001fb440")));
