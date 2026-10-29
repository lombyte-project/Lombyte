/* Level 17 robot_dog: chase and aim, its stun and suction methods, and its glow quads. */
#include "types.h"
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
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/rendering/view/project_to_screen.h"
#include "rnc/ui/help/force_help_message.h"

/* Robot-dog state byte (+0x20) shared by its update helpers. */
enum RobotDogState {
    ROBOT_DOG_STATE_INITIALIZE = 0x0,
    ROBOT_DOG_STATE_WAIT_FOR_DETECTION_TRIGGER = 0x1,
    ROBOT_DOG_STATE_SELECT_RESPONSE_ON_ANIMATION_COMPLETION = 0x2,
    ROBOT_DOG_STATE_WAIT_FOR_IDLE_ANIMATION_COMPLETION = 0x3,
    ROBOT_DOG_STATE_FALL_UNTIL_LANDING = 0x4,
    ROBOT_DOG_STATE_TRACK_TARGET_FACING = 0x5,
    ROBOT_DOG_STATE_WAIT_FOR_CHASE_TRIGGER = 0x6,
    ROBOT_DOG_STATE_WAIT_FOR_CHASE_TRIGGER_FAST_PULSE = 0x7,
    ROBOT_DOG_STATE_TURN_TOWARD_TARGET = 0x8,
    ROBOT_DOG_STATE_APPROACH_TARGET = 0x9,
    ROBOT_DOG_STATE_ATTACK_TARGET = 0xA,
    ROBOT_DOG_STATE_RETURN_HOME = 0xB,
    ROBOT_DOG_STATE_RECOVER_THEN_REINITIALIZE = 0xC,
    ROBOT_DOG_STATE_FALL_THEN_REMOVE = 0xF
};

/* Moves a level-17 platform moby between its stops along its heading, with a looping sound. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002AA068.c: func_L17_002EA2C8), where it is exact; names translated to the US level program. */

extern char D_L17_001676C0[];
void        mark_moby_for_removal(struct Obj *obj) __asm__("FUN_0020c828");

#include "qcopy.h"

extern float         random_float_between(float, float) __asm__("FUN_002132a8");
extern int           random_integer_below(int) __asm__("FUN_00213260");
extern unsigned char D_0013E533[];

extern char D_L17_001B0DB0[];
/* UpdateMoby for class 1382: hides or shows the moby by its clip volumes, then runs its state machine. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002AA068.c: func_L17_002F0678), where it is exact; names translated to the US level program. */

typedef struct {
    char           pad0[0x20];
    float          f20;
    short          s24;
    char           pad26[3];
    unsigned char  b29;
    char           pad2A[0x58 - 0x2A];
    unsigned char  b58;
    char           pad59;
    unsigned char  b5A;
    char           pad5B[0xC8 - 0x5B];
    short          sC8;
    char           padCA[0xD0 - 0xCA];
    void          *pD0;
    char           padD4[0x120 - 0xD4];
    char           a120[0x180 - 0x120];
    float          v180[4];
    char           pad190[0x1C0 - 0x190];
    unsigned char *target;
    char           pad1C4[0x1D0 - 0x1C4];
    int            path;
    float          f1D4;
    float          speed;
    float          yawvel;
    short          t1E0;
    short          t1E2;
    unsigned char  mode;
    unsigned char  b1E5;
    char           pad1E6[2];
    int            i1E8;
    float          f1EC;
    OvlQuad        home;
    char           pad200[0x230 - 0x200];
    char           a230[0x2B0 - 0x230];
    int            c2B0;
    int            c2B4;
    int            c2B8;
    int            c2BC;
} Vars1382;

extern float scale_time(float) __asm__("FUN_001f96b0");
extern int   point_in_path_xy(void *, void *, int) __asm__("FUN_L00_00259740");
extern int   FUN_L00_002dc9f0(void *, void *);
extern int   FUN_L17_002efd48_c(char *moby, float *target, float speed) __asm__("FUN_L17_002efd48");
short        D_L17_00162200 = 0x909;
extern void *FUN_L00_00263fd8(char  *src,
                              int    cls,
                              float *pos,
                              void  *mat,
                              int    a8,
                              int    a9,
                              float  scale,
                              float *v10,
                              float *v11,
                              float *v12);
extern void  FUN_L00_00257470(void *, int, int);
extern void  FUN_L00_00258278(char *moby, float *vel, float target, float k, float d, float max);
extern void  FUN_L00_0025a120(void *);
extern void
     spawn_moby_break_fragments(void *, int, int, int, int, int, int) __asm__("FUN_L00_00263e30");
void update_robot_dog_hit_reaction_and_target_detection(
    void *moby_v, int clip_volume_suppressed) __asm__("FUN_L17_002efa48");

void update_robot_dog_hit_reaction_and_target_detection(void *moby_v,
                                                        int   arg) __asm__("FUN_L17_002efa48");
