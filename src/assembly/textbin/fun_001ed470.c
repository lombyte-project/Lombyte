#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_001ed470/FUN_001ed470.s", FUN_001ed470);
#else
#include "types.h"
#include "eetypes.h"
#include "qcopy.h"

typedef union {
    u128 q;
    f32 f[4];
} Vec4;

struct Moby {
    u8 pad0[0x18];
    f32 tracked_value;
    u8 pad1C[0x8A];
    s16 oclass;
};

struct Player {
    u8 pad0[0x80];
    Vec4 pos;
    u8 pad90[0x8];
    f32 history_sample;
    u8 pad9C[0x1F4];
    Vec4 target_direction;
    u8 pad2A0[0x5C];
    struct Moby *selected_object;
    u8 pad300[0x1D84];
    s32 secondary_mode;
    u8 pad2088[0x1FC];
    s32 primary_mode;
};

struct CameraTrackingState {
    Vec4 pos;
    f32 position_velocity;
    u8 pad14[0xC];
    Vec4 direction;
    Vec4 target_direction;
    Vec4 previous_target_direction;
    f32 direction_velocity[4];
    Vec4 previous_target_position;
    Vec4 displacement;
    Vec4 perpendicular_displacement;
    Vec4 projected_displacement;
    f32 distance;
    f32 perpendicular_distance;
    f32 forward_distance;
    f32 sample_history[5];
    u8 padC0[0x14];
    struct Moby *tracked_object;
    f32 tracked_value;
    f32 tracked_delta;
};

extern struct Player player_state __asm__("D_0013F350");
extern struct CameraTrackingState camera_tracking_state __asm__("D_001870D0");

extern f32 cam_interp_values(f32 from, f32 to, f32 stiffness, f32 damping, f32 max, f32 *vel) __asm__("FUN_001ebd78");
extern float AbsoluteFloat(float input) __asm__("func_001F99C0");
extern void subtract_vector_xyz(void *out, void *a, void *b) __asm__("FUN_001f9a28");
extern void scale_vector_xyz(void *out, void *a, f32 s) __asm__("FUN_001f9a68");
extern f32 dot_vectors_xyz(void *a, void *b) __asm__("FUN_001f9ab0");
extern f32 vector_length_xyz(void *a) __asm__("FUN_001f9af0");
extern void normalize_vector_xyz(void *out, void *a, f32 len) __asm__("FUN_001f9bf8");

void update_camera_tracking_state(void) __asm__("FUN_001ed470");

void update_camera_tracking_state(void) {
    Vec4 direction;
    Vec4 projected_displacement;
    struct CameraTrackingState *tracking;
    f32 forward_distance;
    f32 direction_stiffness;
    f32 direction_damping;
    s32 history_index;
    struct Moby *tracked_object;

    tracking = &camera_tracking_state;
    normalize_vector_xyz(&direction, &player_state.target_direction, -1.0f);
    qcopy(&tracking->previous_target_direction, &tracking->target_direction);
    qcopy(&tracking->target_direction, &direction);
    if (dot_vectors_xyz(&tracking->direction, &direction) < -0.98f) {
        /* Only the local damping target is perturbed; the published target stays normalized. */
        direction.f[0] += 0.2f;
        direction.f[1] += 0.2f;
        direction.f[2] += 0.2f;
    }
    direction_stiffness = 0.015f;
    direction_damping = 0.2f;
    tracking->direction.f[0] = cam_interp_values(tracking->direction.f[0], direction.f[0], direction_stiffness, direction_damping, 0.0f, &tracking->direction_velocity[0]);
    tracking->direction.f[1] = cam_interp_values(tracking->direction.f[1], direction.f[1], direction_stiffness, direction_damping, 0.0f, &tracking->direction_velocity[1]);
    tracking->direction.f[2] = cam_interp_values(tracking->direction.f[2], direction.f[2], direction_stiffness, direction_damping, 0.0f, &tracking->direction_velocity[2]);
    normalize_vector_xyz(&tracking->direction, &tracking->direction, 1.0f);

    subtract_vector_xyz(&tracking->displacement, &player_state.pos, &tracking->previous_target_position);
    tracking->distance = vector_length_xyz(&tracking->displacement);
    forward_distance = dot_vectors_xyz(&tracking->displacement, &direction);
    tracking->forward_distance = forward_distance;
    normalize_vector_xyz(&projected_displacement, &direction, forward_distance);
    qcopy(&tracking->projected_displacement, &projected_displacement);
    subtract_vector_xyz(&tracking->perpendicular_displacement, &tracking->displacement, &projected_displacement);
    tracking->perpendicular_distance = vector_length_xyz(&tracking->perpendicular_displacement);
    /* Retail divides by zero here when the perpendicular displacement vanishes. */
    scale_vector_xyz(&tracking->perpendicular_displacement, &tracking->perpendicular_displacement, 1.0f / tracking->perpendicular_distance);
    qcopy(&tracking->previous_target_position, &player_state.pos);

    /* Primary mode 0x50 skips the height update unless secondary mode is 0x11. */
    if (player_state.primary_mode != 0x50 || player_state.secondary_mode == 0x11) {
        tracking->pos.f[0] = player_state.pos.f[0];
        tracking->pos.f[1] = player_state.pos.f[1];
        tracking->pos.f[2] = cam_interp_values(tracking->pos.f[2], player_state.pos.f[2], 0.0075f, 0.175f, 0.0f, &tracking->position_velocity);
        tracking->pos.f[3] = player_state.pos.f[2];
    } else {
        tracking->pos.f[0] = player_state.pos.f[0];
        tracking->pos.f[1] = player_state.pos.f[1];
    }

    for (history_index = 0; history_index < 4; history_index++) {
        tracking->sample_history[history_index] = tracking->sample_history[history_index + 1];
    }
    tracking->sample_history[history_index] = player_state.history_sample;

    tracked_object = player_state.selected_object;
    if (tracked_object != NULL && tracked_object->oclass != 0x4BA && tracked_object->oclass != 0x336) {
        if (tracked_object == tracking->tracked_object) {
            tracking->tracked_delta = tracked_object->tracked_value - tracking->tracked_value;
            if (AbsoluteFloat(tracking->tracked_delta) < 0.001f) {
                tracking->tracked_delta = 0.0f;
            }
            tracking->tracked_value = tracking->tracked_object->tracked_value;
        } else {
            tracking->tracked_object = tracked_object;
            tracking->tracked_delta = 0.0f;
            tracking->tracked_value = tracked_object->tracked_value;
        }
    } else {
        tracking->tracked_delta = 0.0f;
        tracking->tracked_object = NULL;
        tracking->tracked_value = player_state.pos.f[2];
    }
}

extern __typeof__(update_camera_tracking_state) func_001ED470 __attribute__((alias("FUN_001ed470")));

#endif /* NON_MATCHING */
