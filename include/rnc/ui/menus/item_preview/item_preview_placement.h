#ifndef LOMBYTE_RNC_UI_MENUS_ITEM_PREVIEW_ITEM_PREVIEW_PLACEMENT_H
#define LOMBYTE_RNC_UI_MENUS_ITEM_PREVIEW_ITEM_PREVIEW_PLACEMENT_H

#include "types.h"

struct ItemPreviewPlacement {
    f32 alternate_x;
    f32 normal_x;
    f32 y;
    f32 z;
    f32 rotation_x;
    f32 rotation_y;
    f32 side_offset;
    f32 forward_offset;
};

extern struct ItemPreviewPlacement preview_placements[36] __asm__("D_001E0408");

#endif /* LOMBYTE_RNC_UI_MENUS_ITEM_PREVIEW_ITEM_PREVIEW_PLACEMENT_H */
