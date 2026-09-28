/* Ported from rac1-decomp, the PAL decompilation (src/game/mobyutil.c, func_00213F28). */
#include "qcopy.h"
extern int func_0020CC18(int v);
typedef struct {
    char _pad00[0x10];
    unsigned char nframes; /* 0x10 */
} AnimSeq;
typedef struct {
    char _pad00[0x48];
    AnimSeq *seqs[1]; /* 0x48 */
} AnimClass;
typedef struct {
    char _pad00[0x24];
    AnimClass *pClass;       /* 0x24 */
    char _pad28[0x50 - 0x28];
    unsigned char frame;     /* 0x50 */
    unsigned char nextFrame; /* 0x51 */
    unsigned char seq;       /* 0x52 */
    unsigned char prevSeq;   /* 0x53 */
    char _pad54[0x5C - 0x54];
    float unk5C;             /* 0x5C */
    char _pad60[0x68 - 0x60];
    float *frameData;        /* 0x68 */
    char _pad6C[4];
    unsigned char unk70;     /* 0x70 */
} MobyAnim;
extern void func_0020C880(void *);
extern float func_001FA6C0(int arg0);
extern void FUN_0020ede8(void *, int);
extern char D_001B2C00[];
/* Re-registers this MobyAnim in the shared slot table (D_001B2F40, via
   func_0020CC18) when it hasn't settled yet (unk54 > 0.025, or unk60/unk64
   nonzero) or the caller forces it (arg4 & 4): builds a flags byte from
   arg4 bits 0/1 (0x100/0x200), hands it to FUN_0020ede8, snapshots this
   moby's unkF0 vector into the matching D_001B2C00 slot, remembers the
   old seq in unkA5 (unless it was already 0xFF), then marks seq 0xFF and
   frame = slot. Either way it then sets nextFrame/prevSeq from arg2/arg1,
   refreshes frame pointers (func_0020C880), arms the timer (unk58 = 1),
   resets unk54, clears unk70 bit 1, stores 1/func_001FA6C0(arg3) into
   unk5C, and copies a not-yet-named byte (offset 0x11) out of
   pClass->seqs[arg1] into unk7C. */
void blend_moby_animation_ex(MobyAnim *arg0, int arg1, int arg2, int arg3, int arg4) __asm__("FUN_002130d8");

void blend_moby_animation_ex(MobyAnim *arg0, int arg1, int arg2, int arg3, int arg4) {
    int slot;
    int flags;
    unsigned char oldSeq;
    float scale;

    if (*(float *)((char *)arg0 + 0x54) > 0.025f ||
        *(int *)((char *)arg0 + 0x60) != 0 ||
        *(int *)((char *)arg0 + 0x64) != 0 || (arg4 & 4)) {
        slot = func_0020CC18((int)arg0);
        if (slot >= 0) {
            flags = (arg4 & 1) ? (slot | 0x100) : slot;
            FUN_0020ede8(arg0, (arg4 & 2) ? (flags | 0x200) : flags);
            qcopy(D_001B2C00 + slot * 0x10, (char *)arg0 + 0xF0);
            oldSeq = arg0->seq;
            if (oldSeq != 0xFF) {
                *(unsigned char *)((char *)arg0 + 0xA5) = oldSeq;
            }
            arg0->seq = 0xFF;
            arg0->frame = slot;
        }
    }
    arg0->nextFrame = arg2;
    arg0->prevSeq = arg1;
    func_0020C880(arg0);
    *(float *)((char *)arg0 + 0x58) = 1.0f;
    scale = 1.0f / func_001FA6C0(arg3);
    *(float *)((char *)arg0 + 0x54) = 0.0f;
    arg0->unk70 = (unsigned char)(arg0->unk70 & 0xFD);
    arg0->unk5C = scale;
    *(unsigned char *)((char *)arg0 + 0x7C) =
        *((unsigned char *)arg0->pClass->seqs[arg1] + 0x11);
}

extern __typeof__(blend_moby_animation_ex) func_002130D8 __attribute__((alias("FUN_002130d8")));
