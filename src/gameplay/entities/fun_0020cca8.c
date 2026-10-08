#include "types.h"
#include "rnc/gameplay/entities/moby.h"

extern void FUN_001f9a10(s32, s32, s32);
extern void FUN_001f9a68(s32, s32, f32);
extern void func_001FA298(void *, void *);
extern void FUN_001fa378(s32, void *, s32);
extern void FUN_00210850(struct Moby *, s32, s32 *, s32);
void FUN_0020cca8(struct Moby *arg0, s32 arg1, s32 arg2) {
    u8 sp_slot[0x40];
    s32 sp40;
    f32 scale;
    s32 temp_16_22;

    sp40 = arg1;
    scale = (*(f32 *)((u8 *)arg0 + 0x2C)) * 0.0009765625f;
    FUN_00210850(arg0, 1, &sp40, arg2);
    temp_16_22 = arg2 + 0x30;
    FUN_001f9a68(temp_16_22, temp_16_22, scale);
    func_001FA298(sp_slot, ((u8 *)arg0 + (0xC0)));
    FUN_001fa378(arg2, sp_slot, arg2);
    FUN_001f9a10(temp_16_22, temp_16_22, ((u8 *)arg0 + (0x10)));
}

extern __typeof__(FUN_0020cca8) func_0020CCA8 __attribute__((alias("FUN_0020cca8")));
