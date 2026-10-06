#include "types.h"

extern s32 D_0016120C;
extern void switch_thread() __asm__("func_0023A770");
extern void vi_buf_add_dma() __asm__("func_0023BF70");

s32 mpeg_nodata(void) __asm__("FUN_0023d0a8");

s32 mpeg_nodata(void) {
    s32 v;
    switch_thread();
    v = D_0016120C;
    vi_buf_add_dma(v + 0xD9090);
    return 1;
}
