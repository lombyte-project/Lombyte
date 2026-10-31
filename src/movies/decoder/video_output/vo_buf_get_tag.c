#include "types.h"
#include "rnc/movies/decoder/vo_buf.h"

struct VoTag *vo_buf_get_tag(struct VoBuf *buf) __asm__("FUN_0023d2d8");

/* The oldest picture not yet shown. */
struct VoTag *vo_buf_get_tag(struct VoBuf *picture_queue) {
    if (vo_buf_is_empty(picture_queue) != 0) {
        return 0;
    }
    return picture_queue->tag + (picture_queue->write - picture_queue->count + picture_queue->size) % picture_queue->size;
}
