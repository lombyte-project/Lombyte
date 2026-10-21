#include "types.h"
#include "rnc/video/decoder/vi_buf.h"

extern s32 set_d4_chcr(s32 buf) __asm__("FUN_0023bbb0");
extern s32 DeleteSema(s32 buf);

s32 vi_buf_delete(struct ViBuf *buf) __asm__("FUN_0023c5b8");

s32 vi_buf_delete(struct ViBuf *decoder_buffer) {
    volatile s32 *dma4_qwc_register;
    volatile s32 *dma4_madr_register;
    volatile s32 *dma4_tadr_register;
    set_d4_chcr(5);
    dma4_qwc_register = (volatile s32 *)0x1000B420;
    dma4_madr_register = (volatile s32 *)0x1000B410;
    dma4_tadr_register = (volatile s32 *)0x1000B430;
    *dma4_qwc_register = 0;
    *dma4_madr_register = 0;
    *dma4_tadr_register = 0;
    DeleteSema(decoder_buffer->sema);
    return 1;
}

extern __typeof__(vi_buf_delete) func_0023C5B8 __attribute__((alias("FUN_0023c5b8")));

/* Recovered original symbol name. */
extern __typeof__(vi_buf_delete) viBufDelete__FP5ViBuf __attribute__((alias("FUN_0023c5b8")));
