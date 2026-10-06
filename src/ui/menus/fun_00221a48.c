#include "types.h"
struct ModeRef {
    u8 pad_0[0x84];
    s32 unk84;
};

struct MenuScreen {
    u8 pad_0[0x54];
    s32 unk54;
};

extern struct ModeRef *D_001D5BF4[];
extern s32 select_next_stream_buffer() __asm__("func_00225C18");
s32 FUN_00221a48(struct MenuScreen *arg0) {
    D_001D5BF4[0]->unk84 = 0;
    arg0->unk54 = select_next_stream_buffer(0);
    return 0;
}
