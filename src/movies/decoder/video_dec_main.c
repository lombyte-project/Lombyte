#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/movies/decoder/decoder_globals.h"
#include "rnc/movies/player/movie_player.h"

extern s32 vi_buf_reset() __asm__("func_0023BCC0");
extern s32 dec_bs0() __asm__("func_0023CEC8");

void video_dec_main(struct VideoDec *vd) __asm__("FUN_0023ce28");

void video_dec_main(struct VideoDec *vd) {
    vi_buf_reset(&vd->vi_buf);
    vo_buf_reset(&movie_player->vo_buf);
    dec_bs0(vd);
    while (movie_player->vo_buf.count != 0) {
        if (video_dec_get_state(vd) == VD_STATE_ABORT) {
            break;
        }
    }
    video_dec_set_state(vd, VD_STATE_END);
}

extern __typeof__(video_dec_main) func_0023CE28 __attribute__((alias("FUN_0023ce28")));
