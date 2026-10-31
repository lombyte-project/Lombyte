/* Level 17 magnet_elevator. */
#include "types.h"
#include "rnc/math/vector.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/input/pad_globals.h"
#include "rnc/mobys/moby_class_id.h"

#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "asm.h"

#include "sda.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/overlay/quad.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/game/interpolated_record.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/hero/hero_switch_item.h"
#include "rnc/overlay/shared/mobys/test_path_segment_strict_crossing.h"
#include "rnc/overlay/shared/math/interpolation/compute_directed_angle_delta.h"
#include "rnc/overlay/shared/vendor/vendor_spring.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/overlay/shared/mobys/step_moby_motion.h"
#include "rnc/rendering/effects/fade_to_black.h"
#include "rnc/overlay/l17/hero/hero_set_state.h"
#include "rnc/rendering/view/project_to_screen.h"
#include "rnc/ui/help/force_help_message.h"

/* Moves a level-17 platform moby between its stops along its heading, with a looping sound. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002AA068.c: func_L17_002EA2C8), where it is exact; names translated to the US level program. */

extern char D_L17_001676C0[];
void        mark_moby_for_removal(struct Obj *obj) __asm__("FUN_0020c828");

#include "qcopy.h"

extern float normalize_angle(float) __asm__("FUN_L00_0025e310");
#define W(p, o) (*(int *)((char *)(p) + (o)))
#define P(p, o) (*(char **)((char *)(p) + (o)))
#define H(p, o) (*(short *)((char *)(p) + (o)))
#define B(p, o) (*(unsigned char *)((char *)(p) + (o)))
extern unsigned char D_0013E533_u[] __asm__("D_0013E533");

extern float fast_add_rotations(float, float);
extern void  set_camera_position(void *) __asm__("FUN_L00_002ea9d8");
extern void  set_camera_rotation(void *) __asm__("FUN_L00_002eaa30");

/* Updates the class-1380 lift: rises/lowers between two heights, blends the camera and pulses its colour by state. */
extern char  D_L17_001B0DB0[];
extern f32   advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("func_00213F38");
extern float random_angle_radians(void) __asm__("FUN_00213308");
extern int   fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern void  update_entity_matrix(char *, void *, void *, void *) __asm__("FUN_L00_00260738");
extern void  FUN_L17_002ee9c0_c(char *, char *, char *) __asm__("FUN_L17_002ee9c0");
extern void  FUN_L17_002eea70_c(char *) __asm__("FUN_L17_002eea70");

void magnet_elevator_update(struct Moby *moby) __asm__("FUN_L17_002ee2b0");

