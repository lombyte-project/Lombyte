#include "types.h"
struct MenuScreen {
    u8 pad_0[0x48];
    s32 unk48;
    s32 unk4C;
};

extern s32 initialize_graphics_buffer_descriptors() __asm__("FUN_00225ac0");
extern s32 select_next_stream_buffer() __asm__("FUN_00225c18");
s32 FUN_00222f18(struct MenuScreen *arg0) {
    initialize_graphics_buffer_descriptors(1);
    arg0->unk48 = select_next_stream_buffer(0);
    arg0->unk4C = 0;
    return 0;
}
