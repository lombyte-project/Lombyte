#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_DEVASTATOR_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_DEVASTATOR_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/gameplay/entities/devastator.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern int linked_moby_selection_delay __asm__("D_L00_00161478");
extern int owned_record_cooldown __asm__("D_L00_00161480");
extern float visibomb_missile_default_speed __asm__("D_L00_0016149C");
extern float visibomb_missile_max_speed __asm__("D_L00_001614A0");
extern float visibomb_missile_speed_target_blend_factor __asm__("D_L00_001614A4");
extern float visibomb_missile_speed_blend_factor __asm__("D_L00_001614A8");
extern s32 taunter_effect_reset_pending __asm__("D_L00_001614CC");
extern s32 taunter_effect_emission_disabled __asm__("D_L00_001614D4");
extern int taunter_effect_spawn_interval_frames __asm__("D_L00_001614B0");
extern float taunter_effect_travel_distance __asm__("D_L00_001614B4");
extern float taunter_effect_travel_speed __asm__("D_L00_001614B8");
extern int taunter_effect_lifetime_frames __asm__("D_L00_001614D8");
extern f32 taunter_effect_quad_final_scale __asm__("D_L00_001614C8");
extern f32 taunter_effect_quad_initial_scale __asm__("D_L00_001614C4");
extern u32 taunter_effect_quad_rgb __asm__("D_L00_001614C0");
extern int tesla_beam_initial_frame_count __asm__("D_L00_00161624");
extern int tesla_beam_scaled_initial_frame_count __asm__("D_L00_00161678");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_DEVASTATOR_GLOBALS_H */
