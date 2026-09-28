#include "types.h"
struct M2c_arg0 {
    u8 pad_0[0x40];
    f32 unk40;
};
extern f32 func_001FA580(f32, f32);
void FUN_0021f120(struct M2c_arg0 *arg0) {
    arg0->unk40 = func_001FA580(arg0->unk40, 0.02f);
}
