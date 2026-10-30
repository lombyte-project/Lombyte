#include "rnc/audio/decoder/audio_dec.h"

int audio_dec_is_preset(struct AudioDec *dec) __asm__("func_0023AEE0");

int audio_dec_is_preset(struct AudioDec *dec) {
    return dec->iopLastPos >= 0x1000;
}
