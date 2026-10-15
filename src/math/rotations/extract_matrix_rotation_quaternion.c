#include "types.h"
extern s32 copy_matrix_rotation() __asm__("func_001FA298");
extern s32 copy_matrix3x4() __asm__("func_001FA2B8");
extern s32 matrix_to_quaternion() __asm__("func_00214260");
void extract_matrix_rotation_quaternion(s32 arg0, s32 arg1) __asm__("FUN_002144d8");

void extract_matrix_rotation_quaternion(s32 arg0, s32 arg1) {
    u8 sp_slot[0x40];
    u8 *buf;

    buf = sp_slot;
    copy_matrix_rotation(buf);
    /* The single-iteration do/while wrappers are allocator-shape levers:
       both compile to plain calls (code-identical) but select retail's
       callee-saved register roles. */
    do {
        matrix_to_quaternion(arg0, sp_slot);
        do {
            copy_matrix3x4(arg1, sp_slot);
        } while (0);
    } while (0);
}

extern __typeof__(extract_matrix_rotation_quaternion) func_002144D8 __attribute__((alias("FUN_002144d8")));
