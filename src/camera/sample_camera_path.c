#include "types.h"
#include "eetypes.h"
#include "qcopy.h"

#include "rnc/math/vector.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"

struct CameraPath {
    s32  count;
    s32  pad[3];
    u128 points[1];
};

extern f32 fast_add_rotations(f32, f32) __asm__("func_001FA580");
extern f32 fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");
extern s32 truncate_float_to_s32(f32) __asm__("func_001FA6D0");

void sample_camera_path(struct CameraPath *path,
                        s32                loop,
                        void              *position,
                        f32               *rotation,
                        s32                flags,
                        f32                progress) __asm__("FUN_00214e58");

void sample_camera_path(

    struct CameraPath *path, s32 loop, void *position, f32 *rotation, s32 flags, f32 progress) {
    Vec4 current_point;
    Vec4 next_point;
    Vec4 following_point;
    Vec4 segment;
    s32  point_index;
    s32  next_index;
    s32  following_index;
    s32  clamped;
    f32  fraction;
    f32  pitch;
    f32  heading;
    f32  c;
    f32  next_heading;
    f32  bank;
    f32  next_bank;
    f32  next_pitch;

    clamped = 0;
    point_index = truncate_float_to_s32(progress);
    if (loop == 0) {
        s32 last_segment_index = path->count - 2;
        if (!(point_index < last_segment_index)) {
            point_index = last_segment_index;
            clamped = 1;
        }
    }
    fraction = progress - (f32)point_index;
    if (clamped) {
        if (1.0f < fraction) {
            fraction = 1.0f;
        }
    }
    following_index = point_index + 2;
    next_index = point_index + 1;
    if (!(following_index < path->count)) {
        next_index = next_index % path->count;
        following_index = following_index % path->count;
    }
    next_bank = 0.0f;
    qcopy(&current_point.q, &path->points[point_index]);
    qcopy(&next_point.q, &path->points[next_index]);
    lerp_vector(position, &current_point, &next_point, fraction);
    if (!(flags & 1)) {
        fast_vec_sub(&segment, &next_point, &current_point);
        heading = fast_arc_tan(segment.f[0], segment.f[1]);
        pitch = fast_arc_tan(vector_length_xy(&segment), segment.f[2]);
        bank = current_point.f[3];
        next_pitch = pitch;
        if (clamped) {
            next_heading = heading;
            bank = next_bank;
        } else {
            qcopy(&following_point.q, &path->points[following_index]);
            fast_vec_sub(&segment, &following_point, &next_point);
            next_heading = fast_arc_tan(segment.f[0], segment.f[1]);
            next_pitch = fast_arc_tan(vector_length_xy(&segment), segment.f[2]);
            next_bank = next_point.f[3];
        }
        *(s32 *)rotation = 0;
        rotation[1] =
            -fast_add_rotations(fast_subtract_rotations(next_pitch, pitch) * fraction, pitch);
        rotation[2] =
            fast_add_rotations(fast_subtract_rotations(next_heading, heading) * fraction, heading);
        rotation[3] =
            -fast_add_rotations(fast_subtract_rotations(next_bank, bank) * fraction, bank);
    }
}

extern __typeof__(sample_camera_path) func_00214E58 __attribute__((alias("FUN_00214e58")));
