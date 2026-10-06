#include "types.h"

extern s32 CreateSema();
extern s32 D_0012FCA0[];
extern s32 D_0012FCA4[];

struct SemaParam {
    u8 pad0[4];
    s32 unk4;
    s32 unk8;
    u8 padC[8];
    s32 unk14;
};

void _sceFsIobSemaMK(void) {
    struct SemaParam args;

    if (D_0012FCA0[0] == -1) {
        args.unk14 = 0;
        args.unk8 = 1;
        args.unk4 = 1;
        D_0012FCA0[0] = CreateSema(&args);
        D_0012FCA4[0] = CreateSema(&args);
    }
}
