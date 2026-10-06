#include "types.h"
extern s32 WaitSema(s32);
extern s32 SignalSema(s32);
struct ViBuf {
    u8 pad0[0x14];
    s32 pos;
    u8 pad18[0x28];
    s32 sema;
};
void vi_buf_flush(struct ViBuf *s) __asm__("FUN_0023c660");

void vi_buf_flush(struct ViBuf *s) {
    WaitSema(s->sema);
    s->pos = (s->pos + 0x7FF) / 0x800 * 0x800;
    SignalSema(s->sema);
}

extern __typeof__(vi_buf_flush) func_0023C660 __attribute__((alias("FUN_0023c660")));
