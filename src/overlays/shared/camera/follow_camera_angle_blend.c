/* Follow camera angle blends towards a direction or a position. */

#include "types.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/fast_math.h"
#include "sda.h"
#include "rnc/camera/follow_cam.h"

extern float cubic_hermite_scalar(float, float, float, float, float) __asm__("FUN_L00_00257e20");
extern void  FUN_001f9a68_c4(void *, void *, float) __asm__("FUN_001f9a68");
void         FUN_001f9a28_c2(void *, void *, void *) __asm__("FUN_001f9a28");

extern char D_L00_00166E10[];

/* Measures the signed angle between two offsets of an object's data relative to an axis, and (within the limit) stores a scaled blend in the object's data. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002E9B60), where it is exact; names translated to the US level program. */
float update_follow_camera_angle_blend_from_direction(
    char  *camera,
    float *input_direction,
    float  blend_update_angle_limit) __asm__("FUN_L00_002e86b0");

float update_follow_camera_angle_blend_from_direction(char  *camera,
                                                      float *input_direction,

                                                      float blend_update_angle_limit) {
    float axis_projection[4];
    float planar_input_direction[4];
    float planar_reference_direction[4];
    float input_cross_axis[4];
    char *base = D_L00_00166E10;
    char *axis = base + 0x20;
    char *camera_state = *(char **)(camera + 0x70);
    char *blend_output_block = camera_state + 0x1A8;
    char *reference_direction = camera_state + 0x130;
    float dot;
    float planar_input_length_or_length_product;
    float planar_separation_angle;
    float sign;
    float angle_delta = 0.0f;
    float z2;
    float angle_blend_fraction;

    FUN_001f9a68_c4(axis_projection, axis, fast_vec_dot(input_direction, axis));
    fast_vec_sub(planar_input_direction, input_direction, axis_projection);
    FUN_001f9a68_c4(axis_projection, axis, fast_vec_dot(reference_direction, axis));
    fast_vec_sub(planar_reference_direction, (float *)reference_direction, axis_projection);
    dot = fast_vec_dot(planar_input_direction, planar_reference_direction);
    planar_input_length_or_length_product = fast_vec_length(planar_input_direction);
    if (planar_input_length_or_length_product == angle_delta) {
        return angle_delta;
    }
    z2 = 0.0f;
    planar_input_length_or_length_product =
        planar_input_length_or_length_product * fast_vec_length(planar_reference_direction);
    if (planar_input_length_or_length_product == angle_delta) {
        return angle_delta;
    }
    planar_separation_angle =
        1.5707964f - fast_arc_sin(dot / planar_input_length_or_length_product);
    sign = -1.0f;
    fast_vec_cross(input_cross_axis, planar_input_direction, axis);
    if (fast_vec_dot(input_cross_axis, planar_reference_direction) >= 0.0f) {
        sign = 1.0f;
    }
    angle_delta = fast_subtract_rotations(3.1415927f, planar_separation_angle);
    if (blend_update_angle_limit != z2 && blend_update_angle_limit < absolute_float(angle_delta)) {
        return angle_delta;
    }
    angle_blend_fraction = angle_delta / 1.5707964f;
    if (angle_blend_fraction > 1.0f) {
        angle_blend_fraction = 1.0f;
    }
    *(float *)(blend_output_block + 0x1C) =
        cubic_hermite_scalar(-1.0f, 0.0f, 1.0f, 0.0f, angle_blend_fraction) * sign;
    return angle_delta;
}

/* Passes b minus the vector at +0x40 of p's object, with x, to FUN_L00_002e86b0. */
float update_follow_camera_angle_blend_from_position(char *camera,
                                                     void *target_position,
                                                     f32   x) __asm__("FUN_L00_002e88c8");

float update_follow_camera_angle_blend_from_position(char *camera, void *target_position, f32 x) {
    float focus_to_target_direction[4];
    FUN_001f9a28_c2(focus_to_target_direction, target_position, *(char **)(camera + 0x70) + 0x40);
    return update_follow_camera_angle_blend_from_direction(camera, focus_to_target_direction, x);
}

/* For non-zero x: stores x at +0x1BC of D_L00_00166E00+0x70 and calls FUN_L00_002e86b0. */
void set_follow_camera_angle_blend_from_direction(char *input_direction,
                                                  float x,
                                                  float y) __asm__("FUN_L00_002e8918");

void set_follow_camera_angle_blend_from_direction(char *input_direction, float x, float y) {
    char *current_camera = (char *)current_follow_cam;
    if (*(short *)(current_camera + 0x86) == 0 && x != 0.0f) {
        *(float *)(*(char **)(current_camera + 0x70) + 0x1bc) = x;
        update_follow_camera_angle_blend_from_direction(current_camera, input_direction, y);
    }
}

void set_follow_camera_angle_blend_from_position(char *a,
                                                 float x,
                                                 float y) __asm__("FUN_L00_002e8970");

void set_follow_camera_angle_blend_from_position(char *target_position, float x, float y) {
    char *target_position_arg = target_position;
    char *current_camera = (char *)current_follow_cam;
    if (*(short *)(current_camera + 0x86) == 0) {
        *(float *)(*(char **)(current_camera + 0x70) + 0x1bc) = x;
        update_follow_camera_angle_blend_from_position(current_camera, target_position_arg, y);
    }
}
