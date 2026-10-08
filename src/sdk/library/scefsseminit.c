#include "types.h"
#include "kernel.h"

extern s32 CreateSema();
extern s32 D_0012FC9C[];


void _sceFsSemInit(void) {
    struct SemaParam args;

    if (D_0012FC9C[0] == -1) {
        args.option = 0;
        args.initCount = 1;
        args.maxCount = 1;
        D_0012FC9C[0] = CreateSema(&args);
    }
}
