#include "types.h"
#include "rnc/rendering/graphics_buffer.h"
extern void FillTransferWords();
extern s32 get_stream_buffer_size() __asm__("func_00225D88");

s32 select_next_stream_buffer(s32 arg0) __asm__("FUN_00225c18");

s32 select_next_stream_buffer(s32 arg0) {
    s32 i;
    s32 f;

    for (i = 0; i < 5; i++) {
        if (arg0 != 0) {
            f = graphics_buffer_descriptors[i].flags ^ 1;
        } else {
            f = graphics_buffer_descriptors[i].flags;
        }
        if (!(f & 1)) {
            if (graphics_buffer_descriptors[i].address != 0) {
                if (!(graphics_buffer_descriptors[i].flags & 2)) {
                    graphics_buffer_descriptors[i].flags |= 2;
                    FillTransferWords(graphics_buffer_descriptors[i].address, 0xDEADBEEF,
                                      get_stream_buffer_size(graphics_buffer_descriptors[i].address));
                    return graphics_buffer_descriptors[i].address;
                }
            }
        }
    }
    return 0;
}

extern __typeof__(select_next_stream_buffer) func_00225C18 __attribute__((alias("FUN_00225c18")));
