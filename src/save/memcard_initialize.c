#include "types.h"
#include "rnc/sdk/debug.h"
#include "rnc/sdk/storage/memory_card.h"

extern u8 memcard_error_message[] __asm__("D_001E8360");
void      memcard_initialize(void) __asm__("FUN_0020ac58");

void memcard_initialize(void) {
    if (sceMcInit() != 0) {
        DebugPrint(memcard_error_message);
    }
}

extern void func_0020AC58(void) __attribute__((alias("FUN_0020ac58")));
extern void memcard_Init(void) __attribute__((alias("FUN_0020ac58")));
