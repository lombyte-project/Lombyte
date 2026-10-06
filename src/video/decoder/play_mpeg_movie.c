#include "types.h"

extern s32 D_00161208;
extern s32 D_0016120C;
extern s32 ChangeThreadPriority();
extern s32 GetThreadId();
extern s32 read_mpeg() __asm__("func_0023A460");
extern s32 init_all() __asm__("func_0023A7C0");
extern s32 term_all() __asm__("func_0023AA68");

s32 play_mpeg_movie(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) __asm__("FUN_0023a3b8");

s32 play_mpeg_movie(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    D_00161208 = arg2;
    D_0016120C = arg3;
    ChangeThreadPriority(GetThreadId(), 1);
    if (init_all(arg0, arg1, arg4) != 0) {
        read_mpeg(D_0016120C + 0xD9048, D_0016120C, D_0016120C + 0xD9040);
    }
    term_all();
    *(s32 *)0x161208 = 0;
    *(s32 *)0x16120C = 0;
    return 0;
}

extern s32 func_0023A3B8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
    __attribute__((alias("FUN_0023a3b8")));
