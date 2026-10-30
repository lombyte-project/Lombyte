#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/movies/decoder/decoder_globals.h"
#include "rnc/movies/player/movie_player.h"

extern void switch_thread() __asm__("func_0023A770");
extern void vi_buf_add_dma() __asm__("func_0023BF70");

s32 mpeg_nodata(void) __asm__("FUN_0023d0a8");

s32 mpeg_nodata(void) {
    switch_thread();
    vi_buf_add_dma(&movie_player->video_dec.vi_buf);
    return 1;
}
