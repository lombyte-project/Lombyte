#include "types.h"
#include "rnc/movies/decoder/vo_buf.h"

struct VoData *vo_buf_get_data(struct VoBuf *buf) __asm__("FUN_0023d288");

struct VoData *vo_buf_get_data(struct VoBuf *buf) {
    if (vo_buf_is_full(buf) != 0) {
        return 0;
    }
    return buf->data + buf->write;
}

/* Recovered original symbol name. */
extern __typeof__(vo_buf_get_data) voBufGetData__FP5VoBuf __attribute__((alias("FUN_0023d288")));
