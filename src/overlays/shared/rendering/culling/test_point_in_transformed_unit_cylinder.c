/* Culling: point tests against a transformed unit cylinder and unit sphere. */
#include "types.h"
#include "asm.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"

/* Ported from rac1-decomp (src/overlays/shared/mobyutil_00258BC8.c: func_L00_00260AB0), where it is exact; names translated to the US level program. */

typedef struct {
    float x, y, z, w;
} Vp __attribute__((aligned(16)));

int test_point_in_transformed_unit_cylinder(void *point, int i) __asm__("FUN_L00_0025fa38");

int test_point_in_transformed_unit_cylinder(void *point, int i) {
    Vp    point_offset;
    Vp    transformed_point_offset;
    char *volume_entry;
    if (i < 0) {
        return 0;
    }
    volume_entry = D_L00_001600FC + i * 128;
    fast_vec_sub(&point_offset, point, volume_entry + 0x30);
    point_offset.w = 0;
    transform_vector_by_basis(&transformed_point_offset, &point_offset, volume_entry + 0x40);
    if (vector_length_xy(&transformed_point_offset) < 1.0f) {
        if (-1.0f <= transformed_point_offset.z && transformed_point_offset.z <= 1.0f) {
            return 1;
        }
    }
    return 0;
}

/* Ported from rac1-decomp (src/overlays/shared/mobyutil_00258BC8.c: func_L00_00260B68), where it is exact; names translated to the US level program. */

/* True when p lies inside the unit box of D_L00_001600F4 entry idx. */
int test_point_in_transformed_unit_sphere(float *point,
                                          int    volume_index) __asm__("FUN_L00_0025faf0");

int test_point_in_transformed_unit_sphere(float *point, int volume_index) {
    float volume_relative_offset[4] __attribute__((aligned(16)));
    float transformed_volume_offset[4] __attribute__((aligned(16)));
    char *volume_entry;
    if (volume_index >= 0) {
        volume_entry = (char *)D_L00_001600F4 + volume_index * 128;
        fast_vec_sub(volume_relative_offset, point, volume_entry + 0x30);
        volume_relative_offset[3] = 0;
        transform_vector_by_basis(
            transformed_volume_offset, volume_relative_offset, volume_entry + 0x40);
        if (fast_vec_length(transformed_volume_offset) < 1.0f)
            return 1;
    }
    return 0;
}
