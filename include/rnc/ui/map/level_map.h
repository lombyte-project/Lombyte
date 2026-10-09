#ifndef LOMBYTE_RNC_UI_MAP_LEVEL_MAP_H
#define LOMBYTE_RNC_UI_MAP_LEVEL_MAP_H

#include "types.h"

typedef struct {
    s32 x;
    s32 y;
    s32 label_offset_x;
    s32 label_offset_y;
} LevelMapMarker;

typedef struct {
    s32 label_text;
    s32 pad4[2];
} LevelMapMarkerText;

extern LevelMapMarkerText level_map_labels[19] __asm__("D_001DDD44");
extern LevelMapMarker level_map_markers[20] __asm__("D_001DDE28");

#endif /* LOMBYTE_RNC_UI_MAP_LEVEL_MAP_H */
