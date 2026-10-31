/* Falling moby class: falls under gravity, spins and fades out until its timer ends. */
#include "rnc/mobys/moby.h"
#include "types.h"
#include "rnc/globals.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_00292AC0.c: func_L03_002DF738), where it is exact; names translated to the US level program. */

void falling_moby_update(char *moby) __asm__("FUN_L03_002de370");

/* Moby classes 1221, 1222 */
void falling_moby_update(char *moby) {
    float unused_vector_add_argument[4];
    char *moby_data = ((char *)MOBY(moby)->pvars);
    *(float *)(moby_data + 8) = *(float *)(moby_data + 8) - frame_time_sq * 9.8f;
    add_vectors(unused_vector_add_argument, moby + 0x10, moby_data);
    MOBY(moby)->rot.x = fast_add_rotations(MOBY(moby)->rot.x, *(float *)(moby_data + 0x10));
    MOBY(moby)->rot.y = fast_add_rotations(MOBY(moby)->rot.y, *(float *)(moby_data + 0x14));
    MOBY(moby)->rot.z = fast_add_rotations(MOBY(moby)->rot.z, *(float *)(moby_data + 0x18));
    {
        float remaining_timer_value = convert_integer_to_float(*(int *)(moby_data + 0x24));
        float timer_scale_denominator = convert_integer_to_float(*(int *)(moby_data + 0x20));
        moby[0x23] =
            truncate_float_to_s32(remaining_timer_value * 255.0f / timer_scale_denominator);
    }
    if (fast_dec_timer((int *)(moby_data + 0x24)) != 0) {
        mark_moby_for_removal(moby);
    }
}
