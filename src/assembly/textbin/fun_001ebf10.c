#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_001ebf10/FUN_001ebf10.s", FUN_001ebf10);
#else
#include "types.h"
#include "eetypes.h"
#include "qcopy.h"

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} Vec4;

typedef struct CameraRecord {
    u128 m0;
    u128 m1;
    u128 m2;
    Vec4 pos;
    u8 pad40[0x24];
    f32 previous_position[3];
    void *saved_state;
    u8 pad74[4];
    f32 transition_duration;
    u8 pad7C;
    u8 handoff_state;
    s16 transition_state;
    u8 pad80[4];
    s16 descriptor_index;
    u8 pad86[8];
    s16 activation_blocked;
} CameraRecord;

typedef struct {
    u8 pad0[0x1D];
    u8 kind;
} CameraDescriptorInfo;

typedef struct {
    u8 pad0[0x1C];
    CameraDescriptorInfo *descriptor;
} CameraDescriptor;

struct CameraTransitionState {
    u8 pad0[0x140];
    u128 published_position;
    u8 pad150[0x30];
    CameraRecord *current;
    CameraRecord *previous;
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

extern struct CameraTransitionState D_00186F40;
extern CameraDescriptor *D_0015EF90;
extern s32 D_0015ED84;
extern u8 D_00189650[];
extern s32 D_0018C32C;

extern void backup_current_cam(void) __asm__("func_001EBC90");
extern void update_moby(CameraRecord *next_camera) __asm__("func_001EBEC8");
extern void copy_blocks_16_forward(void *dst, void *src, s32 size) __asm__("func_001F98D0");
extern s32 convert_float_to_word(f32 transition_duration) __asm__("func_001FA6D0");

void switch_active_camera_record(CameraRecord *next_camera) __asm__("FUN_001ebf10");

void switch_active_camera_record(CameraRecord *next_camera) {

    CameraRecord *previous_camera;
    CameraDescriptorInfo *descriptor;
    f32 transition_duration;
    f32 *previous_position;
    Vec4 *position;
    s32 kind = 0;
    s32 mode;

    descriptor = D_0015EF90[next_camera->descriptor_index].descriptor;
    previous_camera = D_00186F40.current;
    mode = previous_camera->transition_state;
    if (descriptor != 0) {
        kind = descriptor->kind;
    }
    if (mode == 4) {
        next_camera->activation_blocked = 1;
        position = &next_camera->pos;
    } else if (mode == 2 || kind == 1 || kind == 5) {
        if (kind == 1) {
            transition_duration = next_camera->transition_duration;
            D_00186F40.transition_mode = 0;
            if (transition_duration > 0.0f) {
                D_00186F40.configured_rotation_rate = transition_duration;
                D_00186F40.configured_position_rate = transition_duration;
            } else {
                D_00186F40.configured_position_rate = 0.018f;
                D_00186F40.configured_rotation_rate = 0.018f;
            }
        } else if (kind == 5) {
            transition_duration = next_camera->transition_duration;
            D_00186F40.transition_mode = 2;
            if (transition_duration > 0.0f) {
                D_00186F40.configured_frames = convert_float_to_word(transition_duration);
            } else {
                D_00186F40.configured_frames = 40;
            }
        }
        if (D_00186F40.transition_phase == 0) {
            D_00186F40.transition_phase = 1;
        } else {
            D_00186F40.transition_phase = 2;
        }
        position = &next_camera->pos;
    } else if (mode == 3 || mode == 5 || kind == 3 || kind == 6) {
        qcopy(&next_camera->pos, &previous_camera->pos);
        qcopy(&next_camera->m0, &previous_camera->m0);
        qcopy(&next_camera->m1, &previous_camera->m1);
        qcopy(&next_camera->m2, &previous_camera->m2);
        next_camera->handoff_state = 2;
        position = &next_camera->pos;
        if (previous_camera->transition_state == 5 || kind == 6) {
            transition_duration = next_camera->transition_duration;
            D_00186F40.transition_mode = 0;
            if (transition_duration > 0.0f) {
                D_00186F40.configured_rotation_rate = transition_duration;
                D_00186F40.configured_position_rate = transition_duration;
            } else {
                D_00186F40.configured_position_rate = 0.018f;
                D_00186F40.configured_rotation_rate = 0.018f;
                if (D_0015ED84 == 1) {
                    D_00186F40.configured_position_rate = 0.01f;
                    D_00186F40.configured_rotation_rate = 0.01f;
                }
            }
            if (D_00186F40.transition_phase == 0) {
                D_00186F40.transition_phase = 1;
            } else {
                D_00186F40.transition_phase = 2;
            }
        }
    } else {
        next_camera->activation_blocked = 1;
        position = &next_camera->pos;
    }

    previous_camera->transition_state = 0;
    previous_camera->handoff_state = 0;
    previous_camera->activation_blocked = 0;
    D_00186F40.previous = previous_camera;
    copy_blocks_16_forward(D_00189650, D_00189650 - 0x280, 0x280);
    D_00186F40.previous->saved_state = D_00189650;
    D_00186F40.current = next_camera;
    next_camera->saved_state = D_00189650 - 0x280;
    D_00186F40.snapshot_pending = 0;
    update_moby(next_camera);
    backup_current_cam();
    if (D_0018C32C == 0) {
        qcopy(&D_00186F40.published_position, position);
    }
    previous_position = next_camera->previous_position;
    previous_position[0] = next_camera->pos.x;
    previous_position[1] = position->y;
    previous_position[2] = position->z;
}
#endif /* NON_MATCHING */
