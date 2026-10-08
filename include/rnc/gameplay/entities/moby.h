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
    u8 unk21;
    u8 unk22;                         /* class slot: pclass = D_L00_00197300[unk22] (FUN_L00_002cf218) */
    u8 unk23;                         /* 0x40 for the smoke trail FUN_L09_00307ba8 spawns */
    struct MobyClass *pclass;
    struct Moby *next;
    f32 scale;                        /* draw scale (FUN_L01_002fa068 halves it, FUN_L00_00215ef8 divides by it) */
    u8 unk30;                         /* set to 0xFF (0x7F for beams) by spawners */
    u8 unk31;                         /* set to 1 by spawners */
    s16 unk32;                        /* set to 0xFF (0x7F for beams) by spawners */
    u16 flags;
    u16 unk36;                        /* set to 0x7F80 by spawners */
    u64 spawn_frame;                  /* frame count at which it may respawn */
    Vec4f rot;                        /* z: yaw (FUN_L00_00266448 compares it with atan2 to the hero) */
    u8 frame;                         /* animation frame */
    u8 prev_frame;                    /* frame index in prev_seq */
    u8 seq;                           /* animation sequence id */
    u8 prev_seq;
    f32 unk54;
    f32 unk58;
    u8 pad5C[8];
    struct Manip *manips;
    void *cur_frame_data;
    void *prev_frame_data;
    u8 unk70;
    u8 unk71;                         /* set to 0xFF when a moby changes class */
    u8 unk72;
    u8 unk73;
    void (*update)(struct Moby *moby);
    u8 *pvars;
    u8 unk7C;
    u8 pad7D;
    u8 unk7E;
    u8 unk7F;
    u8 pad80[0x10];
    s32 unk90;
    u32 unk94;                        /* set from the class header's word 0x10 */
    u8 pad98[0xC];
    u8 unkA4;
    u8 padA5;
    s16 oclass;
    u8 padA8[8];
    u8 unkB0;                         /* 0xB0: index into the level's D_0014C050 row (0xFF: not spawned) */
    u8 padB1;
    u16 unkB2;
    s16 unkB4;
    u8 padB6[2];
    void *unkB8;                      /* 0xB8: bolt source record; its byte 0xB1 is a per-level id (FUN_L00_002a6b70) */
    u8 unkBC;
    u8 padBD[3];
    Vec4f unkC0;                      /* 0xC0: first row of a matrix built from rot (FUN_001fa030) */
    Vec4f unkD0;
    Vec4f unkE0;
    u8 padF0[0x10];
};

#endif /* LOMBYTE_RNC_GAMEPLAY_ENTITIES_MOBY_H */
