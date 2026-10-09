#ifndef LOMBYTE_RNC_GAMEPLAY_ENTITIES_MOBY_H
#define LOMBYTE_RNC_GAMEPLAY_ENTITIES_MOBY_H

#include "types.h"
#include "rnc/math/vector.h"

struct Manip;
struct GifEntry;
struct AnimSeq;

/* A moby class header, pointed to by Moby.pclass. Size unknown;
   only fields some matched function reads are named. */
struct MobyClass {
    u8 pad_0[0xC];
    u8 seq_count;                     /* number of entries in seqs[] (menu previews clamp the sequence to it) */
    u8 pad_D[3];
    u32 unk10;                        /* copied into Moby.unk94 when a moby is (re)classed */
    u8 pad_14[0x8];
    void *unk1C;                      /* word table, indexed id * 4 + 4 (0024eec0) */
    struct GifEntry *gifs;            /* patched by patch_moby_gifs */
    f32 scale;                        /* default draw scale: copied into Moby.scale, divides it */
    s32 unk28;
    void **callbacks;                 /* function-pointer table, called with the moby */
    u8 pad_30[0x14];
    u16 flags;                        /* initial Moby.flags */
    s16 unk46;                        /* class category; 5 is tested by targeting code */
    struct AnimSeq *seqs[1];          /* animation sequences, indexed by Moby.seq */
};

/* A moby instance (MobyInstance in the recovered symbols), 0x100 bytes.
   Only fields some matched function reads or writes are named. */
struct Moby {
    Vec4f bsphere;
    Vec4f pos;
    u8 state;                         /* >= 0xFE: dead, waiting to respawn */
    u8 group;                         /* linked group: index into the level moby-list table D_Lxx_001ABCC0 (0xFF: none) */
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
    u8 unk7F;                         /* set to 0x17 by FUN_L09_002c5990 near D_L09_00166F40 */
    u8 pad80[0x10];
    s32 unk90;
    u32 unk94;                        /* set from the class header's word 0x10 */
    s32 unk98;                        /* set to 1 while a carrier holds the moby (FUN_L00_002c7a58) */
    u8 pad9C[8];
    u8 unkA4;
    u8 padA5;
    s16 oclass;
    u8 padA8[8];
    u8 unkB0;                         /* 0xB0: index into the level's D_0014C050 row (0xFF: not spawned) */
    u8 padB1;
    u16 save_id;                      /* index into the level collected[]/killed[] tables and save bits D_0014C190[level][id >> 5] */
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
