#include "types.h"
struct ViBuf {
    s32 unk0;
    u8 pad_4[0x4];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad_1C[0x24];
    s32 unk40;
};

extern s32 FUN_00118990();
extern s32 FUN_001189b0();
void vi_buf_begin_put(struct ViBuf *buf, s32 *ptr0, s32 *len0, s32 *ptr1,
                      s32 *len1) __asm__("FUN_0023be20");

void vi_buf_begin_put(struct ViBuf *buf, s32 *ptr0, s32 *len0, s32 *ptr1, s32 *len1) {
    s32 temp_2_37;
    s32 temp_3_22;
    s32 temp_4_17;
    s32 temp_5_19;
    s32 temp_5_32;
    s32 temp_6_20;
    s32 temp_hi_29;

    FUN_001189b0(buf->unk40);
    temp_4_17 = buf->unk10;
    temp_5_19 = buf->unk14;
    temp_6_20 = temp_4_17 + 2;
    temp_3_22 = buf->unk18;
    temp_5_32 = ((buf->unk8 - temp_6_20) << 0xB) - temp_5_19;
    temp_hi_29 = (s32)(((buf->unkC + temp_4_17) << 0xB) + temp_5_19) % temp_3_22;
    if ((temp_3_22 - temp_hi_29) >= temp_5_32) {
        *ptr0 = buf->unk0 + temp_hi_29;
        *len0 = temp_5_32;
        *ptr1 = 0;
        *len1 = 0;
    } else {
        *ptr0 = buf->unk0 + temp_hi_29;
        *len0 = buf->unk18 - temp_hi_29;
        *ptr1 = buf->unk0;
        *len1 = temp_5_32 - (buf->unk18 - temp_hi_29);
    }
    SignalSema(buf->unk40, temp_5_32, temp_6_20, temp_hi_29);
}
