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
    s32 unk2C;
    u8 pad30[4];
    u16 flags;
    u8 pad36[2];
    u64 spawn_frame;                  /* frame count at which it may respawn */
    u8 pad40[0x10];
    u8 frame;                         /* animation frame */
    u8 prev_frame;                    /* frame index in prev_seq */
    u8 seq;                           /* animation sequence id */
    u8 prev_seq;
    u8 pad54[0x10];
    struct Manip *manips;
    void *cur_frame_data;
    void *prev_frame_data;
    u8 pad70[4];
    void (*update)(struct Moby *moby);
    u8 *pvars;
    u8 unk7C;
    u8 pad7D;
    u8 unk7E;
    u8 pad7F[0x27];
    s16 oclass;
    u8 padA8[0x58];
};

#endif /* LOMBYTE_RNC_GAMEPLAY_ENTITIES_MOBY_H */
