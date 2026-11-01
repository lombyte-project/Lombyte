#ifndef LOMBYTE_RNC_AUDIO_DECODER_H
#define LOMBYTE_RNC_AUDIO_DECODER_H

#include "types.h"

struct AudioDec;

s32  audio_dec_create(struct AudioDec *, s32, s32, s32) __asm__("FUN_0023abd0");
int  audio_dec_is_preset(struct AudioDec *) __asm__("func_0023AEE0");
void audio_dec_reset(struct AudioDec *) __asm__("FUN_0023ad10");

#endif /* LOMBYTE_RNC_AUDIO_DECODER_H */
