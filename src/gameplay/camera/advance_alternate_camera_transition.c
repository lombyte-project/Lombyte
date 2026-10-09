#include "types.h"
#include "rnc/gameplay/hero.h"
#include "qcopy.h"

typedef struct {
    f32 v[4];
} __attribute__((aligned(16))) CameraVector;

extern u8 D_001871B0[];
extern char D_0013F3D0[];
extern char D_0013F5E0[];
extern char D_00187080[];
extern char D_00187290[];
#include "rnc/gameplay/camera/update_cam.h"

extern f32 cosine_interpolate(f32, f32, f32) __asm__("FUN_002133d0");
extern void cross_vectors_xyz(void *, void *, void *) __asm__("func_001F9AD8");
extern void derive_camera_orbit_parameters(f32 *, void *, void *, void *, void *,
                                           void *) __asm__("func_001EC530");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern f32 fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");
extern f32 fast_add_rotations(f32, f32) __asm__("func_001FA580");
extern void build_look_at_matrix(void *, void *, void *, f32) __asm__("func_00214890");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void quaternion_rotation_basis(void *, void *) __asm__("FUN_001fa480");
extern f32 dot_vectors_xyz(void *, void *) __asm__("func_001F9AB0");
extern void scale_vector_xyz(void *, void *, f32) __asm__("FUN_001f9a68");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern f32 vector_length_xyz(void *) __asm__("FUN_001f9af0");
extern f32 approximate_arcsine(f32) __asm__("func_001F9DF8");
extern f32 AbsoluteFloat(f32) __asm__("func_001F99C0");
extern void build_quaternion_from_axis_angle(void *, void *, f32) __asm__("FUN_00214530");
extern void rotate_vector_by_quaternion(void *, void *, void *) __asm__("func_00214800");
extern void extract_matrix_rotation_quaternion(void *, void *) __asm__("func_002144D8");
extern void tick_countdown_32(void *) __asm__("func_001F9740");

int advance_alternate_camera_transition(char *target_camera,
                                        f32 *transition_state) __asm__("FUN_001eccd8");

