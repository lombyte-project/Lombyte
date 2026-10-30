/* Ported from rac1-decomp (src/game/movie/videodec.c, func_0023E4B0). */
#ifndef COMMON_H
#include "sda.h"
#endif /* STRUCTS_H */
#include "types.h"
#include "rnc/movies/player/movie_player.h"

extern struct MoviePlayer *movie_player_macro __asm__("D_0016120C") MACRO_ADDR;
extern int vi_buf_stop_dma(struct ViBuf *) __asm__("func_0023C170"); /* viBufStopDMA */
/* mpegStopDMA */
int mpeg_stop_dma(struct sceMpeg       *mp,
                  struct sceMpegCbData *cbdata,
                  void                 *anyData) __asm__("FUN_0023d0e0");

int mpeg_stop_dma(struct sceMpeg *mp, struct sceMpegCbData *cbdata, void *anyData) {
    vi_buf_stop_dma(&movie_player_macro->video_dec.vi_buf);
    return 1;
}

extern __typeof__(mpeg_stop_dma) func_0023D0E0 __attribute__((alias("FUN_0023d0e0")));
