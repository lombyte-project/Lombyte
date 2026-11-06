/* Ported from rac1-decomp (src/game/camera.c, func_001ECC48). */
#include "sda.h"
#include "rnc/hero/hero.h"
#include "qcopy.h"
#include "rnc/camera/camera_globals.h"
#include "rnc/camera/update_cam.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"

extern void compute_camera_angles(
    float *out, void *p0, void *p1, void *dir0, void *dir1, void *axis) __asm__("func_001EC530");
extern void  capture_camera_orientation(void) __asm__("func_001EC710");
extern void  save_camera_vectors(void) __asm__("func_001EC7F0");
extern void  restore_camera_vectors(void) __asm__("func_001EC868");
extern short cam_trans_state_lui __asm__("D_001871B0") NOT_SDA;
extern void  extract_matrix_rotation_quaternion(void *, void *) __asm__("func_002144D8");
void         start_camera_blend(struct UpdateCam *source) __asm__("FUN_001ec8a0");

void start_camera_blend(struct UpdateCam *source) {
    enum {
        CAMERA_TRANSITION_KIND_BLEND = 0,
        CAMERA_TRANSITION_PHASE_SEED_FROM_SOURCE = 1
    };
    enum {
        CAMERA_TRANSITION_KIND_ORBIT_ACTIVE_CAMERA = 1,
        CAMERA_TRANSITION_KIND_ORBIT_SHARED_TARGET = 2
    };
    struct CameraTransition *transition = &cam_trans_state;
    char                     orbit_reference_direction[16];
    char                     orbit_yaw_side_direction[16];
    char                     orbit_axis[16];
    unsigned char            requested_transition_kind;

    if (transition->mode == CAMERA_TRANSITION_PHASE_SEED_FROM_SOURCE) {
        requested_transition_kind = transition->requested_kind;
        if (requested_transition_kind == CAMERA_TRANSITION_KIND_BLEND) {
            qcopy(transition->blend.position, &source->pos);
            extract_matrix_rotation_quaternion(transition->blend.rotation, source);
        } else if (requested_transition_kind == CAMERA_TRANSITION_KIND_ORBIT_SHARED_TARGET) {
            qcopy(transition->orbit.saved_position, &source->pos);
            extract_matrix_rotation_quaternion(transition->orbit.saved_rotation, source);
            capture_camera_orientation();
        } else {
            struct Hero *orbit_reference_hero = &hero;

            fast_vec_normalize(
                orbit_reference_direction, &orbit_reference_hero->moby->rot_row0, 1.0f);
            fast_vec_normalize(
                orbit_yaw_side_direction, &orbit_reference_hero->moby->rot_row1, 1.0f);
            fast_vec_normalize(orbit_axis, &orbit_reference_hero->moby->rot_row2, 1.0f);
            /* GameCamera.current_camera, reached from this base. */
            compute_camera_angles(transition->orbit.angles,
                                  &source->pos,
                                  *(char **)((char *)transition - 0xF0) + 0x30,
                                  orbit_reference_direction,
                                  orbit_yaw_side_direction,
                                  orbit_axis);
            extract_matrix_rotation_quaternion(transition->orbit.rotation, source);
            qcopy(transition->orbit.saved_rotation, transition->orbit.rotation);
        }
    } else {
        requested_transition_kind = transition->requested_kind;
        if (requested_transition_kind == CAMERA_TRANSITION_KIND_ORBIT_SHARED_TARGET) {
            save_camera_vectors();
            capture_camera_orientation();
        } else if (requested_transition_kind == CAMERA_TRANSITION_KIND_ORBIT_ACTIVE_CAMERA) {
            save_camera_vectors();
            qcopy(transition->orbit.rotation, transition->orbit.saved_rotation);
        } else if (requested_transition_kind == CAMERA_TRANSITION_KIND_BLEND) {
            restore_camera_vectors();
        }
    }
    cam_trans_state_lui = 3;
    transition->kind = transition->requested_kind;
    if (transition->kind == CAMERA_TRANSITION_KIND_BLEND) {
        struct CameraBlend *blend = &transition->blend;

        blend->position_rate = blend->configured_position_rate;
        *(int *)&blend->position_progress = 0;
        qcopy(blend->from_position, blend->position);
        blend->rotation_progress = 0;
        blend->rotation_rate = blend->configured_rotation_rate;
        qcopy(blend->from_rotation, blend->rotation);
    } else {
        struct CameraOrbit *orbit = &transition->orbit;
        int                 configured_frame_count = ++orbit->length;

        orbit->frames = scale_game_frames(configured_frame_count);
        orbit->frame_step = 1.0f / convert_integer_to_float(orbit->frames);
    }
}

extern __typeof__(start_camera_blend) func_001EC8A0 __attribute__((alias("FUN_001ec8a0")));
