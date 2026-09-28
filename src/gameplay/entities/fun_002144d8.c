#include "types.h"
extern s32 func_001FA298();
extern s32 func_001FA2B8();
extern s32 func_00214260();
void FUN_002144d8(s32 arg0, s32 arg1) {
    u8 sp_slot[0x40];
    u8 *buf;

    buf = sp_slot;
    func_001FA298(buf);
    /* The single-iteration do/while wrappers are allocator-shape levers:
       both compile to plain calls (code-identical) but select retail's
       callee-saved register roles. */
    do {
        func_00214260(arg0, sp_slot);
        do {
            func_001FA2B8(arg1, sp_slot);
        } while (0);
    } while (0);
}

extern __typeof__(FUN_002144d8) func_002144D8 __attribute__((alias("FUN_002144d8")));
