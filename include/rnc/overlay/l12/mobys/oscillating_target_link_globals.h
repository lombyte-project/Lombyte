#ifndef LOMBYTE_RNC_OVERLAY_L12_MOBYS_OSCILLATING_TARGET_LINK_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L12_MOBYS_OSCILLATING_TARGET_LINK_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l12/mobys/oscillating_target_link.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float oscillating_link_strip_segment_u_step __asm__("D_L12_00161DF4");
extern float oscillating_link_strip_scroll_phase __asm__("D_L12_00161DF8");
extern int   oscillating_link_strip_reset_ticks __asm__("D_L12_00161E04");
extern int   oscillating_link_strip_fade_pivot_ticks __asm__("D_L12_00161E08");
extern float oscillating_link_strip_scroll_rate __asm__("D_L12_00161DFC");
extern float oscillating_link_strip_random_delta_magnitude_rate __asm__("D_L12_00161E00");

#endif /* LOMBYTE_RNC_OVERLAY_L12_MOBYS_OSCILLATING_TARGET_LINK_GLOBALS_H */
