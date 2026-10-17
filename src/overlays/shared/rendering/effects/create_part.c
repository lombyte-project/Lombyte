/* Level overlay code: stubs become C as functions are matched. */
#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L00_002678b8.s", FUN_L00_002678b8);
#else
extern void *create_part2(s32, s32) __asm__("FUN_L00_002678c0");

/* Calls create_part2 with type and 0. */
void *create_part(s32 type) __asm__("FUN_L00_002678b8");
void *create_part(s32 type) {
    return create_part2(type, 0);
}
#endif /* NON_MATCHING */
INCLUDE_ASM("config/us/overlays/asm/FUN_L00_002678c0.s", FUN_L00_002678c0);
INCLUDE_ASM("config/us/overlays/asm/FUN_L00_00267a08.s", FUN_L00_00267a08);
INCLUDE_ASM("config/us/overlays/asm/FUN_L00_00267b10.s", FUN_L00_00267b10);
