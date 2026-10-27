#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_CAMERA_OBJECT_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_CAMERA_OBJECT_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/gameplay/entities/camera_object.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern int configured_unscaled_frame_count __asm__("D_L00_00161368");
extern int configured_reciprocal_frame_count __asm__("D_L00_0016136C");
extern float wrapped_value_decrement_rate __asm__("D_L00_00161378");
extern int last_bolt_attraction_sound_frame __asm__("D_L00_001613AC");
extern float bolt_spin_angle_limit_degrees __asm__("D_L00_001613A8");

/* Additional scalar data read by this unit. */
extern int fetch_add_accumulator __asm__("D_L00_00161390");
extern int configured_reciprocal_denominator_frames __asm__("D_L00_00161370") __attribute__((sda));
extern f32 vendor_color_pulse_phase __asm__("D_L00_001613A4") __attribute__((sda));
extern f32 vendor_manipulator_rotation_phase __asm__("D_L00_0016139C") __attribute__((sda));
extern f32 vendor_manipulator_counter_rotation_phase __asm__("D_L00_001613A0") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_CAMERA_OBJECT_GLOBALS_H */
