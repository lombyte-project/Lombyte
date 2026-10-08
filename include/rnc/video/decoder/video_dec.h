#ifndef LOMBYTE_RNC_VIDEO_DECODER_VIDEO_DEC_H
#define LOMBYTE_RNC_VIDEO_DECODER_VIDEO_DEC_H

#include "types.h"
#include "rnc/sdk/libmpeg.h"
#include "rnc/video/decoder/vi_buf.h"

/* Movie video decoder (type name from the recovered symbol
   videoDecSetStream__FP8VideoDec...). It lives at decoder context
   +0xD9048, so its ViBuf is the one at +0xD9090; init_all reserves
   0xB8 bytes for it before AudioDec. */
struct VideoDec {
    struct sceMpeg mpeg;  /* 0x00: video_dec_set_stream passes it to sceMpegAddStrCallback */
    struct ViBuf vi_buf;  /* 0x48 */
    s32 state;            /* 0xA8: video_dec_flush moves 0 to 2 */
    s32 sema;             /* 0xAC */
    s32 hid_endimage;     /* 0xB0 */
    s32 hid_vblank;       /* 0xB4 */
}; /* size 0xB8 */

#endif /* LOMBYTE_RNC_VIDEO_DECODER_VIDEO_DEC_H */
