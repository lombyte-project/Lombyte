#include "types.h"
struct BufferPool {
    s32 unk0;
    s32 unk4;
    s32 unk8;
};

extern u8 D_00153A38[];
extern s32 _Error();
s32 reserve_aligned_buffer_space(s32 arg0, struct BufferPool *pool, s32 size,
                                 u32 alignment) __asm__("FUN_0012bc20");

s32 reserve_aligned_buffer_space(s32 arg0, struct BufferPool *pool, s32 size, u32 alignment) {
    s32 start;
    u32 new_end;

    start = ((u32)((pool->unk8 + alignment) - 1) / alignment) * alignment;
    new_end = start + size;
    if ((u32)(pool->unk0 + pool->unk4) >= new_end) {
        pool->unk8 = new_end;
        return start;
    }
    _Error(arg0, D_00153A38);
    return 0;
}