#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_NANOTECH_BALL_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_NANOTECH_BALL_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/gameplay/entities/nanotech_ball.c. */
extern float nanotech_ball_collection_gravity_scale_range_start __asm__("D_L00_00161A60") __attribute__((sda));
extern float nanotech_ball_collection_gravity_scale_range_end __asm__("D_L00_00161A64") __attribute__((sda));
extern float nanotech_ball_collection_first_particle_scale_factor __asm__("D_L00_00161A74") __attribute__((sda));
extern float nanotech_ball_collection_second_particle_scale_factor __asm__("D_L00_00161A78") __attribute__((sda));
extern s32 nanotech_ball_collection_color_range_start __asm__("D_L00_00161A7C") __attribute__((sda));
extern s32 nanotech_ball_collection_color_range_end __asm__("D_L00_00161A80") __attribute__((sda));
extern f32 nanotech_ball_slot_offset_transform_amount __asm__("D_L00_001619F0") __attribute__((sda));
extern float nanotech_ball_bob_phase_step __asm__("D_L00_00161A0C") __attribute__((sda));
extern int nanotech_ball_collection_lifetime_frames __asm__("D_L00_00161A3C") __attribute__((sda));
extern int nanotech_ball_slot_collection_min_frames __asm__("D_L00_00161A44") __attribute__((sda));
extern int nanotech_ball_slot_collection_max_frames __asm__("D_L00_00161A48") __attribute__((sda));
extern float nanotech_ball_slot_scale_pulse_amplitude __asm__("D_L00_00161A5C") __attribute__((sda));
extern int red_orb_pulse_color_start __asm__("D_L00_00161B90") __attribute__((sda));
extern int red_orb_pulse_color_end __asm__("D_L00_00161B94") __attribute__((sda));
extern float red_orb_pulse_angular_speed_degrees __asm__("D_L00_00161B98") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_NANOTECH_BALL_GLOBALS_H */
