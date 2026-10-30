#include "types.h"

struct Vec3Padded {
    f32 values[3];
    s32 pad;
};

extern void fast_vec_normalize(f32 *, f32 *, f32) __asm__("func_001F9BF8");

void normalize_vector_triplet(f32 *matrix) __asm__("FUN_00214128");

void normalize_vector_triplet(f32 *matrix) {
    struct Vec3Padded scratch;
    f32              *base;
    f32              *component_read_cursor;
    f32              *component_write_cursor;
    f32               value;
    s32               offset;
    s32               i;
    s32               next;
    s32               count;

    base = matrix;
    i = 0;
    do {
        next = i + 1;
        offset = i * 4;
        scratch.pad = 0;
        component_write_cursor = scratch.values;
        component_read_cursor = (f32 *)((u8 *)base + offset);
        count = 2;
        do {
            value = *component_read_cursor;
            count--;
            component_read_cursor += 4;
            *component_write_cursor = value;
            component_write_cursor++;
        } while (count >= 0);
        fast_vec_normalize(scratch.values, scratch.values, 1.0f);
        component_write_cursor = (f32 *)((u8 *)base + offset);
        component_read_cursor = scratch.values;
        count = 2;
        do {
            value = *component_read_cursor;
            count--;
            component_read_cursor++;
            *component_write_cursor = value;
            component_write_cursor += 4;
        } while (count >= 0);
        i = next;
    } while (i < 3);
}

extern __typeof__(normalize_vector_triplet) func_00214128 __attribute__((alias("FUN_00214128")));
