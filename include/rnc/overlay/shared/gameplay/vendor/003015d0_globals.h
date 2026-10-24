#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_VENDOR_003015D0_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_VENDOR_003015D0_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/gameplay/vendor/003015d0.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern int vendor_beam_damage_disabled __asm__("D_L14_001620D8");
extern int vendor_beam_min_pulse_duration_frames __asm__("D_L14_001620BC");
extern int vendor_beam_max_pulse_duration_frames __asm__("D_L14_001620C0");
extern float vendor_beam_displacement_radius __asm__("D_L14_001620FC");
extern float vendor_beam_max_point_speed __asm__("D_L14_00162100");
extern int vendor_beam_color_transition_countdown __asm__("D_L14_001620C4");
extern int vendor_beam_initial_color __asm__("D_L14_001620F0");
extern int vendor_beam_transition_color __asm__("D_L14_001620F4");
extern int vendor_beam_final_color __asm__("D_L14_001620F8");
extern int vendor_beam_effect_texture_index __asm__("D_L14_001620DC");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_VENDOR_003015D0_GLOBALS_H */
