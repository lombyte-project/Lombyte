#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00224e18/FUN_00224e18.s", FUN_00224e18);
#else
#include "rnc/fun_00224e18_types.h"
#include "types.h"






extern u8 D_001863D0[];
extern struct M2c_D_001D5E10 D_001D5E10;
extern struct M2c_D_001D5E50 D_001D5E50;
extern s32 func_001E9480();
extern s32 func_001FA2B8();
extern s32 func_0020CB10();
extern s32 func_0020CB88();
extern s32 func_0020CCA8();
extern s32 func_0020D580();
extern s32 func_0020DEF8();
extern s32 func_0020E098();
extern s32 func_00214128();
void FUN_00224e18(struct M2c_arg0 *arg0) {
u8 sp_slot[0xD0];    s64 sp30;
    s32 temp_18_17;
u8 *temp_19_27;
    s32 var_20_38;
    s32 var_21_39;
    void **temp_16_14;
    s32 *temp_16_34;

    temp_16_14 = arg0->unk78;
    temp_18_17 = *(s32 *)((u8 *)(*temp_16_14) + 0x44);
    func_0020D580();
    func_0020DEF8(arg0);
    temp_19_27 = (s32) arg0 == *(s32 *)((u8 *)(*temp_16_14) + 0x5C);
    func_0020CCA8(temp_18_17, (temp_19_27 == 0) ? 2 : 3, sp_slot, 2);
    arg0->unk10 = (s64) sp30;
    temp_16_34 = ((u8 *)arg0 + (0xC0));
    var_20_38 = 0;
    func_001FA2B8(temp_16_34, sp_slot);
    var_21_39 = 0;
    func_00214128(temp_16_34);
    func_0020E098(arg0);
    if (D_001D5E50.unk1 != 0) {
        var_20_38 = 1;
        func_0020CB88(temp_18_17, &D_001D5E50);
    }
    if (D_001D5E10.unk1 != 0) {
        var_21_39 = 1;
        func_0020CB88(temp_18_17, &D_001D5E10);
    }
    if (temp_19_27 == 0) {

    }
    func_001E9480(D_001863D0, D_001863D0, arg0->unk24, 0, temp_18_17);
    arg0->unk6C = D_001863D0;
    arg0->unk68 = D_001863D0;
    if (var_20_38 != 0) {
        func_0020CB10(temp_18_17, 0x17, &D_001D5E50);
        D_001D5E50.unk28 = 0;
        D_001D5E50.unk20 = 0;
        D_001D5E50.unk24 = 0;
    }
    if (var_21_39 != 0) {
        func_0020CB10(temp_18_17, 0x16, &D_001D5E10);
        D_001D5E10.unk28 = 0;
        D_001D5E10.unk20 = 0;
        D_001D5E10.unk24 = 0;
    }
    arg0->unk50 = 0;
    arg0->unk54 = 0;
}
#endif /* NON_MATCHING */
