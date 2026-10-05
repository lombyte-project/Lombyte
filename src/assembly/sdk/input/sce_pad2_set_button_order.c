#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/input/sce_pad2_set_button_order/scePad2SetButtonOrder.s", scePad2SetButtonOrder);
#else
#include "types.h"
struct M2c_var_5_0 { s32 unk0; s32 unk4; s32 unk8; s32 unkC; };
s32 scePad2SetButtonOrder(u8 *arg0, s32 *arg1) {
    register s32 var_6_6 __asm__("a2");
    register s32 var_7_5 __asm__("a3");
    register s32 var_8_3 __asm__("t0");
    register s32 var_9_4 __asm__("t1");
    u8 *var_4_0;
    struct M2c_var_5_0 *var_5_0;
    var_4_0 = arg0;
    var_5_0 = arg1;
    var_8_3 = 0; var_9_4 = 0; var_7_5 = 0; var_6_6 = 0;
    do {
        if (((s32)*var_4_0 >> var_8_3) & 1) {
            var_5_0->unk0 = 1;
            var_5_0->unk8 = var_9_4;
            var_5_0->unkC = var_7_5;
            if ((u32)(var_6_6 - 0x10) < 0x10U) {
                var_5_0->unk4 = 8; var_9_4 += 1;
            } else if ((u32)(var_6_6 - 0x23) < 4U) {
                var_5_0->unk4 = 8; var_9_4 += 1;
            } else {
                var_7_5 += 1; var_5_0->unk4 = 1;
                if (!(var_7_5 & 7)) { var_9_4 += 1; var_7_5 = 0; }
            }
        } else { var_5_0->unk0=0; var_5_0->unk8=0; var_5_0->unkC=0; }
        var_8_3 += 1; var_6_6 += 1;
        if (!(var_8_3 & 7)) { var_4_0 += 1; var_8_3 = 0; }
        var_5_0 += 1;
    } while (var_6_6 < 0x28);
    return 1;
}

#endif /* NON_MATCHING */
