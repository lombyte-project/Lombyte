#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_HERO_HERO_IDLE_SEQUENCE_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_HERO_HERO_IDLE_SEQUENCE_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/gameplay/hero/hero_idle_sequence.c. */
extern s32 hero_extra_manipulator_attachment_indices __asm__("D_L00_0015F6F8") __attribute__((sda));
extern int hero_scalar_pulse_active __asm__("D_L00_0015F680") __attribute__((sda));
extern int hero_scalar_pulse_phase __asm__("D_L00_0015F684") __attribute__((sda));
extern float hero_scalar_pulse_step __asm__("D_L00_0015F67C") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_HERO_HERO_IDLE_SEQUENCE_GLOBALS_H */
