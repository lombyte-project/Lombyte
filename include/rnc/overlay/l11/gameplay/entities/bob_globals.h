#ifndef LOMBYTE_RNC_OVERLAY_L11_GAMEPLAY_ENTITIES_BOB_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L11_GAMEPLAY_ENTITIES_BOB_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l11/gameplay/entities/bob.c. */
extern float resort_owner_gate_jump_target_local_z_offset __asm__("D_L11_00161410") __attribute__((sda));
extern float resort_owner_gate_jump_target_local_y_offset __asm__("D_L11_00161414") __attribute__((sda));
extern float resort_owner_gate_jump_target_local_x_offset __asm__("D_L11_00161418") __attribute__((sda));
extern float resort_owner_follow_hero_speed __asm__("D_L11_001613D0") __attribute__((sda));
extern float resort_owner_distant_follow_and_path_speed __asm__("D_L11_001613D4") __attribute__((sda));
extern float teleporter_parts_acceleration __asm__("D_L11_00161910") __attribute__((sda));
extern float teleporter_parts_deceleration __asm__("D_L11_00161914") __attribute__((sda));
extern float teleporter_parts_max_speed __asm__("D_L11_00161918") __attribute__((sda));
extern float teleporter_ring_local_z_center __asm__("D_L11_00161938") __attribute__((sda));
extern float pokitaru_boat_path_speed __asm__("D_L11_00161DD4") __attribute__((sda));
extern float pokitaru_boat_turn_acceleration_degrees __asm__("D_L11_00161DD8") __attribute__((sda));
extern float pokitaru_boat_max_turn_speed_degrees __asm__("D_L11_00161DDC") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L11_GAMEPLAY_ENTITIES_BOB_GLOBALS_H */
