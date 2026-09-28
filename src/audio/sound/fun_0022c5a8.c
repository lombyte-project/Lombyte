#include "types.h"
extern s32 D_0013E5BC[];
extern u8 D_00187080[];
extern u8 D_00194120[];
extern s32 func_001EFA68();
extern void func_001F9A10();
extern void func_001F9A28();
extern void func_001F9A68(s32, s32, f32);
extern void func_00213358(f32, f32);

void FUN_0022c5a8(s32 arg0) {
    func_00213358(0.5f, 6.0f);
    func_001F9A10(arg0, arg0, D_00187080);
    if (func_001EFA68(D_00187080, arg0, 0x82, D_0013E5BC[0], 0) != 0) {
        func_001F9A28(arg0, D_00194120, D_00187080);
        func_001F9A68(arg0, arg0, 0.75f);
        func_001F9A10(arg0, arg0, D_00187080);
    }
}

extern __typeof__(FUN_0022c5a8) func_0022C5A8 __attribute__((alias("FUN_0022c5a8")));
