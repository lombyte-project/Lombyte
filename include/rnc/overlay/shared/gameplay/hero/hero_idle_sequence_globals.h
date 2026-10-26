#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_HERO_HERO_IDLE_SEQUENCE_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_HERO_HERO_IDLE_SEQUENCE_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/gameplay/hero/hero_idle_sequence.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern s32 hero_extra_manipulator_attachment_indices __asm__("D_L00_0015F6F8");
extern int hero_scalar_pulse_active __asm__("D_L00_0015F680");
extern int hero_scalar_pulse_phase __asm__("D_L00_0015F684");
extern float hero_scalar_pulse_step __asm__("D_L00_0015F67C");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_HERO_HERO_IDLE_SEQUENCE_GLOBALS_H */
