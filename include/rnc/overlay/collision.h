#ifndef LOMBYTE_RNC_OVERLAY_COLLISION_H
#define LOMBYTE_RNC_OVERLAY_COLLISION_H

#include "types.h"
#include "rnc/overlay/quad.h"
#include "sda.h"

/*
 * Collision hit record filled by the overlay collision queries
 * (coll_sphere, rig_probe, coll_line_fix) at D_L00_00173E40
 * (per-level copies at other addresses). FUN_L00_002133a8 copies +0x30 into
 * hero.motion.pos, +0x40 into hero.wall_contact_normal, +0x20 into hero.wall_contact_point and +0x18 into
 * hero.coll_hit_moby.
 */
typedef struct {
    u8 pad0[0x18];
    s32 moby; /* 0x18: passed to suck_get_aim_height (reads moby+0x34, moby+0x78); 0 when no moby was hit */
    s32 unk1C; /* 0x1C: > 0 gates copying point and the normal into the hero (shared/hero/air_accel.c) */
    union {
        OvlQuad q;
        f32     f[4];
    } point;        /* 0x20: f[2] is read as the ground height */
    u8 pad30[0x10]; /* 0x30: position after push-out (copied into hero.motion.pos) */
    f32 normal_x; /* 0x40: start of the surface normal; its angle is tested against 50 degrees (0.87266463) */
    f32 normal_y; /* 0x44: fast_arc_tan (atan2) of normal_x, normal_y is stored in the hero */
    f32 normal_z; /* 0x48: ledge probes compare atan2(normal_z, xy length) with 20 degrees */
} CollisionHit;

struct Moby;

/* Fields of the L00 hit record (D_L00_00173E40), read through their own labels. */
extern struct Moby *coll_hit_moby __asm__("D_L00_00173E58") NOT_SDA;   /* coll_hit.moby */
extern f32          collision_hit_z __asm__("D_L00_00173E68") NOT_SDA; /* coll_hit.point.f[2] */
extern OvlVec4 coll_hit_position __asm__("D_L00_00173E70") NOT_SDA;    /* position after push-out */
extern OvlVec4
    coll_hit_normal __asm__("D_L00_00173E80") NOT_SDA; /* coll_hit.normal_x .. normal_z */

#endif /* LOMBYTE_RNC_OVERLAY_COLLISION_H */