void update_robot_dog_aim_joint(struct Moby *moby) __asm__("FUN_L17_002f00a0");

void update_robot_dog_glow_effects(struct Moby *moby) __asm__("FUN_L17_002f0210");

void blend_moby_animation(void *arg0, int arg1, int arg2, int arg3) __asm__("FUN_00212f90");

extern float D_L17_0015F580_macro[] __asm__("D_L17_0015F580") MACRO_ADDR;
void         robot_dog_update(unsigned char *m) __asm__("FUN_L17_002eeb68");

void robot_dog_update(unsigned char *robot_dog_moby) {
    Vars1382    *robot_dog_vars;
    struct Moby *facing_target_moby;
    int          clip_volume_suppressed;
    int          clip_volume_id_or_animation_value;
    int          secondary_clip_volume_id;
    float        heading_vector[4];
    float        pos[4];

    clip_volume_suppressed = 0;
    robot_dog_vars = ((Vars1382 *)MOBY(robot_dog_moby)->pvars);
    if (robot_dog_moby[0x20] != 12) {
        clip_volume_id_or_animation_value = robot_dog_vars->c2B0;
        if (clip_volume_id_or_animation_value != -1) {
            clip_volume_suppressed =
                is_point_inside_clip_volume(D_L17_001676C0, clip_volume_id_or_animation_value) == 0;
            secondary_clip_volume_id = robot_dog_vars->c2B4;
            if (secondary_clip_volume_id != -1) {
                clip_volume_suppressed &=
                    is_point_inside_clip_volume(D_L17_001676C0, secondary_clip_volume_id) == 0;
            }
        } else if (robot_dog_vars->c2B8 != -1) {
            clip_volume_suppressed =
                is_point_inside_clip_volume(D_L17_001676C0, clip_volume_id_or_animation_value);
            secondary_clip_volume_id = robot_dog_vars->c2BC;
            if (secondary_clip_volume_id != -1) {
                clip_volume_suppressed |=
                    is_point_inside_clip_volume(D_L17_001676C0, secondary_clip_volume_id);
            }
        }
    }
    update_robot_dog_hit_reaction_and_target_detection(robot_dog_moby, clip_volume_suppressed);
    if (clip_volume_suppressed != 0 && robot_dog_moby[0x20] != 15) {
        robot_dog_moby[0x31] = 0;
        MOBY(robot_dog_moby)->flags |= 1;
        return;
    }
    robot_dog_moby[0x31] = 1;
    MOBY(robot_dog_moby)->flags &= 0xFFFE;
    update_robot_dog_aim_joint(robot_dog_moby);
    update_robot_dog_glow_effects(robot_dog_moby);
    facing_target_moby = robot_dog_vars->target;
    manipulator_set_scale(robot_dog_vars->a230, 2.1f);
    if (robot_dog_moby[0x31] != 0 && fast_vec_dist(robot_dog_moby + 0x10, D_L17_001676C0) < 29.0f) {
        FUN_L00_0025a120(robot_dog_moby);
        robot_dog_moby[0x7F] = 0x17;
    }
    switch (robot_dog_moby[0x20]) {
    case ROBOT_DOG_STATE_INITIALIZE:
        MOBY(robot_dog_moby)->flags |= 0x1000;
        qcopy(&robot_dog_vars->home, robot_dog_moby + 0x10);
        robot_dog_vars->b58 = 8;
        robot_dog_vars->b5A = 7;
        robot_dog_vars->b29 = 0;
        robot_dog_vars->s24 = 1;
        robot_dog_vars->f20 = 1.0f;
        robot_dog_vars->pD0 = &D_L17_00162200;
        robot_dog_vars->f1EC = random_float_between(frame_time * 7.2f, frame_time * 8.7f);
        robot_dog_moby[0x20] = 1;
        blend_moby_animation(
            robot_dog_moby,
            0,
            random_integer_below(*(
                unsigned char *)(*(char **)(((char *)MOBY(robot_dog_moby)->pclass) + 0x48) + 0x10)),
            0);
        if (robot_dog_vars->i1E8 == -1) {
            robot_dog_vars->i1E8 = robot_dog_vars->path;
        }
        break;
    case ROBOT_DOG_STATE_RECOVER_THEN_REINITIALIZE:
        if (FUN_L00_002dc9f0(robot_dog_moby, robot_dog_vars->a120)) {
            robot_dog_moby[0x20] = 0;
            robot_dog_vars->sC8 = 0;
        }
        break;
    case ROBOT_DOG_STATE_WAIT_FOR_DETECTION_TRIGGER:
        if (robot_dog_vars->mode != 0 && random_integer_below(0x1F) == 0) {
            robot_dog_moby[0x20] = 2;
            if (robot_dog_moby[0x53] != 1) {
                blend_moby_animation(robot_dog_moby, 1, 0, scale_game_frames(10));
            }
        }
        break;
    case ROBOT_DOG_STATE_SELECT_RESPONSE_ON_ANIMATION_COMPLETION:
        if (robot_dog_moby[0x70] & 2) {
            int s = robot_dog_vars->mode;
            if (s == 2) {
                if (random_integer_below(3)) {
                    if (robot_dog_moby[0x53] != s) {
                        blend_moby_animation(robot_dog_moby, 2, 0, scale_game_frames(10));
                    }
                } else if (D_0013E533[0x2EC1] != 3) {
                    robot_dog_moby[0x20] = 7;
                    if (robot_dog_moby[0x53] != 8) {
                        blend_moby_animation(robot_dog_moby, 8, 0, scale_game_frames(10));
                    }
                } else {
                    robot_dog_moby[0x20] = 6;
                    if (robot_dog_moby[0x53] != 7) {
                        blend_moby_animation(robot_dog_moby, 7, 0, scale_game_frames(10));
                    }
                }
            } else {
                robot_dog_moby[0x20] = 5;
                if (robot_dog_moby[0x53] != 3) {
                    blend_moby_animation(robot_dog_moby, 3, 0, scale_game_frames(10));
                }
                robot_dog_vars->t1E0 = scale_game_frames(120);
                robot_dog_vars->t1E2 = scale_game_frames(120);
            }
        }
        break;
    case ROBOT_DOG_STATE_TRACK_TARGET_FACING:
        if (facing_target_moby != 0) {
            float  turn;
            float  ang;
            float *yaw = (float *)&MOBY(robot_dog_moby)->rot.z;
            int    mode;
            ang = fast_arc_tan(facing_target_moby->pos.x - MOBY(robot_dog_moby)->pos.x,
                               facing_target_moby->pos.y - MOBY(robot_dog_moby)->pos.y);
            turn = frame_time_sq * 4.712389f;
            approach_angle(yaw, &robot_dog_vars->yawvel, ang, turn, turn, frame_time * 3.4906585f);
            if (absolute_float(robot_dog_vars->yawvel) > frame_time * 0.08726646f) {
                if (robot_dog_vars->yawvel < 0.0f) {
                    if (robot_dog_moby[0x53] != 4) {
                        blend_moby_animation(robot_dog_moby, 4, 0, scale_game_frames(10));
                    }
                } else {
                    if (robot_dog_moby[0x53] != 5) {
                        blend_moby_animation(robot_dog_moby, 5, 0, scale_game_frames(10));
                    }
                }
            } else {
                if (robot_dog_moby[0x53] != 3) {
                    blend_moby_animation(robot_dog_moby, 3, 0, scale_game_frames(10));
                }
            }
            MOBY(robot_dog_moby)->anim_speed = 1.7f;
            mode = robot_dog_vars->mode;
            if (mode == 2 &&
                (fast_dec_timer_s16(&robot_dog_vars->t1E2) || D_0013E533[0x2EC1] != 3)) {
                if (random_integer_below(3) == 0) {
                    if (robot_dog_moby[0x53] != mode) {
                        blend_moby_animation(robot_dog_moby, 2, 0, scale_game_frames(10));
                    }
                } else if (D_0013E533[0x2EC1] != 3) {
                    robot_dog_moby[0x20] = 7;
                    if (robot_dog_moby[0x53] != 8) {
                        blend_moby_animation(robot_dog_moby, 8, 0, scale_game_frames(10));
                    }
                } else {
                    robot_dog_moby[0x20] = 6;
                    if (robot_dog_moby[0x53] != 7) {
                        blend_moby_animation(robot_dog_moby, 7, 0, scale_game_frames(10));
                    }
                }
            } else if (robot_dog_vars->t1E0 == 0 && robot_dog_vars->mode == 0) {
                robot_dog_moby[0x20] = 3;
                if (robot_dog_moby[0x53] != 0) {
                    clip_volume_id_or_animation_value = random_integer_below(*(
                        unsigned char *)(*(char **)(((char *)MOBY(robot_dog_moby)->pclass) + 0x48) +
                                         0x10));
                    blend_moby_animation(robot_dog_moby,
                                         0,
                                         clip_volume_id_or_animation_value,
                                         scale_game_frames(10));
                }
            }
        } else if (random_integer_below(0x1F) == 0) {
            robot_dog_moby[0x20] = 3;
            if (robot_dog_moby[0x53] != 0) {
                clip_volume_id_or_animation_value = random_integer_below(
                    *(unsigned char *)(*(char **)(((char *)MOBY(robot_dog_moby)->pclass) + 0x48) +
                                       0x10));
                blend_moby_animation(
                    robot_dog_moby, 0, clip_volume_id_or_animation_value, scale_game_frames(10));
            }
        }
        break;
    case ROBOT_DOG_STATE_WAIT_FOR_IDLE_ANIMATION_COMPLETION:
        if (robot_dog_moby[0x70] & 2) {
            robot_dog_moby[0x20] = 1;
            if (robot_dog_moby[0x53] != 0) {
                clip_volume_id_or_animation_value = random_integer_below(
                    *(unsigned char *)(*(char **)(((char *)MOBY(robot_dog_moby)->pclass) + 0x48) +
                                       0x10) -
                    1);
                blend_moby_animation(
                    robot_dog_moby, 0, clip_volume_id_or_animation_value, scale_game_frames(10));
            }
        }
        break;
    case ROBOT_DOG_STATE_WAIT_FOR_CHASE_TRIGGER:
    case ROBOT_DOG_STATE_WAIT_FOR_CHASE_TRIGGER_FAST_PULSE: {
        int s = robot_dog_vars->mode;
        if (s != 2) {
            if (random_integer_below(20) == 0) {
                robot_dog_moby[0x20] = 5;
                if (robot_dog_moby[0x53] != 3) {
                    blend_moby_animation(robot_dog_moby, 3, 0, scale_game_frames(10));
                }
                robot_dog_vars->t1E2 = scale_game_frames(120);
                robot_dog_vars->t1E0 = scale_game_frames(120);
            }
            break;
        }
        if (robot_dog_vars->f1D4 != 0.0f ||
            point_in_path_xy((&hero.motion.pos),
                             ((int **)D_L17_001B0DB0)[robot_dog_vars->path] + 4,
                             ((int **)D_L17_001B0DB0)[robot_dog_vars->path][0]) != 0) {
            if (random_integer_below(20) == 0) {
                robot_dog_moby[0x20] = 8;
                if (robot_dog_moby[0x53] != 1) {
                    blend_moby_animation(robot_dog_moby, 1, 0, scale_game_frames(10));
                }
            }
        } else if (robot_dog_moby[0x70] & 2) {
            if (random_integer_below(3) == 0) {
                if (robot_dog_moby[0x53] != s) {
                    blend_moby_animation(robot_dog_moby, 2, 0, scale_game_frames(10));
                }
            } else if (random_integer_below(7) == 0) {
                if (robot_dog_moby[0x53] != 8) {
                    blend_moby_animation(robot_dog_moby, 8, 0, scale_game_frames(10));
                }
            } else {
                if (robot_dog_moby[0x53] != 7) {
                    blend_moby_animation(robot_dog_moby, 7, 0, scale_game_frames(10));
                }
            }
        }
        break;
    }
    case ROBOT_DOG_STATE_TURN_TOWARD_TARGET: {
        float ang;
        ang = fast_arc_tan(robot_dog_vars->v180[0] - MOBY(robot_dog_moby)->pos.x,
                           robot_dog_vars->v180[1] - MOBY(robot_dog_moby)->pos.y);
        FUN_L00_00258278((char *)robot_dog_moby,
                         &robot_dog_vars->yawvel,
                         ang,
                         0.03f,
                         0.3f,
                         frame_time * 7.8539815f);
        if (robot_dog_moby[0x70] & 2) {
            if (robot_dog_vars->f1D4 == 0.0f) {
                if (point_in_path_xy((&hero.motion.pos),
                                     ((int **)D_L17_001B0DB0)[robot_dog_vars->path] + 4,
                                     ((int **)D_L17_001B0DB0)[robot_dog_vars->path][0]) == 0) {
                    robot_dog_moby[0x20] = 11;
                    if (robot_dog_moby[0x53] != 9) {
                        blend_moby_animation(robot_dog_moby, 9, 0, scale_game_frames(10));
                    }
                    break;
                }
            }
            robot_dog_moby[0x20] = 9;
            if (robot_dog_moby[0x53] != 9) {
                blend_moby_animation(robot_dog_moby, 9, 0, scale_game_frames(10));
            }
        }
        break;
    }
    case ROBOT_DOG_STATE_APPROACH_TARGET:
        FUN_L17_002efd48_c((char *)robot_dog_moby, robot_dog_vars->v180, robot_dog_vars->f1EC);
        MOBY(robot_dog_moby)->anim_speed = 1.8f;
        if (point_in_path_xy((&hero.motion.pos),
                             ((int **)D_L17_001B0DB0)[robot_dog_vars->path] + 4,
                             ((int **)D_L17_001B0DB0)[robot_dog_vars->path][0]) == 0 &&
            robot_dog_vars->f1D4 == 0.0f) {
            robot_dog_moby[0x20] = 11;
        } else if (vector_distance_xy(robot_dog_moby + 0x10, robot_dog_vars->v180) < 1.8f) {
            robot_dog_moby[0x20] = 10;
            if (robot_dog_moby[0x53] != 10) {
                blend_moby_animation(robot_dog_moby, 10, 5, scale_game_frames(10));
            }
        } else if (fast_vec_dist(robot_dog_vars->v180, &robot_dog_vars->home) > 32.0f ||
                   absolute_float(robot_dog_vars->v180[2] - MOBY(robot_dog_moby)->pos.z) > 3.0f) {
            robot_dog_moby[0x20] = 11;
        }
        break;
    case ROBOT_DOG_STATE_ATTACK_TARGET:
        if (robot_dog_moby[0x70] & 2) {
            robot_dog_moby[0x20] = 11;
            if (robot_dog_moby[0x53] != 9) {
                blend_moby_animation(robot_dog_moby, 9, 0, scale_game_frames(10));
            }
        } else if (absolute_float(compute_interpolated_record_value(robot_dog_moby) - 17.5f) <=
                       0.25f &&
                   absolute_float(robot_dog_vars->v180[2] - MOBY(robot_dog_moby)->pos.z) < 1.0f &&
                   fast_difference_between_rotations(
                       fast_arc_tan(robot_dog_vars->v180[0] - MOBY(robot_dog_moby)->pos.x,
                                    robot_dog_vars->v180[1] - MOBY(robot_dog_moby)->pos.y),
                       MOBY(robot_dog_moby)->rot.z) < 0.2617994f &&
                   robot_dog_vars->target != 0 &&
                   vector_distance_xy(robot_dog_moby + 0x10, robot_dog_vars->v180) < 2.75f) {
            heading_vector[0] = fast_cos(MOBY(robot_dog_moby)->rot.z) * 0.2f;
            heading_vector[1] = fast_sin(MOBY(robot_dog_moby)->rot.z) * 0.2f;
            heading_vector[2] = 0.0f;
            qcopy(pos, robot_dog_vars->v180);
            pos[2] += 0.75f;
            spawn_nearby_effect(
                (int)robot_dog_vars->target, (int)robot_dog_moby, 1, pos, heading_vector, 1.0f);
        }
        break;
    case ROBOT_DOG_STATE_RETURN_HOME: {
        float distance_to_home;
        float far;
        float k1;
        float k2;
        float frame_scaled_speed;
        FUN_L17_002efd48_c(
            (char *)robot_dog_moby, (float *)&robot_dog_vars->home, frame_time * 5.0f);
        MOBY(robot_dog_moby)->anim_speed = 1.35f;
        distance_to_home = vector_distance_xy(robot_dog_moby + 0x10, &robot_dog_vars->home);
        if ((distance_to_home < 1.4f && (robot_dog_moby[0x70] & 2)) || distance_to_home < 0.3f) {
            robot_dog_moby[0x20] = 5;
            if (robot_dog_moby[0x53] != 3) {
                blend_moby_animation(robot_dog_moby, 3, 0, scale_game_frames(10));
            }
            robot_dog_vars->t1E2 = scale_game_frames(120);
            robot_dog_vars->t1E0 = scale_game_frames(120);
        } else if (robot_dog_moby[0x70] & 2) {
            far = vector_distance_xy(robot_dog_moby + 0x10, robot_dog_vars->v180);
            k1 = scale_time(40.0f);
            k2 = scale_time(10.0f);
            frame_scaled_speed = robot_dog_vars->speed * frame_time;
            if (far < frame_scaled_speed * k1 * 0.5f + frame_scaled_speed * k2 + 1.5f) {
                if (point_in_path_xy((&hero.motion.pos),
                                     ((int **)D_L17_001B0DB0)[robot_dog_vars->path] + 4,
                                     ((int **)D_L17_001B0DB0)[robot_dog_vars->path][0]) != 0) {
                    robot_dog_moby[0x20] = 10;
                    if (robot_dog_moby[0x53] != 10) {
                        blend_moby_animation(robot_dog_moby, 10, 0, scale_game_frames(10));
                    }
                }
            }
        }
        break;
    }
    case ROBOT_DOG_STATE_FALL_UNTIL_LANDING:
        if (step_moby_motion(robot_dog_moby, robot_dog_vars->a120) & 0x60) {
            clear_u64_value(heading_vector);
            robot_dog_moby[0x20] = 5;
            if (robot_dog_moby[0x53] != 3) {
                blend_moby_animation(robot_dog_moby, 3, 0, scale_game_frames(10));
            }
            robot_dog_vars->t1E2 = scale_game_frames(120);
            robot_dog_vars->t1E0 = scale_game_frames(120);
        } else if (MOBY(robot_dog_moby)->pos.z < 2.0f) {
            FUN_L00_00257470(robot_dog_moby, 0, -1);
            mark_moby_for_removal(robot_dog_moby);
        }
        break;
    case ROBOT_DOG_STATE_FALL_THEN_REMOVE:
        MOBY(robot_dog_moby)->flags &= 0xEFFF;
        if (step_moby_motion(robot_dog_moby, robot_dog_vars->a120) & 0x60) {
            float *zero;
            clear_u64_value(heading_vector);
            zero = D_L17_0015F580_macro;
            cart_burst(robot_dog_moby, robot_dog_moby + 0x10, -1, 0.5f, 13.0f);
            FUN_L00_00263fd8((char *)robot_dog_moby,
                             0x6CE,
                             (float *)&MOBY(robot_dog_moby)->pos,
                             robot_dog_moby + 0x40,
                             0,
                             0,
                             0.0f,
                             zero,
                             zero,
                             zero);
            FUN_L00_00263fd8((char *)robot_dog_moby,
                             0x6CF,
                             (float *)&MOBY(robot_dog_moby)->pos,
                             robot_dog_moby + 0x40,
                             0,
                             0,
                             0.0f,
                             zero,
                             zero,
                             zero);
            spawn_moby_break_fragments(robot_dog_moby, 0x77D, 1, 0x77D, 1, 3, 2);
            FUN_L00_00257470(robot_dog_moby, 0, -1);
            mark_moby_for_removal(robot_dog_moby);
        } else if (MOBY(robot_dog_moby)->pos.z < 2.0f) {
            FUN_L00_00257470(robot_dog_moby, 0, -1);
            mark_moby_for_removal(robot_dog_moby);
        }
        break;
    }
}