/* Moby class 1380 */
void magnet_elevator_update(struct Moby *moby) {
    enum {
        MAGNET_ELEVATOR_STATE_INITIALIZE = 0,
        MAGNET_ELEVATOR_STATE_BLEND_CAMERA = 1,
        MAGNET_ELEVATOR_STATE_MOVE_TO_FIRST_STOP_WITH_GROUND_CHECK = 2,
        MAGNET_ELEVATOR_STATE_MOVE_TO_SECOND_STOP_WITH_GROUND_CHECK = 3,
        MAGNET_ELEVATOR_STATE_MOVE_TO_FIRST_STOP = 4,
        MAGNET_ELEVATOR_STATE_MOVE_TO_SECOND_STOP = 5,
        MAGNET_ELEVATOR_STATE_WAIT_AT_FIRST_STOP = 6,
        MAGNET_ELEVATOR_STATE_WAIT_AT_SECOND_STOP = 7,
        MAGNET_ELEVATOR_STATE_WAIT_FOR_HERO_TO_LEAVE_FIRST_STOP = 8,
        MAGNET_ELEVATOR_STATE_WAIT_FOR_HERO_TO_LEAVE_SECOND_STOP = 9
    };
    char *state = P(moby, 0x78);
    float delta[4];
    float rot[4];
    float camera_position_offset[4];
    float camera_position[4];
    float camera_rotation[4];

    fast_vec_scale(delta, &moby->pos, -1.0f);
    qcopy(rot, &moby->rot);
    if (W(state, 0x7C) == 0) {
        float f =
            1.0f -
            ((*(float *)((char *)(moby) + (0x18))) - (*(float *)((char *)(state) + (0x64)))) /
                ((*(float *)((char *)(state) + (0x60))) - (*(float *)((char *)(state) + (0x64))));
        if (f > 1.0f) {
            f = 1.0f;
        } else if (f < 0.0f) {
            f = 0.0f;
        }
        (*(float *)((char *)(moby) + (0x44))) = f * 3.1415927f;
        (*(float *)((char *)(moby) + (0x44))) = normalize_angle(f * 3.1415927f);
    }
    switch (B(moby, 0x20)) {
    case MAGNET_ELEVATOR_STATE_INITIALIZE:
        (*(float *)((char *)(state) + (0x78))) = random_angle_radians();
        W(state, 0x6C) = -1;
        if (W(state, 0x80) == -1 || **(int **)(D_L17_001B0DB0 + W(state, 0x80) * 4) != 0xB) {
            mark_moby_for_removal(moby);
            return;
        }
        B(moby, 0x20) = 6;
        break;
    case MAGNET_ELEVATOR_STATE_BLEND_CAMERA: {
        float previous_camera_blend_progress = (*(float *)((char *)(state) + (0x70)));
        advance_accelerated_scalar((float *)(state + 0x70),
                                   (float *)(state + 0x74),
                                   1.0f,
                                   frame_time_sq * 4.0f,
                                   frame_time_sq * 4.0f,
                                   frame_time * 4.0f);
        if ((*(float *)((char *)(state) + (0x70))) < 1.0f) {
            float t = (1.0f - (*(float *)((char *)(state) + (0x70)))) /
                      (1.0f - previous_camera_blend_progress);
            char *camera_pose_base;
            FUN_L17_002ee9c0_c(moby, (char *)camera_position, (char *)camera_rotation);
            fast_vec_sub(camera_position_offset, D_L17_001676C0, camera_position);
            camera_pose_base = D_L17_001676C0 - 0x140;
            fast_vec_scale(camera_position_offset, camera_position_offset, t);
            fast_vec_add(camera_position, camera_position, camera_position_offset);
            camera_rotation[1] = fast_add_rotations(
                fast_subtract_rotations((*(float *)((char *)(camera_pose_base) + (0x154))),
                                        camera_rotation[1]) *
                    t,
                camera_rotation[1]);
            camera_rotation[2] = fast_add_rotations(
                fast_subtract_rotations((*(float *)((char *)(camera_pose_base) + (0x158))),
                                        camera_rotation[2]) *
                    t,
                camera_rotation[2]);
            set_camera_position(camera_position);
            set_camera_rotation(camera_rotation);
        } else {
            unsigned char s = B(moby, 0xBC);
            B(moby, 0xBC) = 1;
            B(moby, 0x20) = s;
        }
        break;
    }
    case MAGNET_ELEVATOR_STATE_MOVE_TO_FIRST_STOP_WITH_GROUND_CHECK:
    case MAGNET_ELEVATOR_STATE_MOVE_TO_SECOND_STOP_WITH_GROUND_CHECK:
        FUN_L17_002eea70_c(moby);
    case MAGNET_ELEVATOR_STATE_MOVE_TO_FIRST_STOP:
    case MAGNET_ELEVATOR_STATE_MOVE_TO_SECOND_STOP: {
        float target;
        int   s;
        W(moby, 0x90) = fast_tween_color(W(moby, 0x90), 0x80808040, 0.1f);
        if (B(moby, 0x20) == 2 && P(D_0013E533_u, 0x1119) != moby) {
            B(moby, 0x20) = 4;
        }
        if (B(moby, 0x20) == 3 && P(D_0013E533_u, 0x1119) != moby) {
            B(moby, 0x20) = 5;
        }
        if (!voice_is_playing(moby, W(state, 0x6C))) {
            W(state, 0x6C) = allocate_voice_for_target_entry(0, 4, (int)moby);
        }
        if (B(moby, 0x20) == 2 || B(moby, 0x20) == 4) {
            target = (*(float *)((char *)(state) + (0x60)));
        } else {
            target = (*(float *)((char *)(state) + (0x64)));
        }
        advance_accelerated_scalar(&moby->pos.z,
                                   (float *)(state + 0x68),
                                   target,
                                   frame_time_sq * 0.3f,
                                   frame_time_sq * 0.3f,
                                   frame_time * 3.0f);
        if (absolute_float((*(float *)((char *)(moby) + (0x18))) - target) == 0.0f) {
            s = B(moby, 0x20);
            if (s == 4) {
                B(moby, 0x20) = 6;
            } else if (s == 5) {
                B(moby, 0x20) = 7;
            } else if (s == 2) {
                B(moby, 0x20) = 8;
            } else {
                B(moby, 0x20) = 9;
            }
            if (B(moby, 0xBC) != 0) {
                hero_set_state(HERO_STATE_IDLE, 0);
            }
            if (voice_is_playing(moby, W(state, 0x6C))) {
                release_voice_slot(W(state, 0x6C));
                W(state, 0x6C) = -1;
            }
        }
        break;
    }
    case MAGNET_ELEVATOR_STATE_WAIT_FOR_HERO_TO_LEAVE_FIRST_STOP:
    case MAGNET_ELEVATOR_STATE_WAIT_FOR_HERO_TO_LEAVE_SECOND_STOP:
        if (voice_is_playing(moby, W(state, 0x6C))) {
            release_voice_slot(W(state, 0x6C));
            W(state, 0x6C) = -1;
        }
        if (P(D_0013E533_u, 0x1119) != moby) {
            if (B(moby, 0x20) == 9) {
                B(moby, 0x20) = 7;
            } else {
                B(moby, 0x20) = 6;
            }
        }
        break;
    case MAGNET_ELEVATOR_STATE_WAIT_AT_FIRST_STOP:
    case MAGNET_ELEVATOR_STATE_WAIT_AT_SECOND_STOP: {
        int   i;
        char *hero_state;
        (*(float *)((char *)(state) + (0x78))) =
            fast_add_rotations((*(float *)((char *)(state) + (0x78))), frame_time * 6.2831855f);
        i = truncate_float_to_s32((fast_sin((*(float *)((char *)(state) + (0x78)))) * 4.0f - 3.0f) *
                                  128.0f);
        if (i > 0x80) {
            i = 0x80;
        } else if (i < 0x20) {
            i = 0x20;
        }
        {
            int t = (i << 8) | 0x80000000;
            W(moby, 0x90) = (i << 16) | t | i;
        }
        hero_state = (char *)((char *)&hero);
        if (H(hero_state, 0x308) != 0 || W(state, 0x7C) == 0) {
            if (P(hero_state, 0x2FC) == moby && H(hero_state, 0x30E) == 0 &&
                vector_distance_xy(hero_state + 0x80, &moby->pos) < 0.5f) {
                allocate_voice_for_target_entry(1, 0, (int)moby);
                if (B(moby, 0x20) == 7) {
                    B(moby, 0x20) = 2;
                } else {
                    B(moby, 0x20) = 3;
                }
            } else if (B(moby, 0x20) == 6) {
                char *p = (char *)(&hero.motion.pos);
                float d = vector_distance_xy(&moby->pos, p);
                float e = absolute_float((*(float *)((char *)(p) + (0x258))) -
                                         (*(float *)((char *)(state) + (0x64))));
                if (e < absolute_float((*(float *)((char *)(state) + (0x60))) -
                                       (*(float *)((char *)(state) + (0x64)))) *
                            0.5f &&
                    d < 32.0f && d > 2.0f) {
                    B(moby, 0xBC) = 0;
                    B(moby, 0x20) = 3;
                    W(state, 0x68) = 0;
                }
            } else if (B(moby, 0x20) == 7) {
                char *p = (char *)(&hero.motion.pos);
                float d = vector_distance_xy(&moby->pos, p);
                float e = absolute_float((*(float *)((char *)(p) + (0x258))) -
                                         (*(float *)((char *)(state) + (0x60))));
                if (e < absolute_float((*(float *)((char *)(state) + (0x60))) -
                                       (*(float *)((char *)(state) + (0x64)))) *
                            0.5f &&
                    d < 32.0f && d > 2.0f) {
                    B(moby, 0xBC) = 0;
                    B(moby, 0x20) = 2;
                    W(state, 0x68) = 0;
                }
            }
        }
        break;
    }
    }
    fast_vec_add(delta, delta, &moby->pos);
    update_entity_matrix(state + 0x20, delta, rot, &moby->rot);
}

