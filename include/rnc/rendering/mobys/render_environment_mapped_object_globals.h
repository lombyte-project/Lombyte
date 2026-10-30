#ifndef LOMBYTE_RNC_RENDERING_MOBYS_RENDER_ENVIRONMENT_MAPPED_OBJECT_GLOBALS_H
#define LOMBYTE_RNC_RENDERING_MOBYS_RENDER_ENVIRONMENT_MAPPED_OBJECT_GLOBALS_H

#include "types.h"

/* Scalar data read by src/rendering/mobys/render_environment_mapped_object.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern s32 environment_mapping_was_active __asm__("D_001604A8");

/* Additional scalar data read by this unit. */
extern s32 environment_mapping_uv_refresh_state __asm__("D_001604A4") __attribute__((sda));

#endif /* LOMBYTE_RNC_RENDERING_MOBYS_RENDER_ENVIRONMENT_MAPPED_OBJECT_GLOBALS_H */
