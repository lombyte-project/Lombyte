#ifndef LOMBYTE_RNC_UI_MAP_LEVEL_MAP_H
#define LOMBYTE_RNC_UI_MAP_LEVEL_MAP_H

#include "types.h"

typedef struct {
    s32 x;
    s32 y;
    s32 label_offset_x;
    s32 label_offset_y;
} LevelMapMarker;

/* One per level id: the level select menu prints description_text and
   label_text (draw_menu_selection_text), the map prints label_text. */
typedef struct {
    s32 description_text;
    s32 label_text;
    s32 unk8;
} LevelSelectText;

extern LevelSelectText level_select_texts[19] __asm__("D_001DDD40");
extern LevelMapMarker     level_map_markers[20] __asm__("D_001DDE28");

#endif /* LOMBYTE_RNC_UI_MAP_LEVEL_MAP_H */
