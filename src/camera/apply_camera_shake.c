#include "types.h"

#include "rnc/camera/update_cam.h"
#include "rnc/camera/camera_globals.h"
#include "rnc/camera/camera_shake.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"

extern struct UpdateCam *cur_cam[] __asm__("D_001870C0");
extern u8                D_001872B0[];

extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_normalize_angle(f32) __asm__("func_001FA610");

void apply_camera_shake(struct CameraShake *shake, s32 axis_index) __asm__("FUN_001ed360");

void apply_camera_shake(struct CameraShake *shake, s32 axis_index) {
    f32 shake_offset[4];
    f32 remaining_frames_float;
    f32 total_frames_float;
    f32 remaining_frame_ratio;
    f32 oscillation_cosine;
    f32 applied_shake_offset;
    s32 remaining_frames;

    if (cur_cam[0] != 0 && cur_cam[0]->unk86 == 6) {
        shake->frames = 0;
        shake->frames_max = 0;
    } else {
        remaining_frames = shake->frames;
        if (remaining_frames != 0) {
            if (shake->frames_max < remaining_frames) {
                shake->frames_max = remaining_frames;
            }
            fast_dec_timer(&shake->frames);
            remaining_frames_float = convert_integer_to_float(shake->frames);
            total_frames_float = convert_integer_to_float(shake->frames_max);
            remaining_frame_ratio = remaining_frames_float / total_frames_float;
            oscillation_cosine =
                fast_cos(fast_normalize_angle(2.0f * convert_integer_to_float(shake->frames)));
            applied_shake_offset = shake->amplitude * oscillation_cosine * remaining_frame_ratio *
                                   remaining_frame_ratio;
            shake->offset = applied_shake_offset;
            if (axis_index == 0) {
                fast_vec_normalize(shake_offset, D_001872B0, applied_shake_offset);
            } else {
                fast_vec_normalize(shake_offset, camera_rows, applied_shake_offset);
            }
            fast_vec_add(camera_position, camera_position, shake_offset);
        } else {
            shake->frames_max = 0;
        }
    }
}

extern __typeof__(apply_camera_shake) func_001ED360 __attribute__((alias("FUN_001ed360")));