int advance_alternate_camera_transition(char *target_camera, f32 *transition_state) {
    CameraVector orbit_angles;
    CameraVector forward_axis;
    CameraVector horizontal_axis;
    CameraVector vertical_axis;
    CameraVector focus_position;
    CameraVector camera_offset;
    CameraVector projected_difference;
    CameraVector axis_projection;
    CameraVector rotation_matrix[3];
    CameraVector smoothed_axis_0;
    CameraVector smoothed_axis_1;
    CameraVector rotation_reference;
    CameraVector rotation_quaternion;
    f32 step;
    f32 orbit_yaw_delta;
    f32 orbit_pitch_delta;
    f32 alignment_angle;
    f32 rotation_step;
    f32 roll_sign;
    f32 tilt_sign;
    f32 projection;
    struct Hero *h;

    if (*(s32 *)(transition_state + 3) <= 0) {
        return 1;
    }
    step = 1.0f / cosine_interpolate(1.0f, (f32) * (s32 *)(transition_state + 5),
                                     (f32) * (s32 *)(transition_state + 3) * transition_state[4]);
    if (D_001871B0[2] == 2) {
        qcopy(&focus_position, D_0013F3D0);
        qcopy(&forward_axis, transition_state + 8);
        qcopy(&vertical_axis, transition_state + 12);
        cross_vectors_xyz(&horizontal_axis, &forward_axis, &vertical_axis);
        derive_camera_orbit_parameters(orbit_angles.v, target_camera + 0x30, &focus_position,
                                       &forward_axis, &horizontal_axis, &vertical_axis);
    } else {
        qcopy(&focus_position, target_camera + 0x30);
        h = &hero;
        normalize_vector_xyz(&forward_axis, (char *)h->moby + 0xC0, 1.0f);
        normalize_vector_xyz(&vertical_axis, (char *)h->moby + 0xE0, 1.0f);
        orbit_angles.v[2] = 0.0f;
        orbit_angles.v[1] = 0.0f;
        orbit_angles.v[0] = 3.1415927f;
    }
    orbit_yaw_delta = fast_subtract_rotations(orbit_angles.v[0], transition_state[0]);
    transition_state[0] = fast_add_rotations(transition_state[0], orbit_yaw_delta * step);
    orbit_pitch_delta = fast_subtract_rotations(orbit_angles.v[1], transition_state[1]);
    transition_state[1] = fast_add_rotations(transition_state[1], orbit_pitch_delta * step);
    transition_state[2] = transition_state[2] + (orbit_angles.v[2] - transition_state[2]) * step;
    normalize_vector_xyz(&camera_offset, &forward_axis, transition_state[2]);
    build_look_at_matrix(&camera_offset, &camera_offset, &vertical_axis, transition_state[0]);
    cross_vectors_xyz(&horizontal_axis, &camera_offset, &vertical_axis);
    normalize_vector_xyz(&horizontal_axis, &horizontal_axis, 1.0f);
    build_look_at_matrix(&camera_offset, &camera_offset, &horizontal_axis, transition_state[1]);
    add_vector_xyz(transition_state + 20, &focus_position, &camera_offset);
    if (camera_position_publication_suppressed[0] == 0) {
        qcopy(D_00187080, transition_state + 20);
    }
    quaternion_rotation_basis(transition_state + 16, rotation_matrix);
    projection = dot_vectors_xyz(&rotation_matrix[2], target_camera);
    scale_vector_xyz(&axis_projection, &rotation_matrix[2], projection);
    subtract_vector_xyz(&projected_difference, target_camera, &axis_projection);
    alignment_angle = 1.5707964f - approximate_arcsine(
                                       dot_vectors_xyz(&rotation_matrix[0], &projected_difference) /
                                       vector_length_xyz(&projected_difference));
    roll_sign = -1.0f;
    if (dot_vectors_xyz(&projected_difference, &rotation_matrix[1]) >= 0.0f) {
        roll_sign = 1.0f;
    }
    alignment_angle = alignment_angle * roll_sign;
    if (AbsoluteFloat(orbit_yaw_delta) > 1.5707964f &&
        ((orbit_yaw_delta >= 0.0f && roll_sign < 0.0f) ||
         (orbit_yaw_delta < 0.0f && roll_sign >= 0.0f))) {
        if (alignment_angle < 0.0f) {
            alignment_angle += 6.2831855f;
        } else {
            alignment_angle -= 6.2831855f;
        }
    }
    rotation_step = alignment_angle * step;
    if (AbsoluteFloat(rotation_step) < 1e-5f) {
        qcopy(&smoothed_axis_0, &rotation_matrix[0]);
        qcopy(&smoothed_axis_1, &rotation_matrix[1]);
    } else {
        build_quaternion_from_axis_angle(&rotation_reference, &rotation_matrix[2], rotation_step);
        rotate_vector_by_quaternion(&smoothed_axis_0, &rotation_matrix[0], &rotation_reference);
        rotate_vector_by_quaternion(&smoothed_axis_1, &rotation_matrix[1], &rotation_reference);
    }
    if (AbsoluteFloat(alignment_angle) < 1e-5f) {
        qcopy(&rotation_reference, &rotation_matrix[0]);
    } else {
        build_quaternion_from_axis_angle(&rotation_quaternion, &rotation_matrix[2],
                                         alignment_angle);
        rotate_vector_by_quaternion(&rotation_reference, &rotation_matrix[0], &rotation_quaternion);
    }
    alignment_angle =
        1.5707964f - approximate_arcsine(dot_vectors_xyz(&rotation_reference, target_camera));
    projection = dot_vectors_xyz(&rotation_reference, target_camera + 0x20);
    tilt_sign = -1.0f;
    if (projection >= 0.0f) {
        tilt_sign = 1.0f;
    }
    alignment_angle *= tilt_sign;
    build_look_at_matrix(&smoothed_axis_0, &smoothed_axis_0, &smoothed_axis_1,
                         alignment_angle * step);
    normalize_vector_xyz(D_00187290, &smoothed_axis_0, 1.0f);
    cross_vectors_xyz(D_00187290 + 0x10, D_00187290, D_0013F5E0);
    normalize_vector_xyz(D_00187290 + 0x10, D_00187290 + 0x10, -1.0f);
    cross_vectors_xyz(D_00187290 + 0x20, D_00187290 + 0x10, D_00187290);
    extract_matrix_rotation_quaternion(transition_state + 24, D_00187290);
    extract_matrix_rotation_quaternion(transition_state + 16, D_00187290);
    tick_countdown_32(transition_state + 3);
    return 0;
}

extern __typeof__(advance_alternate_camera_transition) func_001ECCD8
    __attribute__((alias("FUN_001eccd8")));
