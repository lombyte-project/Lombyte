#include "types.h"
#include "rnc/movies/decoder/vo_buf.h"

void vo_buf_create(struct VoBuf  *buf,
                   struct VoData *data,
                   struct VoTag  *tag,
                   s32            size) __asm__("FUN_0023d190");

void vo_buf_create(struct VoBuf *buf, struct VoData *data, struct VoTag *tag, s32 size) {
    s32 i;

    buf->count = 0;
    buf->data = data;
    buf->tag = tag;
    buf->size = size;
    buf->write = 0;
    for (i = 0; i < size; i++) {
        buf->tag[i].status = 0;
    }
}

extern __typeof__(vo_buf_create) func_0023D190 __attribute__((alias("FUN_0023d190")));
