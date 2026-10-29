#include "sda.h"
extern char *decoder_state_macro __asm__("D_0016120C")MACRO_ADDR;
extern void vi_buf_restart_dma(void *) __asm__("FUN_0023c280");

int mpeg_restart_video_dma(void) __asm__("FUN_0023d110");

int mpeg_restart_video_dma(void) {
    vi_buf_restart_dma(decoder_state_macro + 0xD9090);
    return 1;
}

extern __typeof__(mpeg_restart_video_dma) func_0023D110 __attribute__((alias("FUN_0023d110")));
/* Recovered original symbol name. */
extern __typeof__(mpeg_restart_video_dma) mpegRestartVideoDMA__Fv
    __attribute__((alias("FUN_0023d110")));
