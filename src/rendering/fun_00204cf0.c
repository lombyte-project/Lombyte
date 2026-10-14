/* Ported from rac1-decomp (src/game/map.c, func_00205520). */
#include "sda.h"
#include "rnc/rendering/texture_upload.h"
#include "types.h"

extern int FUN_001f97a0(int);
typedef struct {
    char *clut; /* 0x00 */
    char *pix;  /* 0x04 */
    char pad08[0xC];
    int clutSize; /* 0x14 */
    char pad18[0x34];
    int tw; /* 0x4C */
    int th; /* 0x50 */
    char pad54[0xC];
} TexDesc;

/* Loads a 256-colour texture file p (width at +8, height at +0xC, CLUT
   at +0x20, pixels after it): the CLUT and the pixels take the next VRAM
   space at D_0015EE74 (cbp, then tbp 0x400 later, then the allocator
   moves on by 1 << (tw + th)), and the texture is registered and its GS
   TEX0 value returned. */
long FUN_00204cf0(char *p) __asm__("FUN_00204cf0");

extern s32 gs_texture_allocation_cursor __asm__("D_0015EE74") MACRO_ADDR;
extern s32 pending_texture_upload_count_macro __asm__("D_0015F458") MACRO_ADDR;
long FUN_00204cf0(char *p) {
    TexDesc t;
    int w;
    int cbp;
    int tbp;
    int addr;
    long reg;
    long tex0;
    long *pt;

    t.clut = p + 0x20;
    t.clutSize = 0x400;
    t.tw = FUN_001f97a0(*(int *)(p + 8));
    t.th = FUN_001f97a0(*(int *)(p + 0xC));
    t.pix = p + 0x20 + t.clutSize;
    addr = gs_texture_allocation_cursor;
    cbp = addr >> 8;
    addr += 0x400;
    tbp = addr >> 8;
    gs_texture_allocation_cursor = addr + (1 << (t.tw + t.th));
    w = t.tw - 6;
    if (w < 0) {
        w = 0;
    }
    w = 1 << w;
    tex0 = (long)tbp | ((long)w << 14) | ((long)0x13 << 20) | ((long)t.tw << 26);
    pt = &tex0;
    tex0 = *pt | ((long)t.th << 30) | ((long)1 << 34) | ((long)cbp << 37);
    reg = *pt | ((long)4 << 61);
    if (pending_texture_upload_count_macro < TEXTURE_UPLOAD_MAX) {
        pending_texture_uploads[pending_texture_upload_count_macro].clut_data = (int)t.clut;
        pending_texture_uploads[pending_texture_upload_count_macro].cbp = cbp;
        pending_texture_uploads[pending_texture_upload_count_macro].unk4 = 0;
        pending_texture_uploads[pending_texture_upload_count_macro].image_data = (int)t.pix;
        pending_texture_uploads[pending_texture_upload_count_macro].tw = t.tw;
        pending_texture_uploads[pending_texture_upload_count_macro].th = t.th;
        pending_texture_uploads[pending_texture_upload_count_macro].tbp = tbp;
        pending_texture_upload_count_macro++;
    }
    return reg;
}

extern __typeof__(FUN_00204cf0) func_00204CF0 __attribute__((alias("FUN_00204cf0")));
