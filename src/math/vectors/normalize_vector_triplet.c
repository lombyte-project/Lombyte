#include "types.h"

struct Vec3Padded {
    f32 values[3];
    s32 pad;
};

extern void func_001F9BF8(f32 *, f32 *, f32);

void normalize_vector_triplet(f32 *arg0) __asm__("FUN_00214128");

void normalize_vector_triplet(f32 *arg0) {
    struct Vec3Padded scratch;
    f32 *base;
    f32 *src;
    f32 *dst;
    f32 value;
    s32 offset;
    s32 i;
    s32 next;
    s32 count;

    base = arg0;
    i = 0;
    do {
        next = i + 1;
        offset = i * 4;
        scratch.pad = 0;
        dst = scratch.values;
        src = (f32 *)((u8 *)base + offset);
        count = 2;
        do {
            value = *src;
            count--;
            src += 4;
            *dst = value;
            dst++;
        } while (count >= 0);
        func_001F9BF8(scratch.values, scratch.values, 1.0f);
        dst = (f32 *)((u8 *)base + offset);
        src = scratch.values;
        count = 2;
        do {
            value = *src;
            count--;
            src++;
            *dst = value;
            dst += 4;
        } while (count >= 0);
        i = next;
    } while (i < 3);
}

extern __typeof__(normalize_vector_triplet) func_00214128 __attribute__((alias("FUN_00214128")));
