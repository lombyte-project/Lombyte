/* Mobys: computes the selected world matrices. */
#include "types.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"

typedef struct {
    float m[16];
} __attribute__((aligned(16))) Mtx;

typedef struct {
    float r[12];
    float t[4];
} __attribute__((aligned(16))) M4;

void get_moby_joint_matrices(char *, int, int, M4 *) __asm__("FUN_00210850");
void compute_selected_world_matrices(char *transform_source,
                                     int   matrix_count,
                                     int   matrix_selection_indices,
                                     M4   *out) __asm__("FUN_L00_0024f8f0");

void compute_selected_world_matrices(char *transform_source,
                                     int   matrix_count,
                                     int   matrix_selection_indices,

                                     M4 *world_matrices) {
    Mtx   rotation_matrix;
    float translation_scale = *(float *)(transform_source + 0x2C) * (1.0f / 1024.0f);
    int   i;
    copy_matrix_rotation(&rotation_matrix, transform_source + 0xC0);
    get_moby_joint_matrices(
        transform_source, matrix_count, matrix_selection_indices, world_matrices);
    for (i = 0; i < matrix_count; i++) {
        multiply_matrix(&world_matrices[i], &rotation_matrix, &world_matrices[i]);
        fast_vec_scale(world_matrices[i].t, world_matrices[i].t, translation_scale);
        fast_vec_add(world_matrices[i].t, world_matrices[i].t, transform_source + 0x10);
    }
}
