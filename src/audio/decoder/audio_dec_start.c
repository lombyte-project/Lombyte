#include "types.h"
struct AudioDec {
    s32 unk0;
    u8 pad_4[0x10];
    s32 unk14;
    s32 unk18;
    u8 pad_1C[0x2C];
    s32 unk48;
    s32 unk4C;
    u8 pad_50[0xC];
    s32 unk5C;
};
extern s32 snd_start_movie_sound() __asm__("func_0012F108");
void audio_dec_start(struct AudioDec *dec) __asm__("FUN_0023acb8");

void audio_dec_start(struct AudioDec *dec) {
    s32 temp_2_10;
    s32 u18;
    s32 four;
    s32 copy;

    temp_2_10 = dec->unk4C;
    four = 0x400;
    u18 = dec->unk18;
    snd_start_movie_sound(dec->unk48, ((copy = temp_2_10) / 0x400) * four, dec->unk5C,
                          dec->unk14, u18);
    dec->unk0 = 2;
    /* Allocator-shape pair: GCC removes both stores (code-dead), but the
       read-modify-write sequence drives unk18's register choice to retail's
       schedule.  Verified 100/100/100 bytes. */
    dec->unk18++;
    dec->unk18--;
}

extern __typeof__(audio_dec_start) func_0023ACB8 __attribute__((alias("FUN_0023acb8")));

/* Recovered original symbol name. */
extern __typeof__(audio_dec_start) audioDecStart __attribute__((alias("FUN_0023acb8")));
