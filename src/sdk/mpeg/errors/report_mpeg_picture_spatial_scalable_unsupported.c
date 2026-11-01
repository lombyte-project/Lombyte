#include "types.h"
#include "asm.h"
#include "rnc/sdk/mpeg/errors.h"

const char D_00153B90[] = "_pictureSpatialScalableExtension is not supported";

void report_mpeg_picture_spatial_scalable_unsupported(void *pState) __asm__("FUN_0012cb10");

void report_mpeg_picture_spatial_scalable_unsupported(void *pState) {
    _Error(pState, D_00153B90);
}
