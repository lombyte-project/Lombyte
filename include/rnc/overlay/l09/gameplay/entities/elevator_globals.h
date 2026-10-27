#ifndef LOMBYTE_RNC_OVERLAY_L09_GAMEPLAY_ENTITIES_ELEVATOR_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L09_GAMEPLAY_ENTITIES_ELEVATOR_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l09/gameplay/entities/elevator.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern f32 turret_camera_offset_length __asm__("D_L09_00161CB0");
extern f32 turret_camera_z_offset __asm__("D_L09_00161CB4");
extern float radial_effect_count_reduction_control __asm__("D_L09_0015F5D0");
extern float floating_ship_piece_hero_offset_tilt_gain __asm__("D_L09_00161DC4");
extern float floating_ship_piece_sink_speed __asm__("D_L09_00161DC8");
extern float floating_ship_piece_height_return_rate __asm__("D_L09_00161DCC");

/* Additional scalar data read by this unit. */
extern float barrier_radial_effect_count_reduction_control __asm__("D_L09_0015F5D4");
extern float floating_ship_piece_loaded_tilt_time_squared_gain_gp __asm__("D_L09_00161DD4") __attribute__((sda));
extern float floating_ship_piece_loaded_tilt_frame_scale_squared_gain_gp __asm__("D_L09_00161DD8") __attribute__((sda));
extern float floating_ship_piece_loaded_tilt_snap_tolerance_scale_gp __asm__("D_L09_00161DDC") __attribute__((sda));
extern float floating_ship_piece_return_tilt_time_squared_gain_gp __asm__("D_L09_00161DE0") __attribute__((sda));
extern float floating_ship_piece_return_tilt_frame_scale_squared_gain_gp __asm__("D_L09_00161DE4") __attribute__((sda));
extern float floating_ship_piece_return_tilt_snap_tolerance_scale_gp __asm__("D_L09_00161DE8") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L09_GAMEPLAY_ENTITIES_ELEVATOR_GLOBALS_H */
