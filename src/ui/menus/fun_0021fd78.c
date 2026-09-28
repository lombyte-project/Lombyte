#include "types.h"
struct M2c_arg0 {
    u8 pad_0[0x44];
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    s32 unk54;
};

extern s32 func_00225CD8();
s32 FUN_0021fd78(struct M2c_arg0 *arg0) {
    arg0->unk48 = func_00225CD8(arg0->unk48);
    arg0->unk4C = func_00225CD8(arg0->unk4C);
    arg0->unk50 = -1;
    arg0->unk54 = -1;
    arg0->unk44 = -1;
    return 0;
}
