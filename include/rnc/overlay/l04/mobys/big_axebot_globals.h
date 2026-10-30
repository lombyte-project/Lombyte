#ifndef LOMBYTE_RNC_OVERLAY_L04_MOBYS_BIG_AXEBOT_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L04_MOBYS_BIG_AXEBOT_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l04/mobys/big_axebot.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern s32   swarm_motion_parameters_initialized __asm__("D_L04_001513C0");
extern float circular_saw_flock_separation_weight __asm__("D_L04_00161884");
extern f32   smoke_trail_puff_vertical_velocity_scale __asm__("D_L04_001618B4");

#endif /* LOMBYTE_RNC_OVERLAY_L04_MOBYS_BIG_AXEBOT_GLOBALS_H */
