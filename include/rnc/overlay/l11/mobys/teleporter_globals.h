#ifndef LOMBYTE_RNC_OVERLAY_L11_MOBYS_TELEPORTER_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L11_MOBYS_TELEPORTER_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l11/mobys/teleporter.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float teleporter_parts_acceleration __asm__("D_L11_00161910");
extern float teleporter_parts_deceleration __asm__("D_L11_00161914");
extern float teleporter_parts_max_speed __asm__("D_L11_00161918");
extern float teleporter_ring_local_z_center __asm__("D_L11_00161938");

#endif /* LOMBYTE_RNC_OVERLAY_L11_MOBYS_TELEPORTER_GLOBALS_H */
