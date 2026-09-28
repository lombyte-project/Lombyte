#include "types.h"
struct M2c_D_001A00F0 {
    u8 pad_0[0xC];
    s32 unkC;
    u8 pad_10[0x4];
    s32 unk14;
    u8 pad_18[0x10];
    s32 unk28;
};

extern s32 D_0013D560[];
extern s32 D_0015ED84;
extern struct M2c_D_001A00F0 D_001A00F0;
extern void FillTransferWords();
extern s32 func_001FA860();
extern void func_00208030();
extern void func_00208810();

void FUN_00207b08(s32 arg0) {
    s32 n;

    func_00208810();
    if (D_001A00F0.unk28 == 0) {
        FillTransferWords(arg0, 0, 0x800);
        return;
    }
    n = func_001FA860(arg0, 0x800, D_001A00F0.unk14, D_001A00F0.unkC);
    if (n == -1) {
        func_00208030(arg0);
    }
    if (D_0013D560[D_0015ED84] < n) {
        D_0013D560[D_0015ED84] = n;
    }
}

extern __typeof__(FUN_00207b08) func_00207B08 __attribute__((alias("FUN_00207b08")));