/* Fleet turret/enemy state update: refreshes the hit reaction, fires the aim query and sets the lock flags. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F1558), where it is exact; names translated to the US level program. */

typedef struct {
    char          pad0[0x20];
    float         f20;
    char          pad24[0x14];
    int           i38;
    char          pad3C[0xD4];
    short         h110[3];
    char          b116;
    unsigned char b117;
    char          pad118[8];
    float         v120[4];
    float         f130;
    float         f134;
    float         f138;
    float         f13C;
    int           i140;
    int           i144;
    char          pad148[0x15];
    char          b15D;
    char          pad15E[0x12];
    float         f170;
    float         f174;
    char          pad178[8];
    float         v180[4];
    char          pad190[0x20];
    float         v1B0[4];
    char          pad1C0[0x14];
    float         f1D4;
    char          pad1D8[0xC];
    char          b1E4;
    char          pad1E5[0xB];
    float         f1F0;
    float         f1F4;
} Data;

typedef struct {
    char          pad0[0x10];
    float         pos[4];
    unsigned char state;
    char          pad21[0x27];
    float         f48;
    char          pad4C[0x2C];
    Data         *data;
    char          pad7C[0x28];
    unsigned char bA4;
    char          padA5;
    short         hA6;
} Moby_2F1558;

typedef struct {
    char         pad0[0x10];
    float        pos[4];
    Moby_2F1558 *moby;
} Hit_2F1558;

