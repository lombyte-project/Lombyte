#ifndef RNC_SDK_LIBRARY__PES_PACKET_TYPES_H
#define RNC_SDK_LIBRARY__PES_PACKET_TYPES_H

#include "types.h"

struct M2c_arg1 {
    u8 pad_0[0x18];
    s32 unk18;
};

struct M2c_arg2 {
    s64 unk0;
    s32 unk8;
    s32 unkC;
    s64 unk10;
    s64 unk18;
    s32 unk20;
    s32 unk24;
    s32 unk28;
};

#endif /* RNC_SDK_LIBRARY__PES_PACKET_TYPES_H */
