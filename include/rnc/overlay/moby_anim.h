#ifndef LOMBYTE_RNC_OVERLAY_MOBY_ANIM_H
#define LOMBYTE_RNC_OVERLAY_MOBY_ANIM_H

#include "types.h"

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
    AnimClass *pClass; /* 0x24 */
    char _pad28[0x50 - 0x28];
    unsigned char frame;     /* 0x50 */
    unsigned char nextFrame; /* 0x51 */
    unsigned char seq;       /* 0x52 */
    unsigned char prevSeq;   /* 0x53 */
    char _pad54[0x5C - 0x54];
    float unk5C; /* 0x5C */
    char _pad60[0x68 - 0x60];
    float *frameData; /* 0x68 */
    char _pad6C[4];
    unsigned char unk70; /* 0x70 */
} MobyAnim;

#endif /* LOMBYTE_RNC_OVERLAY_MOBY_ANIM_H */
