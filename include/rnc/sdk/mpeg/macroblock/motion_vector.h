#ifndef LOMBYTE_RNC_SDK_MPEG_MACROBLOCK_MOTION_VECTOR_H
#define LOMBYTE_RNC_SDK_MPEG_MACROBLOCK_MOTION_VECTOR_H

#include "types.h"

struct MotionVector;
struct MotionVectorStore;

void _motionVector(s32, struct MotionVector *, struct MotionVectorStore *, s32, s32, s32, s32, s32);

#endif /* LOMBYTE_RNC_SDK_MPEG_MACROBLOCK_MOTION_VECTOR_H */
