#ifndef LOMBYTE_RNC_SDK_MPEG_MACROBLOCK_SLICEA0_H
#define LOMBYTE_RNC_SDK_MPEG_MACROBLOCK_SLICEA0_H

#include "types.h"

struct MpegMotionState;
struct MpegSliceDecoder;

s32 _sliceA0(struct MpegSliceDecoder *, s32, s32 *, s32 *, volatile struct MpegMotionState *);

#endif /* LOMBYTE_RNC_SDK_MPEG_MACROBLOCK_SLICEA0_H */
