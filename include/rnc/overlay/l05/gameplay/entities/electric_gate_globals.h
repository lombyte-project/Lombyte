#ifndef LOMBYTE_RNC_OVERLAY_L05_GAMEPLAY_ENTITIES_ELECTRIC_GATE_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L05_GAMEPLAY_ENTITIES_ELECTRIC_GATE_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l05/gameplay/entities/electric_gate.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float drain_cover_tilt_acceleration __asm__("D_L05_00161DDC");
extern float drain_cover_tilt_deceleration __asm__("D_L05_00161DE0");
extern float drain_cover_tilt_max_speed __asm__("D_L05_00161DE4");
extern float gate_rise_acceleration __asm__("D_L05_00161E80");
extern float gate_rise_deceleration __asm__("D_L05_00161E84");
extern float gate_rise_max_speed __asm__("D_L05_00161E88");
extern float gate_swing_acceleration __asm__("D_L05_00161E8C");
extern float gate_swing_deceleration __asm__("D_L05_00161E90");
extern float gate_swing_max_speed __asm__("D_L05_00161E94");
extern f32 elevator_camera_transition_fade __asm__("D_L05_0015F3FC");
extern int hero_inside_clip_volume __asm__("D_L05_001612DC");
extern float hero_clip_volume_output_w __asm__("D_L05_001612EC");

#endif /* LOMBYTE_RNC_OVERLAY_L05_GAMEPLAY_ENTITIES_ELECTRIC_GATE_GLOBALS_H */
