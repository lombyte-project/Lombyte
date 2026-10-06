#include "types.h"
struct M2c_D_001A00F0 {
    u8 pad_0[0x278];
    s32 unk278[5];
    s32 unk28C[5];
};
extern struct M2c_D_001A00F0 D_001A00F0;
extern s32 find_free_map_slot() __asm__("func_00204EF8");
extern s32 move_map_entry_slot() __asm__("func_00205000");

s32 promote_first_available_map_entry(void) __asm__("FUN_00204f60");

s32 promote_first_available_map_entry(void) {
    s32 i;

    i = find_free_map_slot(1);
    if (i != 0) {
        return i;
    }
    for (i = 1; i < 5; i++) {
        if (!(D_001A00F0.unk28C[i] & 0x1000) && D_001A00F0.unk278[i] != 0) {
            break;
        }
    }
    move_map_entry_slot(0, i);
    return i;
}

extern __typeof__(promote_first_available_map_entry) func_00204F60 __attribute__((alias("FUN_00204f60")));
