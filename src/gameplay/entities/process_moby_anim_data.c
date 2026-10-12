#include "types.h"
extern s32 D_0015F63C;
extern u8 normal_lut[] __asm__("D_00165500");
extern s32 FlushCache();
extern s32 FUN_001f98d0();
extern s32 FUN_00211728();
void process_moby_anim_data(void) __asm__("FUN_0020d1a8");

void process_moby_anim_data(void) {
    FlushCache(0);
    FUN_001f98d0(0x70003800, normal_lut, 0x800);
    FUN_00211728(*(s32 *)0x0015F638, D_0015F63C);
}
