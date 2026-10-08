#include "types.h"
#include "rnc/audio/decoder/audio_dec.h"
extern s32 send_adpcm() __asm__("FUN_0023afc0");
void audio_dec_send(struct AudioDec *dec) __asm__("FUN_0023aef0");

void audio_dec_send(struct AudioDec *dec) {
    if (dec->state != 0) {
        send_adpcm();
    }
}

extern void func_0023AEF0(struct AudioDec *dec) __attribute__((alias("FUN_0023aef0")));
