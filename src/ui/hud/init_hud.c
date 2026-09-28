#include "rnc/ui_hud_init_hud_types.h"
#include "types.h"

extern u8 D_0015F6D8[];
extern struct M2c_D_0015FA00 *D_0015FA00;
extern s32 D_0015FA04 __attribute__((sda));
extern s32 D_0015FA08;
extern s32 D_0015FA0C;
extern u8 D_00199B60[];
extern s32 D_0019A3E8[];
extern s32 func_001F9810();
extern s32 func_001FF288();
extern s32 func_001FF308();
/* retail small-data globals, declared to GAS before the body */

void init_hud(void) __asm__("FUN_001fee88");

void init_hud(void) {
    s32 var_17_14;
    s32 *temp_2_41;
    struct M2c_var_16_16 *var_16_16;

    var_17_14 = 0;
    {
        u8 *base = D_00199B60;
        var_16_16 = (struct M2c_var_16_16 *)((u8 *)base + 0x24);
    }
    D_0019A3E8[0] = 0;
    D_0019A3E8[1] = 0;
    do {
        var_16_16->unk40 = -1;
        *(s32 *)((u8 *)var_16_16 - 0x4) = 0x10000;
        func_001FF308(var_17_14, 0xFFFF, 0, 0, 0, 0, 1);
        var_17_14 += 1;
        var_16_16->unk58 = 0;
        var_16_16->unk48 = -6;
        *(s32 *)((u8 *)var_16_16 - 0x20) = 0;
        var_16_16->unk0 = 0;
        var_16_16 = (struct M2c_var_16_16 *)((u8 *)var_16_16 + 0x90);
    } while (var_17_14 < 0xD);
    temp_2_41 = D_0015FA00;
    if (temp_2_41 == NULL) {
        D_0015FA00 = func_001FF288(0x2800, 0, D_0015F6D8, 0x115);
        D_0015FA0C = func_001FF288(0x1400, 0, D_0015F6D8, 0x116);
    }
    D_0015FA08 = (s32)((u8 *)D_0015FA00 + 0x2800);
    D_0015FA04 = (s32)D_0015FA00;
    func_001F9810(D_0015FA00, 0x2800);
    D_0015FA00->unk20 = 0xFF;
}
