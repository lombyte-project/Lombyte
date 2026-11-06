#ifndef LOMBYTE_RNC_MOBYS_ANIMATION_MOBY_ANIM_H
#define LOMBYTE_RNC_MOBYS_ANIMATION_MOBY_ANIM_H

#include "types.h"

/* The animation view of a moby that set_moby_animation and the
   blend_moby_animation pair use (the same bytes as struct Moby). */
typedef struct {
    char          _pad00[0x10];
    unsigned char nframes; /* 0x10 */
    unsigned char sound;   /* 0x11: copied into MobyAnim.seq_sound by a blend */
} AnimSeq;

typedef struct {
    char     _pad00[0x48];
    AnimSeq *seqs[1]; /* 0x48 */
} AnimClass;

typedef struct {
    char          _pad00[0x24];
    AnimClass    *pClass; /* 0x24 */
    char          _pad28[0x50 - 0x28];
    unsigned char frame;      /* 0x50 */
    unsigned char nextFrame;  /* 0x51 */
    unsigned char seq;        /* 0x52 */
    unsigned char prevSeq;    /* 0x53 */
    float         anim_time;  /* 0x54: Moby.anim_time; a blend restarts it, > 0.025 not settled */
    float         anim_speed; /* 0x58: Moby.anim_speed; 1 when a blend starts */
    float         unk5C;      /* 0x5C: 1 / blend frames after a blend */
    int           unk60;      /* 0x60: nonzero: not settled */
    void         *manips;     /* 0x64: Moby.manips; nonzero: not settled */
    float        *frameData;  /* 0x68 */
    char          _pad6C[4];
    unsigned char unk70; /* 0x70: Moby.anim_flags; a blend clears bit 1 */
    char          _pad71[0xB];
    unsigned char seq_sound; /* 0x7C */
    char          _pad7D[0x28];
    unsigned char saved_seq; /* 0xA5: seq before a blend parked the pose */
    char          _padA6[0x4A];
    char          unkF0[0x10]; /* 0xF0: copied into the pose's D_001B2C00 slot */
} MobyAnim;

#endif /* LOMBYTE_RNC_MOBYS_ANIMATION_MOBY_ANIM_H */
