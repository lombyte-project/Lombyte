#include "types.h"
extern s32 D_001610A8[];
extern s32 D_001E63E4[];
extern s32 func_001FB8F0();
extern s32 func_0020D330();
void FUN_00238630(void) {
    func_001FB8F0(0, 0, 0x200, 0x80, 0x200, 0x80, 0);
    if (*(s32 *)0x1610A8 == 1) {
        func_0020D330(D_001E63E4[0], 1);
    }
}
