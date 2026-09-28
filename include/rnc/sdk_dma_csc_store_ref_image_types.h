#ifndef RNC_SDK_DMA_CSC_STORE_REF_IMAGE_TYPES_H
#define RNC_SDK_DMA_CSC_STORE_REF_IMAGE_TYPES_H

#include "types.h"

struct M2c_arg0 {
    u8 pad_0[0xD8];
    s32 unkD8;
    u8 pad_DC[0x77C];
    s32 unk858;
};

struct M2c_arg1 {
    s32 unk0;
    u8 pad_4[0x8];
    s32 unkC;
    s32 unk10;
};

#endif /* RNC_SDK_DMA_CSC_STORE_REF_IMAGE_TYPES_H */
