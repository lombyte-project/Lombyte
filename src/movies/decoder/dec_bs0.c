/* Ported from rac1-decomp (src/game/movie/videodec.c, func_0023E298). */
#ifndef COMMON_H
#include "sda.h"
#endif /* STRUCTS_H */

#include "types.h"
#include "rnc/movies/decoder/video_dec.h"
#include "rnc/movies/player/movie_player.h"
#include "rnc/sdk/debug.h"
#include "rnc/sdk/mpeg/api.h"

extern struct MoviePlayer *movie_player_macro __asm__("D_0016120C") MACRO_ADDR;
#define voBuf (movie_player_macro->vo_buf)
extern void set_image_tag(void *, int, int, int) __asm__("func_0023B210"); /* setImageTag */
extern void switch_thread(void) __asm__("func_0023A770");                  /* switchThread */
extern void err_message(const char *) __asm__("func_0023AB78");            /* ErrMessage */
/* decBs0(VideoDec *) */
int dec_bs0(struct VideoDec *vd) __asm__("FUN_0023cec8");

const char decode_thread_aborted_message[] __asm__("D_001E8B38") = "decode thread: aborted\n";
const char mpeg_get_picture_decode_error_message[] __asm__("D_001E8B50") =
    "sceMpegGetPicture() decode error";

int dec_bs0(struct VideoDec *vd) {
    enum {
        DEC_BS0_COMPLETED = 1
    };
    struct VoData *voData;
    int            completion_status = DEC_BS0_COMPLETED;
    int            i;

    while (!sceMpegIsEnd(&vd->mpeg)) {
        if (video_dec_get_state(vd) == VD_STATE_ABORT) {
            completion_status = -1;
            DebugPrint(decode_thread_aborted_message);
            break;
        }

        while (!(voData = vo_buf_get_data(&voBuf))) {
            switch_thread();
        }

        if (sceMpegGetPicture(&vd->mpeg, voData->v, 0x340) < 0) {
            err_message(mpeg_get_picture_decode_error_message);
        }

        if (vd->mpeg.frameCount == 0) {
            int image_w = vd->mpeg.width;
            int image_h = vd->mpeg.height;

            for (i = 0; i < voBuf.size; i++) {
                set_image_tag(voBuf.tag[i].body, (int)voBuf.data[i].v, image_w, image_h);
            }
        }

        vo_buf_inc_count(&voBuf);
        switch_thread();
    }
    sceMpegReset(&vd->mpeg);
    return completion_status;
}

extern __typeof__(dec_bs0) func_0023CEC8 __attribute__((alias("FUN_0023cec8")));
