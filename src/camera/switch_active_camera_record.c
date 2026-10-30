#include "types.h"
#include "asm.h"

#include "types.h"
#include "eetypes.h"
#include "qcopy.h"

#include "rnc/camera/update_cam.h"
#include "rnc/globals.h"
#include "rnc/camera/backup_current_cam.h"

typedef struct {
    u8 pad0[0x1D];
    u8 descriptor_kind;
} CameraDescriptorInfo;

typedef struct {
    u8                    pad0[0x1C];
    CameraDescriptorInfo *descriptor;
} CameraDescriptor;

struct CameraTransitionState {
    u8                pad0[0x140];
    u128              published_position;
    u8                pad150[0x30];
    struct UpdateCam *current;
    struct UpdateCam *previous;
    u8                pad188[0xE8];
    s16               transition_phase;
    u8                pad272;
    u8                transition_mode;
    u8                pad274[0x14];
    f32               configured_rotation_rate;
    u8                pad28C[8];
    f32               configured_position_rate;
    u8                pad298[0x5C];
    s32               configured_frames;
    u8                pad2F8[0xA0];
    s32               snapshot_pending;
};

extern struct CameraTransitionState preview_camera __asm__("D_00186F40");

extern void camera_run_setup_to_new_cam(struct UpdateCam *next_camera) __asm__("func_001EBEC8");
extern void fast_mem_copy(void *dst, void *src, s32 size) __asm__("func_001F98D0");
extern s32  truncate_float_to_s32(f32 transition_duration) __asm__("func_001FA6D0");

void switch_active_camera_record(struct UpdateCam *next_camera) __asm__("FUN_001ebf10");

extern CameraDescriptor *D_0015EF90;

void switch_active_camera_record(struct UpdateCam *next_camera) {
    struct UpdateCam     *previous_camera;
    CameraDescriptorInfo *descriptor;
    f32                   transition_duration;
    f32                   mode_one_duration;
    f32                   handoff_duration;
    f32                  *previous_position;
    Vec4f                *position;
    s32                   descriptor_kind = 0;
    s32                   previous_transition_state;

    descriptor = D_0015EF90[next_camera->descriptor_index].descriptor;
    previous_camera = preview_camera.current;
    previous_transition_state = previous_camera->transition_state;
    if (descriptor != 0) {
        descriptor_kind = descriptor->descriptor_kind;
    }
    if (previous_transition_state == 4) {
        next_camera->activation_blocked = 1;
    } else if (previous_transition_state == 2 || descriptor_kind == 1 || descriptor_kind == 5) {
        if (descriptor_kind == 1) {
            mode_one_duration = next_camera->transition_duration;
            preview_camera.transition_mode = 0;
            if (mode_one_duration > 0.0f) {
                preview_camera.configured_position_rate = mode_one_duration;
                preview_camera.configured_rotation_rate = mode_one_duration;
            } else {
                preview_camera.configured_position_rate = 0.018f;
                preview_camera.configured_rotation_rate = 0.018f;
            }
        } else if (descriptor_kind == 5) {
            transition_duration = next_camera->transition_duration;
            preview_camera.transition_mode = 2;
            if (transition_duration > 0.0f) {
                preview_camera.configured_frames = truncate_float_to_s32(transition_duration);
            } else {
                preview_camera.configured_frames = 40;
            }
        }
        if (preview_camera.transition_phase == 0) {
            preview_camera.transition_phase = 1;
        } else {
            preview_camera.transition_phase = 2;
        }
    } else if (previous_transition_state == 3 || previous_transition_state == 5 ||
               descriptor_kind == 3 || descriptor_kind == 6) {
        qcopy(&next_camera->pos, &previous_camera->pos);
        qcopy(&next_camera->m0, &previous_camera->m0);
        qcopy(&next_camera->m1, &previous_camera->m1);
        qcopy(&next_camera->m2, &previous_camera->m2);
        next_camera->handoff_state = 2;
        if (previous_camera->transition_state == 5 || descriptor_kind == 6) {
            handoff_duration = next_camera->transition_duration;
            preview_camera.transition_mode = 0;
            if (handoff_duration > 0.0f) {
                preview_camera.configured_position_rate = handoff_duration;
                preview_camera.configured_rotation_rate = handoff_duration;
            } else {
                preview_camera.configured_position_rate = 0.018f;
                preview_camera.configured_rotation_rate = 0.018f;
                if (current_level_index == 1) {
                    preview_camera.configured_position_rate = 0.01f;
                    preview_camera.configured_rotation_rate = 0.01f;
                }
            }
            if (preview_camera.transition_phase == 0) {
                preview_camera.transition_phase = 1;
            } else {
                preview_camera.transition_phase = 2;
            }
        }
    } else {
        next_camera->activation_blocked = 1;
    }

    position = &next_camera->pos;
    previous_camera->transition_state = 0;
    previous_camera->handoff_state = 0;
    previous_camera->activation_blocked = 0;
    preview_camera.previous = previous_camera;
    fast_mem_copy(&camera_saved_states[1], &camera_saved_states[0], 0x280);
    preview_camera.previous->saved_state = &camera_saved_states[1];
    preview_camera.current = next_camera;
    next_camera->saved_state = &camera_saved_states[0];
    preview_camera.snapshot_pending = 0;
    camera_run_setup_to_new_cam(next_camera);
    backup_current_cam();
    /* The callbacks run before this flag is read; previous_position is updated either way. */
    if (camera_position_publication_suppressed[0] == 0) {
        qcopy(&preview_camera.published_position, &next_camera->pos);
    }
    previous_position = &next_camera->prev_pos.x;
    previous_position[0] = next_camera->pos.x;
    previous_position[1] = position->y;
    previous_position[2] = position->z;
}

extern __typeof__(switch_active_camera_record) func_001EBF10 __attribute__((alias("FUN_001ebf10")));
