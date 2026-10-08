#include "types.h"
#include "rnc/video/decoder/vi_buf.h"

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

    FUN_001189b0(buf->sema);
    temp_4_17 = buf->dma_n;
    temp_5_19 = buf->read_bytes;
    temp_6_20 = temp_4_17 + 2;
    temp_3_22 = buf->buff_size;
    temp_5_32 = ((buf->n - temp_6_20) << 0xB) - temp_5_19;
    temp_hi_29 = (s32)(((buf->dma_start + temp_4_17) << 0xB) + temp_5_19) % temp_3_22;
    if ((temp_3_22 - temp_hi_29) >= temp_5_32) {
        *ptr0 = (s32)buf->data + temp_hi_29;
        *len0 = temp_5_32;
        *ptr1 = 0;
        *len1 = 0;
    } else {
        *ptr0 = (s32)buf->data + temp_hi_29;
        *len0 = buf->buff_size - temp_hi_29;
        *ptr1 = (s32)buf->data;
        *len1 = temp_5_32 - (buf->buff_size - temp_hi_29);
    }
    SignalSema(buf->sema, temp_5_32, temp_6_20, temp_hi_29);
}
