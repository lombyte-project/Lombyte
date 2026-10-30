#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/movies/decoder/decoder_globals.h"
#include "rnc/movies/player/movie_player.h"

extern s32 ChangeThreadPriority();
extern s32 GetThreadId();
extern s32 read_mpeg() __asm__("func_0023A460");
extern s32 init_all() __asm__("func_0023A7C0");
extern s32 term_all() __asm__("func_0023AA68");

s32 play_mpeg_movie(s32                 movie_source,
                    s32                 movie_size,
                    s32                 decoder_buffer_base,
                    struct MoviePlayer *player,
                    s32                 callback_registration_value) __asm__("FUN_0023a3b8");

s32 play_mpeg_movie(s32                 movie_source,
                    s32                 movie_size,
                    s32                 decoder_buffer_base,
                    struct MoviePlayer *player,

                    s32 callback_registration_value) {
    decoder_buffer_address = decoder_buffer_base;
    movie_player = player;
    ChangeThreadPriority(GetThreadId(), 1);
    if (init_all(movie_source, movie_size, callback_registration_value) != 0) {
        read_mpeg(&movie_player->video_dec, &movie_player->read_buf, &movie_player->cd_stream);
    }
    term_all();
    *(s32 *)0x161208 = 0;
    *(s32 *)0x16120C = 0;
    return 0;
}

extern s32 func_0023A3B8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
    __attribute__((alias("FUN_0023a3b8")));
