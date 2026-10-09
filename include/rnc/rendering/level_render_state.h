#ifndef LOMBYTE_RNC_RENDERING_LEVEL_RENDER_STATE_H
#define LOMBYTE_RNC_RENDERING_LEVEL_RENDER_STATE_H

#include "types.h"
#include "rnc/math/vector.h"

struct ResidentRenderObject;

/* Level-wide scene state: the scripted cinematic camera (path, blend) and the
   two 32-entry position histories the trail effect draws from. */
typedef struct LevelRenderState {
    struct ResidentRenderObject *player; /* 0x00 */
    u8 pad4[4];
    struct ResidentRenderObject *attachment; /* 0x08 */
    u8 padC[4];
    struct ResidentRenderObject *companion_a; /* 0x10 */
    struct ResidentRenderObject *companion_b; /* 0x14 */
    u8 pad18[8];
    s32 state;           /* 0x20: 4 after level init */
    s16 timer;           /* 0x24 */
    s16 content_variant; /* 0x26: picks the effect and flare tables */
    s16 skip;            /* 0x28 */
    s16 unk2A;           /* 0x2A: set to 1 by startlevel */
    s16 unk2C; /* 0x2C */
    u8 pad2E[2];
    s32 path;                     /* 0x30 */
    s32 source_camera_index;      /* 0x34 */
    s32 destination_camera_index; /* 0x38 */
    f32 path_progress;            /* 0x3C */
    f32 speed;                    /* 0x40 */
    f32 path_segment_length;      /* 0x44 */
    f32 blend;                    /* 0x48 */
    f32 interpolation_velocity;   /* 0x4C */
    s32 history_index;            /* 0x50 */
    s32 history_count;            /* 0x54 */
    s32 scene_mode;               /* 0x58: scene variant set at level init */
    s32 scene_state;              /* 0x5C */
    Vec4 unk60;                   /* 0x60 */
    Vec4 unk70;                   /* 0x70 */
    Vec4 startPos;                /* 0x80 */
    Vec4 pathPos;                 /* 0x90 */
    Vec4 startRot;                /* 0xA0 */
    f32 unkB0;
    f32 rotY; /* 0xB4 */
    f32 rotZ; /* 0xB8 */
    f32 unkBC;
    Vec4 primary_history[32];   /* 0xC0 */
    Vec4 secondary_history[32]; /* 0x2C0 */
} LevelRenderState;

extern LevelRenderState level_render_state __asm__("D_0013E030");

#endif /* LOMBYTE_RNC_RENDERING_LEVEL_RENDER_STATE_H */
