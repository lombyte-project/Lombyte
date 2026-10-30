#include "types.h"
#include "rnc/movies/decoder/vo_buf.h"

struct VoTag *vo_buf_get_tag(struct VoBuf *buf) __asm__("FUN_0023d2d8");

/* The oldest picture not yet shown. */
struct VoTag *vo_buf_get_tag(struct VoBuf *buf) {
    if (vo_buf_is_empty(buf) != 0) {
        return 0;
    }
    return buf->tag + (buf->write - buf->count + buf->size) % buf->size;
}
