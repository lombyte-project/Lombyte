#include "types.h"
struct M2c_temp_3_10 {
    u8 pad_0[0x20];
    u8 unk20;
    u8 pad_21[0x31];
    u8 unk52;
};

extern u8 D_001B2BC0[];
void prune_moby_references(void) __asm__("FUN_0020cc60");

void prune_moby_references(void) {
    s32 var_5_8;
    void **var_4_7;
    struct M2c_temp_3_10 *temp_3_10;
    s32 mask = 0xFF;

    var_4_7 = D_001B2BC0;
    var_5_8 = 0xF;
    do {
        temp_3_10 = *var_4_7;
        var_5_8 -= 1;
        if ((temp_3_10 != NULL) && ((temp_3_10->unk20 & 0x80) || (temp_3_10->unk52 != mask))) {
            *var_4_7 = NULL;
        }
        var_4_7 += 1;
    } while (var_5_8 >= 0);
}
