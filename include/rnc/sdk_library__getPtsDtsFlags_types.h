#ifndef RNC_SDK_LIBRARY__GETPTSDTSFLAGS_TYPES_H
#define RNC_SDK_LIBRARY__GETPTSDTSFLAGS_TYPES_H

#include "types.h"

struct M2c_arg0 {
    u8 pad_0[0x70];
    s32 unk70;
    u8 pad_74[0x4];
    s32 unk78;
    u8 pad_7C[0x4];
    s32 unk80;
    u8 pad_84[0x4];
    s32 unk88;
    u8 pad_8C[0x4];
    s32 unk90;
    u8 pad_94[0x5C];
    s32 unkF0;
    u8 pad_F4[0x4];
    s32 unkF8;
};

struct M2c_arg1 {
    u8 pad_0[0x18];
    s32 unk18;
    u8 pad_1C[0x4];
    s32 unk20;
    u8 pad_24[0x8];
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
};

#endif /* RNC_SDK_LIBRARY__GETPTSDTSFLAGS_TYPES_H */
