/* Ported from rac1-decomp (src/game/mobyutil.c, func_002156E0). */
#include "qcopy.h"
#include "rnc/math/fast_math.h"

extern void
build_quaternion_from_axis_angle(void *arg0, void *axis, float angle) __asm__("FUN_00214530");
extern void
rotate_vector_by_quaternion(void *out, void *source_vector, void *quat) __asm__("func_00214800");
/* dst = vec rotated `angle` around axis. A tiny angle skips the rotation
   (dst = vec); otherwise the axis is normalised to unit length and turned
   into an axis-angle quaternion in a scratch buffer, which then rotates
   vec into dst. */
void build_look_at_matrix(void *dst, void *vec, void *axis, float angle) __asm__("FUN_00214890");

void build_look_at_matrix(void *rotated_vector, void *source_vector, void *axis, float angle) {
    float rotation_quaternion[4];

    if (absolute_float(angle) < 0.00001f) {
        qcopy(rotated_vector, source_vector);
        return;
    }
    fast_vec_normalize(rotation_quaternion, axis, 1.0f);
    build_quaternion_from_axis_angle(rotation_quaternion, rotation_quaternion, angle);
    rotate_vector_by_quaternion(rotated_vector, source_vector, rotation_quaternion);
}

extern __typeof__(build_look_at_matrix) func_00214890 __attribute__((alias("FUN_00214890")));
