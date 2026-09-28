#include "types.h"
extern f32 func_00213308(void);
extern f32 func_002132A8(f32, f32);
extern void func_00214DB0(void *, f32, f32, f32);
void FUN_00213358(void *out, f32 a, f32 b) {
    f32 x = func_00213308();
    f32 y = func_00213308();

    func_00214DB0(out, func_002132A8(a, b), x, y);
}

extern __typeof__(FUN_00213358) func_00213358 __attribute__((alias("FUN_00213358")));
