#ifndef RNC_RENDERING_SETUP_GIF_PAGING_TYPES_H
#define RNC_RENDERING_SETUP_GIF_PAGING_TYPES_H

#include "types.h"

struct M2c_D_0019A3E8 {
    u8 pad_0[0x18];
    struct M2c_D_0019A3E8_unk18 * unk18;
    u8 pad_1C[0x8];
    s32 unk24;
    s32 unk28;
};

struct M2c_D_0019A3E8_unk18 {
    u8 pad_0[0x24];
    s32 unk24;
    u8 pad_28[0x1C];
    s32 unk44;
};

struct M2c_temp_4_45 {
    u8 pad_0[0x4];
    s32 unk4;
};

#endif /* RNC_RENDERING_SETUP_GIF_PAGING_TYPES_H */
