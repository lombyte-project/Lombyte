#include "types.h"
#include "rnc/video/decoder/vi_buf.h"

extern s32 FUN_00118990();
extern s32 FUN_001189b0();
s32 vi_buf_count(struct ViBuf *buf) __asm__("FUN_0023c610");

s32 vi_buf_count(struct ViBuf *buf) {
    s32 count;

    FUN_001189b0(buf->sema);
    count = (buf->dma_n << 0xB) + buf->read_bytes;
    FUN_00118990(buf->sema);
    return count;
}

extern s32 func_0023C610(struct ViBuf *buf) __attribute__((alias("FUN_0023c610")));
