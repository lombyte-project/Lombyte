#include "types.h"
#include "rnc/mobys/moby.h"

extern u8   attachment_point_indices[] __asm__("D_00160ED0");
extern void transform_scaled_vertex_batch() __asm__("func_0020CD48");
extern void fast_vec_sub() __asm__("func_001F9A28");
extern f32  fast_vec_length() __asm__("func_001F9AF0");

struct AttachmentVars {
    f32 points[4][4];  /* 0x00: the four vertices attachment_point_indices names */
    f32 first_length;  /* 0x40: points[1] - points[0] */
    f32 second_length; /* 0x44: points[2] - points[0] */
};

struct __attribute__((packed)) Pair64 {
    u64 a;
    u64 b;
};

void update_instance_attachment_points_and_lengths(struct Moby *instance) __asm__("FUN_0023a318");

void update_instance_attachment_points_and_lengths(struct Moby *instance) {
    u8                     offsets[0x30];
    struct AttachmentVars *attachment_points_and_lengths;
    attachment_points_and_lengths = (struct AttachmentVars *)instance->pvars;
    *(struct Pair64 *)(offsets + 0x20) = *(struct Pair64 *)attachment_point_indices;
    transform_scaled_vertex_batch(instance, 4, offsets + 0x20, attachment_points_and_lengths);
    fast_vec_sub(offsets, attachment_points_and_lengths->points[1], attachment_points_and_lengths);
    fast_vec_sub(offsets + 0x10, attachment_points_and_lengths->points[2], attachment_points_and_lengths);
    attachment_points_and_lengths->first_length = fast_vec_length(offsets);
    attachment_points_and_lengths->second_length = fast_vec_length(offsets + 0x10);
}

extern __typeof__(update_instance_attachment_points_and_lengths) func_0023A318
    __attribute__((alias("FUN_0023a318")));