extern float random_float_between(float a, float b) __asm__("FUN_002132a8");
extern int   collision_line(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern int
hit_reaction(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_L00_0025a478");
extern int  FUN_L00_0025ff38(float, char *, void *, int, int, void *, int);
extern void trooper_knockback_arc(void *, float *, void *, void *) __asm__("FUN_L00_0025ab48");
extern void
trooper_start_knockback(float, void *, void *, int, int, int) __asm__("FUN_L00_0025c558");
extern void trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");
extern void update_float_countdown(float *) __asm__("FUN_L08_00211b08");

void update_robot_dog_hit_reaction_and_target_detection(void *moby_v, int clip_volume_suppressed) {
    Moby_2F1558 *moby = (Moby_2F1558 *)moby_v;
    Data        *moby_vars;
    Hit_2F1558  *hit_record;
    float        position_buffer[4];
    int          x;
    float        y;
    float        knockback_heading;
    int          line_query_clear;
    float        yaw;
    float        target_heading;

    moby_vars = moby->data;
    if (moby->state == 0) {
        return;
    }
    if (moby_vars->i38 != 0) {
        moby_vars->f1D4 = truncate_float_to_s32(scale_time(random_float_between(180.0f, 240.0f)));
        moby_vars->i38 = 0;
    }
    update_float_countdown(&moby_vars->f1D4);
    y = 0;
    hit_record = (Hit_2F1558 *)find_moby_hit(moby, 0x330000, 0);
    hit_reaction(moby, hit_record, &moby_vars->f20, 0, &x, &y, 0, 4);
    if (x != 1 && moby->state != 15) {
        moby_vars->f20 -= y;
        moby_vars->f130 = frame_time_sq * 42.0f;
        moby_vars->f134 = frame_time_sq * 24.0f;
        moby_vars->b15D = 0;
        moby_vars->i144 = 9;
        moby_vars->f13C = frame_time * 15.7f;
        moby_vars->f138 = frame_time * 19.0f;
        moby_vars->f170 = -1.0f;
        moby_vars->f174 = -1.0f;
        *(OvlQuad *)position_buffer = *(OvlQuad *)hit_record->pos;
        trooper_knockback_arc(
            position_buffer, &knockback_heading, &moby_vars->f138, &moby_vars->f13C);
        if (hit_record != 0 && hit_record->moby != 0 && hit_record->moby->hA6 == 0x63) {
            knockback_heading =
                fast_arc_tan(moby_vars->f1F0 - moby->pos[0], moby_vars->f1F4 - moby->pos[1]);
        }
        trooper_start_knockback(knockback_heading, moby, moby_vars->v120, 0xB, 1, 0);
        moby->state = 15;
        moby_vars->b117 = 0xF0;
        trooper_anim_update(moby, moby_vars->h110);
    }
    moby->bA4 = 0xFF;
    if (clip_volume_suppressed != 0) {
        return;
    }
    if (FUN_L00_0025ff38(128.0f, (char *)moby, moby_vars->v180, 0, 0, 0, 0) == 2) {
        return;
    }
    if (fast_vec_dist(moby->pos, moby_vars->v1B0) < 64.0f) {
        target_heading =
            fast_arc_tan(moby_vars->v180[0] - moby->pos[0], moby_vars->v180[1] - moby->pos[1]);
        yaw = fast_difference_between_rotations(moby->f48, target_heading);
        qcopy(position_buffer, moby->pos);
        position_buffer[2] += 1.0f;
        line_query_clear =
            collision_line(position_buffer, (&hero.motion.body_center), 2, 0, 0) == 0;
        moby_vars->b1E4 = line_query_clear;
        if (yaw < 1.0471976f) {
            moby_vars->b1E4 = line_query_clear << 1;
        }
    }
}
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F1858), where it is exact; names translated to the US level program. */

extern f32  approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
extern void FUN_L00_00258830(int, int, float, float, float, int);

int move_robot_dog_toward_position(struct Moby *moby,
                                   float       *target,
                                   float        speed) __asm__("FUN_L17_002efd48");

int move_robot_dog_toward_position(struct Moby *moby, float *target, float speed) {
    char  *robot_dog_motion_vars;
    float *yaw = &moby->rot.z;
    float  target_yaw;
    float  turn;
    float  movement_delta[3];
    robot_dog_motion_vars = (char *)moby->pvars;
    target_yaw = fast_arc_tan(target[0] - moby->pos.x, target[1] - moby->pos.y);
    turn = frame_time_sq * 12.566371f;
    approach_angle(yaw, (float *)(robot_dog_motion_vars + 0x1DC), target_yaw, turn, turn, frame_time * 7.3303828f);
    approach_value((float *)(robot_dog_motion_vars + 0x1D8), speed, frame_time_sq * 13.0f);
    movement_delta[0] = fast_cos(moby->rot.z) * *(float *)(robot_dog_motion_vars + 0x1D8);
    movement_delta[1] = fast_sin(moby->rot.z) * *(float *)(robot_dog_motion_vars + 0x1D8);
    movement_delta[2] = -(frame_time * 0.5f);
    FUN_L00_00258830((int)moby, (int)movement_delta, 0.5f, 0.5f, 0.0f, 0);
    return vector_distance_xy(&moby->pos, target) < 0.2f;
}

/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F19B8), where it is exact; names translated to the US level program. */

