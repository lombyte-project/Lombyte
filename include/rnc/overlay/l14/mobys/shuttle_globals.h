#ifndef LOMBYTE_RNC_OVERLAY_L14_MOBYS_SHUTTLE_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L14_MOBYS_SHUTTLE_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l14/mobys/shuttle.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern s32 shuttle_enter_riding_transition_frames __asm__("D_L14_00161B8C");
extern s32 shuttle_exit_riding_transition_frames __asm__("D_L14_00161B90");
extern f32 shuttle_horizontal_rider_correction_limit_scale __asm__("D_L14_00161B88");

#endif /* LOMBYTE_RNC_OVERLAY_L14_MOBYS_SHUTTLE_GLOBALS_H */
