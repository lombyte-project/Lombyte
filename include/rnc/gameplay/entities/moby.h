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
    u32 collision;                    /* collision data; copied into Moby.collision when a moby is (re)classed */
    u8 pad_14[0x8];
    void *retarget_table;             /* retarget table: word table, indexed id * 4 + 4 (0024eec0) */
    struct GifEntry *gifs;            /* patched by patch_moby_gifs */
    f32 scale;                        /* default draw scale: copied into Moby.scale, divides it */
    s32 sound_defs;                   /* sound definitions, 0x20 bytes each */
    void **callbacks;                 /* function-pointer table, called with the moby */
    u8 pad_30[0x14];
    u16 flags;                        /* initial Moby.flags */
    s16 type;                         /* class type; 5 (a creature) is tested by targeting code */
    struct AnimSeq *seqs[1];          /* animation sequences, indexed by Moby.seq */
};

/* A moby instance (MobyInstance in the recovered symbols), 0x100 bytes.
   Only fields some matched function reads or writes are named. */
struct Moby {
    Vec4f bsphere;
    Vec4f pos;
    u8 state;                         /* >= 0xFE: dead, waiting to respawn */
    u8 group;                         /* linked group: index into the level moby-list table D_Lxx_001ABCC0 (0xFF: none) */
    u8 class_slot;                    /* class slot: pclass = D_L00_00197300[class_slot] (crate_update) */
    u8 alpha;                         /* 0x80 by default; 0x40 for the smoke trail FUN_L09_00307ba8 spawns */
    struct MobyClass *pclass;
    struct Moby *next;
    f32 scale;                        /* draw scale (FUN_L01_002fa068 halves it, FUN_L00_00215ef8 divides by it) */
    u8 update_dist;                   /* update distance; 0xFF (always) or 0x7F for beams, set by spawners */
    u8 visible;                       /* drawn last frame; set to 1 by spawners */
    s16 draw_dist;                    /* draw distance; 0xFF (0x7F for beams), set by spawners */
    u16 flags;
    u16 occlusion;                    /* occlusion bits; 0x7F80 set by spawners */
    u64 spawn_frame;                  /* frame count at which it may respawn */
    Vec4f rot;                        /* z: yaw (update_npc_dialogue compares it with atan2 to the hero) */
    u8 frame;                         /* animation frame */
    u8 prev_frame;                    /* frame index in prev_seq */
    u8 seq;                           /* animation sequence id */
    u8 prev_seq;
    f32 anim_time;
    f32 anim_speed;
    u8 pad5C[8];
    struct Manip *manips;
    void *cur_frame_data;
    void *prev_frame_data;
    u8 anim_flags;
    u8 unk71;                         /* set to 0xFF when a moby changes class */
    u8 unk72;
    u8 unk73;
    void (*update)(struct Moby *moby);
    u8 *pvars;
    u8 unk7C;
    u8 sound_slot;                    /* 0x7D: D_0013E550 slot of its looping sound, 0xFF: none (advance_hero_animation) */
    u8 anim_trigger_count;
    u8 shadow_range;                  /* shadow range in units of 1024; 0x17 from seeker_update near D_L09_00166F40 */
    u8 pad80[6];
    s16 unk86;
    u8 pad88[8];
    s32 glow_color;                   /* glow colour (class header + 0x40) */
    u32 collision;                    /* collision data, from the class header (MobyClass.collision) */
    s32 collision_disable;            /* per-primitive collision disable bits; 1 while a carrier holds the moby (glove_of_doom_update) */
    u8 pad9C[8];
    u8 hit_slot;                      /* hit-message slot (0xFF: none) */
    u8 padA5;
    s16 oclass;
    u8 padA8[8];
    u8 mission;                       /* 0xB0: index into the level's D_0014C050 row (0xFF: not spawned) */
    u8 padB1;
    u16 save_id;                      /* index into the level collected[]/killed[] tables and save bits D_0014C190[level][id >> 5] */
    s16 unkB4;
    u8 padB6[2];
    void *parent;                     /* 0xB8: spawner (bolt source record); its byte 0xB1 is a per-level id (FUN_L00_002a6b70) */
    u8 command;                       /* external command byte */
    u8 padBD[3];
    Vec4f rot_row0;                   /* 0xC0: rows of the matrix built from rot (euler_rotation_basis) */
    Vec4f rot_row1;
    Vec4f rot_row2;
    u8 padF0[0x10];
};

#endif /* LOMBYTE_RNC_GAMEPLAY_ENTITIES_MOBY_H */
