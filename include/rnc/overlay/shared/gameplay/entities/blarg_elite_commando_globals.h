#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_BLARG_ELITE_COMMANDO_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_BLARG_ELITE_COMMANDO_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/gameplay/entities/blarg_elite_commando.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float class_32_symmetric_angle_bound_degrees __asm__("D_L16_001613E0");
extern float class_32_zero_based_angle_bound_degrees __asm__("D_L16_001613E4");
extern int blarg_elite_commando_reward_kill_count __asm__("D_0015EE10");
extern float blarg_elite_commando_extra_glow_gate __asm__("D_L16_0015F5D0");

/* Additional scalar data read by this unit. */
extern int blarg_elite_commando_bobbing_and_voice_enabled_gp __asm__("D_L16_00161A74") __attribute__((sda));
extern int blarg_elite_commando_joint_aim_enabled_gp __asm__("D_L16_00161A78") __attribute__((sda));
extern int blarg_elite_commando_joint_sparks_enabled_gp __asm__("D_L16_00161A7C") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_BLARG_ELITE_COMMANDO_GLOBALS_H */
