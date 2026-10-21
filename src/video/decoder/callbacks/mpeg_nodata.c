#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/video/decoder/decoder_globals.h"

extern void switch_thread() __asm__("func_0023A770");
extern void vi_buf_add_dma() __asm__("func_0023BF70");

s32 mpeg_nodata(void) __asm__("FUN_0023d0a8");

s32 mpeg_nodata(void) {
    s32 decoder_state_base;
    switch_thread();
    decoder_state_base = decoder_state;
    vi_buf_add_dma(decoder_state_base + 0xD9090);
    return 1;
}
