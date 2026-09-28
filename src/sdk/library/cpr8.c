#include "rnc/sdk_library_cpr8_types.h"
#include "types.h"

extern s32 DIntr();
extern s32 EnableInterrupts();

void _cpr8(struct M2c_arg0 *arg0, struct M2c_arg1 *arg1) {
    struct M2c_sp0 *sp0;
    s32 sp4;
    s32 sp8;
    s32 temp_2_106;
    s32 temp_2_73;
    s32 temp_4_27;
    s32 var_16_70;
    s32 var_17_68;
    s32 var_18_22;
    s32 var_20_40;
    s32 var_21_38;
    s32 var_30_43;
    s32 var_3_29;
    s32 var_5_64;

    sp0 = (struct M2c_sp0 *)arg0;
    var_18_22 = arg1->unk0 & 0x0FFFFFFF;
    sp8 = arg0->unkD8 & 0x0FFFFFFF;
    if (sp0->unk174 == 3) {
        goto block_3;
    }
    temp_4_27 = sp0->unkE0;
    var_3_29 = temp_4_27;
    if (temp_4_27 != 0) {
        goto block_8;
    }
    goto block_4;
block_3:
    var_3_29 = sp0->unkE0;
block_4:
    var_21_38 = arg1->unk10 * 0x180;
    var_20_40 = var_21_38 >> 4;
    if (var_3_29 == 0) {
        goto block_6;
    }
    var_30_43 = (var_3_29 >> 4) * 0x180;
    goto block_7;
block_6:
    var_30_43 = var_21_38;
block_7:
    sp4 = 1;
    goto block_9;
block_8:
    var_30_43 = (temp_4_27 >> 4) * 0xC0;
    var_21_38 = ((s32) arg1->unk10 >> 1) * 0x180;
    sp4 = 2;
    var_20_40 = var_21_38 >> 4;
block_9:
    var_5_64 = 0;
    while (var_5_64 < sp4) {
        var_17_68 = sp8;
        for (var_16_70 = 0; var_16_70 < arg1->unkC; var_16_70++) {
            temp_2_73 = DIntr();
            *(volatile s32 *)0x1000D480 = 0;
            *(volatile s32 *)0x1000D410 = var_18_22;
            *(volatile s32 *)0x1000D420 = var_20_40;
            *(volatile s32 *)0x1000D400 = 0x101;
            if (temp_2_73 == 0) {
                goto block_15;
            }
            EnableInterrupts();
        block_15:
            while (*(volatile u32 *)0x1000D400 & 0x100) {
            }
            temp_2_106 = DIntr();
            *(volatile s32 *)0x1000D080 = 0;
            *(volatile s32 *)0x1000D010 = var_17_68;
            *(volatile s32 *)0x1000D020 = var_20_40;
            *(volatile s32 *)0x1000D000 = 0x100;
            if (temp_2_106 == 0) {
                goto block_19;
            }
            EnableInterrupts();
        block_19:
            while (*(volatile u32 *)0x1000D000 & 0x100) {
            }
            while (*(volatile u32 *)0x1000D020 != 0) {
            }
            var_17_68 += var_30_43;
            var_18_22 += var_21_38;
        }
        var_5_64++;
        sp8 += sp0->unkE4 * 0xC0;
    }
    return;
}
