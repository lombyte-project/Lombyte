#ifndef LOMBYTE_RNC_MOVIES_DECODER_VIDEO_DEC_H
#define LOMBYTE_RNC_MOVIES_DECODER_VIDEO_DEC_H

#include "types.h"
#include "rnc/sdk/libmpeg.h"
#include "rnc/movies/decoder/vi_buf.h"

/* Movie video decoder (type name from the recovered symbol
   videoDecSetStream__FP8VideoDec...). It lives at decoder context
   +0xD9048, so its ViBuf is the one at +0xD9090; init_all reserves
   0xB8 bytes for it before AudioDec. */
struct VideoDec {
    struct sceMpeg mpeg;         /* 0x00: video_dec_set_stream passes it to sceMpegAddStrCallback */
    struct ViBuf   vi_buf;       /* 0x48 */
    s32            state;        /* 0xA8: VideoDecState */
    s32            sema;         /* 0xAC */
    s32            hid_endimage; /* 0xB0 */
    s32            hid_vblank;   /* 0xB4 */
}; /* size 0xB8 */

enum VideoDecState {
    VD_STATE_NORMAL = 0,
    VD_STATE_ABORT = 1, /* video_dec_abort: dec_bs0 stops */
    VD_STATE_FLUSH = 2, /* video_dec_flush, from VD_STATE_NORMAL */
    VD_STATE_END = 3    /* video_dec_main, after the last picture */
};

void video_dec_reset(struct VideoDec *vd) __asm__("func_0023CC30");
s32  video_dec_abort(struct VideoDec *vd) __asm__("func_0023CC70");
s32  video_dec_get_state(struct VideoDec *vd) __asm__("func_0023CC80");
s32  video_dec_set_state(struct VideoDec *vd, s32 state) __asm__("func_0023CC88");

#endif /* LOMBYTE_RNC_MOVIES_DECODER_VIDEO_DEC_H */
