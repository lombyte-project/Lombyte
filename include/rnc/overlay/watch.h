#ifndef LOMBYTE_RNC_OVERLAY_WATCH_H
#define LOMBYTE_RNC_OVERLAY_WATCH_H

#include "types.h"

typedef struct {
    char pad00[0x80];
    float position[4];
    char pad90[0x40];
    float aim[4];
    char padE0[0x22E];
    short disabled;
    char pad310[0x1D7C];
    int mode;
} L16WatchPlayer;

typedef struct {
    float target[4];
    float eye[4];
    float delta[4];
} L16WatchScratch;

#endif /* LOMBYTE_RNC_OVERLAY_WATCH_H */
