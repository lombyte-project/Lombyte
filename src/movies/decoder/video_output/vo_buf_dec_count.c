#include "types.h"
#include "rnc/movies/decoder/vo_buf.h"

void vo_buf_dec_count(struct VoBuf *buf) __asm__("func_0023D340");

void vo_buf_dec_count(struct VoBuf *video_output_buffer) {
    if (video_output_buffer->count > 0) {
        video_output_buffer->count--;
    }
}
