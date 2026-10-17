#include "types.h"

struct VertexBatch {
    u8 pad_0[0x2C];
    s32 unk2C;
};

extern void add_vector_xyz(void *arg0, void *arg1, void *arg2) __asm__("func_001F9A10");
extern void fast_vec_scale(void *arg0, void *arg1, f32 arg2) __asm__("func_001F9A68");
extern void transform_vector_by_basis(void *arg0, void *arg1, void *arg2) __asm__("func_001F9CF8");
extern void func_002106F8(void);

void transform_scaled_vertex_batch(struct VertexBatch *object, s32 count, s32 arg2,
                                   u8 *cursor) __asm__("FUN_0020cd48");

void transform_scaled_vertex_batch(struct VertexBatch *object, s32 count, s32 point_indices, u8 *cursor) {
    f32 scale;

    scale = *(f32 *)((u8 *)object + 0x2C) * 0.0009765625f;
    func_002106F8();
    if (count > 0) {
        do {
            fast_vec_scale(cursor, cursor, scale);
            count -= 1;
            transform_vector_by_basis(cursor, cursor, (u8 *)object + 0xC0);
            add_vector_xyz(cursor, cursor, (u8 *)object + 0x10);
            cursor += 0x10;
        } while (count != 0);
    }
}

extern __typeof__(transform_scaled_vertex_batch) func_0020CD48
    __attribute__((alias("FUN_0020cd48")));
