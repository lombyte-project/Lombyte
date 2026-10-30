/* Shared moby class sand_mouse. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "rnc/data_labels.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/mobys/get_moby_target_info.h"
#include "rnc/overlay/shared/hero/set_hero_animation.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/l01/hero/hero_set_state.h"
#include "qcopy.h"
#include "rnc/overlay/level_save_state.h"
#include "qzero.h"
#include "rnc/overlay/quad.h"
#include "rnc/overlay/shared/enemies/sand_mouse_data/update_hover_moby_steering.h"
#include "rnc/overlay/shared/enemies/sand_mouse_data/sand_mouse_update.h"
#include "rnc/world/collision/collision_line.h"

typedef struct {
    float a[4];
} Vs __attribute__((aligned(16)));

struct DriftEmitter {
    s32 count; /* 0x00 */
    u8  pad4[0x2C];
    f32 spread; /* 0x30 */
};

typedef union {
    OvlQuad q;
    float   f[4];
} V4_w22c;

/* State for the parented moby.  The parent index is -1 when unattached. */
typedef int u128_2e3f20 __attribute__((mode(TI)));

typedef struct {
    float         target[4], vel[4], start[4], home[4];
    int           vol;
    char          pad44[4];
    unsigned char b48;
    char          pad49;
    short         s4A, timer, s4E;
    int           parent;
    char          pad54[0xC];
    int           t60, count;
    void         *tgt;
} Data;

typedef struct {
    int   unk0;
    float unk4, unk8, unkC, unk10, yaw_speed, unk18, unk1C, unk20, unk24, unk28, unk2C, unk30;
    int   unk34, unk38;
    float unk3C, unk40, unk44;
    int   unk48, unk4C;
    void *owner;
} Mover;

typedef struct {
    char          pad0[0x80];
    float         pos[4];
    char          pad90[0x10A0];
    struct Moby  *helper;
    char          pad1134[0x24];
    int           i1158;
    char          pad115C[0xF24];
    void         *p2080;
    char          pad2084[8];
    int           i208C;
    char          pad2090[0x14];
    unsigned char b20A4;
} Hero_2e3f20;

extern int D_0015ED84;

extern int
probe_vertical_ground_collision(Vs *p, int x, int y, float f, float g) __asm__("FUN_L00_002583f0");

extern float sand_mouse_follow_ground_probe_up_distance[] __asm__("D_L01_001DEB40");

extern struct DriftEmitter D_L01_001DEB38;
extern s32 move_moby_with_collision_integer_radius(void *, void *, s32, s32, f32, f32) __asm__("FUN_L00_00258ad0");

extern char D_L01_00174340_h[] __asm__("D_L01_001742C0");

extern void *select_sand_mouse_spawn_target(
    void *, void *, void *, float, float, float, float, float, float) __asm__("FUN_L01_002e32a8");
extern void
spawn_sandmouse_projectile(void *, void *, float, float, float, float) __asm__("FUN_L01_0030c898");

extern float sand_mouse_spawn_target_range_gp __asm__("D_L01_00161904") __attribute__((sda));
extern float sand_mouse_spawn_target_yaw_limit_degrees_gp __asm__("D_L01_00161908")
    __attribute__((sda));
extern float sand_mouse_spawn_target_pitch_limit_degrees_gp __asm__("D_L01_0016190C")
    __attribute__((sda));
extern float sand_mouse_spawn_offset_toward_target_distance_gp __asm__("D_L01_00161910")
    __attribute__((sda));
extern float sand_mouse_spawn_and_fallback_aim_height_offset_gp __asm__("D_L01_00161914")
    __attribute__((sda));
extern int sand_mouse_projectile_spawn_interval_frames_gp __asm__("D_L01_00161900")
    __attribute__((sda));

extern void FUN_L00_00258278(struct Moby *, f32, f32 *, f32, f32, f32);
extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
extern int  rand(void) __asm__("FUN_001160d8");

extern char sand_mouse_already_completed_message[] __asm__("D_L01_0020AE80");
extern char sand_mouse_missing_start_house_message[] __asm__("D_L01_0020AEA0");
extern void debug_print(char *) __asm__("FUN_001e93b0");
extern void initialize_mover_2e3f20(void *) __asm__("FUN_L01_0026d930");
extern void
rig_probe_setup(void *, int, int *, float *, float *, int *) __asm__("FUN_L01_00263a28");
extern void
approach_w(struct Moby *, float, float *, float, float, float) __asm__("FUN_L00_00258278");
extern int  step_mover_2e3f20(void *, void *, float *, float *) __asm__("FUN_L00_00259028");
extern void add_random_offset_to_vector_xyz(void *, float) __asm__("FUN_L00_0025f8e0");
extern void spawn_particle_2e3f20(
    float *, int, int, int, int, float *, float, float, float) __asm__("FUN_L00_00272060");
