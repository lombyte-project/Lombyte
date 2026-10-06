#ifndef LOMBYTE_RNC_SDK_LIBRARY_MOTION_VECTOR_H
#define LOMBYTE_RNC_SDK_LIBRARY_MOTION_VECTOR_H

#include "types.h"

struct MotionVector {
    u8 pad_0[0x4];
    s32 unk4;
};

struct MotionVectorStore {
    s32 unk0;
    s32 unk4;
};

#endif /* LOMBYTE_RNC_SDK_LIBRARY_MOTION_VECTOR_H */
