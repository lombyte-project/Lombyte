#include "types.h"

extern s32 D_0016120C;
extern void func_0023A770();
extern void func_0023BF70();

s32 handle_mpeg_no_data(void) __asm__("FUN_0023d0a8");

s32 handle_mpeg_no_data(void) {
    s32 v;
    func_0023A770();
    v = D_0016120C;
    func_0023BF70(v + 0xD9090);
    return 1;
}
