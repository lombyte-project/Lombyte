#ifndef RNC_SDK_LIBRARY__DECMB0_TYPES_H
#define RNC_SDK_LIBRARY__DECMB0_TYPES_H

#include "types.h"

struct M2c_arg0 {
    u8 pad_0[0x11C];
    int unk11C;
    u8 pad_120[0x30];
    int unk150;
    int unk154;
    int unk158;
    int unk15C;
    int unk160;
    int unk164;
    int unk168;
    int unk16C;
    int unk170;
    int unk174;
    u8 pad_178[0x4];
    int unk17C;
    int unk180;
    u8 pad_184[0x2C];
    int unk1B0;
    int unk1B4;
    u8 pad_1B8[0x658];
    int unk810;
    u8 pad_814[0x34];
    int unk848;
};

struct M2c_arg4 {
    int unk0;
    int unk4;
    int unk8;
    int unkC;
    int unk10;
    int unk14;
    int unk18;
    int unk1C;
};

#endif /* RNC_SDK_LIBRARY__DECMB0_TYPES_H */