extern void jet_animate(void *, float) __asm__("FUN_L00_00259fe8");
extern void compute_sand_mouse_approach_target(int, void *) __asm__("FUN_L01_002e30b8");
extern void sand_mouse_compute_target(char *, char *) __asm__("FUN_L01_002e3110");
extern s32  update_sand_mouse_fall_motion(void *, struct Moby *) __asm__("FUN_L01_002e3208");
extern void update_sand_mouse_targeted_spawning(char *, char *) __asm__("FUN_L01_002e35a8");
extern void update_hover_moby_steering(struct Moby *, char *) __asm__("FUN_L01_002e3830");
extern unsigned char D_0013D4C5 NOT_SDA;

/* Ported from rac1-decomp (src/overlays/shared/vendor_002B90A8.c: func_L01_002E4430), where it is exact; names translated to the US level program. */

void compute_sand_mouse_approach_target(int unused, void *out) __asm__("FUN_L01_002e30b8");

void compute_sand_mouse_approach_target(int unused, void *out) {
    char  scratch[16];
    char *base = D_0013E533 + 0xE1D;
    fast_vec_normalize(scratch, *(char **)(base + 0x2080) + 0xD0, 1.0f);
    fast_vec_add(out, scratch, base + 0x80);
}

/* Builds a camera-relative transform for a moby and steps its height. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B90A8.c: func_L01_002E4488), where it is exact; names translated to the US level program. */

void sand_mouse_compute_target(char *following_moby,
                               char *follow_movement_state) __asm__("FUN_L01_002e3110");

void sand_mouse_compute_target(char *following_moby, char *follow_movement_state) {
    Vs           scaled_hero_follow_offset;
    Vs           unit_hero_follow_offset;
    struct Hero *hero_context = &hero;
    fast_vec_normalize(&scaled_hero_follow_offset, ((char *)hero_context->moby) + 0xD0, 1.1f);
    fast_vec_normalize(&unit_hero_follow_offset, ((char *)hero_context->moby) + 0xE0, 1.0f);
    fast_vec_add(follow_movement_state, &scaled_hero_follow_offset, &unit_hero_follow_offset);
    fast_vec_add(follow_movement_state, follow_movement_state, &hero_context->motion.pos);
    if (*(int *)(follow_movement_state + 0x68) == 0) {
        if (probe_vertical_ground_collision((Vs *)follow_movement_state,
                                            (int)following_moby,
                                            0,
                                            sand_mouse_follow_ground_probe_up_distance[0],
                                            2.0f)) {
            *(float *)(follow_movement_state + 8) = D_L01_001742E8;
        }
    } else {
        *(float *)(follow_movement_state + 8) = *(float *)(follow_movement_state + 8) + 1.0f;
    }
}

/* shrinks and sinks the moby, then hands it to the drift emitter */

s32 update_sand_mouse_fall_motion(void        *ctx,
                                  struct Moby *movement_state) __asm__("FUN_L01_002e3208");

s32 update_sand_mouse_fall_motion(void *ctx, struct Moby *movement_state) {
    f32 shrink;

    movement_state->pos.z -= frame_time_sq * 29.7f;
    shrink = frame_scale * -0.050000012f + 1.0f;
    movement_state->pos.x *= shrink;
    movement_state->pos.y *= shrink;
    return move_moby_with_collision_integer_radius(
        ctx, &movement_state->pos, D_L01_001DEB38.count, 0, 0.3f, D_L01_001DEB38.spread);
}

/* Picks the best lock-on target for moby from the visible-moby list: a living, drawn class-5 enemy within range whose direction from `from`
 * is inside the yaw/pitch cone (or the wider near cone when close), scored by angle error and distance,
 * with a clear line of sight (or the blocker is the same kind of enemy). */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B90A8.c: func_L01_002E4620), where it is exact; names translated to the US level program. */

void *select_sand_mouse_spawn_target(void *moby,
                                     void *from,
                                     void *reference_rotation,
                                     float yawMax,
                                     float pitchMax,
                                     float range,
                                     float near,
                                     float nearYaw,
                                     float nearPitch) __asm__("FUN_L01_002e32a8");

