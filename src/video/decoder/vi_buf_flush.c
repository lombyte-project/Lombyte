#include "types.h"
#include "rnc/video/decoder/vi_buf.h"
extern s32 WaitSema(s32);
extern s32 SignalSema(s32);
void vi_buf_flush(struct ViBuf *s) __asm__("FUN_0023c660");

void vi_buf_flush(struct ViBuf *vi_buf) {
    WaitSema(vi_buf->sema);
    vi_buf->read_bytes = (vi_buf->read_bytes + 0x7FF) / 0x800 * 0x800;
    SignalSema(vi_buf->sema);
}

extern __typeof__(vi_buf_flush) func_0023C660 __attribute__((alias("FUN_0023c660")));
