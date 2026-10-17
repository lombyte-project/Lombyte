#include "types.h"

#include "rnc/gameplay/camera/update_cam.h"

struct CameraShake {
    f32 unk0;
    f32 unk4;
    s32 unk8;
    s32 unkC;
};

extern struct UpdateCam *cur_cam[] __asm__("D_001870C0");
extern u8 D_00187080[];
extern char camera_rows[] __asm__("D_00187290");
extern u8 D_001872B0[];

extern s32 fast_dec_timer(s32 *) __asm__("func_001F9740");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_normalize(f32 *, f32 *, f32) __asm__("FUN_001f9bf8");
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_normalize_angle(f32) __asm__("func_001FA610");
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");

void apply_camera_shake(struct CameraShake *shake, s32 arg1) __asm__("FUN_001ed360");

void apply_camera_shake(struct CameraShake *shake, s32 axis_index) {
    f32 sp_slot[4];
    f32 remaining_frames_float;
    f32 total_frames_float;
    f32 remaining_frame_ratio;
    f32 oscillation_cosine;
    f32 val;
    s32 remaining_frames;

    if (cur_cam[0] != 0 && cur_cam[0]->unk86 == 6) {
        shake->unk8 = 0;
        shake->unkC = 0;
    } else {
        remaining_frames = shake->unk8;
        if (remaining_frames != 0) {
            if (shake->unkC < remaining_frames) {
                shake->unkC = remaining_frames;
            }
            fast_dec_timer(&shake->unk8);
            remaining_frames_float = convert_integer_to_float(shake->unk8);
            total_frames_float = convert_integer_to_float(shake->unkC);
            remaining_frame_ratio = remaining_frames_float / total_frames_float;
            oscillation_cosine = fast_cos(fast_normalize_angle(2.0f * convert_integer_to_float(shake->unk8)));
            val = shake->unk0 * oscillation_cosine * remaining_frame_ratio * remaining_frame_ratio;
            shake->unk4 = val;
            if (axis_index == 0) {
                fast_vec_normalize(sp_slot, D_001872B0, val);
            } else {
                fast_vec_normalize(sp_slot, camera_rows, val);
            }
            add_vector_xyz(D_00187080, D_00187080, sp_slot);
        } else {
            shake->unkC = 0;
        }
    }
}

extern __typeof__(apply_camera_shake) func_001ED360 __attribute__((alias("FUN_001ed360")));
