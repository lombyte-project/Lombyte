#include "types.h"
struct VoBuf {
    u8 pad_0[0x4];
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
};

extern s32 func_0023D2C8();
s32 vo_buf_get_tag(struct VoBuf *vo_buf) __asm__("FUN_0023d2d8");

s32 vo_buf_get_tag(struct VoBuf *vo_buf) {
    s32 ring_size;
    s32 unk8;
    s32 unkC;

    if (func_0023D2C8() != 0) {
        return 0;
    }
    unk8 = *((volatile s32 *)(&vo_buf->unk8));
    unkC = *((volatile s32 *)(&vo_buf->unkC));
    ring_size = vo_buf->unk10;
    return vo_buf->unk4 + (((s32)((unk8 - unkC) + ring_size) % ring_size) * 0x138C0);
}
