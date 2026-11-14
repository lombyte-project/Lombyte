#include "types.h"
#include "rnc/audio/decoder/audio_dec.h"

void audio_dec_begin_put(
    struct AudioDec *dec, void **ptr1, s32 *size1, void **ptr2, s32 *size2) __asm__("FUN_0023ad58");

void audio_dec_begin_put(struct AudioDec *dec,
                         void           **first_destination,
                         s32             *first_destination_byte_count,
                         void           **second_destination,

                         s32 *second_destination_byte_count) {
    enum {
        AUDIO_DEC_STATE_HEADER_PENDING = 0
    };
    s32 writable_byte_count;

    if (dec->state == AUDIO_DEC_STATE_HEADER_PENDING) {
        if (dec->strType != 4) {
            *first_destination = ((u8 *)&dec->hdr + dec->hdrCount);
            *first_destination_byte_count = 0x28 - dec->hdrCount;
            *second_destination = dec->data;
            *second_destination_byte_count = dec->size;
        } else {
            *first_destination = dec->data;
            *first_destination_byte_count = dec->size;
            *second_destination = NULL;
            *second_destination_byte_count = 0;
        }
    } else {
        writable_byte_count = dec->size - dec->count;
        if ((dec->size - dec->put) >= writable_byte_count) {
            *first_destination = dec->data + dec->put;
            *first_destination_byte_count = writable_byte_count;
            *second_destination = NULL;
            *second_destination_byte_count = 0;
        } else {
            *first_destination = dec->data + dec->put;
            *first_destination_byte_count = dec->size - dec->put;
            *second_destination = dec->data;
            *second_destination_byte_count = writable_byte_count - (dec->size - dec->put);
        }
    }
}

extern void func_0023AD58(struct AudioDec *, void **, s32 *, void **, s32 *)
    __attribute__((alias("FUN_0023ad58")));
