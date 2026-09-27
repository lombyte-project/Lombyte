#include "types.h"

struct M2c_arg0 {
    u8 pad_0[0x38];
    s64 unk38;
};

extern s32 D_0015F60C;
extern s32 func_0020C828();

s32 FUN_00225530(struct M2c_arg0 *arg0) {
    if (arg0 == 0) {
        return 0;
    }
    func_0020C828();
    arg0->unk38 = (s64) D_0015F60C;
    return 0;
}
