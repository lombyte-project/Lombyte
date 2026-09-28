/* Ported from rac1-decomp, the PAL decompilation (src/game/mobyutil.c, func_00213DE0). */
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
/* As func_00213F28 below, for a plain sequence change: clamp the frame
   to the sequence's last one; if the moby hasn't settled, park the
   current pose in a D_001B2C00 blend slot (seq 0xFF, frame = slot). Then
   set the new sequence and frame and arm the blend timer from arg3. */
void blend_moby_animation(MobyAnim *arg0, int arg1, int arg2, int arg3) __asm__("FUN_00212f90");

void blend_moby_animation(MobyAnim *arg0, int arg1, int arg2, int arg3) {
    int n = arg0->pClass->seqs[arg1]->nframes;
    int slot;
    unsigned char oldSeq;
    float scale;

    if (arg2 >= n) {
        arg2 = n - 1;
    }
    if (*(float *)((char *)arg0 + 0x54) > 0.025f ||
        *(int *)((char *)arg0 + 0x60) != 0 ||
        *(int *)((char *)arg0 + 0x64) != 0) {
        slot = func_0020CC18((int)arg0);
        if (slot >= 0) {
            FUN_0020ede8(arg0, slot | 0x300);
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

extern __typeof__(blend_moby_animation) func_00212F90 __attribute__((alias("FUN_00212f90")));
