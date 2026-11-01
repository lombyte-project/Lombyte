#include "sda.h"
#include "rnc/movies/player/movie_player.h"
#include "rnc/movies/decoder/video_input.h"

extern struct MoviePlayer *movie_player_macro __asm__("D_0016120C") MACRO_ADDR;

int mpeg_restart_video_dma(void) __asm__("FUN_0023d110");

int mpeg_restart_video_dma(void) {
    vi_buf_restart_dma(&movie_player_macro->video_dec.vi_buf);
    return 1;
}

extern __typeof__(mpeg_restart_video_dma) func_0023D110 __attribute__((alias("FUN_0023d110")));
/* Recovered original symbol name. */
extern __typeof__(mpeg_restart_video_dma) mpegRestartVideoDMA__Fv
    __attribute__((alias("FUN_0023d110")));
