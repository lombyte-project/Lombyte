/* Mobys: captures and applies the transforms of linked mobys relative to their parent. */
#include "types.h"
#include "asm.h"
#include "rnc/overlay/level_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/rotations/matrix_to_euler.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"

/* Ported from rac1-decomp (src/overlays/shared/mobyutil_0027C260.c: func_L12_0027C368), where it is exact; names translated to the US level program. */

void apply_linked_moby_relative_transforms(char *parent,
                                           char *list,
                                           int   entry_count) __asm__("FUN_L12_0027b370");

void apply_linked_moby_relative_transforms(char *parent, char *list, int entry_count) {
    float parent_rotation_matrix[16];
    float composed_rotation_matrix[16];
    float rotated_offset[4];
    char *linked_object;
    int   i;
    euler_rotation_matrix(parent_rotation_matrix, (float *)(parent + 0x40));
    for (i = 0; i < entry_count; i++) {
        int linked_object_index = *(int *)(list + i * 0x30 + 0x20);
        if (linked_object_index != -1) {
            linked_object = D_L12_0015FFD8_p + linked_object_index * 256;
            euler_rotation_matrix(composed_rotation_matrix, (float *)(list + i * 0x30 + 0x10));
            multiply_matrix(
                composed_rotation_matrix, parent_rotation_matrix, composed_rotation_matrix);
            matrix_to_euler(composed_rotation_matrix, linked_object + 0x40);
            transform_vector(rotated_offset, list + i * 0x30, parent_rotation_matrix);
            fast_vec_add(linked_object + 0x10, parent + 0x10, rotated_offset);
            update_entity(linked_object);
        }
    }
}

/* Ported from rac1-decomp (src/overlays/shared/mobyutil_0027C260.c: func_L12_0027C260), where it is exact; names translated to the US level program. */

void capture_linked_moby_relative_transforms(char *moby,
                                             char *arr,
                                             int   entry_count) __asm__("FUN_L12_0027b268");

void capture_linked_moby_relative_transforms(char *moby, char *arr, int entry_count) {
    float moby_rotation_basis[12];
    float inverse_moby_rotation_matrix[16];
    float relative_rotation_matrix[16];
    int   i;
    euler_rotation_basis(moby_rotation_basis, moby + 0x40);
    transpose_rotation_matrix(inverse_moby_rotation_matrix, moby_rotation_basis);
    for (i = 0; i < entry_count; i++) {
        char *linked_moby_transform_record = arr + i * 0x30;
        int   linked_moby_index = *(int *)(linked_moby_transform_record + 0x20);
        if (linked_moby_index != -1) {
            char *linked_moby = D_L12_0015FFD8_p + (linked_moby_index << 8);
            fast_vec_sub(linked_moby_transform_record, linked_moby + 0x10, moby + 0x10);
            transform_vector(linked_moby_transform_record,
                             linked_moby_transform_record,
                             inverse_moby_rotation_matrix);
            euler_rotation_matrix(relative_rotation_matrix, linked_moby + 0x40);
            multiply_matrix(
                relative_rotation_matrix, inverse_moby_rotation_matrix, relative_rotation_matrix);
            matrix_to_euler(relative_rotation_matrix, linked_moby_transform_record + 0x10);
        }
    }
}
