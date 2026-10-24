#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_002D6810_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_002D6810_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/gameplay/entities/002d6810.c. */
extern float electric_water_on_duration_seconds __asm__("D_L15_00161BC8") __attribute__((sda));
extern float electric_water_off_duration_seconds __asm__("D_L15_00161BCC") __attribute__((sda));
extern int electric_water_bolt_init_requested __asm__("D_L15_00161BF8") __attribute__((sda));
extern float electric_water_bolt_initial_length_scale __asm__("D_L15_00161BFC") __attribute__((sda));
extern int electric_water_bolt_initial_frame_count __asm__("D_L15_00161C8C") __attribute__((sda));
extern int electric_water_bolt_initial_scaled_frame_count __asm__("D_L15_00161CE0");
extern int second_side_arc_fade_level __asm__("D_L15_00161CCA");
extern int third_side_arc_fade_level __asm__("D_L15_00161CCC");
extern int fourth_side_arc_fade_level __asm__("D_L15_00161CCE");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_002D6810_GLOBALS_H */
