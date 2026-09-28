#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_001ee338/FUN_001ee338.s", FUN_001ee338);
#else
#include "rnc/fun_001ee338_types.h"
#include "types.h"








extern struct M2c_D_0013E500 D_0013E500;
extern s32 D_001413D4[];
extern struct M2c_D_00189300 D_00189300;
extern s32 func_001EE008();
extern s32 func_001F2070();
extern s32 func_001FA6C0();
extern void sp10();
void FUN_001ee338(void) {
    f32 sp0;
    f32 sp4;
    f32 sp10;
    f32 sp14;
    struct M2c_var_16_43 *var_16_43;
    s32 var_17_26;
    u8 temp_2_48;
    struct M2c_temp_2_45 *temp_2_45;

    if (D_001413D4[0] == 0x72) {
        D_00189300.unkC0 = 0;
    }
    if (D_00189300.unkC0 != 0) {
        var_17_26 = 0;
        sp0 = func_001FA6C0(D_0013E500.unk8);
        sp4 = func_001FA6C0(D_0013E500.unkC);
        if (D_00189300.unkC0 > 0) {
            var_16_43 = &D_00189300;
            do {
                temp_2_45 = var_16_43->unk20;
                if (temp_2_45 != NULL) {
                    temp_2_48 = temp_2_45->unk20;
                    if (temp_2_48 != 0xFE) {
                        if (temp_2_48 != 0xFD) {
                            if (var_16_43->unk24 != 0) {
                                func_001F2070(&sp10, var_16_43);
                                sp10 = (sp10 - func_001FA6C0(D_0013E500.unk10)) * 0.0625f;
                                sp14 = (sp14 - func_001FA6C0(D_0013E500.unk14)) * 0.0625f;
                            } else {
                                sp10 = (s64)(u64) sp0;
                            }
                            func_001EE008(var_16_43, sp10, sp14);
                        }
                    }
                }
                var_17_26 += 1;
                var_16_43 += 0x30;
            } while (var_17_26 < D_00189300.unkC0);
        }
        D_00189300.unkC0 = 0;
    }
}
#endif /* NON_MATCHING */
