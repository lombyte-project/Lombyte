#include "types.h"
#include "rnc/video/decoder/vi_buf.h"

extern s32 signal_semaphore() __asm__("FUN_00118990");
extern s32 wait_semaphore() __asm__("FUN_001189b0");
s32 vi_buf_count(struct ViBuf *buf) __asm__("FUN_0023c610");

s32 vi_buf_count(struct ViBuf *buf) {
    s32 count;

    wait_semaphore(buf->sema);
    count = (buf->dma_n << 0xB) + buf->read_bytes;
    signal_semaphore(buf->sema);
    return count;
}

extern s32 func_0023C610(struct ViBuf *buf) __attribute__((alias("FUN_0023c610")));
