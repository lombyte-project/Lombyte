#include "types.h"
#include "kernel.h"

extern s32 CreateSema();
extern s32 D_0012FCA0[];
extern s32 D_0012FCA4[];


void _sceFsIobSemaMK(void) {
    struct SemaParam args;

    if (D_0012FCA0[0] == -1) {
        args.option = 0;
        args.initCount = 1;
        args.maxCount = 1;
        D_0012FCA0[0] = CreateSema(&args);
        D_0012FCA4[0] = CreateSema(&args);
    }
}
