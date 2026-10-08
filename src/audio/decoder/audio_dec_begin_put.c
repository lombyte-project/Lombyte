#include "types.h"
#include "rnc/audio/decoder/audio_dec.h"

void audio_dec_begin_put(struct AudioDec *dec, void **ptr1, s32 *size1, void **ptr2,
                         s32 *size2) __asm__("FUN_0023ad58");

void audio_dec_begin_put(struct AudioDec *dec, void **ptr1, s32 *size1, void **ptr2, s32 *size2) {
    s32 t2;

    if (dec->state == 0) {
        if (dec->strType != 4) {
            *ptr1 = ((u8 *)&dec->hdr + dec->hdrCount);
            *size1 = 0x28 - dec->hdrCount;
            *ptr2 = dec->data;
            *size2 = dec->size;
        } else {
            *ptr1 = dec->data;
            *size1 = dec->size;
            *ptr2 = NULL;
            *size2 = 0;
        }
    } else {
        t2 = dec->size - dec->count;
        if ((dec->size - dec->put) >= t2) {
            *ptr1 = dec->data + dec->put;
            *size1 = t2;
            *ptr2 = NULL;
            *size2 = 0;
        } else {
            *ptr1 = dec->data + dec->put;
            *size1 = dec->size - dec->put;
            *ptr2 = dec->data;
            *size2 = t2 - (dec->size - dec->put);
        }
    }
}
extern void func_0023AD58(struct AudioDec *, void **, s32 *, void **, s32 *)
    __attribute__((alias("FUN_0023ad58")));
