#include "types.h"
#include "rnc/movies/decoder/vo_buf.h"

void vo_buf_dec_count(struct VoBuf *buf) __asm__("func_0023D340");

void vo_buf_dec_count(struct VoBuf *buf) {
    if (buf->count > 0) {
        buf->count--;
    }
}
