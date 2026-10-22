#include "types.h"

extern f32 absolute_float(f32) __asm__("func_001F99C0");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern s32 fast_vector_to_packed_chars(void *) __asm__("FUN_001f9d68");

void pack_vector_xyz_with_shared_scale(f32 *v, s32 *out) __asm__("FUN_00214a98");

void pack_vector_xyz_with_shared_scale(f32 *source_vector, s32 *out) {
    f32 encoded_xyz_and_scale[4];
    f32 abs_x;
    f32 max_abs_xy_component;
    f32 abs_z;
    f32 max_abs_component;
    s32 shared_scale_byte;
    f32 component_encode_scale;

    abs_x = absolute_float(source_vector[0]);
    max_abs_xy_component = absolute_float(source_vector[1]);
    abs_z = absolute_float(source_vector[2]);
    if (max_abs_xy_component < abs_x) {
        max_abs_xy_component = abs_x;
    }
    max_abs_component = (abs_z < max_abs_xy_component) ? max_abs_xy_component : abs_z;
    shared_scale_byte = truncate_float_to_s32(max_abs_component * 10000.0f / 63.0f);
    shared_scale_byte = (shared_scale_byte < 0x100) ? shared_scale_byte : 0xFF;
    if (shared_scale_byte <= 0) {
        shared_scale_byte = 1;
    }
    encoded_xyz_and_scale[3] = (f32)shared_scale_byte;
    component_encode_scale = 1.0f / (encoded_xyz_and_scale[3] * 0.0001f);
    encoded_xyz_and_scale[0] = source_vector[0] * component_encode_scale + 127.0f;
    encoded_xyz_and_scale[1] = source_vector[1] * component_encode_scale + 127.0f;
    encoded_xyz_and_scale[2] = source_vector[2] * component_encode_scale + 127.0f;
    *out = fast_vector_to_packed_chars(encoded_xyz_and_scale);
}

extern __typeof__(pack_vector_xyz_with_shared_scale) func_00214A98 __attribute__((alias("FUN_00214a98")));
