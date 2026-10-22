/* Ported from rac1-decomp (src/game/mobyutil.c, func_002156E0). */
#include "qcopy.h"
extern float func_001F99C0(float arg0);
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern void build_quaternion_from_axis_angle(void *arg0, void *axis,
                                             float angle) __asm__("FUN_00214530");
extern void rotate_vector_by_quaternion(void *arg0, void *arg1,
                                        void *arg2) __asm__("func_00214800");
/* dst = vec rotated `angle` around axis. A tiny angle skips the rotation
   (dst = vec); otherwise the axis is normalised to unit length and turned
   into an axis-angle quaternion in a scratch buffer, which then rotates
   vec into dst. */
void build_look_at_matrix(void *dst, void *vec, void *axis, float angle) __asm__("FUN_00214890");

void build_look_at_matrix(void *rotated_vector, void *source_vector, void *axis, float angle) {
    float rotation_quaternion[4];

    if (func_001F99C0(angle) < 0.00001f) {
        qcopy(rotated_vector, source_vector);
        return;
    }
    fast_vec_normalize(rotation_quaternion, axis, 1.0f);
    build_quaternion_from_axis_angle(rotation_quaternion, rotation_quaternion, angle);
    rotate_vector_by_quaternion(rotated_vector, source_vector, rotation_quaternion);
}

extern __typeof__(build_look_at_matrix) func_00214890 __attribute__((alias("FUN_00214890")));
