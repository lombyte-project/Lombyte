#ifndef LOMBYTE_RNC_OVERLAY_POINT_LIGHT_H
#define LOMBYTE_RNC_OVERLAY_POINT_LIGHT_H

#include "types.h"
#include "rnc/overlay/quad.h"

/* One of the 8 point light slots. allocate_colored_effect_slot and
   allocate_packed_color_point_light_slot fill a free one and return its
   index; the owner then moves it each frame and release_point_light_slot
   clears it. */
typedef struct {
    f32     color[3];
    f32     unkC;
    OvlVec4 position; /* w: radius */
} PointLight;

extern PointLight point_lights[8] __asm__("D_L00_001802C0");

#endif /* LOMBYTE_RNC_OVERLAY_POINT_LIGHT_H */
