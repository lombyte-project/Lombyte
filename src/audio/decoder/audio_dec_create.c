#include "types.h"
#include "rnc/audio/decoder/audio_dec.h"

struct Globals_001612BC;
extern struct Globals_001612BC *D_001612BC;
extern s32 snd_init_movie_sound() __asm__("FUN_0012f068");
extern void func_001F9810();
s32 audio_dec_create(struct AudioDec *dec, s32 buffer, s32 buffer_size, s32 arg3) __asm__("FUN_0023abd0");

s32 audio_dec_create(struct AudioDec *dec, s32 buffer, s32 buffer_size, s32 arg3) {
    s32 movie_handle;
    s32 three;
    s32 four;

    func_001F9810(&dec->hdr, 0x20);
    dec->data = buffer;
    dec->size = buffer_size;
    three = 3;
    dec->strType = three;
    dec->state = 0;
    dec->hdrCount = 0;
    dec->put = 0;
    dec->count = 0;
    dec->totalBytes = 0;
    dec->iopLastPos = 0;
    dec->totalBytesSent = 0;
    dec->iopZero = 0;
    dec->spuPos = 0;
    D_001612BC = (struct Globals_001612BC *)arg3;
    four = 0x400;
    dec->iopBuffSize = four;
    movie_handle = snd_init_movie_sound(0x400, 0x1000, 0x400, 0, 5, 3);
    dec->iopBuff = movie_handle;
    if (movie_handle < 0) {
        return 0;
    }
    return 1;
}
