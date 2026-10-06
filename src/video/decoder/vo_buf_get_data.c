#include "types.h"
struct VoBuf {
    s32 unk0;
    u8 pad_4[0x4];
    s32 unk8;
};

extern s32 FUN_0023d1f8();
s32 vo_buf_get_data(struct VoBuf *arg0) __asm__("FUN_0023d288");

s32 vo_buf_get_data(struct VoBuf *arg0) {
    if (FUN_0023d1f8() != 0) {
        return 0;
    }
    return arg0->unk0 + (arg0->unk8 * 0xD0000);
}

/* Recovered original symbol name. */
extern __typeof__(vo_buf_get_data) voBufGetData__FP5VoBuf __attribute__((alias("FUN_0023d288")));
