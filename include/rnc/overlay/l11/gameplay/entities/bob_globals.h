#ifndef LOMBYTE_RNC_OVERLAY_L11_GAMEPLAY_ENTITIES_BOB_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L11_GAMEPLAY_ENTITIES_BOB_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l11/gameplay/entities/bob.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float resort_owner_gate_jump_target_local_z_offset __asm__("D_L11_00161410");
extern float resort_owner_gate_jump_target_local_y_offset __asm__("D_L11_00161414");
extern float resort_owner_gate_jump_target_local_x_offset __asm__("D_L11_00161418");
extern float resort_owner_follow_hero_speed __asm__("D_L11_001613D0");
extern float resort_owner_distant_follow_and_path_speed __asm__("D_L11_001613D4");
extern float teleporter_parts_acceleration __asm__("D_L11_00161910");
extern float teleporter_parts_deceleration __asm__("D_L11_00161914");
extern float teleporter_parts_max_speed __asm__("D_L11_00161918");
extern float teleporter_ring_local_z_center __asm__("D_L11_00161938");
extern float pokitaru_boat_path_speed __asm__("D_L11_00161DD4");
extern float pokitaru_boat_turn_acceleration_degrees __asm__("D_L11_00161DD8");
extern float pokitaru_boat_max_turn_speed_degrees __asm__("D_L11_00161DDC");

#endif /* LOMBYTE_RNC_OVERLAY_L11_GAMEPLAY_ENTITIES_BOB_GLOBALS_H */
