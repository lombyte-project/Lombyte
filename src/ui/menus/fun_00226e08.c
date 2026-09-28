#include "types.h"
extern s32 D_0015ED80;
extern s32 D_0015EE24;
extern s32 D_0015F5EC;
void FUN_00226e08(void) {
    s32 temp_2_10;
    s32 temp_5_8;
    s32 fifty;

    temp_5_8 = D_0015EE24;
    if (D_0015F5EC == 0) {
        fifty = 50;
        temp_2_10 = temp_5_8 + 1;
        D_0015EE24 = temp_2_10;
        if (((temp_2_10 % fifty) == 0) && (D_0015ED80 != 0)) {
            D_0015EE24 = temp_5_8 + 0xB;
        }
    }
}
