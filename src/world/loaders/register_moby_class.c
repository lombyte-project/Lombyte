#include "types.h"
#include "rnc/gameplay/entities/moby.h"
extern s32 D_0015FF00;
extern u8 D_001B3AC0[];
extern s16 D_001B3900[];
extern struct Moby *D_001B3200[];
extern s32 D_001B6180[];
extern void assign_moby_class_data(s32) __asm__("FUN_00212d68");
extern void prepare_resident_class_render_data(struct Moby *, s32, s32,
                                               s32) __asm__("FUN_00203338");
void register_moby_class(struct Moby *moby, s32 arg1, s32 arg2, s32 oclass) __asm__("FUN_00203640");

void register_moby_class(struct Moby *moby, s32 arg1, s32 arg2, s32 oclass) {
    s32 n;

    n = D_0015FF00;
    D_001B3AC0[oclass] = D_0015FF00;
    D_001B3900[n] = oclass;
    D_001B3200[n] = moby;
    if (moby == 0) {
        assign_moby_class_data(oclass);
        D_0015FF00++;
    } else {
        D_001B6180[n] = moby->unk2C;
        assign_moby_class_data(oclass);
        D_0015FF00++;
        prepare_resident_class_render_data(moby, arg1, arg2, oclass);
    }
}

extern __typeof__(register_moby_class) func_00203640 __attribute__((alias("FUN_00203640")));
