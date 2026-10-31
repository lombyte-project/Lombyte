#ifndef LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_EFFECTS_PARTICLES_H
#define LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_EFFECTS_PARTICLES_H

#include "types.h"
#include "rnc/overlay/shared/math/quad.h"
#include "rnc/sdk/eetypes.h"

void *spawn_random_alpha_rotated_particle(void *, void *, f32) __asm__("FUN_L00_00271b18");
void *spawn_mode_controlled_rotating_alpha_fade_particle(void *,
                                                         void *,
                                                         s32,
                                                         void *) __asm__("FUN_L00_00274948");

struct EmberParticle;
struct EffectParticle;

unsigned char *spawn_timed_hero_relative_particle(u128 *, int, unsigned char, int, int, int, int, float) __asm__("FUN_L00_00272f68");
unsigned char *spawn_color_tween_emitter_particle(void *,
                                                  unsigned char,
                                                  unsigned char,
                                                  unsigned char,
                                                  unsigned char,
                                                  unsigned char,
                                                  int,
                                                  float,
                                                  float) __asm__("FUN_L00_002729c8");
unsigned char *spawn_splash(u128 *, u128 *, int, float, float) __asm__("FUN_L00_002718d0");
unsigned char *spawn_moving_hero_relative_alpha_fade_particle(
    float *, OvlQuad *, int, u8, u8, int, float) __asm__("FUN_L00_002730e0");
unsigned char      *
FUN_L00_002712b8(OvlQuad *, float *, int, int, int, int, float, float, float, float, float);
void load_misc_animations_into_stash(void) __asm__("FUN_L00_00276078");
void update_moving_rotating_resizing_alpha_fade_particle(struct EffectParticle *) __asm__("FUN_L00_00271450");
void update_moving_resizing_height_snapped_alpha_fade_particle(struct EffectParticle *) __asm__("FUN_L00_00271748");
void update_expanding_rotating_alpha_fade_particle(struct EffectParticle *) __asm__("FUN_L00_002719f0");
void update_expanding_damped_fading_particle(struct EffectParticle *) __asm__("FUN_L00_00271be8");
void update_color_tween_particle(struct EffectParticle *) __asm__("FUN_L00_00271c88");
void update_timed_line_collision_particle(void) __asm__("FUN_L00_00271d20");
void update_moving_height_culled_particle(struct EffectParticle *) __asm__("FUN_L00_00271df8");
void update_damped_fading_particle(struct EffectParticle *) __asm__("FUN_L00_00271ec8");
void update_color_size_tween_particle(struct EffectParticle *) __asm__("FUN_L00_00271fc0");
void update_moving_size_alpha_tween_particle(struct EffectParticle *) __asm__("FUN_L00_002721f0");
void update_timed_sphere_probed_particle(struct EffectParticle *) __asm__("FUN_L00_00272438");
void update_moby_point_attached_particle(void) __asm__("FUN_L00_002726d8");
void update_expanding_damped_rgb_fading_particle(struct EffectParticle *) __asm__("FUN_L00_00272838");
void update_expanding_alpha_pulse_particle(struct EffectParticle *) __asm__("FUN_L00_002728e8");
void update_color_tween_emitter_particle(struct EffectParticle *) __asm__("FUN_L00_00272bc0");
void update_timed_hero_relative_particle(struct EffectParticle *) __asm__("FUN_L00_00273088");
void update_moving_hero_relative_alpha_fade_particle(void) __asm__("FUN_L00_00273298");
void update_moby_point_attached_alpha_pulse_particle(void) __asm__("FUN_L00_00273448");
void update_mode_controlled_alpha_fade_particle(struct EffectParticle *) __asm__("FUN_L00_00273800");
void update_gravity_floor_burst_particle(struct EmberParticle *) __asm__("FUN_L00_00273a68");
void update_line_collision_rgb_fading_particle(struct EffectParticle *) __asm__("FUN_L00_00273ee0");
void update_moving_resizing_periodic_alpha_fade_particle(struct EffectParticle *) __asm__("FUN_L00_00274308");
void update_mode_controlled_orbiting_trail_particle(void) __asm__("FUN_L00_00274518");
void update_randomized_moby_point_attached_particle(struct EffectParticle *) __asm__("FUN_L00_00274818");
void update_mode_controlled_rotating_alpha_fade_particle(struct EffectParticle *) __asm__("FUN_L00_00274a70");
void update_periodic_alpha_fade_particle(struct EffectParticle *) __asm__("FUN_L00_00274de0");
void update_three_phase_motion_alpha_fade_particle(void) __asm__("FUN_L00_00274e48");
void update_size_gated_rotating_particle(struct EffectParticle *) __asm__("FUN_L00_002752e0");
void update_gravity_ground_culled_particle(void) __asm__("FUN_L00_00275320");
void update_damped_moby_relative_color_size_tween_particle(struct EffectParticle *) __asm__("FUN_L00_00275510");
void update_gravity_damped_resizing_particle(struct EffectParticle *) __asm__("FUN_L00_002756f0");
void update_timed_alpha_fade_particle(struct EffectParticle *) __asm__("FUN_L00_00275810");
void update_moving_lifetime_scaled_alpha_particle(void) __asm__("FUN_L00_00275860");
void update_proximity_corrected_size_alpha_tween_particle(struct EffectParticle *) __asm__("FUN_L00_00275aa0");
void update_moving_fading_particle(struct EffectParticle *) __asm__("FUN_L00_00275d68");
void update_mode_controlled_shrinking_color_tween_particle(void) __asm__("FUN_L00_00275eb0");
unsigned char *spawn_proximity_corrected_size_alpha_tween_particle(
    OvlQuad *, int, int, int, int, float *, int, float, float) __asm__("FUN_L00_00275910");
unsigned char *FUN_L00_00272060(void *, int, int, int, int, float *, float, float, float);

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_EFFECTS_PARTICLES_H */
