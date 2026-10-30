#include "types.h"
#include "rnc/movies/decoder/vo_buf.h"

void vo_buf_reset(struct VoBuf *buf) __asm__("func_0023D1E8");

void vo_buf_reset(struct VoBuf *buf) {
    buf->count = 0;
    buf->write = 0;
}

/* Recovered original symbol name. */
extern __typeof__(vo_buf_reset) voBufReset__FP5VoBuf __attribute__((alias("func_0023D1E8")));
