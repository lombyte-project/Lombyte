#ifndef LOMBYTE_RNC_SDK_MPEG_MACROBLOCK_H
#define LOMBYTE_RNC_SDK_MPEG_MACROBLOCK_H

#include "types.h"

struct MotionVector;
struct MotionVectorStore;
struct MpegCopyParams;
struct MpegDecoder;
struct MpegMbState;
struct MpegMotionRef;
struct MpegMotionState;
struct MpegSkipState;
struct MpegSliceDecoder;

int  _decMB0(struct MpegDecoder *, int *, int *, int *, struct MpegMbState *, int *, int);
s32  _slice0(struct MpegDecoder *, s32);
void _cpr8(struct MpegDecoder *, struct MpegCopyParams *);
void _doMC(struct MpegDecoder *, s32);
s32  _mbAddressIncrement();
void _motionVector(s32, struct MotionVector *, struct MotionVectorStore *, s32, s32, s32, s32, s32);
void _motionVectors();
s32  _skipMB0(struct MpegDecoder *, struct MpegSkipState *, s32 *, struct MpegMotionRef *, s32 *);
s32  _sliceA0(struct MpegSliceDecoder *, s32, s32 *, s32 *, volatile struct MpegMotionState *);

#endif /* LOMBYTE_RNC_SDK_MPEG_MACROBLOCK_H */
