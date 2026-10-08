#include "types.h"
#include "rnc/audio/decoder/audio_dec.h"
extern s32 snd_start_movie_sound() __asm__("func_0012F108");
void audio_dec_start(struct AudioDec *dec) __asm__("FUN_0023acb8");

void audio_dec_start(struct AudioDec *dec) {
    s32 temp_2_10;
    s32 u18;
    s32 four;
    s32 copy;

    temp_2_10 = dec->iopBuffSize;
    four = 0x400;
    u18 = dec->hdr.ch;
    snd_start_movie_sound(dec->iopBuff, ((copy = temp_2_10) / 0x400) * four, dec->iopZero,
                          dec->hdr.rate, u18);
    dec->state = 2;
    /* Allocator-shape pair: GCC removes both stores (code-dead), but the
       read-modify-write sequence drives hdr.ch's register choice to retail's
       schedule.  Verified 100/100/100 bytes. */
    dec->hdr.ch++;
    dec->hdr.ch--;
}

extern __typeof__(audio_dec_start) func_0023ACB8 __attribute__((alias("FUN_0023acb8")));

/* Recovered original symbol name. */
extern __typeof__(audio_dec_start) audioDecStart __attribute__((alias("FUN_0023acb8")));
