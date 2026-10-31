#ifndef LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_EFFECTS_UPDATE_THREE_PHASE_EFFECT_PARTICLE_H
#define LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_EFFECTS_UPDATE_THREE_PHASE_EFFECT_PARTICLE_H

#include "types.h"

void spawn_effect_particle_with_target_scalar(void *, void *, s32, s32, f32, s32) __asm__("FUN_L00_0026ced0");
void  spawn_color_decaying_effect_particle(void *, void *, s32) __asm__("FUN_L00_0026d598");
void *spawn_source_attached_fading_effect_particle(void *, void *, s32, s32, f32) __asm__("FUN_L00_0026daa0");

struct Moby;

unsigned char *mine_debris(void *, void *, int, int, float, int, int) __asm__("FUN_L00_0026bed0");
void           FUN_L00_0026a9f0(void *, void *, int, int, int, int, int, int, float, float);
char          *FUN_L00_0026cbb0(void *, void *, int, int, int, int, float);
char          *FUN_L00_0026dd20(void *, void *, int, int, float);
void          *spawn_splitting_effect_particle_with_collision_response_and_fade(
             char *, char *, unsigned char, int) __asm__("FUN_L00_0026fd28");
void           spawn_jittered_effect_particle_with_optional_basis(void *,
                                                                  void *,
                                                                  f32) __asm__("FUN_L00_0026de90");
void           spawn_two_point_effect_particle(void *, void *) __asm__("FUN_L00_0026c6e8");
unsigned char *spawn_shrinking_effect_particle_with_gravity_and_secondary_spawns(
    void *, void *, int, int, float, int, int, int, int) __asm__("FUN_L00_0026bb70");
void update_three_phase_effect_particle(char *) __asm__("FUN_L00_00269290");
void update_drifting_effect_particle_with_distance_fade(char *) __asm__("FUN_L00_002694f8");
void update_damped_effect_particle(void) __asm__("FUN_L00_00269a70");
void update_scalar_and_fade_effect_particle(char *) __asm__("FUN_L00_00269c70");
void update_colliding_effect_particle(unsigned char *) __asm__("FUN_L00_00269d20");
void update_animated_fading_effect_particle(unsigned char *) __asm__("FUN_L00_0026a5b8");
void update_rotating_grayscale_effect_particle(unsigned char *) __asm__("FUN_L00_0026a6f0");
void update_damped_blue_shift_effect_particle(unsigned char *) __asm__("FUN_L00_0026a880");
void update_branching_and_fading_effect_particle(char *) __asm__("FUN_L00_0026ac78");
void update_spinning_and_fading_effect_particle(unsigned char *) __asm__("FUN_L00_0026b500");
void update_spinning_fade_in_out_effect_particle(void) __asm__("FUN_L00_0026b910");
void update_attached_or_fading_effect_particle(unsigned char *) __asm__("FUN_L00_0026ba68");
void update_shrinking_effect_particle_with_gravity_and_secondary_spawns(unsigned char *) __asm__("FUN_L00_0026bca8");
void update_fading_debris_particle_with_landing_spawn(void) __asm__("FUN_L00_0026c088");
void update_descending_fade_in_out_effect_particle(void) __asm__("FUN_L00_0026c558");
void move_and_fade_two_point_particle(char *) __asm__("FUN_L00_0026c7f0");
void update_spiraling_fade_in_out_effect_particle(void) __asm__("FUN_L00_0026c860");
void update_shrinking_effect_particle_with_secondary_spawns(void) __asm__("FUN_L00_0026ccb0");
void update_moving_spinning_fade_in_out_effect_particle(unsigned char *) __asm__("FUN_L00_0026d1b0");
void update_source_bound_effect_particle(void) __asm__("FUN_L00_0026d470");
void update_color_decaying_effect_particle_with_collision(void) __asm__("FUN_L00_0026d700");
void update_source_attached_fading_effect_particle(unsigned char *) __asm__("FUN_L00_0026dbe0");
void move_and_fade_effect_particle_by_lifetime(char *) __asm__("FUN_L00_0026de00");
void update_moving_fade_in_out_effect_particle(void) __asm__("FUN_L00_0026e150");
void update_animated_source_attached_fade_in_out_effect_particle(void) __asm__("FUN_L00_0026e3a8");
void update_linked_effect_particle_with_collision_and_fade(void) __asm__("FUN_L00_0026e670");
void update_target_relative_two_point_effect_particle(unsigned char *) __asm__("FUN_L00_0026eb88");
void update_moby_bound_fade_in_out_effect_particle(unsigned char *) __asm__("FUN_L00_0026ee48");
void tick_effect_particle_lifetime(char *) __asm__("FUN_L00_0026f048");
void update_swaying_and_fading_effect_particle(char *) __asm__("FUN_L00_0026f280");
void update_ballistic_effect_particle_with_contact_spawns(char *) __asm__("FUN_L00_0026f660");
void update_flag_controlled_pulsing_effect_particle(unsigned char *) __asm__("FUN_L00_0026f898");
void update_offset_and_pulse_effect_particle(char *) __asm__("FUN_L00_0026fa00");
void update_damped_and_fading_effect_particle(void) __asm__("FUN_L00_0026fb00");
void update_animated_offset_effect_particle(void) __asm__("FUN_L00_0026fc38");
void update_splitting_effect_particle_with_collision_response_and_fade(void) __asm__("FUN_L00_0026ff10");
void tick_finite_effect_particle_lifetime(char *) __asm__("FUN_L00_00270710");
void update_splitting_effect_particle_with_collision_and_fade(void) __asm__("FUN_L00_00270948");
void update_accelerating_fade_in_out_effect_particle(u8 *) __asm__("FUN_L00_002710a0");
void spawn_moby_bound_effect_particle(struct Moby *) __asm__("FUN_L00_0026ed28");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_EFFECTS_UPDATE_THREE_PHASE_EFFECT_PARTICLE_H */
