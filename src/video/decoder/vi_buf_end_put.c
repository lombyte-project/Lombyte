#include "types.h"
struct ViBuf {
    u8 pad_0[0x14];
    s32 unk14;
    u8 pad_18[0x28];
    s32 unk40;
    u8 pad_44[0x4];
    s64 unk48;
};

extern s32 FUN_00118990();
extern s32 FUN_001189b0();
void vi_buf_end_put(struct ViBuf *buf, s32 bytes) __asm__("FUN_0023bf18");

void vi_buf_end_put(struct ViBuf *buf, s32 bytes) {
    FUN_001189b0(buf->unk40);
    buf->unk14 = (s32)(buf->unk14 + bytes);
    buf->unk48 = (s64)(bytes + buf->unk48);
    FUN_00118990(buf->unk40);
}

extern void func_0023BF18(struct ViBuf *buf, s32 bytes) __attribute__((alias("FUN_0023bf18")));
