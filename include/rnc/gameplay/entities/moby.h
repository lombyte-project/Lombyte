#ifndef LOMBYTE_RNC_GAMEPLAY_ENTITIES_MOBY_H
#define LOMBYTE_RNC_GAMEPLAY_ENTITIES_MOBY_H

#include "types.h"
#include "rnc/math/vector.h"

struct MobyClass;
struct Manip;

/* A moby instance (MobyInstance in the recovered symbols), 0x100 bytes.
   Only fields some matched function reads or writes are named. */
struct Moby {
    Vec4f bsphere;
    Vec4f pos;
    u8 state;                         /* >= 0xFE: dead, waiting to respawn */
    u8 pad21[3];
    struct MobyClass *pclass;
    struct Moby *next;
    f32 scale;                        /* draw scale (FUN_L01_002fa068 halves it, FUN_L00_00215ef8 divides by it) */
    u8 unk30;                         /* set to 0xFF (0x7F for beams) by spawners */
    u8 unk31;                         /* set to 1 by spawners */
    s16 unk32;                        /* set to 0xFF (0x7F for beams) by spawners */
    u16 flags;
    u8 pad36[2];
    u64 spawn_frame;                  /* frame count at which it may respawn */
    Vec4f rot;                        /* z: yaw (FUN_L00_00266448 compares it with atan2 to the hero) */
    u8 frame;                         /* animation frame */
    u8 prev_frame;                    /* frame index in prev_seq */
    u8 seq;                           /* animation sequence id */
    u8 prev_seq;
    u8 pad54[4];
    f32 unk58;
    u8 pad5C[8];
    struct Manip *manips;
    void *cur_frame_data;
    void *prev_frame_data;
    u8 unk70;
    u8 pad71[3];
    void (*update)(struct Moby *moby);
    u8 *pvars;
    u8 unk7C;
    u8 pad7D;
    u8 unk7E;
    u8 pad7F[0x27];
    s16 oclass;
    u8 padA8[0x10];
    void *unkB8;                      /* 0xB8: bolt source record; its byte 0xB1 is a per-level id (FUN_L00_002a6b70) */
    u8 padBC[0x44];
};

#endif /* LOMBYTE_RNC_GAMEPLAY_ENTITIES_MOBY_H */
