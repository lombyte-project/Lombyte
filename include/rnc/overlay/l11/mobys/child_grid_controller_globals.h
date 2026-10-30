#ifndef LOMBYTE_RNC_OVERLAY_L11_MOBYS_CHILD_GRID_CONTROLLER_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L11_MOBYS_CHILD_GRID_CONTROLLER_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l11/mobys/child_grid_controller.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float child_grid_pose_blend_acceleration __asm__("D_L11_00161F58");
extern float child_grid_pose_blend_deceleration __asm__("D_L11_00161F5C");
extern float child_grid_pose_blend_max_speed __asm__("D_L11_00161F60");
extern float child_grid_parent_axis0_offset_length __asm__("D_L11_00161F40");
extern float child_grid_parent_axis1_offset_length __asm__("D_L11_00161F44");
extern float child_grid_child_axis1_offset_length __asm__("D_L11_00161F48");

#endif /* LOMBYTE_RNC_OVERLAY_L11_MOBYS_CHILD_GRID_CONTROLLER_GLOBALS_H */
