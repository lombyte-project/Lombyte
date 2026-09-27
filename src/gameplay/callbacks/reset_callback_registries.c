#include "types.h"
extern s32 D_0015F330;
extern s32 D_0015F334;
extern s32 D_0015F34C __attribute__((sda));
extern s32 D_0015F360 __attribute__((sda));
extern s32 D_0015F370 __attribute__((sda));
extern s32 D_0015F444;
extern s32 D_0015F448;
extern s32 D_0015F464;
extern s32 D_0015F468;
extern s32 D_0015F46C;
extern s32 D_0015F470;
extern s32 D_0015F474;
extern s32 D_0015F648;
extern s32 D_00161190;
extern s32 D_00161194;
extern s32 D_00161198;
extern s32 D_0016119C __attribute__((sda));

void reset_callback_registries(void) __asm__("FUN_001f37e8");

void reset_callback_registries(void) {
    D_0015F464 = 0;
    D_0015F46C = 0;
    D_0015F470 = 0;
    D_0015F468 = 0;
    D_0015F474 = 0;
    D_0015F330 = 0;
    D_0015F334 = 0;
    D_0015F444 = 0;
    D_0015F448 = 0;
    D_0015F34C = 0;
    D_0015F360 = 0;
    D_0015F370 = 0;
    D_0015F648 = 0;
    D_00161190 = 0;
    D_00161194 = 0;
    D_00161198 = 0;
    D_0016119C = 0;
}

extern __typeof__(reset_callback_registries) func_001F37E8 __attribute__((alias("FUN_001f37e8")));
