#include "types.h"
extern u8 D_00160ED0[];
extern void transform_scaled_vertex_batch() __asm__("func_0020CD48");
extern void subtract_vector_xyz() __asm__("func_001F9A28");
extern f32 fast_vec_length() __asm__("func_001F9AF0");
struct Node {
    u8 pad[0x40];
    f32 a;
    f32 b;
};
struct Arg {
    u8 pad[0x78];
    struct Node *node;
};
struct __attribute__((packed)) Pair64 {
    u64 a;
    u64 b;
};
void update_instance_attachment_points_and_lengths(struct Arg *arg) __asm__("FUN_0023a318");

void update_instance_attachment_points_and_lengths(struct Arg *instance) {
    u8 sp_slot[0x30];
    struct Node *node;
    node = instance->node;
    *(struct Pair64 *)(sp_slot + 0x20) = *(struct Pair64 *)D_00160ED0;
    transform_scaled_vertex_batch(instance, 4, sp_slot + 0x20, node);
    subtract_vector_xyz(sp_slot, (u8 *)node + 0x10, node);
    subtract_vector_xyz(sp_slot + 0x10, (u8 *)node + 0x20, node);
    node->a = fast_vec_length(sp_slot);
    node->b = fast_vec_length(sp_slot + 0x10);
}

extern __typeof__(update_instance_attachment_points_and_lengths) func_0023A318 __attribute__((alias("FUN_0023a318")));
