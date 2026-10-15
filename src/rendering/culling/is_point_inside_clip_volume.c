#include "types.h"
#include "rnc/data_labels.h"
struct Vec4 {
    f32 x, y, z, w;
};
extern void subtract_vector_xyz(void *, s32, void *) __asm__("func_001F9A28");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("func_001F9CF8");
s32 is_point_inside_clip_volume(s32 point, s32 volume_index) __asm__("FUN_00214720");

s32 is_point_inside_clip_volume(s32 point, s32 volume_index) {
    struct Vec4 in;
    struct Vec4 out;
    u8 *entry;
    if (volume_index == -1)
        return 0;
    entry = (u8 *)D_00160034 + (volume_index << 7);
    subtract_vector_xyz(&in, point, entry + 0x30);
    in.w = 0.0f;
    transform_vector_by_basis(&out, &in, entry + 0x40);
    if ((out.x >= -1.0f) && (out.x <= 1.0f) && (out.y >= -1.0f) && (out.y <= 1.0f) &&
        (out.z >= -1.0f) && (out.z <= 1.0f))
        return 1;
    return 0;
}

extern __typeof__(is_point_inside_clip_volume) func_00214720 __attribute__((alias("FUN_00214720")));
