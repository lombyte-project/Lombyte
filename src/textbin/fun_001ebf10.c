#include "types.h"
#include "asm.h"

#include "types.h"
#include "eetypes.h"
#include "qcopy.h"

#include "rnc/gameplay/camera/update_cam.h"
#include "rnc/globals.h"

typedef struct {
    u8 pad0[0x1D];
    u8 descriptor_kind;
} CameraDescriptorInfo;

typedef struct {
    u8 pad0[0x1C];
    CameraDescriptorInfo *descriptor;
} CameraDescriptor;

struct CameraTransitionState {
    u8 pad0[0x140];
    u128 published_position;
    u8 pad150[0x30];
    struct UpdateCam *current;
    struct UpdateCam *previous;
    u8 pad188[0xE8];
    s16 transition_phase;
    u8 pad272;
    u8 transition_mode;
    u8 pad274[0x14];
    f32 configured_rotation_rate;
    u8 pad28C[8];
    f32 configured_position_rate;
    u8 pad298[0x5C];
    s32 configured_frames;
    u8 pad2F8[0xA0];
    s32 snapshot_pending;
};

extern struct CameraTransitionState camera_transition_state __asm__("D_00186F40");
extern CameraDescriptor *camera_descriptors __asm__("D_0015EF90");

extern void backup_current_cam(void) __asm__("FUN_001ebc90");
extern void camera_run_setup_to_new_cam(struct UpdateCam *next_camera) __asm__("func_001EBEC8");
extern void copy_blocks_16_forward(void *dst, void *src, s32 size) __asm__("func_001F98D0");
extern s32 convert_float_to_word(f32 transition_duration) __asm__("func_001FA6D0");

void switch_active_camera_record(struct UpdateCam *next_camera) __asm__("FUN_001ebf10");

void switch_active_camera_record(struct UpdateCam *next_camera) {

    struct UpdateCam *previous_camera;
    CameraDescriptorInfo *descriptor;
    f32 transition_duration;
    f32 mode_one_duration;
    f32 handoff_duration;
    f32 *previous_position;
    Vec4f *position;
    s32 descriptor_kind = 0;
    s32 previous_transition_state;

    descriptor = camera_descriptors[next_camera->descriptor_index].descriptor;
    previous_camera = camera_transition_state.current;
    previous_transition_state = previous_camera->transition_state;
    if (descriptor != 0) {
        descriptor_kind = descriptor->descriptor_kind;
    }
    if (previous_transition_state == 4) {
        next_camera->activation_blocked = 1;
    } else if (previous_transition_state == 2 || descriptor_kind == 1 || descriptor_kind == 5) {
        if (descriptor_kind == 1) {
            mode_one_duration = next_camera->transition_duration;
            camera_transition_state.transition_mode = 0;
            if (mode_one_duration > 0.0f) {
                camera_transition_state.configured_position_rate = mode_one_duration;
                camera_transition_state.configured_rotation_rate = mode_one_duration;
            } else {
                camera_transition_state.configured_position_rate = 0.018f;
                camera_transition_state.configured_rotation_rate = 0.018f;
            }
        } else if (descriptor_kind == 5) {
            transition_duration = next_camera->transition_duration;
            camera_transition_state.transition_mode = 2;
            if (transition_duration > 0.0f) {
                camera_transition_state.configured_frames =
                    convert_float_to_word(transition_duration);
            } else {
                camera_transition_state.configured_frames = 40;
            }
        }
        if (camera_transition_state.transition_phase == 0) {
            camera_transition_state.transition_phase = 1;
        } else {
            camera_transition_state.transition_phase = 2;
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
            camera_transition_state.transition_mode = 0;
            if (handoff_duration > 0.0f) {
                camera_transition_state.configured_position_rate = handoff_duration;
                camera_transition_state.configured_rotation_rate = handoff_duration;
            } else {
                camera_transition_state.configured_position_rate = 0.018f;
                camera_transition_state.configured_rotation_rate = 0.018f;
                if (current_level_index == 1) {
                    camera_transition_state.configured_position_rate = 0.01f;
                    camera_transition_state.configured_rotation_rate = 0.01f;
                }
            }
            if (camera_transition_state.transition_phase == 0) {
                camera_transition_state.transition_phase = 1;
            } else {
                camera_transition_state.transition_phase = 2;
            }
        }
    } else {
        next_camera->activation_blocked = 1;
    }

    position = &next_camera->pos;
    previous_camera->transition_state = 0;
    previous_camera->handoff_state = 0;
    previous_camera->activation_blocked = 0;
    camera_transition_state.previous = previous_camera;
    copy_blocks_16_forward(&camera_saved_states[1], &camera_saved_states[0], 0x280);
    camera_transition_state.previous->saved_state = &camera_saved_states[1];
    camera_transition_state.current = next_camera;
    next_camera->saved_state = &camera_saved_states[0];
    camera_transition_state.snapshot_pending = 0;
    camera_run_setup_to_new_cam(next_camera);
    backup_current_cam();
    /* The callbacks run before this flag is read; previous_position is updated either way. */
    if (camera_position_publication_suppressed[0] == 0) {
        qcopy(&camera_transition_state.published_position, &next_camera->pos);
    }
    previous_position = &next_camera->prev_pos.x;
    previous_position[0] = next_camera->pos.x;
    previous_position[1] = position->y;
    previous_position[2] = position->z;
}
extern __typeof__(switch_active_camera_record) func_001EBF10 __attribute__((alias("FUN_001ebf10")));
