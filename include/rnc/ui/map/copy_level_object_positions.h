#ifndef LOMBYTE_RNC_UI_MAP_COPY_LEVEL_OBJECT_POSITIONS_H
#define LOMBYTE_RNC_UI_MAP_COPY_LEVEL_OBJECT_POSITIONS_H

#include "types.h"

void copy_level_object_positions(void) __asm__("FUN_00208770");

/* First level object (map icon) index of each level, and one past the last:
   copy_level_object_positions reads [level] and [level + 1] for levels
   0..0x12. */
extern u32 level_object_index_boundaries[20] __asm__("D_001E03B8");

#endif /* LOMBYTE_RNC_UI_MAP_COPY_LEVEL_OBJECT_POSITIONS_H */
