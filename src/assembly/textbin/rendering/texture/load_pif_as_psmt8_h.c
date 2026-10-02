#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/rendering/texture/load_pif_as_psmt8_h/FUN_001e9168.s", FUN_001e9168);
#else
#include "types.h"

/* sceGsLoadImage */
typedef struct {
    long q[12];
} GsLoadImage __attribute__((aligned(16)));

typedef struct {
    void *clut;    /* 0x00 */
    void *image;   /* 0x04 */
    int unk08[3];
    int clutSize;  /* 0x14 */
    int imageSize; /* 0x18 */
    int unk1C[4];
    int tbp;       /* 0x2C */
    int unk30[3];
    int tbw;       /* 0x3C */
    int unk40[3];
    int tw;        /* 0x4C */
    int th;        /* 0x50 */
} PifTex; /* 0x54 */

typedef struct {
    char unk00[8];
    int w;   /* 0x08 */
    int h;   /* 0x0C */
    int unk10;
    int psm; /* 0x14 */
} PifHeader;

extern void FillTransferWords(void *, int, int);
extern s32 func_001F97A0(s32);
extern s32 sceGsSetDefLoadImage(GsLoadImage *, short, short, short, short, short,
                                short, short);
extern void FlushCache(int);
extern s32 sceGsExecLoadImage(GsLoadImage *, void *);
extern s32 func_00120558(int, unsigned short);

/* LoadPifAsPSMT8H: uploads a PIF image (header arg0, CLUT at +0x20 of
   0x200 bytes when +0x14 is set, else 0x400, then w * h pixels) to GS
   memory: the 16x16 CLUT to block cbp >> 8, the pixels as PSMT8H to
   tbp >> 8 with a buffer width of w / 64 (at least 1), and writes the
   matching TEX0 (SCE_GS_SET_TEX0 with TCC 1, CLD 4) plus 1, 0 to arg1.
   The header's fields are ints cast to short at the call, which is where
   retail's second, narrower load of each comes from. The descriptor is a
   zeroed stack struct, so its fields are re-read after the calls. */
void load_pif_as_psmt8_h(void *arg0, void *arg1, int tbp, int cbp) __asm__("FUN_001e9168");

void load_pif_as_psmt8_h(void *arg0, void *arg1, int tbp, int cbp) {
    PifTex d;
    GsLoadImage li;
    PifHeader *pif = arg0;
    unsigned long *out = arg1;
    int c;
    long t;
    long u;
    long v;

    FillTransferWords(&d, 0, sizeof(d));
    d.clut = (char *)pif + 0x20;
    if (pif->psm == 0) {
        d.clutSize = 0x400;
    } else {
        d.clutSize = 0x200;
    }
    c = cbp >> 8;
    d.tw = func_001F97A0(pif->w);
    d.th = func_001F97A0(pif->h);
    d.image = (char *)pif + (d.clutSize + 0x20);
    d.imageSize = pif->w * pif->h;
    sceGsSetDefLoadImage(&li, c, 1, (short)pif->psm, 0, 0, 16, 16);
    FlushCache(0);
    sceGsExecLoadImage(&li, d.clut);
    func_00120558(0, 0);
    d.tbw = pif->w >> 6;
    if (d.tbw <= 0) {
        d.tbw = 1;
    }
    d.tbp = tbp >> 8;
    sceGsSetDefLoadImage(&li, d.tbp, d.tbw, 0x1B, 0, 0, (short)pif->w,
                        (short)pif->h);
    FlushCache(0);
    sceGsExecLoadImage(&li, d.image);
    func_00120558(0, 0);
    v = (long)0x1B << 20;
    t = (long)d.tbp | ((long)d.tbw << 14);
    u = ((long)d.tw << 26) | v;
    t |= u;
    t |= (long)d.th << 30;
    v = ((long)c << 37) | ((long)1 << 34);
    t |= v;
    t |= (long)pif->psm << 51;
    t |= (unsigned long)1 << 63;
    out[0] = t;
    out[1] = 1;
    out[2] = 0;
}
#endif /* NON_MATCHING */
