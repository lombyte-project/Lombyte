#include "types.h"
#include "rnc/movies/decoder/vo_buf.h"

s32 vo_buf_is_empty(struct VoBuf *buf) __asm__("func_0023D2C8");

s32 vo_buf_is_empty(struct VoBuf *buf) {
    return buf->count == 0;
}