extern int process_moby_suck_cannon_suction(void *) __asm__("FUN_L00_002db8f8");

int enemy_stun_update(struct Moby *moby) __asm__("FUN_L17_002efea8");

int enemy_stun_update(struct Moby *moby) {
    int suction_status = process_moby_suck_cannon_suction(moby);
    if (suction_status) {
        moby->state = 12;
    } else if (moby->state == 12) {
        moby->state = 6;
        if (moby->prev_seq != 7) {
            int animation_blend_frames = scale_game_frames(10);
            blend_moby_animation(moby, 7, 0, animation_blend_frames);
        }
    }
    return suction_status;
}

extern int cancel_suck_cannon_suction(void *) __asm__("FUN_L00_002dbe20");

int cancel_suck_cannon_suction_and_set_state_12(struct Moby *moby) __asm__("FUN_L17_002f0040");

int cancel_suck_cannon_suction_and_set_state_12(struct Moby *moby) {
    int helper_result = cancel_suck_cannon_suction(moby);
    moby->state = 0xc;
    return helper_result;
}

extern float vector_length_xy(void *);

/* Aims the turret at the hero: yaw and pitch to him, clamped, become the eased part's targets. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F1BB0), where it is exact; names translated to the US level program. */

typedef struct {
    char  pad0[0x64];
    float pitch;
    float yaw;
} AimPart_1BB0;

