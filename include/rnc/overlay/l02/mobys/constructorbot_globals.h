#ifndef LOMBYTE_RNC_OVERLAY_L02_MOBYS_CONSTRUCTORBOT_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L02_MOBYS_CONSTRUCTORBOT_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l02/mobys/constructorbot.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern f32 shard_vertical_velocity_approach_scale __asm__("D_L02_00161F58");
extern s32 view_gated_smoke_burst_interval_min __asm__("D_L02_0016210C");
extern s32 view_gated_smoke_burst_interval_max __asm__("D_L02_00162110");
extern s32 alternating_smoke_burst_interval_min __asm__("D_L02_0016215C");
extern s32 alternating_smoke_burst_interval_max __asm__("D_L02_00162160");

#endif /* LOMBYTE_RNC_OVERLAY_L02_MOBYS_CONSTRUCTORBOT_GLOBALS_H */
