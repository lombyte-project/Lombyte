#include "types.h"

struct VdecCb {
    s32 type;
    s32 pad4;
    u8 *data;
    s32 len;
    s64 pts;
    s64 dts;
};

#include "rnc/movies/decoder/decoder_globals.h"
#include "rnc/movies/player/movie_player.h"

extern u8   pts_buffer_overflow_message[] __asm__("D_001E8B08");
extern void err_message(u8 *) __asm__("func_0023AB78");
extern s32  cpy2area(u32, s32, u32, s32, u8 *, s32, struct ReadBuf *, s32) __asm__("func_0023B810");
extern void
video_dec_begin_put(struct VideoDec *, u32 *, s32 *, u32 *, s32 *) __asm__("func_0023CBF0");
extern void video_dec_end_put(struct VideoDec *, s32) __asm__("func_0023CC10");
extern s32  video_dec_put_ts(struct VideoDec *, s64, s64, u32, s32) __asm__("func_0023CC98");

s32 video_callback(s32             unused_callback_arg,
                   struct VdecCb  *cb,
                   struct ReadBuf *ab) __asm__("FUN_0023b5f0");

s32 video_callback(s32 unused_callback_arg, struct VdecCb *cb, struct ReadBuf *ab) {
    u32 first_destination_address;
    s32 first_destination_byte_capacity;
    u32 second_destination_address;
    s32 second_destination_byte_capacity;
    u8 *ps;
    u8 *end;
    s32 len;
    s32 first_source_byte_count;
    s32 rest;
    s32 copied_byte_count;

    len = cb->len;
    ps = cb->data;
    end = ab->data + ab->size;
    first_source_byte_count = end - ps;
    if (len < first_source_byte_count) {
        first_source_byte_count = len;
    }
    rest = len - first_source_byte_count;
    video_dec_begin_put(&movie_player->video_dec,
                        &first_destination_address,
                        &first_destination_byte_capacity,
                        &second_destination_address,
                        &second_destination_byte_capacity);
    copied_byte_count = cpy2area((first_destination_address & 0x0FFFFFFF) | 0x20000000,
                                 first_destination_byte_capacity,
                                 (second_destination_address & 0x0FFFFFFF) | 0x20000000,
                                 second_destination_byte_capacity,
                                 ps,
                                 first_source_byte_count,
                                 ab,
                                 rest);
    if (copied_byte_count > 0) {
        if (video_dec_put_ts(&movie_player->video_dec,
                             cb->pts,
                             cb->dts,
                             first_destination_address,
                             copied_byte_count) == 0) {
            err_message(pts_buffer_overflow_message);
        }
    }
    video_dec_end_put(&movie_player->video_dec, copied_byte_count);
    return copied_byte_count > 0;
}

extern __typeof__(video_callback) func_0023B5F0 __attribute__((alias("FUN_0023b5f0")));
