#include "types.h"
struct ViBuf {
    u8 pad_0[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad_18[0x28];
    s32 unk40;
};

extern s32 FUN_00118990();
extern s32 FUN_001189b0();
s32 vi_buf_count(struct ViBuf *buf) __asm__("FUN_0023c610");

s32 vi_buf_count(struct ViBuf *buf) {
    s32 count;

    FUN_001189b0(buf->unk40);
    count = (buf->unk10 << 0xB) + buf->unk14;
    FUN_00118990(buf->unk40);
    return count;
}

extern s32 func_0023C610(struct ViBuf *buf) __attribute__((alias("FUN_0023c610")));
