#include "types.h"
struct M2c_D_001D5BF4 {
    u8 pad_0[0x84];
    s32 unk84;
};

struct M2c_arg0 {
    u8 pad_0[0x54];
    s32 unk54;
};

extern struct M2c_D_001D5BF4 *D_001D5BF4[];
extern s32 func_00225C18();
s32 FUN_00221a48(struct M2c_arg0 *arg0) {
    D_001D5BF4[0]->unk84 = 0;
    arg0->unk54 = func_00225C18(0);
    return 0;
}
