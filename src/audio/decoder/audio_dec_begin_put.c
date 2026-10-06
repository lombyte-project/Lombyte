#include "types.h"
struct AudioDec {
    s32 unk0;
    s32 unk4;
    u8 pad_8[0x28];
    s32 unk30;
    u8 *unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
};

void audio_dec_begin_put(struct AudioDec *dec, void **ptr1, s32 *size1, void **ptr2,
                         s32 *size2) __asm__("FUN_0023ad58");

void audio_dec_begin_put(struct AudioDec *dec, void **ptr1, s32 *size1, void **ptr2, s32 *size2) {
    s32 t2;

    if (dec->unk0 == 0) {
        if (dec->unk4 != 4) {
            *ptr1 = ((u8 *)dec + (dec->unk30 + 8));
            *size1 = 0x28 - dec->unk30;
            *ptr2 = dec->unk34;
            *size2 = dec->unk40;
        } else {
            *ptr1 = dec->unk34;
            *size1 = dec->unk40;
            *ptr2 = NULL;
            *size2 = 0;
        }
    } else {
        t2 = dec->unk40 - dec->unk3C;
        if ((dec->unk40 - dec->unk38) >= t2) {
            *ptr1 = dec->unk34 + dec->unk38;
            *size1 = t2;
            *ptr2 = NULL;
            *size2 = 0;
        } else {
            *ptr1 = dec->unk34 + dec->unk38;
            *size1 = dec->unk40 - dec->unk38;
            *ptr2 = dec->unk34;
            *size2 = t2 - (dec->unk40 - dec->unk38);
        }
    }
}
extern void func_0023AD58(struct AudioDec *, void **, s32 *, void **, s32 *)
    __attribute__((alias("FUN_0023ad58")));
