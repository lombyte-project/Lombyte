#include "types.h"
#include "rnc/movies/decoder/vo_buf.h"
#include "rnc/sdk/interrupts.h"

extern s32 DIntr(void);
void       vo_buf_inc_count(struct VoBuf *buf) __asm__("FUN_0023d210");

void vo_buf_inc_count(struct VoBuf *video_output_buffer) {
    DIntr();
    video_output_buffer->tag[video_output_buffer->write].status = VO_TAG_FILLED;
    video_output_buffer->count++;
    video_output_buffer->write = (video_output_buffer->write + 1) % video_output_buffer->size;
    EnableInterrupts();
}

extern __typeof__(vo_buf_inc_count) func_0023D210 __attribute__((alias("FUN_0023d210")));
