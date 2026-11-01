#ifndef LOMBYTE_RNC_SDK_MPEG_REF_IMAGES__UPDATEREFIMAGE_H
#define LOMBYTE_RNC_SDK_MPEG_REF_IMAGES__UPDATEREFIMAGE_H

#include "types.h"

struct MpegReferenceState;

s32 UpdateMpegReferenceImages(struct MpegReferenceState *, s32) __asm__("_updateRefImage");

#endif /* LOMBYTE_RNC_SDK_MPEG_REF_IMAGES__UPDATEREFIMAGE_H */
