#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_BOT_DOOR_LOCK_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_BOT_DOOR_LOCK_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/gameplay/entities/bot_door_lock.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float bot_door_lock_number_bob_amplitude __asm__("D_L06_001622DC");
extern float bot_door_lock_number_bob_period __asm__("D_L06_001622E0");
extern float bot_door_lock_color_pulse_period __asm__("D_L06_001622E4");
extern float bot_door_lock_number_color_pulse_amplitude __asm__("D_L06_001622E8");
extern float bot_door_lock_max_effect_horizontal_scale __asm__("D_L06_001622EC");
extern s32 projected_quad_texture_selector __asm__("D_L06_00162414");
extern s32 projected_quad_base_radius __asm__("D_L06_00162418");
extern s32 projected_quad_top_right_uv __asm__("D_L06_0016241C");
extern s32 projected_quad_top_left_uv __asm__("D_L06_00162420");
extern s32 projected_quad_bottom_right_uv __asm__("D_L06_00162424");
extern s32 projected_quad_bottom_left_uv __asm__("D_L06_00162428");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_BOT_DOOR_LOCK_GLOBALS_H */
