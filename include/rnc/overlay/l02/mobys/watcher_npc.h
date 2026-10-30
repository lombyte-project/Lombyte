#ifndef LOMBYTE_RNC_OVERLAY_L02_MOBYS_WATCHER_NPC_H
#define LOMBYTE_RNC_OVERLAY_L02_MOBYS_WATCHER_NPC_H

#include "types.h"
#include "rnc/math/vector.h"

/* The watcher NPC types skids_agent.c and skid_mcmarx.c share. */

/* Pvars of the l02 watcher NPC: it idles, turns its head toward the hero and hands out a reward. */
typedef struct {
    u8    pad0[4];
    s16   talk_state; /* 0x04: 2 once the conversation is over */
    u8    pad6[2];
    u8    zone_check; /* 0x08: 1 when the talk radius depends on a clip volume */
    u8    pad9[3];
    f32   talk_radius; /* 0x0C */
    u8    pad10[0x10];
    void *dialog; /* 0x20 */
    u8    pad24[0x1C];
    u8    body[0x64]; /* 0x40: body joint driver */
    f32   pitch;      /* 0xA4 */
    f32   yaw;        /* 0xA8 */
    u8    padAC[4];
    f32   height; /* 0xB0 */
    u8    padB4[0xC];
    u8    head[0x68]; /* 0xC0: head joint driver */
    f32   head_yaw;   /* 0x128 */
    u8    pad12C[0x3E];
    s16   idle_timer; /* 0x16A */
    s32   zone;       /* 0x16C: clip volume index */
    Vec4f target;     /* 0x170: where it looks when the hero is away */
    s32   unk180;
    u8    pad184[4];
    s32   moving_timer; /* 0x188: frames left looking at the moving hero */
    s32   target_timer; /* 0x18C: frames until a new look target */
} WatcherVars;

/* The reward pickup the watcher drops when it leaves. */
typedef struct {
    u8    pad0[0x10];
    Vec4f pos; /* 0x10 */
    Vec4f vel; /* 0x20 */
    u8    pad30[0x16];
    s16   placed; /* 0x46 */
} WatcherReward;

typedef struct {
    Vec4f target;
    Vec4f eye;
    Vec4f delta;
} WatcherLook;

#endif /* LOMBYTE_RNC_OVERLAY_L02_MOBYS_WATCHER_NPC_H */
