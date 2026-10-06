#include "types.h"
extern s32 D_0015ED80;
extern s32 D_0015EE24;
extern s32 D_0015F5EC;
void FUN_00226e08(void) {
    s32 new_value;
    s32 old_value;
    s32 fifty;

    old_value = D_0015EE24;
    if (D_0015F5EC == 0) {
        fifty = 50;
        new_value = old_value + 1;
        D_0015EE24 = new_value;
        if (((new_value % fifty) == 0) && (D_0015ED80 != 0)) {
            D_0015EE24 = old_value + 0xB;
        }
    }
}
