#ifndef RNC_SDK_LIBRARY__DECODEORSKIP_TYPES_H
#define RNC_SDK_LIBRARY__DECODEORSKIP_TYPES_H

#include "types.h"

struct M2c_arg0 {
    u8 pad_0[0x40];
    struct M2c_arg0_unk40 * unk40;
};

struct M2c_arg0_unk40 {
    u8 pad_0[0x174];
    s32 unk174;
};

#endif /* RNC_SDK_LIBRARY__DECODEORSKIP_TYPES_H */
