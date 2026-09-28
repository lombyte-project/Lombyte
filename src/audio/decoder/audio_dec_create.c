#include "types.h"
struct M2c_arg0 {
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

struct M2c_D_001612BC;
extern struct M2c_D_001612BC *D_001612BC;
extern s32 snd_init_movie_sound() __asm__("FUN_0012f068");
extern void func_001F9810();
s32 audio_dec_create(struct M2c_arg0 *arg0, s32 arg1, s32 arg2, s32 arg3) __asm__("FUN_0023abd0");

s32 audio_dec_create(struct M2c_arg0 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_2_38;
    s32 three;
    s32 four;

    func_001F9810(((u8 *)arg0 + (8)), 0x20);
    arg0->unk34 = arg1;
    arg0->unk40 = arg2;
    three = 3;
    arg0->unk4 = three;
    arg0->unk0 = 0;
    arg0->unk30 = 0;
    arg0->unk38 = 0;
    arg0->unk3C = 0;
    arg0->unk44 = 0;
    arg0->unk50 = 0;
    arg0->unk58 = 0;
    arg0->unk5C = 0;
    arg0->unk60 = 0;
    D_001612BC = (struct M2c_D_001612BC *)arg3;
    four = 0x400;
    arg0->unk4C = four;
    temp_2_38 = snd_init_movie_sound(0x400, 0x1000, 0x400, 0, 5, 3);
    arg0->unk48 = temp_2_38;
    if (temp_2_38 < 0) {
        return 0;
    }
    return 1;
}
