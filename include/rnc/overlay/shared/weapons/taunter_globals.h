#ifndef LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_TAUNTER_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_TAUNTER_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/weapons/taunter.c. */
extern s32 taunter_effect_reset_pending __asm__("D_L00_001614CC");
extern s32 taunter_effect_emission_disabled __asm__("D_L00_001614D4");
extern int taunter_effect_lifetime_frames __asm__("D_L00_001614D8");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_TAUNTER_GLOBALS_H */
