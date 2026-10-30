#ifndef LOMBYTE_RNC_OVERLAY_L03_MOBYS_MINE_LAYER_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L03_MOBYS_MINE_LAYER_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l03/mobys/mine_layer.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern s32 mine_layer_fire_interval_frames __asm__("D_L03_00161918");
extern f32 mine_layer_patrol_animation_speed __asm__("D_L03_0016191C");
extern f32 mine_layer_initial_scale_multiplier __asm__("D_L03_00161920");
extern f32 mine_layer_projectile_lateral_spread_speed __asm__("D_L03_00161910");
extern f32 mine_layer_projectile_vertical_launch_speed __asm__("D_L03_00161914");
extern f32 mine_layer_lethal_knockback_speed_scale __asm__("D_L03_001618F8");
extern f32 mine_layer_nonlethal_knockback_speed_scale __asm__("D_L03_001618FC");

#endif /* LOMBYTE_RNC_OVERLAY_L03_MOBYS_MINE_LAYER_GLOBALS_H */
