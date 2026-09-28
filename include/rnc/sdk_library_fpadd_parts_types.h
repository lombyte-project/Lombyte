#ifndef RNC_SDK_LIBRARY_FPADD_PARTS_TYPES_H
#define RNC_SDK_LIBRARY_FPADD_PARTS_TYPES_H

#include "types.h"

struct M2c_arg0 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    u8 pad_C[0x4];
    s32 unk10;
};

struct M2c_arg1 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    u8 pad_C[0x4];
    u64 unk10;
};

struct M2c_arg2 {
    s64 unk0;
    s32 unk4;
    s64 unk8;
    u64 unk10;
};

#endif /* RNC_SDK_LIBRARY_FPADD_PARTS_TYPES_H */
