/* Ported from rac1-decomp (src/game/movie/videodec.c, func_0023E298). */
#ifndef COMMON_H
#include "sda.h"
#endif /* STRUCTS_H */
extern int DebugPrint();

#include "types.h"
#include "rnc/movies/decoder/video_dec.h"
#include "rnc/movies/player/movie_player.h"

extern struct MoviePlayer *movie_player_macro __asm__("D_0016120C") MACRO_ADDR;
#define voBuf (movie_player_macro->vo_buf)
extern s32  sceMpegIsEnd(struct sceMpeg *mpeg) __asm__("func_0012BA48");
extern int  sceMpegGetPicture(struct sceMpeg *, void *, int);
extern void sceMpegReset(struct sceMpeg *);
extern void set_image_tag(void *, int, int, int) __asm__("func_0023B210"); /* setImageTag */
extern void switch_thread(void) __asm__("func_0023A770");                  /* switchThread */
extern void err_message(char *) __asm__("func_0023AB78");                  /* ErrMessage */
extern char decode_thread_aborted_message[] __asm__("D_001E8B38");
extern char mpeg_get_picture_decode_error_message[] __asm__("D_001E8B50");
/* decBs0(VideoDec *) */
int dec_bs0(struct VideoDec *vd) __asm__("FUN_0023cec8");

int dec_bs0(struct VideoDec *vd) {
    struct VoData *voData;
    int            status = 1;
    int            i;

    while (!sceMpegIsEnd(&vd->mpeg)) {
        if (video_dec_get_state(vd) == VD_STATE_ABORT) {
            status = -1;
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
    return status;
}

extern __typeof__(dec_bs0) func_0023CEC8 __attribute__((alias("FUN_0023cec8")));
