#ifndef LOMBYTE_RNC_OVERLAY_L05_HERO_UPDATE_HERO_STATE_MACHINE_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L05_HERO_UPDATE_HERO_STATE_MACHINE_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l05/hero/update_hero_state_machine.c. */
extern float fatal_fall_entry_z_threshold __asm__("D_L05_0015F638");
extern int   item_selector_hud_open_blocker __asm__("D_L05_0015F594") __attribute__((sda));
extern int   landing_run_entry_animation_frame __asm__("D_L05_0015F6EC");

#endif /* LOMBYTE_RNC_OVERLAY_L05_HERO_UPDATE_HERO_STATE_MACHINE_GLOBALS_H */
