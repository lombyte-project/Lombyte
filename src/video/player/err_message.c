#include "types.h"
extern u8 movie_error_message[] __asm__("D_001611F8");
extern s32 DebugPrint();
void err_message(s32 error_code) __asm__("FUN_0023ab78");

void err_message(s32 error_code) {
    DebugPrint(movie_error_message, error_code);
}

extern void func_0023AB78(s32 error_code) __attribute__((alias("FUN_0023ab78")));
