#include "types.h"
#include "rnc/movies/decoder/vo_buf.h"

extern s32 DIntr(void);
extern s32 EnableInterrupts(void);
void       vo_buf_inc_count(struct VoBuf *buf) __asm__("FUN_0023d210");

void vo_buf_inc_count(struct VoBuf *buf) {
    DIntr();
    buf->tag[buf->write].status = VO_TAG_FILLED;
    buf->count++;
    buf->write = (buf->write + 1) % buf->size;
    EnableInterrupts();
}

extern __typeof__(vo_buf_inc_count) func_0023D210 __attribute__((alias("FUN_0023d210")));
