#ifndef LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_MINE_H
#define LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_MINE_H

#include "types.h"
#include "rnc/overlay/shared/math/quad.h"

typedef union {
    OvlQuad q;
    float   f[4];
} U910;

typedef struct {
    OvlQuad q[5];
} B910;

typedef struct M910 M910;

typedef struct W910 W910;

struct W910 {
    U910  vel;
    char  p10[0x20];
    M910 *owner;
    short h34;
    short prediction_done; /* 0x36: set once the owner stops the impact preview */
    char  p38[8];
    float homing_speed; /* 0x40: grows toward frame_time * 4 */
    char  p44[4];
    int   homing_phase_timer; /* 0x48: frames of the rise or fall phase */
    M910 *attached_moby;      /* 0x4C: moby the mine came to rest on */
    short in_water;           /* 0x50: set by the splash; gravity is a tenth */
    short homing_phase;       /* 0x52: 1 rise, 2 fall, 0 level */
    M910 *target;             /* 0x54 */
    int   homing_timer;       /* 0x58: the mine explodes when it runs out */
    int   i5C;
    float f60;
    float f64;
    int   voice;           /* 0x68: -1 none */
    float bob_amplitude;   /* 0x6C: set at the first bounce, decays to 0.05 */
    float bob_phase;       /* 0x70 */
    int   bob_start_frame; /* 0x74: frame of the first bounce; the bob ramps in from it */
    int   extended_range;  /* 0x78: nonzero triples the target range for one frame */
};

struct M910 {
    char           p0[0x10];
    U910           pos;
    unsigned char  state;
    char           p21[3];
    char          *p24;
    char           p28[0xC];
    unsigned short flags;
    char           p36[0xA];
    float          rx;
    float          ry;
    float          rz;
    char           p4C[6];
    unsigned char  b52;
    unsigned char  anim;
    char           p54[0x1C];
    unsigned char  b70;
    char           p71[7];
    W910          *v;
    char           p7C[0x18];
    int            i94;
    char           p98[0xC];
    unsigned char  bA4;
    char           pA5;
    short          cls;
    char           pA8[0x14];
    unsigned char  bBC;
    char           pBD[3];
    float          mtx[4][4];
};

typedef struct {
    float          dir[4];
    M910          *moby;
    int            flags;
    unsigned char  b8;
    unsigned char  b9;
    unsigned short cls;
    float          fC;
    int            i10;
} __attribute__((aligned(16))) Qry910;

typedef struct {
    char          p0[0x11];
    unsigned char b11;
} E910;

typedef struct {
    char  p0[0x18];
    M910 *moby;
    int   i1C;
    U910  pos;
    char  p30[0x10];
    U910  nrm;
} H910;

void mine_update(M910 *) __asm__("FUN_L00_002aa670");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_MINE_H */
