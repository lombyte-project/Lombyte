#include "types.h"
#include "eetypes.h"
#include "qcopy.h"

typedef union { u128 q; f32 f[4]; } Vec4;

struct CameraPath {
    s32 count;
    s32 pad[3];
    u128 points[1];
};

extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void FUN_001f9a40(void *, void *, void *, f32);
extern f32 vector_length_xy(void *) __asm__("FUN_001f9b20");
extern f32 FUN_001f9e90(f32, f32);
extern f32 fast_add_rotations(f32, f32) __asm__("func_001FA580");
extern f32 fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");
extern s32 convert_float_to_word(f32) __asm__("func_001FA6D0");

void sample_camera_path(struct CameraPath *path, s32 loop, void *position, f32 *rotation, s32 flags, f32 progress) __asm__("FUN_00214e58");

void sample_camera_path(struct CameraPath *path, s32 loop, void *position, f32 *rotation, s32 flags, f32 progress) {
    Vec4 current_point;
    Vec4 next_point;
    Vec4 following_point;
    Vec4 segment;
    s32 point_index;
    s32 next_index;
    s32 following_index;
    s32 clamped;
    f32 fraction;
    f32 pitch;
    f32 heading;
    f32 c;
    f32 next_heading;
    f32 bank;
    f32 next_bank;
    f32 next_pitch;

    clamped = 0;
    point_index = convert_float_to_word(progress);
    if (loop == 0) {
        s32 n = path->count - 2;
        if (!(point_index < n)) {
            point_index = n;
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
    FUN_001f9a40(position, &current_point, &next_point, fraction);
    if (!(flags & 1)) {
        subtract_vector_xyz(&segment, &next_point, &current_point);
        heading = FUN_001f9e90(segment.f[0], segment.f[1]);
        pitch = FUN_001f9e90(vector_length_xy(&segment), segment.f[2]);
        bank = current_point.f[3];
        next_pitch = pitch;
        if (clamped) {
            next_heading = heading;
            bank = next_bank;
        } else {
            qcopy(&following_point.q, &path->points[following_index]);
            subtract_vector_xyz(&segment, &following_point, &next_point);
            next_heading = FUN_001f9e90(segment.f[0], segment.f[1]);
            next_pitch = FUN_001f9e90(vector_length_xy(&segment), segment.f[2]);
            next_bank = next_point.f[3];
        }
        *(s32 *)rotation = 0;
        rotation[1] = -fast_add_rotations(fast_subtract_rotations(next_pitch, pitch) * fraction, pitch);
        rotation[2] = fast_add_rotations(fast_subtract_rotations(next_heading, heading) * fraction, heading);
        rotation[3] = -fast_add_rotations(fast_subtract_rotations(next_bank, bank) * fraction, bank);
    }
}

extern __typeof__(sample_camera_path) func_00214E58 __attribute__((alias("FUN_00214e58")));
