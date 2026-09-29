#include "types.h"
#include "sda.h"

typedef struct {
    s32 addr;  /* +0 */
    s16 unk4;  /* +4 */
    s16 cbp;   /* +6 */
    s32 unk8;  /* +8 */
    u8 tw;     /* +C */
    u8 th;     /* +D */
    s16 tbp;   /* +E */
} TexSlot;

extern s32 D_0015F458 MACRO_ADDR;
extern TexSlot D_0018D040[];

/* Registers an 8-bit texture: returns its GS TEX0 value (tbp = tex >> 8,
   tbw = 1 << max(tw - 6, 0), PSMT8, tw, th, tcc 1, cbp = clut >> 8,
   cld 4) and, while there is room (64 entries), records it in the next
   D_0018D040 slot. The tcc term is the one unsigned field: it keeps fold
   from re-pairing the constants, which gives retail's OR tree. */
long FUN_00204e30(s32 tw, s32 th, s32 a2, s32 a3, s32 cbp, s32 tbp) {
    s32 w;
    long reg;

    w = tw - 6;
    if (w < 0) {
        w = 0;
    }
    w = 1 << w;
    cbp >>= 8;
    tbp >>= 8;
    reg = (long)tbp | ((long)w << 14) | ((long)0x13 << 20) | ((long)tw << 26)
        | ((unsigned long)1 << 34) | ((long)cbp << 37) | ((long)4 << 61) | ((long)th << 30);
    if (D_0015F458 < 0x40) {
        D_0018D040[D_0015F458].addr = a2;
        D_0018D040[D_0015F458].cbp = cbp;
        D_0018D040[D_0015F458].unk4 = 0;
        D_0018D040[D_0015F458].unk8 = a3;
        D_0018D040[D_0015F458].tw = tw;
        D_0018D040[D_0015F458].th = th;
        D_0018D040[D_0015F458].tbp = tbp;
        D_0015F458++;
    }
    return reg;
}

extern __typeof__(FUN_00204e30) func_00204E30 __attribute__((alias("FUN_00204e30")));
