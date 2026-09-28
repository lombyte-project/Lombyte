#include "types.h"
struct M2c_arg0 {
    u8 pad_0[0x48];
    f32 unk48;
};
extern f32 func_001FA580(f32, f32);
void FUN_0021e1f8(struct M2c_arg0 *arg0) {
    arg0->unk48 = func_001FA580(arg0->unk48, 0.01f);
}
