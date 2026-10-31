/* Level 04 moby class 0x269: a moby placed on a path at the progress another moby holds, carrying a linked moby with it. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "qcopy.h"
#include "rnc/overlay/level_path.h"
#include "sda.h"

/* update: blend two keyframes of a path and write the result to a position */
/* Ported from rac1-decomp (src/overlays/l04_eudora/vendor_002CB800.c: func_L04_002D8348), where it is exact; names translated to the US level program. */

extern float FUN_L00_00200260(float, float);

extern char *D_L04_0015FFD0[] MACRO_ADDR;
void         path_progress_mover_update(char *moby) __asm__("FUN_L04_002d6f68");

/* Moby class 617 */
void path_progress_mover_update(char *moby) {
    int   *moby_vars = ((int *)MOBY(moby)->pvars);
    int   *path_data = ((int *)l04_paths[moby_vars[2]]);
    char  *moby_array_base = D_L04_0015FFD0[2];
    int    source_moby_byte_offset = moby_vars[0] * 256;
    float *source_moby_vars = *(float **)(moby_array_base + source_moby_byte_offset + 0x78);
    char  *destination_moby = moby_array_base + moby_vars[1] * 256;
    float  weighted_first_point[4];
    float  weighted_second_point[4];
    float  path_interpolation_factor, path_point_coordinate;
    int    first_point_index;
    int    second_point_index = 1;
    float  one = 1.0f;

    path_interpolation_factor = convert_integer_to_float(path_data[0] - 1);
    path_point_coordinate = (one - source_moby_vars[0]) * path_interpolation_factor;
    path_interpolation_factor = FUN_L00_00200260(path_point_coordinate, one);
    first_point_index = truncate_float_to_s32(path_point_coordinate);
    if (path_interpolation_factor == 0.0f)
        second_point_index = 0;
    second_point_index += first_point_index;
    fast_vec_scale(weighted_first_point,
                   (char *)path_data + (first_point_index * 16 + 0x10),
                   one - path_interpolation_factor);
    fast_vec_scale(weighted_second_point,
                   (char *)path_data + (second_point_index * 16 + 0x10),
                   path_interpolation_factor);
    fast_vec_add(moby + 0x10, weighted_first_point, weighted_second_point);
    qcopy(destination_moby + 0x10, moby + 0x10);
}
