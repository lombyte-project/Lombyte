#include "types.h"

extern s32 CreateSema();
extern s32 D_0012FC9C[];

struct SemaParam {
    u8 pad0[4];
    s32 unk4;
    s32 unk8;
    u8 padC[8];
    s32 unk14;
};

void _sceFsSemInit(void) {
    struct SemaParam args;

    if (D_0012FC9C[0] == -1) {
        args.unk14 = 0;
        args.unk8 = 1;
        args.unk4 = 1;
        D_0012FC9C[0] = CreateSema(&args);
    }
}
