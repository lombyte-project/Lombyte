#include "types.h"
extern s32 copy_matrix_rotation() __asm__("func_001FA298");
extern s32 copy_matrix3x4() __asm__("func_001FA2B8");
extern s32 matrix_to_quaternion() __asm__("func_00214260");
void extract_matrix_rotation_quaternion(s32 arg0, s32 arg1) __asm__("FUN_002144d8");

void extract_matrix_rotation_quaternion(s32 output_quaternion, s32 destination_matrix) {
    u8 sp_slot[0x40];
    u8 *buf;

    buf = sp_slot;
    copy_matrix_rotation(buf);
    /* Fakematch: the single-iteration do/while wrappers compile to plain
       calls but select retail's callee-saved register roles. */
    do {
        matrix_to_quaternion(output_quaternion, sp_slot);
        /* Fakematch: see above. */
        do {
            copy_matrix3x4(destination_matrix, sp_slot);
        } while (0);
    } while (0);
}

extern __typeof__(extract_matrix_rotation_quaternion) func_002144D8 __attribute__((alias("FUN_002144d8")));
