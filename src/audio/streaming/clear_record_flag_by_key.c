#include "types.h"
struct KeyedFlagRecord {
    s32 key;
    s32 flags;
};

extern struct KeyedFlagRecord D_001D60B8[];
s32 clear_record_flag_by_key(s32 arg0) __asm__("FUN_00225e20");

s32 clear_record_flag_by_key(s32 arg0) {
    s32 i;

    for (i = 0; i < 5; i++) {
        if (D_001D60B8[i].key == arg0) {
            D_001D60B8[i].flags &= ~4;
            return 0;
        }
    }
    return 1;
}

/* ACCEPTED: attempt-1 (ee-gcc-2.9-O2) direct 100/100/100; 8-byte record loop, key at 0 / flags at 4, i<5. */
