#include "types.h"
#include "rnc/movies/decoder/vo_buf.h"

s32 vo_buf_is_full(struct VoBuf *buf) __asm__("func_0023D1F8");

s32 vo_buf_is_full(struct VoBuf *buf) {
    return buf->count == buf->size;
}
