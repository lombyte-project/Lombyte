#ifndef LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_MINE_H
#define LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_MINE_H

#include "types.h"
#include "rnc/overlay/quad.h"

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
    short h36;
    char  p38[8];
    float f40;
    char  p44[4];
    int   i48;
    M910 *p4C;
    short h50;
    short h52;
    M910 *p54;
    int   i58;
    int   i5C;
    float f60;
    float f64;
    int   i68;
    float f6C;
    float f70;
    int   i74;
    int   i78;
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

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_MINE_H */
