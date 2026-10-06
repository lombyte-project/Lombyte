#include "types.h"
struct MenuScreen {
    u8 pad_0[0x48];
    s32 unk48;
};

extern s32 complete_stream_buffer_transfer() __asm__("FUN_00225cd8");
s32 FUN_00222f58(struct MenuScreen *arg0) {
    arg0->unk48 = complete_stream_buffer_transfer(arg0->unk48);
    return 0;
}