void *select_sand_mouse_spawn_target(void *moby,
                                     void *from,
                                     void *reference_rotation,
                                     float yawMax,
                                     float pitchMax,
                                     float range,
                                     float near,
                                     float nearYaw,

                                     float nearPitch) {
    float tp[4];
    char *best = 0;
    float bestScore = 1.0e9f;
    char *candidate_moby;
    for (candidate_moby = D_L01_0015FFE4; candidate_moby != 0;
         candidate_moby = ((char *)MOBY(candidate_moby)->next)) {
        char        *hs = D_L01_00174340_h;
        struct Hero *pl = &hero;
        float       *hp;
        int          candidate_class;
        float        candidate_distance, score, pitch_error_squared;
        if (MOBY(candidate_moby)->draw_dist == 0)
            continue;
        hp = get_moby_target_info(candidate_moby);
        if (!(MOBY(candidate_moby)->flags & 0x1000) || candidate_moby == 0 ||
            ((char *)MOBY(candidate_moby)->pclass) == 0)
            continue;
        candidate_class = *(short *)(((char *)MOBY(candidate_moby)->pclass) + 0x46);
        if (candidate_class != 5 || hp == 0 || !(0.0f < *hp))
            continue;
        candidate_distance = fast_vec_dist(from, candidate_moby + 0x10);
        if (!(candidate_distance < range))
            continue;
        qcopy(tp, candidate_moby + 0x10);
        tp[2] += 0.4f;
        score = fast_difference_between_rotations(
            ((float *)reference_rotation)[2],
            fast_arc_tan(tp[0] - ((float *)from)[0], tp[1] - ((float *)from)[1]));
        score = score * score;
        if (!(score < yawMax * yawMax)) {
            if (!(candidate_distance < near) || !(score < nearYaw * nearYaw))
                continue;
        }
        pitch_error_squared = fast_difference_between_rotations(
            ((float *)reference_rotation)[1],
            fast_arc_tan(candidate_distance, tp[2] - ((float *)from)[2]));
        pitch_error_squared = pitch_error_squared * pitch_error_squared;
        if (!(pitch_error_squared < pitchMax * pitchMax)) {
            if (!(candidate_distance < near) || !(pitch_error_squared < nearPitch * nearPitch))
                continue;
        }
        score *= pitch_error_squared;
        score *= square_root(fast_vec_dist(from, tp));
        if (!(score < bestScore))
            continue;
        if (collision_line(from, tp, 0, moby, 0)) {
            char *hit = *(char **)(hs + 0x18);
            if (hit == 0 || hit == ((char *)pl->moby) || hit == 0 || *(char **)(hit + 0x24) == 0 ||
                *(short *)(*(char **)(hit + 0x24) + 0x46) != candidate_class) {
                continue;
            }
        }
        bestScore = score;
        best = candidate_moby;
    }
    return best;
}

void update_sand_mouse_targeted_spawning(char *moby, char *state) __asm__("FUN_L01_002e35a8");

void update_sand_mouse_targeted_spawning(char *moby, char *state) {
    V4_w22c spawn_position;
    V4_w22c target_aim_position;
    V4_w22c spawn_offset;
    char   *target_moby;
    float   aim_yaw, aim_pitch, initial_yaw, initial_pitch, target_yaw, spawn_to_target_distance;

    if (fast_dec_timer_s16(state + 0x4E) == 0)
        return;
    qzero(&spawn_position);
    fast_vec_add(&spawn_position, &spawn_position, moby + 0x10);
    spawn_position.f[2] = spawn_position.f[2] + sand_mouse_spawn_and_fallback_aim_height_offset_gp;
    target_moby =
        select_sand_mouse_spawn_target(moby,
                                       &spawn_position,
                                       moby + 0x40,
                                       sand_mouse_spawn_target_yaw_limit_degrees_gp * DEG_TO_RAD,
                                       sand_mouse_spawn_target_pitch_limit_degrees_gp * DEG_TO_RAD,
                                       sand_mouse_spawn_target_range_gp,
                                       2.0f,
                                       3.1415927f,
                                       3.1415927f);
    if (target_moby != 0) {
        *(char **)(state + 0x68) = target_moby;
        if (hero.motion.pos.f[2] + 1.1f < MOBY(moby)->pos.z) {
            target_yaw = fast_arc_tan(MOBY(target_moby)->pos.x - MOBY(moby)->pos.x,
                                      MOBY(target_moby)->pos.y - MOBY(moby)->pos.y);
            if (fast_difference_between_rotations(MOBY(moby)->rot.z, target_yaw) < 0.5235988f) {
                if (cutscene_mode != 2) {
                    char *target_aim_data;
                    qcopy(&target_aim_position, target_moby + 0x10);
                    target_aim_data = get_moby_target_info(target_moby);
                    if (target_aim_data != 0)
                        target_aim_position.f[2] =
                            target_aim_position.f[2] + *(float *)(target_aim_data + 0x10);
                    else
                        target_aim_position.f[2] =
                            target_aim_position.f[2] +
                            sand_mouse_spawn_and_fallback_aim_height_offset_gp;
                    fast_vec_sub(&spawn_offset, &target_aim_position, &spawn_position);
                    fast_vec_normalize(&spawn_offset,
                                       &spawn_offset,
                                       sand_mouse_spawn_offset_toward_target_distance_gp);
                    fast_vec_add(&spawn_position, &spawn_position, &spawn_offset);
                    aim_yaw = fast_arc_tan(target_aim_position.f[0] - spawn_position.f[0],
                                           target_aim_position.f[1] - spawn_position.f[1]);
                    spawn_to_target_distance =
                        vector_distance_xy(&spawn_position, &target_aim_position);
                    aim_pitch = -fast_arc_tan(spawn_to_target_distance,
                                              target_aim_position.f[2] - spawn_position.f[2]);
                    initial_yaw = fast_arc_tan(target_aim_position.f[0] - spawn_position.f[0],
                                               target_aim_position.f[1] - spawn_position.f[1]);
                    spawn_to_target_distance =
                        vector_distance_xy(&spawn_position, &target_aim_position);
                    initial_pitch = -fast_arc_tan(spawn_to_target_distance,
                                                  target_aim_position.f[2] - spawn_position.f[2]);
                    spawn_sandmouse_projectile(
                        moby, &spawn_position, aim_yaw, aim_pitch, initial_yaw, initial_pitch);
                    *(short *)(state + 0x4E) =
                        scale_game_frames(sand_mouse_projectile_spawn_interval_frames_gp);
                    *(int *)(state + 0x64) = *(int *)(state + 0x64) + 1;
                }
            }
        }
    } else {
        *(int *)(state + 0x68) = 0;
    }
}

