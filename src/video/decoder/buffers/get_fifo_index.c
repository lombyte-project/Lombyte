#include "types.h"
struct M2c_arg0 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
};

u32 get_fifo_index(struct M2c_arg0 *arg0, s32 arg1) __asm__("FUN_0023baf8");

u32 get_fifo_index(struct M2c_arg0 *arg0, s32 arg1) {
    if (arg1 == (((arg0->unk8 * 0x10) + arg0->unk4 + 0x10) & 0x0FFFFFFF)) {
        return 0U;
    }
    return (u32)(arg1 - arg0->unk0) >> 0xB;
}
