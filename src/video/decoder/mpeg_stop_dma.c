/* Ported from rac1-decomp (src/game/movie/videodec.c, func_0023E4B0). */
#ifndef COMMON_H
#include "sda.h"
#endif /* STRUCTS_H */
#include "types.h"
#include "rnc/video/decoder/video_dec.h"
typedef struct {
    char _pad0[0xD9048];
    struct VideoDec videoDec; /* 0xD9048 */
} MovieGlobals;
extern MovieGlobals *D_0016120C MACRO_ADDR;
#define videoDec (D_0016120C->videoDec)
extern int vi_buf_stop_dma(struct ViBuf *) __asm__("func_0023C170"); /* viBufStopDMA */
/* mpegStopDMA */
int mpeg_stop_dma(struct sceMpeg *mp, struct sceMpegCbData *cbdata,
                  void *anyData) __asm__("FUN_0023d0e0");

int mpeg_stop_dma(struct sceMpeg *mp, struct sceMpegCbData *cbdata, void *anyData) {
    vi_buf_stop_dma(&videoDec.vi_buf);
    return 1;
}

extern __typeof__(mpeg_stop_dma) func_0023D0E0 __attribute__((alias("FUN_0023d0e0")));
