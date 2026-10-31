#include "types.h"
#include "rnc/data_labels.h"

struct Vec4 {
    f32 x, y, z, w;
};

extern void fast_vec_sub(void *, void *, void *) __asm__("func_001F9A28");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("func_001F9CF8");
s32         is_point_inside_clip_volume(void *point, s32 volume_index) __asm__("FUN_00214720");

s32 is_point_inside_clip_volume(void *point, s32 volume_index) {
    struct Vec4 in;
    struct Vec4 volume_space_point;
    u8         *clip_volume_record;
    if (volume_index == -1)
        return 0;
    clip_volume_record = (u8 *)D_00160034 + (volume_index << 7);
    fast_vec_sub(&in, point, clip_volume_record + 0x30);
    in.w = 0.0f;
    transform_vector_by_basis(&volume_space_point, &in, clip_volume_record + 0x40);
    if ((volume_space_point.x >= -1.0f) && (volume_space_point.x <= 1.0f) &&
        (volume_space_point.y >= -1.0f) && (volume_space_point.y <= 1.0f) &&
        (volume_space_point.z >= -1.0f) && (volume_space_point.z <= 1.0f))
        return 1;
    return 0;
}

extern __typeof__(is_point_inside_clip_volume) func_00214720 __attribute__((alias("FUN_00214720")));
