#include "types.h"
struct M2c_arg0 {
    u8 pad_0[0x54];
    s32 unk54;
};

extern s32 complete_stream_buffer_transfer() __asm__("FUN_00225cd8");
s32 FUN_00221a88(struct M2c_arg0 *arg0) {
    arg0->unk54 = complete_stream_buffer_transfer(arg0->unk54);
    return 0;
}
