#include "types.h"
extern s32 send_adpcm() __asm__("FUN_0023afc0");
void audio_dec_send(s32 *dec) __asm__("FUN_0023aef0");

void audio_dec_send(s32 *dec) {
    if (*dec != 0) {
        send_adpcm();
    }
}

extern void func_0023AEF0(s32 *dec) __attribute__((alias("FUN_0023aef0")));
