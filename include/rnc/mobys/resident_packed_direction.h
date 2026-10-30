#ifndef LOMBYTE_RNC_MOBYS_RESIDENT_PACKED_DIRECTION_H
#define LOMBYTE_RNC_MOBYS_RESIDENT_PACKED_DIRECTION_H

#include "rnc/math/vector.h"

/* Light direction for moby shadows: update_resident_packed_direction writes
   x, y and z; update_moby_shadow_range and the moby renderers read the
   whole quadword (lq). */
extern Vec4f resident_packed_direction __asm__("D_001CAA80");

#endif /* LOMBYTE_RNC_MOBYS_RESIDENT_PACKED_DIRECTION_H */
