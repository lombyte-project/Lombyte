#include "types.h"
struct M2c_arg0 {
    u8 pad_0[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad_18[0x28];
    s32 unk40;
};

extern s32 FUN_00118990();
extern s32 FUN_001189b0();
s32 vi_buf_count(struct M2c_arg0 *arg0) __asm__("FUN_0023c610");

s32 vi_buf_count(struct M2c_arg0 *arg0) {
    s32 count;

    FUN_001189b0(arg0->unk40);
    count = (arg0->unk10 << 0xB) + arg0->unk14;
    FUN_00118990(arg0->unk40);
    return count;
}

extern s32 func_0023C610(struct M2c_arg0 *arg0) __attribute__((alias("FUN_0023c610")));