void update_hover_moby_steering(struct Moby *moby,
                                char        *movement_state) __asm__("FUN_L01_002e3830");

void update_hover_moby_steering(struct Moby *moby, char *movement_state) {
    enum {
        HOVER_FACE_GOAL_AND_WAIT = 0,
        HOVER_MOVE_TOWARD_GOAL = 2
    };
    Vec4         velocity_adjustment;
    f32          reference_distance, direction_yaw, movement_magnitude, ground, near;
    s32          hit;
    s16          blocked_frames;
    struct Moby *target;

    fast_vec_sub(&velocity_adjustment, movement_state, &moby->pos);
    if (moby->state == 6)
        reference_distance = vector_distance_xy(&moby->pos, movement_state + 0x30);
    else
        reference_distance = vector_distance_xy(&moby->pos, &hero.motion.pos);
    vector_distance_xy(&moby->pos, movement_state);

    switch (moby->command) {
    case HOVER_FACE_GOAL_AND_WAIT:
        direction_yaw = fast_arc_tan(*(f32 *)(movement_state + 0) - moby->pos.x,
                                     *(f32 *)(movement_state + 4) - moby->pos.y);
        FUN_L00_00258278(moby,
                         direction_yaw,
                         &D_L01_001DEB4C,
                         frame_time_sq * 11.519173f,
                         frame_time_sq * 3.1415927f,
                         frame_time * 11.519173f);
        if (moby->anim_flags & 2) {
            moby->anim_speed = 1.0f;
            if (moby->prev_seq != rand() % 3) {
                int seq = rand() % 3;
                blend_moby_animation(moby, seq, 0, scale_game_frames(20));
            }
        }
        if (1.3f < reference_distance) {
            if (moby->prev_seq != 5)
                blend_moby_animation(moby, 5, 0, scale_game_frames(10));
            moby->command = 2;
        }
        return;
    case HOVER_MOVE_TOWARD_GOAL:
    case 3:
        break;
    default:
        goto inactive;
    }

    target = *(struct Moby **)(movement_state + 0x68);
    if (target == 0) {
        direction_yaw = fast_arc_tan(*(f32 *)(movement_state + 0) - moby->pos.x,
                                     *(f32 *)(movement_state + 4) - moby->pos.y);
        FUN_L00_00258278(moby,
                         direction_yaw,
                         &D_L01_001DEB4C,
                         frame_time_sq * 11.519173f,
                         frame_time_sq * 3.1415927f,
                         frame_time * 11.519173f);
    } else {
        direction_yaw = fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y);
        FUN_L00_00258278(moby,
                         direction_yaw,
                         &D_L01_001DEB4C,
                         hover_target_yaw_spring_strength_degrees_gp * DEG_TO_RAD * frame_time_sq,
                         hover_target_yaw_damping_degrees_gp * DEG_TO_RAD * frame_time_sq,
                         hover_target_yaw_turn_rate_limit_degrees_gp * DEG_TO_RAD * frame_time);
    }
    fast_vec_scale(&velocity_adjustment, &velocity_adjustment, frame_scale * 0.04f);
    fast_vec_sub(&velocity_adjustment, &velocity_adjustment, ((f32 *)(movement_state + 0x10)));
    if (1.3f < reference_distance) {
        fast_vec_scale(&velocity_adjustment, &velocity_adjustment, frame_scale_sq * 0.13f);
    } else {
        fast_vec_scale(&velocity_adjustment, &velocity_adjustment, frame_scale_sq * 0.25f);
    }
    velocity_adjustment.f[2] *= frame_scale * -0.100000024f + 1.0f;
    fast_vec_add(
        ((f32 *)(movement_state + 0x10)), ((f32 *)(movement_state + 0x10)), &velocity_adjustment);
    if (moby->state == 6)
        clamp_vector_length(
            ((f32 *)(movement_state + 0x10)), ((f32 *)(movement_state + 0x10)), frame_time * 2.7f);
    hit = move_moby_with_collision_integer_radius(moby,
                                                  ((f32 *)(movement_state + 0x10)),
                                                  D_L01_001DEB38.count,
                                                  0,
                                                  1.0f,
                                                  D_L01_001DEB38.spread);
    movement_magnitude = fast_vec_length(((f32 *)(movement_state + 0x10)));
    if (hit != 0) {
        if (movement_magnitude < 0.005f) {
            blocked_frames = ++*(s16 *)(movement_state + 0x4a);
            if (scale_game_frames(20) < blocked_frames && moby->visible == 0) {
                qcopy(&moby->pos, movement_state);
                *(s16 *)(movement_state + 0x4a) = 0;
            }
        }
        ground = probe_ground_height(&moby->pos, 0, 0.5f);
        if ((hit & 2) || moby->pos.z - ground < hover_ground_animation_height_threshold_gp) {
            if (movement_magnitude < 0.005f) {
                if (moby->prev_seq >= 3 && moby->prev_seq != rand() % 3) {
                    int seq = rand() % 3;
                    blend_moby_animation(moby, seq, 0, scale_game_frames(10));
                }
            } else {
                goto animation3;
            }
        } else {
            if (moby->prev_seq != 5)
                blend_moby_animation(moby, 5, 0, scale_game_frames(7));
        }
    } else {
        ground = probe_ground_height(&moby->pos, 0, 0.5f);
        if (moby->pos.z - ground < hover_ground_animation_height_threshold_gp) {
        animation3:
            if (moby->prev_seq != 3)
                blend_moby_animation(moby, 3, 0, scale_game_frames(11));
        } else {
            if (moby->prev_seq != 5)
                blend_moby_animation(moby, 5, 0, scale_game_frames(7));
            *(s16 *)(movement_state + 0x4a) = 0;
        }
    }
