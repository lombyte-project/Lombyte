#include "types.h"

struct M2c_arg0 {
    u8 pad_0[0x40];
    s32 unk40;
};

extern s32 set_dma_channel_4_control_register(s32 buf) __asm__("FUN_0023bbb0");
extern s32 DeleteSema(s32 buf);

s32 vi_buf_delete(struct M2c_arg0 *buf) __asm__("FUN_0023c5b8");

s32 vi_buf_delete(struct M2c_arg0 *buf) {
    volatile s32 *p1;
    volatile s32 *p2;
    volatile s32 *p3;
    set_dma_channel_4_control_register(5);
    p1 = (volatile s32 *)0x1000B420;
    p2 = (volatile s32 *)0x1000B410;
    p3 = (volatile s32 *)0x1000B430;
    *p1 = 0;
    *p2 = 0;
    *p3 = 0;
    DeleteSema(buf->unk40);
    return 1;
}

extern __typeof__(vi_buf_delete) func_0023C5B8 __attribute__((alias("FUN_0023c5b8")));

/* Recovered original symbol name. */
extern __typeof__(vi_buf_delete) viBufDelete__FP5ViBuf __attribute__((alias("FUN_0023c5b8")));
