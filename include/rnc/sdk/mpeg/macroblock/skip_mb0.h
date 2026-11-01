#ifndef LOMBYTE_RNC_SDK_MPEG_MACROBLOCK_SKIP_MB0_H
#define LOMBYTE_RNC_SDK_MPEG_MACROBLOCK_SKIP_MB0_H

#include "types.h"

struct MpegDecoder;
struct MpegMotionRef;
struct MpegSkipState;

s32 _skipMB0(struct MpegDecoder *, struct MpegSkipState *, s32 *, struct MpegMotionRef *, s32 *);

#endif /* LOMBYTE_RNC_SDK_MPEG_MACROBLOCK_SKIP_MB0_H */
