#ifndef LOMBYTE_RNC_OVERLAY_COLLECTION_STATE_H
#define LOMBYTE_RNC_OVERLAY_COLLECTION_STATE_H

#include "types.h"

/* A level's collection record: byte id of collected[] is set once the moby
   with save id id has been collected or destroyed. */
typedef struct {
    u8 pad0[0x454];
    u8 collected[1];
} LevelCollectionState;

extern LevelCollectionState l01_save_id_collection_state __asm__("D_L01_001BB6B0");

#endif /* LOMBYTE_RNC_OVERLAY_COLLECTION_STATE_H */
