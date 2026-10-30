#ifndef LOMBYTE_RNC_OVERLAY_L10_MOBYS_DATA_PROCESSOR_LINKED_EFFECT_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L10_MOBYS_DATA_PROCESSOR_LINKED_EFFECT_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l10/mobys/data_processor_linked_effect.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float data_processor_linked_effect_fade_duration_seconds __asm__("D_L10_00161C98");
extern f32   data_processor_linked_effect_row_uv_phase_step __asm__("D_L10_00161D60");
extern int   data_processor_linked_effect_color_at_zero_progress __asm__("D_L10_00161C90");
extern int   data_processor_linked_effect_color_at_full_progress __asm__("D_L10_00161C94");

#endif /* LOMBYTE_RNC_OVERLAY_L10_MOBYS_DATA_PROCESSOR_LINKED_EFFECT_GLOBALS_H */
