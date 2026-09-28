#ifndef RNC_SDK_LIBRARY_SEQUENCEHEADER_TYPES_H
#define RNC_SDK_LIBRARY_SEQUENCEHEADER_TYPES_H

#include "types.h"

struct M2c_arg0 {
    u8 pad_0[0xD4];
    s32 unkD4;
    u8 pad_D8[0x4C];
    u32 unk124;
    s32 unk128;
    u8 pad_12C[0x8];
    u32 unk134;
    s32 unk138;
    u8 pad_13C[0x704];
    s32 unk840;
    s32 unk844;
    u8 pad_848[0x10];
    s32 unk858;
};

struct M2c_arg0__func_0012C4C8 {
    s32 unk0;
    s32 unk4;
    u8 pad_8[0x38];
    struct M2c_temp_30_15__func_0012C4C8 * unk40;
};

struct M2c_temp_30_15__func_0012C4C8 {
    u8 pad_0[0xFC];
    s32 unkFC;
    s32 unk100;
    s32 unk104;
    u8 pad_108[0x1C];
    s32 unk124;
    s32 unk128;
    s32 unk12C;
    s32 unk130;
    u8 pad_134[0x8];
    s32 unk13C;
    s32 unk140;
    s32 unk144;
    u8 pad_148[0x2C];
    s32 unk174;
    u8 pad_178[0x4];
    s32 unk17C;
    u8 pad_180[0x8];
    s32 unk188;
    u8 pad_18C[0x6BC];
    s32 unk848;
};

#endif /* RNC_SDK_LIBRARY_SEQUENCEHEADER_TYPES_H */
