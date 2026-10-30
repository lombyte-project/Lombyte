#include "types.h"
#include "rnc/movies/decoder/vi_buf.h"

extern s32 signal_semaphore() __asm__("FUN_00118990");
extern s32 wait_semaphore() __asm__("FUN_001189b0");
void       vi_buf_begin_put(struct ViBuf *buf, s32 *ptr0, s32 *len0, s32 *ptr1, s32 *len1) __asm__("FUN_0023be20");

void vi_buf_begin_put(struct ViBuf *ring_buffer,
                      s32          *first_write_address_out,
                      s32          *first_write_byte_count_out,
                      s32          *wrapped_write_address_out,

                      s32 *wrapped_write_byte_count_out) {
    s32 size;
    s32 pending;
    s32 offset;
    s32 free_bytes;
    s32 used_blocks;
    s32 write_pos;

    wait_semaphore(ring_buffer->sema);
    pending = ring_buffer->dma_n;
    offset = ring_buffer->read_bytes;
    used_blocks = pending + 2;
    size = ring_buffer->buff_size;
    free_bytes = ((ring_buffer->n - used_blocks) << 0xB) - offset;
    write_pos = (s32)(((ring_buffer->dma_start + pending) << 0xB) + offset) % size;
    if ((size - write_pos) >= free_bytes) {
        *first_write_address_out = (s32)ring_buffer->data + write_pos;
        *first_write_byte_count_out = free_bytes;
        *wrapped_write_address_out = 0;
        *wrapped_write_byte_count_out = 0;
    } else {
        *first_write_address_out = (s32)ring_buffer->data + write_pos;
        *first_write_byte_count_out = ring_buffer->buff_size - write_pos;
        *wrapped_write_address_out = (s32)ring_buffer->data;
        *wrapped_write_byte_count_out = free_bytes - (ring_buffer->buff_size - write_pos);
    }
    SignalSema(ring_buffer->sema, free_bytes, used_blocks, write_pos);
}
