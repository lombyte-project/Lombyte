#ifndef LOMBYTE_RNC_OVERLAY_L13_MOBYS_PROJECTILE_MOBY_H
#define LOMBYTE_RNC_OVERLAY_L13_MOBYS_PROJECTILE_MOBY_H

/* The moby, class and hero fields that the level 13 homing rocket and mine
   classes read (homing_rocket.c, mine.c). */

#include "types.h"

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) ProjectileVec4;

typedef struct {
    char  pad0[0x24];
    float scale;
} ProjectileMobyClass;

typedef struct ProjectileMoby {
    char                 pad0[0x10];
    ProjectileVec4       pos;
    unsigned char        state;
    char                 pad21[3];
    ProjectileMobyClass *cls;
    char                 pad28[4];
    float                scale;
    char                 pad30;
    unsigned char        b31;
    char                 pad32[0xE];
    ProjectileVec4       rot;
    char                 pad50[0x28];
    void                *data;
    char                 pad7C[0x2A];
    short                cls_id;
    char                 padA8[0x18];
    ProjectileVec4       mat;
} ProjectileMoby;

typedef struct {
    char            pad0[0x15F0];
    ProjectileMoby *veh;
} ProjectileHero;

#endif /* LOMBYTE_RNC_OVERLAY_L13_MOBYS_PROJECTILE_MOBY_H */
