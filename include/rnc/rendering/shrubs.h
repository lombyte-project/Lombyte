#ifndef LOMBYTE_RNC_RENDERING_SHRUBS_H
#define LOMBYTE_RNC_RENDERING_SHRUBS_H

#include "rnc/rendering/resident_class.h"
#include "rnc/rendering/shrub_render_class.h"
#include "rnc/sdk/eetypes.h"
#include "types.h"

void register_shrub_render_class(ShrubRenderClass *,
                                 ResidentRenderTextureDefinition *,
                                 u128 *,
                                 ShrubMipTextureDefinition *,
                                 s32) __asm__("FUN_00203b08");

#endif /* LOMBYTE_RNC_RENDERING_SHRUBS_H */
