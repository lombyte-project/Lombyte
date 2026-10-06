#include "types.h"
#include "rnc/rendering/geometry/transform_scaled_vertex_batch.h"

extern void func_001F9A10(void *arg0, void *arg1, void *arg2);
extern void func_001F9A68(void *arg0, void *arg1, f32 arg2);
extern void func_001F9CF8(void *arg0, void *arg1, void *arg2);
extern void func_002106F8(void);

void transform_scaled_vertex_batch(struct VertexBatch *object, s32 count, s32 arg2,
                                   u8 *cursor) __asm__("FUN_0020cd48");

void transform_scaled_vertex_batch(struct VertexBatch *object, s32 count, s32 arg2, u8 *cursor) {
    f32 scale;

    scale = *(f32 *)((u8 *)object + 0x2C) * 0.0009765625f;
    func_002106F8();
    if (count > 0) {
        do {
            func_001F9A68(cursor, cursor, scale);
            count -= 1;
            func_001F9CF8(cursor, cursor, (u8 *)object + 0xC0);
            func_001F9A10(cursor, cursor, (u8 *)object + 0x10);
            cursor += 0x10;
        } while (count != 0);
    }
}

extern __typeof__(transform_scaled_vertex_batch) func_0020CD48
    __attribute__((alias("FUN_0020cd48")));
