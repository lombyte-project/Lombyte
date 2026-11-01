#ifndef LOMBYTE_RNC_AUDIO_MOVIE_H
#define LOMBYTE_RNC_AUDIO_MOVIE_H

#include "types.h"

void snd_close_movie_sound(void) __asm__("FUN_0012f0e0");
void snd_update_movie_adpcm(s32, s32) __asm__("FUN_0012f148");

#endif /* LOMBYTE_RNC_AUDIO_MOVIE_H */
