#ifndef LOMBYTE_RNC_OVERLAY_L13_MOBYS_POISON_VESSEL_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L13_MOBYS_POISON_VESSEL_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l13/mobys/poison_vessel.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern int poison_vessel_blast_duration_frames __asm__("D_L13_00161E2C");
extern f32 poison_vessel_blast_start_radius __asm__("D_L13_00161E30");
extern f32 poison_vessel_blast_end_radius __asm__("D_L13_00161E34");
extern f32 poison_vessel_blast_damage __asm__("D_L13_00161E38");

#endif /* LOMBYTE_RNC_OVERLAY_L13_MOBYS_POISON_VESSEL_GLOBALS_H */
