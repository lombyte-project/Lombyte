#include "types.h"

extern void subtract_vector_xyz(void *out, void *a, void *b) __asm__("FUN_001f9a28");
extern f32 fast_vec_dot(void *a, void *b) __asm__("func_001F9AB0");
extern void fast_vec_normalize(void *out, void *a, f32 len) __asm__("FUN_001f9bf8");
extern f32 fast_vec_length(void *a) __asm__("FUN_001f9af0");
extern f32 fast_arc_sin(f32) __asm__("func_001F9DF8");
extern void build_look_at_matrix(void *out, void *dir, void *axis, f32 ang) __asm__("FUN_00214890");

void compute_camera_angles(f32 *out, void *p0, void *p1, void *dir0, void *dir1,
                           void *axis) __asm__("FUN_001ec530");

void compute_camera_angles(f32 *out, void *position, void *focus, void *dir0, void *dir1, void *axis) {
    f32 diff[4];
    f32 proj[4];
    f32 perp[4];
    f32 unit[4];
    f32 rotated[4];
    f32 offset_axis_dot, planar_reference_dot, yaw_side_dot, rotated_reference_dot, pitch_axis_dot;
    f32 lenPerp, lenDiff;
    f32 angle1, angle2;
    f32 yaw, pitch;

    subtract_vector_xyz(diff, position, focus);
    offset_axis_dot = fast_vec_dot(diff, axis);
    fast_vec_normalize(proj, axis, offset_axis_dot);
    subtract_vector_xyz(perp, diff, proj);

    planar_reference_dot = fast_vec_dot(dir0, perp);
    lenPerp = fast_vec_length(perp);
    if (lenPerp == 0.0f) {
        lenPerp = 0.0001f;
    }
    angle1 = fast_arc_sin(planar_reference_dot / lenPerp);
    yaw = 1.57079637f - angle1;

    fast_vec_normalize(unit, perp, 1.0f);
    yaw_side_dot = fast_vec_dot(dir1, unit);
    if (yaw_side_dot < 0.0f) {
        yaw = -yaw;
    }
    out[0] = yaw;

    build_look_at_matrix(rotated, dir0, axis, yaw);
    rotated_reference_dot = fast_vec_dot(rotated, diff);
    lenDiff = fast_vec_length(diff);
    if (lenDiff == 0.0f) {
        lenDiff = 0.0001f;
    }
    angle2 = fast_arc_sin(rotated_reference_dot / lenDiff);
    pitch = -(1.57079637f - angle2);

    fast_vec_normalize(unit, diff, 1.0f);
    pitch_axis_dot = fast_vec_dot(axis, unit);
    if (pitch_axis_dot < 0.0f) {
        pitch = 1.57079637f - angle2;
    }
    out[1] = pitch;

    out[2] = fast_vec_length(diff);
}

extern __typeof__(compute_camera_angles) func_001EC530 __attribute__((alias("FUN_001ec530")));
