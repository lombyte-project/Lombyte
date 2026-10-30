#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/movies/decoder/decoder_globals.h"
#include "rnc/movies/player/movie_player.h"

extern s32 audio_dec_is_preset(struct AudioDec *dec) __asm__("func_0023AEE0");

s32 is_audio_ok(void) __asm__("FUN_0023a790");

s32 is_audio_ok(void) {
    s32 result;
    result = audio_dec_is_preset(&movie_player->audio_dec);
    return result;
}

extern s32 func_0023A790(void) __attribute__((alias("FUN_0023a790")));
