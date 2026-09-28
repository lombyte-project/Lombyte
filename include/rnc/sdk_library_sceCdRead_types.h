#ifndef RNC_SDK_LIBRARY_SCECDREAD_TYPES_H
#define RNC_SDK_LIBRARY_SCECDREAD_TYPES_H

#include "types.h"

struct M2c_D_001313C0 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    u8 unkC;
    u8 unkD;
    u8 unkE;
    u8 pad_F[0x1];
    s32 unk10;
    s32 unk14;
};

struct M2c_arg3 {
    u8 unk0;
    u8 unk1;
    s32 unk2;
};

#endif /* RNC_SDK_LIBRARY_SCECDREAD_TYPES_H */