/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002AA068.c: func_L17_002F04D0), where it is exact; names translated to the US level program. */

void build_magnet_elevator_camera_pose(struct Moby *moby,
                                       char        *camera_position,
                                       char        *state) __asm__("FUN_L17_002ee9c0");

void build_magnet_elevator_camera_pose(struct Moby *moby, char *camera_position, char *state) {
    fast_vec_sub(camera_position, D_L17_001676C0, &moby->pos);
    set_vector_length_xy(camera_position, camera_position, 2.5f);
    *(float *)(camera_position + 8) = 1.5f;
    fast_vec_add(camera_position, camera_position, &moby->pos);
    clear_u64_value(state);
    *(float *)(state + 8) =
        fast_arc_tan(hero.motion.body_center.f[0] - *(float *)camera_position,
                     hero.motion.body_center.f[1] - *(float *)(camera_position + 4));
    *(int *)(state + 4) = 0;
}

extern void transform_vector_by_basis(void *, void *, void *);
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002AA068.c: func_L17_002F0580), where it is exact; names translated to the US level program. */

/* The 11 path points (loop i = 10..0, 16-byte stride) in the elevator's
   frame; transform_vector_by_basis loads each one as a quadword (lqc2). */
extern Vec4 magnet_elevator_local_path_points[11] __asm__("D_L17_001D9F70");

void rebuild_magnet_elevator_world_path(struct Moby *moby) __asm__("FUN_L17_002eea70");

void rebuild_magnet_elevator_world_path(struct Moby *moby) {
    char *state = (char *)moby->pvars;
    if (hero.unk308 && hero.ground_moby == moby) {
        char *world_path_point = *(char **)(D_L17_001B0DB0 + *(int *)(state + 0x80) * 4) + 0x10;
        char *point = &moby->rot_row0;
        int   i;
        for (i = 10; i >= 0; i--) {
            transform_vector_by_basis(
                world_path_point, &magnet_elevator_local_path_points[10 - i], point);
            fast_vec_add(world_path_point, world_path_point, &moby->pos);
            *(float *)(world_path_point + 0xC) = 1.0f;
            world_path_point += 0x10;
        }
        hero.path_constrain_pending = *(unsigned short *)(state + 0x80);
    }
}
