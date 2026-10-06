#include "types.h"

struct AudioDec {
    s32 unk0;
    u8 pad_4[0x2C];
    s32 unk30;
    u8 pad_34[0x4];
    s32 unk38;
    s32 unk3C;
    u8 pad_40[0x4];
    s32 unk44;
    u8 pad_48[0x8];
    s32 unk50;
    u8 pad_54[0x4];
    s32 unk58;
    s32 unk5C;
};
extern s32 snd_reset_movie_sound() __asm__("func_0012F0A8");
void audio_dec_reset(struct AudioDec *dec) __asm__("FUN_0023ad10");

void audio_dec_reset(struct AudioDec *dec) {
    snd_reset_movie_sound();
    dec->unk0 = 0;
    dec->unk30 = 0;
    dec->unk38 = 0;
    dec->unk3C = 0;
    dec->unk44 = 0;
    dec->unk50 = 0;
    dec->unk58 = 0;
    dec->unk5C = 0;
}
