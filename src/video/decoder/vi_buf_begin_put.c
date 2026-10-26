#include "types.h"
#include "rnc/video/decoder/vi_buf.h"

extern s32 signal_semaphore() __asm__("FUN_00118990");
extern s32 wait_semaphore() __asm__("FUN_001189b0");
void vi_buf_begin_put(struct ViBuf *buf, s32 *ptr0, s32 *len0, s32 *ptr1,
                      s32 *len1) __asm__("FUN_0023be20");

void vi_buf_begin_put(struct ViBuf *ring_buffer, s32 *first_write_address_out, s32 *first_write_byte_count_out, s32 *wrapped_write_address_out, s32 *wrapped_write_byte_count_out) {
    s32 temp_2_37;
    s32 temp_3_22;
    s32 temp_4_17;
    s32 temp_5_19;
    s32 temp_5_32;
    s32 temp_6_20;
    s32 temp_hi_29;

    wait_semaphore(ring_buffer->sema);
    temp_4_17 = ring_buffer->dma_n;
    temp_5_19 = ring_buffer->read_bytes;
    temp_6_20 = temp_4_17 + 2;
    temp_3_22 = ring_buffer->buff_size;
    temp_5_32 = ((ring_buffer->n - temp_6_20) << 0xB) - temp_5_19;
    temp_hi_29 = (s32)(((ring_buffer->dma_start + temp_4_17) << 0xB) + temp_5_19) % temp_3_22;
    if ((temp_3_22 - temp_hi_29) >= temp_5_32) {
        *first_write_address_out = (s32)ring_buffer->data + temp_hi_29;
        *first_write_byte_count_out = temp_5_32;
        *wrapped_write_address_out = 0;
        *wrapped_write_byte_count_out = 0;
    } else {
        *first_write_address_out = (s32)ring_buffer->data + temp_hi_29;
        *first_write_byte_count_out = ring_buffer->buff_size - temp_hi_29;
        *wrapped_write_address_out = (s32)ring_buffer->data;
        *wrapped_write_byte_count_out = temp_5_32 - (ring_buffer->buff_size - temp_hi_29);
    }
    SignalSema(ring_buffer->sema, temp_5_32, temp_6_20, temp_hi_29);
}
