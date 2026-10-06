#include "types.h"
struct M2c_arg0 {
    s32 unk0;
    s32 unk4;
    u8 pad_8[0x28];
    s32 unk30;
    u8 *unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
};

void audio_dec_begin_put(struct M2c_arg0 *arg0, void **arg1, s32 *arg2, void **arg3,
                         s32 *arg4) __asm__("FUN_0023ad58");

void audio_dec_begin_put(struct M2c_arg0 *arg0, void **arg1, s32 *arg2, void **arg3, s32 *arg4) {
    s32 t2;

    if (arg0->unk0 == 0) {
        if (arg0->unk4 != 4) {
            *arg1 = ((u8 *)arg0 + (arg0->unk30 + 8));
            *arg2 = 0x28 - arg0->unk30;
            *arg3 = arg0->unk34;
            *arg4 = arg0->unk40;
        } else {
            *arg1 = arg0->unk34;
            *arg2 = arg0->unk40;
            *arg3 = NULL;
            *arg4 = 0;
        }
    } else {
        t2 = arg0->unk40 - arg0->unk3C;
        if ((arg0->unk40 - arg0->unk38) >= t2) {
            *arg1 = arg0->unk34 + arg0->unk38;
            *arg2 = t2;
            *arg3 = NULL;
            *arg4 = 0;
        } else {
            *arg1 = arg0->unk34 + arg0->unk38;
            *arg2 = arg0->unk40 - arg0->unk38;
            *arg3 = arg0->unk34;
            *arg4 = t2 - (arg0->unk40 - arg0->unk38);
        }
    }
}
extern void func_0023AD58(struct M2c_arg0 *, void **, s32 *, void **, s32 *)
    __attribute__((alias("FUN_0023ad58")));
