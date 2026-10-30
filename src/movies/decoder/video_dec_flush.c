#include "types.h"
#include "rnc/movies/decoder/video_dec.h"
#include "rnc/data_labels.h"
#include "rnc/movies/decoder/decoder_globals.h"
#include "rnc/movies/player/movie_player.h"

struct Code4 {
    u8 b[4];
};

extern struct Code4 video_dec_flush_code[] __asm__("D_00161218");
extern void
video_dec_begin_put(struct VideoDec *, u32 *, s32 *, u32 *, s32 *) __asm__("func_0023CBF0");
extern s32  cpy2area(u32, s32, u32, s32, void *, s32, s32, s32) __asm__("func_0023B810");
extern void video_dec_end_put(struct VideoDec *, s32) __asm__("func_0023CC10");
extern void vi_buf_flush(void *) __asm__("func_0023C660");
s32         video_dec_flush(struct VideoDec *vd) __asm__("FUN_0023cd08");

s32 video_dec_flush(struct VideoDec *vd) {
    struct Code4 code = video_dec_flush_code[0];
    u32          destination_a;
    s32          destination_a_len;
    u32          destination_b;
    s32          destination_b_len;
    s32          copied_bytes;

    video_dec_begin_put(vd, &destination_a, &destination_a_len, &destination_b, &destination_b_len);
    if (destination_a_len + destination_b_len < 4) {
        return 0;
    }
    copied_bytes = cpy2area((destination_a & 0x0FFFFFFF) | 0x20000000,
                            destination_a_len,
                            (destination_b & 0x0FFFFFFF) | 0x20000000,
                            destination_b_len,
                            &code,
                            4,
                            0,
                            0);
    video_dec_end_put(&movie_player->video_dec, copied_bytes);
    vi_buf_flush(&vd->vi_buf);
    if (vd->state == VD_STATE_NORMAL) {
        vd->state = VD_STATE_FLUSH;
    }
    return 1;
}

extern __typeof__(video_dec_flush) func_0023CD08 __attribute__((alias("FUN_0023cd08")));
