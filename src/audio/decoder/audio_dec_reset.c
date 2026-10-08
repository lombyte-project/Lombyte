#include "types.h"
#include "rnc/audio/decoder/audio_dec.h"

extern s32 snd_reset_movie_sound() __asm__("func_0012F0A8");
void audio_dec_reset(struct AudioDec *dec) __asm__("FUN_0023ad10");

void audio_dec_reset(struct AudioDec *dec) {
    snd_reset_movie_sound();
    dec->state = 0;
    dec->hdrCount = 0;
    dec->put = 0;
    dec->count = 0;
    dec->totalBytes = 0;
    dec->iopLastPos = 0;
    dec->totalBytesSent = 0;
    dec->iopZero = 0;
}
