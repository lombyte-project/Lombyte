#ifndef LOMBYTE_RNC_RENDERING_TIES_REGISTER_OBJECT_RENDER_CLASS_H
#define LOMBYTE_RNC_RENDERING_TIES_REGISTER_OBJECT_RENDER_CLASS_H

#include "rnc/rendering/object_render_class.h"
#include "rnc/rendering/resident_class.h"
#include "rnc/sdk/eetypes.h"
#include "types.h"

void register_object_render_class(ObjectRenderClass *,
                                  ResidentRenderTextureDefinition *,
                                  u128 *,
                                  s32) __asm__("FUN_00203730");

#endif /* LOMBYTE_RNC_RENDERING_TIES_REGISTER_OBJECT_RENDER_CLASS_H */
