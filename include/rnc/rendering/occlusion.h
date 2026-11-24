#ifndef RCP2P_RNC_RENDERING_OCCLUSION_H
#define RCP2P_RNC_RENDERING_OCCLUSION_H

#include "types.h"

void build_occlusion_visibility(void) __asm__("FUN_001f2820");
void update_occlusion(void) __asm__("FUN_001f2c10");

#endif /* RCP2P_RNC_RENDERING_OCCLUSION_H */
