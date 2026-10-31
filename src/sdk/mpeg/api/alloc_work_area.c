#include "rnc/sdk/mpeg_work_area.h"

extern u8  work_area_too_small_message[] __asm__("D_00153A38");
extern s32 _Error();

s32 alloc_work_area(s32                  decoder_state,
                    struct MpegWorkArea *area,
                    s32                  size,
                    u32                  alignment) __asm__("FUN_0012bc20");

s32 alloc_work_area(s32 decoder_state, struct MpegWorkArea *area, s32 size, u32 alignment) {
    s32 start;
    u32 new_end;

    start = ((u32)((area->cursor + alignment) - 1) / alignment) * alignment;
    new_end = start + size;
    if ((u32)(area->base + area->size) >= new_end) {
        area->cursor = new_end;
        return start;
    }
    _Error(decoder_state, work_area_too_small_message);
    return 0;
}
