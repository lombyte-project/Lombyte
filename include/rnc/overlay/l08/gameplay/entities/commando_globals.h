#ifndef LOMBYTE_RNC_OVERLAY_L08_GAMEPLAY_ENTITIES_COMMANDO_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L08_GAMEPLAY_ENTITIES_COMMANDO_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l08/gameplay/entities/commando.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern s32 swarm_update_disabled __asm__("D_L08_00161D40");
extern f32 swarm_path_movement_speed __asm__("D_L08_00161D34");
extern int scrolled_mesh_unscaled_fixed_alpha __asm__("D_L08_00161DD0");
extern int scrolled_mesh_subtractive_base_fixed_alpha __asm__("D_L08_00161DD4");
extern int scrolled_mesh_additive_base_fixed_alpha __asm__("D_L08_00161DD8");
extern int offset_mesh_first_pass_fixed_alpha __asm__("D_L08_00161DF0");
extern int offset_mesh_second_pass_fixed_alpha __asm__("D_L08_00161DF4");
extern int pair_offset_mesh_fixed_alpha __asm__("D_L08_00161E00");
extern float layered_mesh_effect_center_y __asm__("D_L08_00161DC4");
extern float layered_mesh_effect_center_z __asm__("D_L08_00161DC8");
extern float layered_mesh_effect_culling_radius __asm__("D_L08_00161DCC");
extern f32 path_follower_y_rotation_rate_degrees __asm__("D_L08_00161F70");
extern int commando_height_probe_distance_limit __asm__("D_L08_001621D8");

#endif /* LOMBYTE_RNC_OVERLAY_L08_GAMEPLAY_ENTITIES_COMMANDO_GLOBALS_H */
