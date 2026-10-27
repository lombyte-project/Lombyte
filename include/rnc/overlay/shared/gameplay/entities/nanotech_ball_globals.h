#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_NANOTECH_BALL_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_NANOTECH_BALL_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/gameplay/entities/nanotech_ball.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float nanotech_ball_collection_gravity_scale_range_start __asm__("D_L00_00161A60");
extern float nanotech_ball_collection_gravity_scale_range_end __asm__("D_L00_00161A64");
extern float nanotech_ball_collection_first_particle_scale_factor __asm__("D_L00_00161A74");
extern float nanotech_ball_collection_second_particle_scale_factor __asm__("D_L00_00161A78");
extern s32 nanotech_ball_collection_color_range_start __asm__("D_L00_00161A7C");
extern s32 nanotech_ball_collection_color_range_end __asm__("D_L00_00161A80");
extern f32 nanotech_ball_slot_offset_transform_amount __asm__("D_L00_001619F0");
extern float nanotech_ball_bob_phase_step __asm__("D_L00_00161A0C");
extern int nanotech_ball_collection_lifetime_frames __asm__("D_L00_00161A3C");
extern int nanotech_ball_slot_collection_min_frames __asm__("D_L00_00161A44");
extern int nanotech_ball_slot_collection_max_frames __asm__("D_L00_00161A48");
extern float nanotech_ball_slot_scale_pulse_amplitude __asm__("D_L00_00161A5C");
extern int red_orb_pulse_color_start __asm__("D_L00_00161B90");
extern int red_orb_pulse_color_end __asm__("D_L00_00161B94");
extern float red_orb_pulse_angular_speed_degrees __asm__("D_L00_00161B98");

/* Additional scalar data read by this unit. */
extern int nanotech_ball_collection_first_particle_color __asm__("D_L00_00161A6C") __attribute__((sda));
extern int nanotech_ball_collection_second_particle_color __asm__("D_L00_00161A70") __attribute__((sda));
extern s32 nanotech_ball_collection_second_particle_mode_selector __asm__("D_L00_00161A84") __attribute__((sda));
extern s32 nanotech_ball_collection_particle_byte_source_index __asm__("D_L00_00161A88") __attribute__((sda));
extern int nanotech_ball_particle_color_gp __asm__("D_L00_00161A00") __attribute__((sda));
extern int nanotech_ball_full_health_invulnerable_collection_min_frames_gp __asm__("D_L00_00161A40") __attribute__((sda));
extern float nanotech_ball_collection_curve_radius_gp __asm__("D_L00_00161A50") __attribute__((sda));
extern float nanotech_ball_collection_target_gravity_offset_gp __asm__("D_L00_00161A54") __attribute__((sda));
extern float suction_effect_texture_u_offset __asm__("D_L00_00161B48") __attribute__((sda));
extern float suction_effect_texture_v_offset __asm__("D_L00_00161B4C") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_NANOTECH_BALL_GLOBALS_H */
