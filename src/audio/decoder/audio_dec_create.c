#include "types.h"
struct AudioDec {
    s32 unk0;
    s32 unk4;
    u8 pad_8[0x28];
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 pad_54[0x4];
    s32 unk58;
    s32 unk5C;
    s32 unk60;
};

struct Globals_001612BC;
extern struct Globals_001612BC *D_001612BC;
extern s32 snd_init_movie_sound() __asm__("FUN_0012f068");
extern void func_001F9810();
s32 audio_dec_create(struct AudioDec *dec, s32 buffer, s32 buffer_size, s32 arg3) __asm__("FUN_0023abd0");

s32 audio_dec_create(struct AudioDec *dec, s32 buffer, s32 buffer_size, s32 arg3) {
    s32 movie_handle;
    s32 three;
    s32 four;

    func_001F9810(((u8 *)dec + (8)), 0x20);
    dec->unk34 = buffer;
    dec->unk40 = buffer_size;
    three = 3;
    dec->unk4 = three;
    dec->unk0 = 0;
    dec->unk30 = 0;
    dec->unk38 = 0;
    dec->unk3C = 0;
    dec->unk44 = 0;
    dec->unk50 = 0;
    dec->unk58 = 0;
    dec->unk5C = 0;
    dec->unk60 = 0;
    D_001612BC = (struct Globals_001612BC *)arg3;
    four = 0x400;
    dec->unk4C = four;
    movie_handle = snd_init_movie_sound(0x400, 0x1000, 0x400, 0, 5, 3);
    dec->unk48 = movie_handle;
    if (movie_handle < 0) {
        return 0;
    }
    return 1;
}
