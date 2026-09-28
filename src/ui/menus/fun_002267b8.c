#include "types.h"
struct M2c_D_001D5BF0 {
    u8 pad_0[0xA8];
    s32 unkA8;
    s32 unkAC;
};

struct M2c_var_5_18 {
    s32 unk0;
    s32 unk4;
};

extern u8 D_001B3200[];
extern u8 D_001B3AC0[];
extern u8 D_001D59D8[];
extern struct M2c_D_001D5BF0 D_001D5BF0;
void FUN_002267b8(void) {
    register s32 inner_6;
    register s32 tbl_6;
    s32 var_6_6;
    register u8 *base18;
    struct M2c_var_5_18 *var_5_18;
    register s32 addr;
    register s32 a8;
    register s32 ac;
    register s32 bound;

    var_6_6 = D_001D5BF0.unkA8;
    if (var_6_6 < (var_6_6 + D_001D5BF0.unkAC)) {    u8 *astra_ptr1 = D_001B3AC0;
    u8 *astra_ptr2 = D_001B3200;
    u8 *astra_ptr0 = D_001D59D8;

        base18 = astra_ptr0;
        var_5_18 = (struct M2c_var_5_18 *) ((var_6_6 * 8) + base18);
        do {
            var_6_6 += 1;
            inner_6 = *((u8 *) (var_5_18->unk0 + (s32) astra_ptr1));
            tbl_6 = *(s32 *) ((inner_6 * 4) + astra_ptr2);
            addr = tbl_6 + (var_5_18->unk4 * 4);
            *(s32 *) (addr + 0x48) = 0;
            var_5_18 += 1;
            a8 = D_001D5BF0.unkA8;
            ac = D_001D5BF0.unkAC;
            bound = a8 + ac;
        } while (var_6_6 < bound);
    }
    D_001D5BF0.unkAC = 0;
}