collision_done:
    if (moby->state == 6 || !(reference_distance < 1.1f))
        return;
    near = 1.1f - reference_distance;
    direction_yaw =
        fast_arc_tan(hero.motion.pos.f[0] - moby->pos.x, hero.motion.pos.f[1] - moby->pos.y);
    ((f32 *)(movement_state + 0x10))[0] -= near * fast_cos(direction_yaw) * (frame_scale_sq * 0.1f);
    ((f32 *)(movement_state + 0x10))[1] -= near * fast_sin(direction_yaw) * (frame_scale_sq * 0.1f);
    return;
inactive:
    moby->command = 0;
    if (1.3f < reference_distance)
        moby->command = 2;
}
#define hero_2e3f20  ((Hero_2e3f20 *)&hero)
#define mover_2e3f20 (*(Mover *)&D_L01_001DEB38)

void sand_mouse_update(struct Moby *moby) __asm__("FUN_L01_002e3f20");

void sand_mouse_update(struct Moby *moby) {
    enum {
        SAND_MOUSE_INITIALIZE = 0,
        SAND_MOUSE_WAIT_FOR_HERO_IN_VOLUME = 1,
        SAND_MOUSE_WAIT_FOR_PARENT_READY = 2,
        SAND_MOUSE_MOVE_TOWARD_HERO = 3,
        SAND_MOUSE_BEGIN_FOLLOW_HERO = 4,
        SAND_MOUSE_FOLLOW_HERO = 5,
        SAND_MOUSE_FADE_OUT_AND_RESET_HOME = 6,
        SAND_MOUSE_DAMP_VELOCITY_BEFORE_RETURN = 19,
        SAND_MOUSE_RETURN_TO_PARENT = 20
    };
    Data *sand_mouse_vars;
    float va[4];
    float vb[4];
    float desired_facing_yaw;

    if (moby == 0) {
        return;
    }
    sand_mouse_vars = *(Data **)&moby->pvars;
    if (sand_mouse_vars == 0) {
        return;
    }
    if (sand_mouse_vars->parent != -1) {
        if (moby->state >= 5) {
            ((Data *)((struct Moby *)ents)[sand_mouse_vars->parent].pvars)->target[0] =
                fast_arc_tan(moby->pos.x - ((struct Moby *)ents)[sand_mouse_vars->parent].pos.x,
                             moby->pos.y - ((struct Moby *)ents)[sand_mouse_vars->parent].pos.y);
        } else {
            ((Data *)((struct Moby *)ents)[sand_mouse_vars->parent].pvars)->target[0] =
                fast_arc_tan(
                    hero_2e3f20->pos[0] - ((struct Moby *)ents)[sand_mouse_vars->parent].pos.x,
                    hero_2e3f20->pos[1] - ((struct Moby *)ents)[sand_mouse_vars->parent].pos.y);
        }
        if (sand_mouse_vars->parent != -1) {
            *(int *)&((struct Moby *)ents)[sand_mouse_vars->parent].collision = 0;
        }
    }
    switch (moby->state) {
    case SAND_MOUSE_INITIALIZE:
        qzero(sand_mouse_vars->target);
        qzero(sand_mouse_vars->vel);
        sand_mouse_vars->s4A = 0;
        sand_mouse_vars->timer = 0;
        if (moby->prev_seq != 0) {
            blend_moby_animation(moby, 0, 0, scale_game_frames(10));
        }
        moby->update_dist = 0xFF;
        moby->draw_dist = 0xFF;
        if (sand_mouse_vars->parent != -1) {
            unsigned short id;
            qcopy(((float *)&moby->pos),
                  ((float *)&((struct Moby *)ents)[sand_mouse_vars->parent].pos));
            moby->pos.z = moby->pos.z + 0.02f;
            if (l01_save_state.cleared[(short)(id = moby->save_id)] != 0 ||
                (*(int *)((char *)D_0014C190_257218 + (((short)id >> 5) * 4 + (D_0015ED84 << 8))) >>
                 (id & 0x1F)) &
                    1) {
                debug_print(sand_mouse_already_completed_message);
                mark_moby_for_removal(moby);
                return;
            }
        } else {
            debug_print(sand_mouse_missing_start_house_message);
            mark_moby_for_removal(moby);
            return;
        }
        qcopy(sand_mouse_vars->start, ((float *)&moby->pos));
        qcopy(sand_mouse_vars->home, ((float *)&moby->pos));
        initialize_mover_2e3f20((char *)&mover_2e3f20);
        {
            int   kind;
            float collision_probe_extent;
            int   flag;
            rig_probe_setup(moby, 0, &kind, va, &collision_probe_extent, &flag);
            mover_2e3f20.yaw_speed = frame_scale * 0.4f;
            mover_2e3f20.unk8 = 0.15f;
            mover_2e3f20.unkC = 0.15f;
            mover_2e3f20.unk10 = frame_scale * 0.01f;
            mover_2e3f20.unk24 = frame_time * 2.7f;
            mover_2e3f20.unk1C = frame_scale * 0.01f;
            mover_2e3f20.unk0 = truncate_float_to_s32(collision_probe_extent * 1024.0f) + 1;
            mover_2e3f20.unk38 |= 8;
            mover_2e3f20.unk4 = 0.3f;
            mover_2e3f20.unk30 = 0.005f;
            mover_2e3f20.unk3C = frame_time_sq * 11.519173f;
            mover_2e3f20.unk40 = frame_time_sq * 3.1415927f;
            mover_2e3f20.unk44 = frame_time * 11.519173f;
            mover_2e3f20.unk48 = scale_game_frames(4);
            mover_2e3f20.unk28 = 3.1415927f;
        }
        moby->state = 1;
        moby->draw_dist = 0;
        break;
    case SAND_MOUSE_WAIT_FOR_HERO_IN_VOLUME:
        if (D_0013D4C5 != 0 && hero_2e3f20->i1158 == 5) {
            sand_mouse_vars->count = 0;
            sand_mouse_vars->t60 = scale_game_frames(sand_mouse_follow_duration_frames_gp);
            if (moby->visible != 0 && (moby->anim_flags & 2)) {
                if (moby->prev_seq != rand() % 3) {
                    blend_moby_animation(moby, rand() % 3, 0, scale_game_frames(10));
                }
            }
            if (sand_mouse_vars->parent != -1) {
                if (is_point_inside_clip_volume(hero_2e3f20->pos, sand_mouse_vars->vol)) {
                    ((struct Moby *)ents)[sand_mouse_vars->parent].command = 3;
                } else {
                    ((struct Moby *)ents)[sand_mouse_vars->parent].command = 1;
                }
            }
            if (hero_2e3f20->helper != 0 && hero_2e3f20->i1158 == 5) {
                desired_facing_yaw = fast_arc_tan(hero_2e3f20->pos[0] - moby->pos.x,
                                                  hero_2e3f20->pos[1] - moby->pos.y);
                approach_w(moby,
                           desired_facing_yaw,
                           &mover_2e3f20.yaw_speed,
                           frame_time_sq * 11.519173f,
                           frame_time_sq * 3.1415927f,
                           frame_time * 11.519173f);
                qcopy(va, ((float *)&((struct Moby *)ents)[sand_mouse_vars->parent].pos));
                va[2] = va[2] + 0.5f;
                qcopy(vb, hero_2e3f20->pos);
                vb[2] = vb[2] + 0.2f;
                if (is_point_inside_clip_volume(hero_2e3f20->pos, sand_mouse_vars->vol)) {
                    if (!collision_line(va, vb, 0, moby, 0) ||
                        D_L01_001742D8 == hero_2e3f20->p2080) {
                        compute_sand_mouse_approach_target((int)moby, sand_mouse_vars);
                        if (hero_2e3f20->helper != 0 && hero_2e3f20->helper->oclass == 0x1B1 &&
                            hero_2e3f20->helper->state != 0xFE &&
                            hero_2e3f20->helper->state != 0xFD) {
                            hero_2e3f20->helper->state = 1;
                        }
                        *(void **)((char *)&mover_2e3f20 + 0x50) = moby;
                        l01_hero_set_state(0x1F, 0);
                        set_hero_animation(0, 0, (float)scale_game_frames(6));
                        if (sand_mouse_vars->parent != -1) {
                            ((struct Moby *)ents)[sand_mouse_vars->parent].command = 5;
                            moby->state = 2;
                        } else {
                            moby->draw_dist = 0xFF;
                            moby->state = 3;
                        }
                    }
                }
            }
        }
        break;
    case SAND_MOUSE_WAIT_FOR_PARENT_READY:
        if (sand_mouse_vars->parent != -1) {
            if (((struct Moby *)ents)[sand_mouse_vars->parent].state == 5) {
                moby->state = 3;
                sand_mouse_vars->timer = 0;
                moby->draw_dist = 0xFF;
            }
        } else {
            sand_mouse_vars->timer = 0;
            moby->draw_dist = 0xFF;
            moby->state = 3;
        }
        break;
    case SAND_MOUSE_MOVE_TOWARD_HERO:
        compute_sand_mouse_approach_target((int)moby, sand_mouse_vars);
        desired_facing_yaw = fast_arc_tan(sand_mouse_vars->target[0] - moby->pos.x,
                                          sand_mouse_vars->target[1] - moby->pos.y);
        approach_w(moby,
                   desired_facing_yaw,
                   &mover_2e3f20.yaw_speed,
                   frame_time_sq * 11.519173f,
                   frame_time_sq * 3.1415927f,
                   frame_time * 11.519173f);
        if ((step_mover_2e3f20(moby, &mover_2e3f20, sand_mouse_vars->target, sand_mouse_vars->vel) &
             6) ||
            vector_distance_xy(((float *)&moby->pos), hero_2e3f20->pos) < 1.0f) {
            qzero(sand_mouse_vars->vel);
            moby->state = 4;
            sand_mouse_vars->timer = scale_game_frames(0x78);
        }
        if (vector_distance_xy(((float *)&moby->pos), sand_mouse_vars->home) > 0.4f &&
            sand_mouse_vars->timer == 0) {
            *(float *)((char *)&mover_2e3f20 + 4) = 0.7f;
            if (moby->prev_seq != 3) {
                blend_moby_animation(moby, 3, 0, scale_game_frames(3));
            }
        }
        break;
    case SAND_MOUSE_BEGIN_FOLLOW_HERO:
        l01_hero_set_state(0, 1);
        moby->flags &= 0xFFBE;
        if (sand_mouse_vars->parent != -1) {
            ((struct Moby *)ents)[sand_mouse_vars->parent].flags &= 0xFFBE;
        }
        moby->state = 5;
        if (hero_2e3f20->helper != 0 && hero_2e3f20->helper->oclass == 0x1B1 &&
            hero_2e3f20->helper->state != 0xFE && hero_2e3f20->helper->state != 0xFD) {
            sand_mouse_vars->b48 = 0xFF;
            hero_2e3f20->helper->state = 2;
        }
        break;
    case SAND_MOUSE_FOLLOW_HERO:
        if (hero_2e3f20->helper != 0 && hero_2e3f20->i1158 == 5 && hero_2e3f20->i208C != 0x16 &&
            hero_2e3f20->i208C != 0x14 && hero_2e3f20->b20A4 == 0 &&
            !fast_dec_timer(&sand_mouse_vars->t60) &&
            sand_mouse_vars->count < sand_mouse_follow_projectile_count_limit_gp) {
            sand_mouse_compute_target((char *)moby, (char *)sand_mouse_vars);
            update_hover_moby_steering(moby, sand_mouse_vars);
        } else {
            if (moby->prev_seq != 5) {
                blend_moby_animation(moby, 5, 0, scale_game_frames(10));
            }
            qzero(sand_mouse_vars->vel);
            sand_mouse_vars->timer = sand_mouse_fade_duration_frames_gp;
            moby->state = 6;
            if (sand_mouse_vars->parent != -1) {
                ((struct Moby *)ents)[sand_mouse_vars->parent].command = 5;
            }
        }
        update_sand_mouse_targeted_spawning((char *)moby, (char *)sand_mouse_vars);
        break;
    case SAND_MOUSE_FADE_OUT_AND_RESET_HOME:
        if (fast_dec_timer_s16(&sand_mouse_vars->timer)) {
            moby->alpha = 0x80;
            qcopy(((float *)&moby->pos), sand_mouse_vars->home);
            qcopy(sand_mouse_vars->target, sand_mouse_vars->home);
            qzero(sand_mouse_vars->vel);
            moby->state = 0x13;
            moby->flags &= 0xFDFF;
        } else {
            int   cr;
            int   cg;
            int   cb;
            int   col;
            int   particle_random_remainder;
            int   particle_random_integer;
            float particle_random_float;
            *(u128_2e3f20 *)vb = 0;
            qcopy(va, ((float *)&moby->pos));
            add_random_offset_to_vector_xyz(va, sand_mouse_fade_particle_position_jitter_gp);
            cr = (rand() + 0x30) & 0x3F;
            cg = (rand() + 0x20) & 0x3F;
            cb = rand() & 0x2F;
            col = cr | (cg << 8) | (cb << 16) | (sand_mouse_fade_particle_color_alpha_gp << 24);
            particle_random_remainder = rand() % 2;
            if (sand_mouse_vars->timer & 1) {
                particle_random_remainder = -particle_random_remainder;
            }
            particle_random_integer =
                random_int_between(sand_mouse_fade_particle_random_integer_min_gp,
                                   sand_mouse_fade_particle_random_integer_max_gp);
            particle_random_float =
                random_float_between(sand_mouse_fade_particle_random_float_range_start_gp,
                                     sand_mouse_fade_particle_random_float_range_end_gp);
            spawn_particle_2e3f20(va,
                                  particle_random_integer,
                                  col,
                                  2,
                                  particle_random_remainder,
                                  vb,
                                  particle_random_float,
                                  0.001f,
                                  sand_mouse_fade_particle_time_squared_coefficient_gp *
                                      frame_time_sq);
            moby->flags |= 0x200;
            moby->alpha -= 0x80 / sand_mouse_fade_duration_frames_gp;
            moby->rot.z = fast_add_rotations(moby->rot.z, frame_time * 25.132742f);
        }
        break;
    case SAND_MOUSE_DAMP_VELOCITY_BEFORE_RETURN: {
        float velocity_damping_factor = frame_scale * -0.050000012f + 1.0f;
        sand_mouse_vars->vel[0] = sand_mouse_vars->vel[0] * velocity_damping_factor;
        sand_mouse_vars->vel[1] = sand_mouse_vars->vel[1] * velocity_damping_factor;
    }
        if (update_sand_mouse_fall_motion(moby, sand_mouse_vars) & 2) {
            qzero(sand_mouse_vars->vel);
            qcopy(sand_mouse_vars->target,
                  ((float *)&((struct Moby *)ents)[sand_mouse_vars->parent].pos));
            moby->state = 0x14;
        }
        break;
    case SAND_MOUSE_RETURN_TO_PARENT: {
        float z;
        int   mover_result_flags;
        mover_2e3f20.owner = 0;
        desired_facing_yaw = fast_arc_tan(sand_mouse_vars->target[0] - moby->pos.x,
                                          sand_mouse_vars->target[1] - moby->pos.y);
        approach_w(moby,
                   desired_facing_yaw,
                   &mover_2e3f20.yaw_speed,
                   frame_time_sq * 11.519173f,
                   frame_time_sq * 3.1415927f,
                   frame_time * 11.519173f);
        z = moby->pos.z;
        mover_result_flags =
            step_mover_2e3f20(moby, &mover_2e3f20, sand_mouse_vars->target, sand_mouse_vars->vel);
        moby->pos.z = z;
        if (mover_result_flags & 4) {
            if (moby->prev_seq != 0) {
                blend_moby_animation(moby, 0, 0, scale_game_frames(6));
            }
            ((struct Moby *)ents)[sand_mouse_vars->parent].command = 1;
            moby->state = 1;
            moby->draw_dist = 0;
        }
    } break;
    case 21:
        break;
    }
    jet_animate(moby, 0.3f);
    if (sand_mouse_vars->parent != -1) {
        *(int *)&((struct Moby *)ents)[sand_mouse_vars->parent].collision =
            ((struct Moby *)ents)[sand_mouse_vars->parent].pclass->collision;
    }
}

struct DriftEmitter D_L02_001D3110 = {0};

float               D_L02_0016189C = 2e+01f;
float               D_L02_001618A0 = 1.8e+02f;
float               D_L02_001618A4 = 9e+01f;
float               D_L02_001618A8 = 0.3f;
float               D_L02_001618AC = 0.3f;
int                 D_L02_00161898 = 6;
struct DriftEmitter D_L04_001D2E10 = {0};

float               D_L04_0016190C = 2e+01f;
float               D_L04_00161910 = 1.8e+02f;
float               D_L04_00161914 = 9e+01f;
float               D_L04_00161918 = 0.3f;
float               D_L04_0016191C = 0.3f;
int                 D_L04_00161908 = 6;
struct DriftEmitter D_L11_001D3550 = {0};

int sand_mouse_projectile_spawn_interval_frames_gp __asm__("D_L01_00161900") = 6;
