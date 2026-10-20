#include "types.h"

extern f32 absolute_float(f32) __asm__("func_001F99C0");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern s32 fast_vector_to_packed_chars(void *) __asm__("FUN_001f9d68");

void pack_vector_xyz_with_shared_scale(f32 *v, s32 *out) __asm__("FUN_00214a98");

void pack_vector_xyz_with_shared_scale(f32 *v, s32 *out) {
    f32 buf[4];
    f32 abs_x;
    f32 b;
    f32 abs_z;
    f32 max_abs_component;
    s32 shared_scale_byte;
    f32 component_encode_scale;

    abs_x = absolute_float(v[0]);
    b = absolute_float(v[1]);
    abs_z = absolute_float(v[2]);
    if (b < abs_x) {
        b = abs_x;
    }
    max_abs_component = (abs_z < b) ? b : abs_z;
    shared_scale_byte = truncate_float_to_s32(max_abs_component * 10000.0f / 63.0f);
    shared_scale_byte = (shared_scale_byte < 0x100) ? shared_scale_byte : 0xFF;
    if (shared_scale_byte <= 0) {
        shared_scale_byte = 1;
    }
    buf[3] = (f32)shared_scale_byte;
    component_encode_scale = 1.0f / (buf[3] * 0.0001f);
    buf[0] = v[0] * component_encode_scale + 127.0f;
    buf[1] = v[1] * component_encode_scale + 127.0f;
    buf[2] = v[2] * component_encode_scale + 127.0f;
    *out = fast_vector_to_packed_chars(buf);
}

extern __typeof__(pack_vector_xyz_with_shared_scale) func_00214A98 __attribute__((alias("FUN_00214a98")));