typedef struct {
    char         pad0[0x230];
    AimPart_1BB0 part;
} AimData_1BB0;

void update_robot_dog_aim_joint(struct Moby *moby) {
    AimData_1BB0 *moby_vars = (AimData_1BB0 *)moby->pvars;
    float         aim_joint_smoothing_factor = 0.02f;
    float         speed = 0.3f;
    float         aim_origin[4];
    float         aim_delta[4];
    float         yaw;
    float         pitch;
    qcopy(aim_origin, &moby->pos);
    aim_origin[2] += 0.5f;
    fast_vec_sub(aim_delta, (&hero.motion.body_center), aim_origin);
    yaw = fast_subtract_rotations(fast_arc_tan(aim_delta[0], aim_delta[1]), moby->rot.z);
    pitch = -fast_arc_tan(vector_length_xy(aim_delta), aim_delta[2]);
    if (yaw > 1.0471976f) {
        yaw = 1.0471976f;
    } else if (yaw < -1.0471976f) {
        yaw = -1.0471976f;
    }
    if (pitch > 0.5235988f) {
        pitch = 0.5235988f;
    } else if (pitch < -0.5235988f) {
        pitch = -0.5235988f;
    }
    moby_vars->part.pitch = pitch;
    moby_vars->part.yaw = yaw;
    update_joint_animation(moby,
                           (char *)&moby_vars->part,
                           0,
                           aim_joint_smoothing_factor * frame_scale_sq,
                           speed * frame_scale_sq);
}

extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
void       draw_three_moby_effect_quads(struct Moby *effect_owner) __asm__("FUN_L17_002f0448");

void update_robot_dog_glow_effects(struct Moby *moby) {
    char *robot_dog_glow_vars;
    int   glow_pulse_period_frames;
    int   alpha;
    int   mode;
    int   i;
    int   x, y, z;
    int   r, g, b;
    float glow_pulse_sine;
    int   color;

    /* Period and base colour by state range; both ranges have the same values in this build. */
    if (moby->state < 16) {
        glow_pulse_period_frames = 0xAA;
        r = 0x32;
        g = 0xB4;
        b = 0x14;
    } else {
        glow_pulse_period_frames = 0xAA;
        r = 0x32;
        g = 0xB4;
        b = 0x14;
    }
    robot_dog_glow_vars = (char *)moby->pvars;
    glow_pulse_period_frames = scale_game_frames(glow_pulse_period_frames);
    alpha = 0x80;
    mode = 0;
    switch (moby->state) {
    case ROBOT_DOG_STATE_FALL_UNTIL_LANDING:
    case ROBOT_DOG_STATE_TRACK_TARGET_FACING:
    case ROBOT_DOG_STATE_WAIT_FOR_CHASE_TRIGGER:
        mode = 1;
        break;
    case ROBOT_DOG_STATE_WAIT_FOR_CHASE_TRIGGER_FAST_PULSE:
    case ROBOT_DOG_STATE_TURN_TOWARD_TARGET:
    case ROBOT_DOG_STATE_APPROACH_TARGET:
    case ROBOT_DOG_STATE_ATTACK_TARGET:
    case ROBOT_DOG_STATE_RETURN_HOME:
    case 13:
    case 14:
    case ROBOT_DOG_STATE_FALL_THEN_REMOVE:
        mode = 2;
        break;
    case ROBOT_DOG_STATE_RECOVER_THEN_REINITIALIZE:
        mode = -1;
        break;
    }
    if (mode >= 0) {
        if (mode == 2) {
            glow_pulse_period_frames = scale_game_frames(0x32);
        } else if (mode == 1) {
            glow_pulse_period_frames = scale_game_frames(0x5A);
        }
        glow_pulse_sine = fast_sin(((float)(D_L17_0015F5CC % glow_pulse_period_frames) /
                                    convert_integer_to_float(glow_pulse_period_frames)) *
                                       2.0f * 3.1415927f +
                                   -3.1415927f);
        x = truncate_float_to_s32(glow_pulse_sine * 20.0f);
        y = truncate_float_to_s32(glow_pulse_sine * 70.0f);
        z = truncate_float_to_s32(glow_pulse_sine * 10.0f);
        r += x;
        g += y;
        b += z;
        if (r > 255) {
            r = 255;
        }
        if (g > 255) {
            g = 255;
        }
        if (mode == 2) {
            color = (alpha << 24) | (b << 16) | ((r / 2) << 8) | g;
        } else if (mode != 0) {
            color = (alpha << 24) | (b << 16) | ((r / 2) << 8) | g;
        } else {
            color = (alpha << 24) | (b << 16) | (g << 8) | r;
        }
        moby->glow_color = fast_tween_color(moby->glow_color, color, 0.1f);
        for (i = 0; i < 3; i++) {
            moby_point_position(moby, i + 1, robot_dog_glow_vars + 0x200 + i * 16);
        }
        add_draw_callback(draw_three_moby_effect_quads, moby);
    }
}

extern void draw_offset_textured_effect_quad(void *, int, float, float) __asm__("FUN_L00_00263618");

/* Applies the same effect to three consecutive 16-byte slots of a moby's data, tagged with its colour word. */
void draw_three_moby_effect_quads(struct Moby *effect_owner) {
    int   effect_color = (effect_owner->glow_color & 0xFFFFFF) | 0x30000000;
    char *effect_position = (char *)effect_owner->pvars + 0x200;
    int   i;
    for (i = 2; i >= 0; i--) {
        draw_offset_textured_effect_quad(effect_position, effect_color, 0.2f, 0.08f);
        effect_position += 0x10;
    }
}
