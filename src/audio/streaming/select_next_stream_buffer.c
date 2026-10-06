#include "types.h"
struct E {
    s32 unk0;
    s32 flags;
};
extern struct E D_001D60B8[];
extern void FillTransferWords();
extern s32 get_stream_buffer_size() __asm__("func_00225D88");

s32 select_next_stream_buffer(s32 arg0) __asm__("FUN_00225c18");

s32 select_next_stream_buffer(s32 arg0) {
    s32 i;
    s32 f;

    for (i = 0; i < 5; i++) {
        if (arg0 != 0) {
            f = D_001D60B8[i].flags ^ 1;
        } else {
            f = D_001D60B8[i].flags;
        }
        if (!(f & 1)) {
            if (D_001D60B8[i].unk0 != 0) {
                if (!(D_001D60B8[i].flags & 2)) {
                    D_001D60B8[i].flags |= 2;
                    FillTransferWords(D_001D60B8[i].unk0, 0xDEADBEEF,
                                      get_stream_buffer_size(D_001D60B8[i].unk0));
                    return D_001D60B8[i].unk0;
                }
            }
        }
    }
    return 0;
}

extern __typeof__(select_next_stream_buffer) func_00225C18 __attribute__((alias("FUN_00225c18")));
