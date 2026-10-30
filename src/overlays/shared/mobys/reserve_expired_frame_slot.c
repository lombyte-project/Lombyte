/* Reserves the last frame slot whose time has passed. */
#include "types.h"
#include "qcopy.h"

extern s32 level_frame_count __asm__("D_L01_0015F5CC");

/* Search backward for the first entry below the loaded threshold. */

s32 reserve_expired_frame_slot(s32 *values, s32 count, s32 offset) __asm__("FUN_L01_002efbf8");

s32 reserve_expired_frame_slot(s32 *values, s32 count, s32 offset) {
    s32 index = (s32)((u32)count - 1);
    if (index >= 0) {
        for (; index >= 0; index--) {
            if (values[index] < level_frame_count) {
                values[index] = (s32)((u32)level_frame_count + (u32)offset);
                break;
            }
        }
    }
    return index;
}
