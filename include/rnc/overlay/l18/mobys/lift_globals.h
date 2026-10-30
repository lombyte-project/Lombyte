#ifndef LOMBYTE_RNC_OVERLAY_L18_MOBYS_LIFT_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L18_MOBYS_LIFT_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l18/mobys/lift.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern int   lift_alternate_height_requested __asm__("D_L18_001622FC");
extern int   next_lift_instance_index __asm__("D_L18_001622F8");
extern float lift_texture_u_scroll_rate __asm__("D_L18_0016230C");

#endif /* LOMBYTE_RNC_OVERLAY_L18_MOBYS_LIFT_GLOBALS_H */
