#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/video/decoder/decoder_globals.h"

extern s32 audio_dec_send() __asm__("func_0023AEF0");

s32 proceed_audio(void) __asm__("FUN_0023aba0");

s32 proceed_audio(void) {
    s32 result;
    result = audio_dec_send(decoder_state + 0xD9100);
    return result;
}

extern s32 func_0023ABA0(void) __attribute__((alias("FUN_0023aba0")));
