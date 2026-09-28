#ifndef RNC_VIDEO_DECODER_VI_BUF_MODIFY_PTS_TYPES_H
#define RNC_VIDEO_DECODER_VI_BUF_MODIFY_PTS_TYPES_H

#include "types.h"

struct M2c_arg0 {
    u8 pad_0[0x8];
    s32 unk8;
    u8 pad_C[0x44];
    struct M2c_var_6_23 * unk50;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
};

struct M2c_arg1 {
    u8 pad_0[0x10];
    s32 unk10;
    s32 unk14;
};

struct M2c_var_6_23 {
    s32 unk0;
    u8 pad_4[0x4];
    s32 unk8;
    u8 pad_C[0x4];
    s32 unk10;
    s32 unk14;
};

#endif /* RNC_VIDEO_DECODER_VI_BUF_MODIFY_PTS_TYPES_H */
