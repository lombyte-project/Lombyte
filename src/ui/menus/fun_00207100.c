#include "types.h"
struct M2c_D_0013F350 {
    u8 pad_0[0x12E4];
    u8 unk12E4;
    u8 pad_12E5[0xDA7];
    s32 unk208C;
};

extern struct M2c_D_0013F350 D_0013F350;
extern u32 D_001A03BC[];
extern s32 func_00208818();
s32 FUN_00207100(s32 arg0, s32 arg1, f32 unused1, f32 unused2, f32 arg3) {
    if (arg1 < 0xBB) {
        struct M2c_D_0013F350 *s = &D_0013F350;
        s32 a = s->unk208C == 17 || s->unk208C == 18 || s->unk12E4 == 1;

        if (!a && D_001A03BC[0] != 0) {
            return 1;
        }
        return 0;
    }
    if (arg3 >= 51.5f && arg3 <= 54.0f
        && func_00208818(arg0, arg1, 0x10A, 0xE5, 0x124, 0xF9) != 0) {
        return 1;
    }
    return 0;
}
