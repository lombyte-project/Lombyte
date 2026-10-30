#ifndef LOMBYTE_RNC_OVERLAY_L11_MOBYS_WATCHER_H
#define LOMBYTE_RNC_OVERLAY_L11_MOBYS_WATCHER_H

#include "types.h"

/* Level 11 watcher NPCs (bob, the resort owner): the moby fields their look tracking reads. */
typedef struct {
    char          pad00[0x10];
    float         position[4];
    unsigned char state;
    char          pad21[0x27];
    float         yaw;
    char          pad4C[7];
    unsigned char animation;
    char          pad54[0x24];
    char         *data;
} WatcherMoby;

#endif /* LOMBYTE_RNC_OVERLAY_L11_MOBYS_WATCHER_H */
