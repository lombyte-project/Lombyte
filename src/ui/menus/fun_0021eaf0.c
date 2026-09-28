#include "types.h"
struct M2c_arg0 {
    u8 pad_0[0x44];
    s32 unk44;
};

extern s32 FUN_00225530();
s32 FUN_0021eaf0(struct M2c_arg0 *arg0) {
    arg0->unk44 = FUN_00225530(arg0->unk44);
    return 0;
}
