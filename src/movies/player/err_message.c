#include "types.h"
#include "rnc/sdk/debug.h"

extern const char movie_error_message[] __asm__("D_001611F8");
void              err_message(s32 error_code) __asm__("FUN_0023ab78");

/* Moby class 676 */
void err_message(s32 error_code) {
    DebugPrint(movie_error_message, error_code);
}

extern void func_0023AB78(s32 error_code) __attribute__((alias("FUN_0023ab78")));

const char movie_error_message[] __asm__("D_001611F8")
    __attribute__((section(".sdata"))) = "[ Error ] %s\n";
