#ifndef LOMBYTE_RNC_OVERLAY_L12_MOBYS_WATCHER_H
#define LOMBYTE_RNC_OVERLAY_L12_MOBYS_WATCHER_H

#include "types.h"
#include "rnc/mobys/moby.h"

/* Level 12 watcher NPCs (miner, edwina): the L16 watcher moby and pvars layout they share. */

typedef struct {
    char  pad00[0xC];
    float radius;
    char  pad10[0x30];
} L16WatchActivation;

typedef struct {
    char               pad00[0x20];
    L16WatchActivation activation;
    char               body[0x60];
    float              padC0;
    float              pitch;
    float              yaw;
    float              padCC;
    float              height;
    char               padD4[0xC];
    char               head[0x60];
    char               pad140[8];
    float              head_yaw;
    char               pad14C[0x14];
    float              target[4];
    int                region;
    char               pad174[4];
    int                moving_timer;
    int                target_timer;
} L16WatchData;

typedef struct {
    char          pad00[0x10];
    float         position[4];
    unsigned char state;
    char          pad21[0xF];
    unsigned char opacity;
    char          pad31[0x17];
    float         yaw;
    char          pad4C[7];
    unsigned char animation;
    char          pad54[0x24];
    L16WatchData *data;
    char          pad7C[0x34];
    unsigned char widget;
} L16WatchMoby;

#endif /* LOMBYTE_RNC_OVERLAY_L12_MOBYS_WATCHER_H */
