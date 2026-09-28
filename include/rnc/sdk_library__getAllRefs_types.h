#ifndef RNC_SDK_LIBRARY__GETALLREFS_TYPES_H
#define RNC_SDK_LIBRARY__GETALLREFS_TYPES_H

#include "types.h"

struct M2c_arg0 {
    u8 pad_0[0x120];
    s32 unk120;
    u8 pad_124[0x2C];
    s32 unk150;
    u8 pad_154[0x20];
    s32 unk174;
    u8 pad_178[0x40];
    s32 unk1B8;
    s32 unk1BC;
    u8 pad_1C0[0x8];
    s32 unk1C8;
    s32 unk1CC;
    u8 pad_1D0[0x8];
    s32 unk1D8;
    s32 unk1DC;
    u8 pad_1E0[0x630];
    s32 unk810;
};

struct M2c_arg5 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
};

struct M2c_arg6 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
};

#endif /* RNC_SDK_LIBRARY__GETALLREFS_TYPES_H */
