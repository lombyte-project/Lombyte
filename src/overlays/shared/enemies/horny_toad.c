/* Shared moby class horny_toad. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/mobys/step_moby_motion.h"
#include "rnc/overlay/shared/mobys/update_object_wander_movement.h"
#include "rnc/math/random.h"
#include "rnc/math/interpolation.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/rendering/debug/debug_menu.h"
#include "rnc/overlay/quad.h"

typedef struct Moby Moby;
typedef float       ToadVec[4] __attribute__((aligned(16)));

typedef struct {
    u8    pad0[0x24];
    float unk24;
} ToadClass;

typedef struct {
    int     count;
    int     pad4[3];
    ToadVec pts[1];
} ToadPath;

typedef struct {
    u8      pad0[0x29];
    u8      unk29;
    u8      pad2A[0x58 - 0x2A];
    u8      unk58;
    u8      pad59;
    u8      unk5A;
    u8      pad5B[0x70 - 0x5B];
    u8      unk70[0x138 - 0x70];
    s16     unk138;
    u8      pad13A[0x140 - 0x13A];
    void   *unk140;
    u8      pad144[0x180 - 0x144];
    ToadVec unk180;
    u8      pad190[0x1C0 - 0x190];
    Moby   *target;
    int     unk1C4;
    u8      pad1C8[8];
    ToadVec home;
    int     unk1E0;
    int     unk1E4;
    int     path;
    int     follow_moby;
    int     pickup_moby;
    float   turn_speed;
    float   unk1F8;
    float   unk1FC;
    u8      unk200[0x40];
    ToadVec unk240;
    float   unk250;
    float   unk254;
    float   unk258;
    float   unk25C;
    u8      pad260[0x14];
    s16     path_point;
    s16     unk276;
} ToadVars;

typedef struct {
    char pad[0x78];
    s32  f;
} T6;

typedef struct {
    u8 pad[0x454];
    u8 killed[1]; /* per spawn id: spawn slot + 2 */
} KillFlags;

/* Row 0 holds the count; the keyed rows follow. */
typedef struct {
    s32 count;
    u8  pad04[8];
    f32 key;
} KeyRow;

typedef struct {
    s32 table; /* -1, or an index into D_L00_001B04B0 */
    f32 key;
} KeyVars;

extern s32   random_integer_below(s32) __asm__("FUN_00213260");
extern float D_0015ED70;
extern u8   *D_L00_001B04B0_2d0538[] __asm__("D_L00_001B04B0") NOT_SDA;

extern ToadPath *toad_paths[] __asm__("D_L00_001B04B0");

extern void  FUN_L00_002d5160_2d4610(void *) __asm__("FUN_L00_002d5160");
extern void  FUN_L00_002d5830_2d4610(void *) __asm__("FUN_L00_002d5830");
extern void  FUN_L00_0025a120_2d4610(void *) __asm__("FUN_L00_0025a120");
extern void  FUN_L00_00263ac8_2d4610(float, int, int, unsigned char *) __asm__("FUN_L00_00263ac8");
extern int   FUN_L00_002dc9f0_2d4610(void *, void *) __asm__("FUN_L00_002dc9f0");
extern float approach_angle(void *, float, void *, float, float, float) __asm__("FUN_L00_0025be00");
extern float
move_horny_toad_toward_position(unsigned char *m, float *t, float vel) __asm__("FUN_L00_002d54c8");
extern int  is_value_within_interpolated_window(void *, float) __asm__("FUN_00214cc8");
extern void collect_mobies_in_sphere_at_moby_point(
    void *, float, float, float, int, int, int, int, int) __asm__("FUN_L00_002599e8");
extern int test_path_segment_strict_crossing(int, void *, void *) __asm__("FUN_L00_00261968");
extern int random_integer_below(int) __asm__("FUN_00213260");
extern void
spawn_color_swapped_explosion_burst(void *, void *, int, float, float) __asm__("FUN_L00_0025f3e8");
extern float probe_ground_height(void *, int, float) __asm__("FUN_00213508");
extern void
resolve_point_path_segment_xy_clearance(int, float, void *, void *) __asm__("FUN_L00_00261d78");

void         horny_toad_update(Moby *moby) __asm__("FUN_L00_002d4610");
extern float D_L00_0016196C __attribute__((sda));
extern float horny_toad_target_approach_distance_threshold_gp __asm__("D_L00_00161980")
    __attribute__((sda));
extern float horny_toad_target_animation_start_index_gp __asm__("D_L00_00161984")
    __attribute__((sda));
extern short D_L00_00161960 __attribute__((sda));

extern s32            D_L00_0015F5C4_2d5160 __asm__("D_L00_0015F5C4");
extern f32            D_L00_00161970_gp __asm__("D_L00_00161970") __attribute__((sda));
extern f32            D_L00_00161974_gp __asm__("D_L00_00161974") __attribute__((sda));
extern f32            D_L00_00161978_gp __asm__("D_L00_00161978") __attribute__((sda));
extern f32            D_L00_0016197C_gp __asm__("D_L00_0016197C") __attribute__((sda));
extern f32            D_L00_00161988_gp __asm__("D_L00_00161988") __attribute__((sda));
extern f32            D_L00_0016198C_gp __asm__("D_L00_0016198C") __attribute__((sda));
extern unsigned char *D_L00_001B04B0_2d5160[] __asm__("D_L00_001B04B0");
extern void
hit_reaction(void *, void *, void *, s32, s32 *, f32 *, s32, s32) __asm__("FUN_L00_0025a478");
extern void trooper_knockback_arc(void *, f32 *, void *, void *) __asm__("FUN_L00_0025ab48");
extern void trooper_start_knockback(void *, void *, f32, s32, s32, s32) __asm__("FUN_L00_0025c558");
extern void trooper_anim_update(void *, void *) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern void
activate_effect_l08(f32, void *, void *, s32, s32, void *, s32) __asm__("FUN_L00_0025ff38");
extern void FUN_L00_00257470_2d5160(void *, s32, s32) __asm__("FUN_L00_00257470");
extern s32  FUN_L00_00257b50_2d5160(s32) __asm__("FUN_L00_00257b50");
extern f32  FUN_L00_00257be8_2d5160(f32, f32) __asm__("FUN_L00_00257be8");
extern f32  scale_time(f32) __asm__("FUN_001f96b0");
extern void FUN_L00_00257880_2d5160(void *, s32, s32, s32) __asm__("FUN_L00_00257880");

float approach_angle(void *, float, void *, float, float, float) __asm__("FUN_L00_0025be00");
float advance_accelerated_scalar(float *, float, float, float, float, float *) __asm__("FUN_00213f38");
float FUN_001f9dc8_2d54c8(float) __asm__("FUN_001f9dc8");
float FUN_001f9de0_2d54c8(float) __asm__("FUN_001f9de0");
void  FUN_L00_00258830_2d54c8(void *, void *, int, float, float, float) __asm__("FUN_L00_00258830");

extern KillFlags D_L00_001BA5D0 NOT_SDA;
extern KillFlags D_L00_001BB230 NOT_SDA;
extern u8                       mission_state[8] __asm__("D_L00_0015FC88");
extern u8                       D_0014C050[][16] NOT_SDA;

/* Ported from rac1-decomp (src/overlays/shared/vendor_002D1168.c: func_L00_002D5AC0), where it is exact; names translated to the US level program. */

/* Class 749 update (horny toad): state machine that walks a path or follows a target moby, hops, turns and dies; then settles its height and checks a pickup moby. */

void horny_toad_update(Moby *moby) {
    enum {
        HORNY_TOAD_INITIALIZE = 0,
        HORNY_TOAD_FACE_FOLLOW_MOBY = 1,
        HORNY_TOAD_MOVE_TO_PATH_POINT = 2,
        HORNY_TOAD_WAIT_TO_FOLLOW_PATH = 3,
        HORNY_TOAD_FOLLOW_PATH = 4,
        HORNY_TOAD_ROAM = 5,
        HORNY_TOAD_APPROACH_AND_FACE_TARGET = 6,
        HORNY_TOAD_PLAY_TARGET_ANIMATION = 7,
        HORNY_TOAD_RETURN_HOME = 8,
        HORNY_TOAD_RUN_MOVEMENT_SEQUENCE = 9,
        HORNY_TOAD_MOVE_TO_POINT_OR_FACE_TARGET = 10,
        HORNY_TOAD_RECOVER_FROM_REACTION = 11,
        HORNY_TOAD_DYING = 12
    };
    ToadVars *horny_toad_vars;
    Moby     *target;
    ToadVec   reference_point_or_movement_endpoint;

    horny_toad_vars = (ToadVars *)moby->pvars;
    FUN_L00_002d5160_2d4610(moby);
    target = horny_toad_vars->target;
    if (moby->visible != 0 &&
        fast_vec_dist((*(ToadVec *)&moby->pos), (char *)&g_debug_camera) < 26.0f) {
        FUN_L00_0025a120_2d4610(moby);
        moby->shadow_range = 0x15;
    }
    FUN_L00_00263ac8_2d4610(2.1f, (int)moby, 1, horny_toad_vars->unk200);
    switch (moby->state) {
    case HORNY_TOAD_INITIALIZE: {
        float k;
        moby->scale = ((ToadClass *)moby->pclass)->unk24 * D_L00_0016196C;
        qcopy(horny_toad_vars->home, (*(ToadVec *)&moby->pos));
        if (moby->prev_seq != 1)
            blend_moby_animation(moby, 1, 0, 0);
        horny_toad_vars->unk29 = 1;
        horny_toad_vars->unk140 = &D_L00_00161960;
        qcopy(horny_toad_vars->unk240, (*(ToadVec *)&moby->pos));
        k = frame_time;
        horny_toad_vars->unk25C = 1.5f;
        horny_toad_vars->unk254 = k * 2.0f;
        horny_toad_vars->unk258 = k * 2.9670596f;
        {
            *(u128 *)reference_point_or_movement_endpoint = 0;
            reference_point_or_movement_endpoint[0] = 150.0f;
            reference_point_or_movement_endpoint[1] = 127.0f;
            if (vector_distance_xy((*(ToadVec *)&moby->pos), reference_point_or_movement_endpoint) <
                5.0f) {
                horny_toad_vars->unk25C = 0.75f;
            }
        }
        horny_toad_vars->unk58 = truncate_float_to_s32(12.0f);
        horny_toad_vars->unk5A = truncate_float_to_s32(2.0f);
        if (horny_toad_vars->path >= 0) {
            if (horny_toad_vars->unk1E4 >= 0) {
                ToadPath *path = toad_paths[horny_toad_vars->path];
                qcopy(horny_toad_vars->home, path->pts[path->count - 1]);
                qcopy((*(ToadVec *)&moby->pos), path->pts[0]);
                moby->state = 3;
            } else if (horny_toad_vars->follow_moby >= 0) {
                qcopy(horny_toad_vars->home, (*(ToadVec *)&moby->pos));
                moby->state = 1;
                if (moby->prev_seq != 2)
                    blend_moby_animation(moby, 2, 0, scale_game_frames(10));
            } else {
                mark_moby_for_removal(moby);
                return;
            }
        } else {
            moby->state = 5;
        }
        break;
    }
    case HORNY_TOAD_RUN_MOVEMENT_SEQUENCE:
        if (FUN_L00_002dc9f0_2d4610(moby, horny_toad_vars->unk70) != 0) {
            moby->state = 5;
            horny_toad_vars->unk138 = 0;
        }
        break;
    case HORNY_TOAD_FACE_FOLLOW_MOBY: {
        Moby *followed_moby = (Moby *)(moby_table + (horny_toad_vars->follow_moby << 8));
        if (followed_moby != 0 && followed_moby->state != 0xFE && followed_moby->state != 0xFD) {
            approach_angle(
                &moby->rot.z,
                fast_arc_tan((*(ToadVec *)&followed_moby->pos)[0] - (*(ToadVec *)&moby->pos)[0],
                             (*(ToadVec *)&followed_moby->pos)[1] - (*(ToadVec *)&moby->pos)[1]),
                &horny_toad_vars->turn_speed,
                frame_time_sq * 12.566371f,
                frame_time_sq * 12.566371f,
                frame_time * 12.566371f);
        } else {
            moby->state = 8;
            if (moby->prev_seq != 4)
                blend_moby_animation(moby, 4, 0, scale_game_frames(10));
        }
        break;
    }
    case HORNY_TOAD_MOVE_TO_PATH_POINT:
        if (move_horny_toad_toward_position((unsigned char *)moby,
                                            toad_paths[horny_toad_vars->path]->pts[horny_toad_vars->path_point],
                                            0.5f) < 1.0f) {
            moby->state = 1;
            if (moby->prev_seq != 2)
                blend_moby_animation(moby, 2, 0, scale_game_frames(10));
        }
        break;
    case HORNY_TOAD_WAIT_TO_FOLLOW_PATH:
        if (horny_toad_vars->unk1C4 != 2) {
            moby->state = 4;
            if (moby->prev_seq != 4)
                blend_moby_animation(moby, 4, 0, scale_game_frames(10));
        }
        break;
    case HORNY_TOAD_FOLLOW_PATH: {
        ToadPath *path = toad_paths[horny_toad_vars->path];
        if (move_horny_toad_toward_position(
                (unsigned char *)moby, path->pts[horny_toad_vars->path_point], 0.5f) < 1.0f) {
            if (horny_toad_vars->path_point < path->count - 1) {
                horny_toad_vars->path_point++;
            } else {
                moby->state = 5;
                if (moby->prev_seq != 1)
                    blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            }
        } else if (horny_toad_vars->unk1C4 != 2) {
            moby->state = 6;
            if (moby->prev_seq != 4)
                blend_moby_animation(moby, 4, 0, scale_game_frames(10));
        }
        break;
    }
    case HORNY_TOAD_ROAM:
        if (moby->prev_seq != 4) {
            int animation_start_index = random_int_between(0, 5);
            if (moby->prev_seq != 4)
                blend_moby_animation(moby, 4, animation_start_index, scale_game_frames(10));
            moby->anim_speed = random_float_between(0.92f, 1.08f);
        }
        update_object_wander_movement((char *)moby, (char *)horny_toad_vars->unk240, 0.5f, 0.5f);
        if (horny_toad_vars->unk1C4 != 2) {
            horny_toad_vars->unk1FC = random_float_between(-20.0f, 20.0f) * 0.017453292f;
            moby->state = 6;
            if (moby->prev_seq != 4)
                blend_moby_animation(moby, 4, 0, scale_game_frames(10));
        }
        break;
    case HORNY_TOAD_APPROACH_AND_FACE_TARGET:
        if (move_horny_toad_toward_position((unsigned char *)moby, horny_toad_vars->unk180, 0.5f) <
                horny_toad_target_approach_distance_threshold_gp &&
            fast_difference_between_rotations(
                moby->rot.z,
                fast_arc_tan((*(ToadVec *)&target->pos)[0] - (*(ToadVec *)&moby->pos)[0],
                             (*(ToadVec *)&target->pos)[1] - (*(ToadVec *)&moby->pos)[1])) <
                0.17453292f) {
            moby->state = 7;
            if (moby->prev_seq != 5)
                blend_moby_animation(moby,
                                     5,
                                     (int)horny_toad_target_animation_start_index_gp,
                                     scale_game_frames(10));
        } else if (horny_toad_vars->unk1C4 == 2) {
            moby->state = 8;
            if (moby->prev_seq != 4)
                blend_moby_animation(moby, 4, 0, scale_game_frames(10));
        }
        break;
    case HORNY_TOAD_PLAY_TARGET_ANIMATION:
        if (is_value_within_interpolated_window(moby, 34.0f) != 0) {
            collect_mobies_in_sphere_at_moby_point(moby, 0.333f, 1.0f, 1.0f, 0, 1, 0, 1, 0);
        } else if (moby->anim_flags & 2) {
            if (horny_toad_vars->unk1C4 == 2) {
                moby->state = 8;
                if (moby->prev_seq != 4)
                    blend_moby_animation(moby, 4, 0, scale_game_frames(10));
            } else if (vector_distance_xy((*(ToadVec *)&moby->pos), (*(ToadVec *)&target->pos)) >
                           1.5f ||
                       fast_difference_between_rotations(
                           moby->rot.z,
                           fast_arc_tan((*(ToadVec *)&target->pos)[0] - (*(ToadVec *)&moby->pos)[0],
                                        (*(ToadVec *)&target->pos)[1] -
                                            (*(ToadVec *)&moby->pos)[1])) > 0.17453292f) {
                moby->state = 6;
                if (moby->prev_seq != 4)
                    blend_moby_animation(moby, 4, 0, scale_game_frames(10));
            }
        }
        break;
    case HORNY_TOAD_RETURN_HOME:
        if (move_horny_toad_toward_position((unsigned char *)moby, horny_toad_vars->home, 0.5f) < 1.5f) {
            moby->state = 5;
            if (moby->prev_seq != 1)
                blend_moby_animation(moby, 1, 0, scale_game_frames(10));
        } else if (horny_toad_vars->unk1C4 != 2) {
            moby->state = 6;
            if (moby->prev_seq != 4)
                blend_moby_animation(moby, 4, 0, scale_game_frames(10));
        }
        break;
    case HORNY_TOAD_MOVE_TO_POINT_OR_FACE_TARGET: {
        fast_vec_sub(reference_point_or_movement_endpoint, horny_toad_vars->unk180, (*(ToadVec *)&moby->pos));
        reference_point_or_movement_endpoint[2] = 0.0f;
        fast_vec_normalize(
            reference_point_or_movement_endpoint, reference_point_or_movement_endpoint, 2.5f);
        fast_vec_add(reference_point_or_movement_endpoint,
                     reference_point_or_movement_endpoint,
                     (*(ToadVec *)&moby->pos));
        if (test_path_segment_strict_crossing(horny_toad_vars->unk1E0,
                                              (*(ToadVec *)&moby->pos),
                                              reference_point_or_movement_endpoint) != 0) {
            approach_angle(
                &moby->rot.z,
                fast_arc_tan((*(ToadVec *)&horny_toad_vars->target->pos)[0] - (*(ToadVec *)&moby->pos)[0],
                             (*(ToadVec *)&horny_toad_vars->target->pos)[1] - (*(ToadVec *)&moby->pos)[1]),
                &horny_toad_vars->turn_speed,
                frame_time_sq * 12.566371f,
                frame_time_sq * 12.566371f,
                frame_time * 12.566371f);
            if (moby->anim_flags & 2) {
                if (moby->prev_seq != random_integer_below(2) + 2)
                    blend_moby_animation(
                        moby, random_integer_below(2) + 2, 0, scale_game_frames(20));
            }
        } else {
            if (moby->prev_seq != 4)
                blend_moby_animation(moby, 4, 0, scale_game_frames(20));
            move_horny_toad_toward_position((unsigned char *)moby, horny_toad_vars->unk180, 0.5f);
        }
        if (moby->anim_flags & 2) {
            if (horny_toad_vars->unk1C4 != 2) {
                moby->state = 6;
                if (moby->prev_seq != 4)
                    blend_moby_animation(moby, 4, 0, scale_game_frames(10));
            } else if (horny_toad_vars->unk276 == 0) {
                moby->state = 8;
                if (moby->prev_seq != 4)
                    blend_moby_animation(moby, 4, 0, scale_game_frames(20));
            }
        }
        break;
    }
    case HORNY_TOAD_RECOVER_FROM_REACTION:
        if (step_moby_motion(moby, horny_toad_vars->unk70) & 0x140) {
            moby->state = 5;
            if (moby->prev_seq != 1)
                blend_moby_animation(moby, 1, 0, scale_game_frames(10));
        }
        break;
    case HORNY_TOAD_DYING:
        if (step_moby_motion(moby, horny_toad_vars->unk70) & 0x140) {
            allocate_voice_for_target_entry(7, 0, moby);
            spawn_color_swapped_explosion_burst(moby, (*(ToadVec *)&moby->pos), -1, 0.75f, 10.0f);
            mark_moby_for_removal(moby);
            return;
        }
        break;
    }
    if (moby->state != 11 && moby->state != 12 && moby->state != 9) {
        approach_value(probe_ground_height((*(ToadVec *)&moby->pos), 0, 0.5f),
                       frame_time * 7.0f,
                       &(*(ToadVec *)&moby->pos)[2]);
        if (moby->state != 4) {
            resolve_point_path_segment_xy_clearance(
                horny_toad_vars->unk1E0, 0.25f, (*(ToadVec *)&moby->pos), (*(ToadVec *)&moby->pos));
        }
    }
    if (horny_toad_vars->pickup_moby != -1) {
        Moby *pickup_target_moby = (Moby *)(moby_table + (horny_toad_vars->pickup_moby << 8));
        if (vector_distance_xy((*(ToadVec *)&moby->pos), (*(ToadVec *)&pickup_target_moby->pos)) <
            1.0f) {
            if (pickup_target_moby->oclass == 0x33E) {
                FUN_L00_002d5830_2d4610(pickup_target_moby);
                horny_toad_vars->pickup_moby = -1;
            }
        }
    }
}

void process_horny_toad_hit_reaction(unsigned char *m) __asm__("FUN_L00_002d5160");

void process_horny_toad_hit_reaction(unsigned char *toad_moby) {
    unsigned char *toad_vars = *(unsigned char **)(toad_moby + 0x78);
    unsigned char *hit;
    s32            result;
    f32            damage;
    f32            angle;
    f32            hit_knockback_vector[4] __attribute__((aligned(16)));
    f32            forceX, forceY, forceZ, forceW, health, duration;
    f32            reactionC0, reactionC4;
    unsigned char *row;
    s32            index;
    unsigned char *effect;

    if (toad_moby[0x20] == 0)
        return;
    if (D_L00_0015F5C4_2d5160 == 2) {
        toad_moby[0x31] = 0;
        *(u16 *)(toad_moby + 0x34) |= 1;
    } else {
        toad_moby[0x31] = 1;
        *(u16 *)(toad_moby + 0x34) &= ~1;
    }
    damage = 0.0f;
    hit = find_moby_hit(toad_moby, 0x330000, 0);
    hit_reaction(toad_moby, hit, toad_vars + 0x20, 0, &result, &damage, 0, 4);
    if (result != 1 && toad_moby[0x20] != 12 && damage != 0.0f) {
        /* Fakematch: register pins give retail's choices: 41 in $t0, the frame time and
           later 0.75 in $f4, the squared frame time first in $f0. */
        register s32 hit_state __asm__("$8") = 0x29;
        register f32 frame_time_reg __asm__("$f4") = frame_time;
        register f32 knockback_scale __asm__("$f4");
        register f32 frame_time_sq_reg __asm__("$f0") = D_0015ED70;
        forceX = D_L00_00161970_gp * frame_time_sq_reg;
        forceY = D_L00_00161974_gp * frame_time_sq_reg;
        forceZ = D_L00_00161978_gp * frame_time_reg;
        forceW = D_L00_0016197C_gp * frame_time_reg;
        health = *(f32 *)(toad_vars + 0x20) - damage;
        toad_vars[0xAD] = 0;
        duration = frame_time + frame_time;
        *(s32 *)(toad_vars + 0x94) = hit_state;
        *(s32 *)(toad_vars + 0x90) = 0x200;
        knockback_scale = 0.75f;
        *(f32 *)(toad_vars + 0x98) = knockback_scale;
        *(f32 *)(toad_vars + 0x20) = health;
        *(f32 *)(toad_vars + 0x80) = forceX;
        *(f32 *)(toad_vars + 0x84) = forceY;
        *(f32 *)(toad_vars + 0x88) = forceZ;
        *(f32 *)(toad_vars + 0x8C) = forceW;
        *(f32 *)(toad_vars + 0xBC) = duration;
        *(u16 *)(toad_moby + 0x34) &= ~0x1000;
        *(OvlQuad *)hit_knockback_vector = *(OvlQuad *)(hit + 0x10);
        trooper_knockback_arc(hit_knockback_vector, &angle, toad_vars + 0x88, toad_vars + 0x8C);
        trooper_start_knockback(toad_moby, toad_vars + 0x70, angle, FUN_L00_00257b50_2d5160(2) + 6, 1, 0);
        reactionC0 = D_L00_0016198C_gp;
        reactionC4 = D_L00_00161988_gp;
        *(f32 *)(toad_vars + 0xC0) = reactionC0;
        *(f32 *)(toad_vars + 0xC4) = reactionC4;
        toad_moby[0x20] = 12;
        toad_vars[0x67] = 0x78;
        trooper_anim_update(toad_moby, toad_vars + 0x60);
        FUN_L00_00257470_2d5160(toad_moby, 0, -1);
    }
    toad_moby[0xA4] = 0xFF;
    trooper_anim_finish(toad_moby, toad_vars + 0x60);
    if (toad_moby[0x20] == 3) {
        index = *(s32 *)(toad_vars + 0x1E4);
        effect = toad_vars + 0x180;
        activate_effect_l08(64.0f,
                            toad_moby,
                            effect,
                            0,
                            0,
                            D_L00_001B04B0_2d5160[index] + 0x10,
                            *(s32 *)D_L00_001B04B0_2d5160[index]);
    } else {
        index = *(s32 *)(toad_vars + 0x1E0);
        effect = toad_vars + 0x180;
        activate_effect_l08(24.0f,
                            toad_moby,
                            effect,
                            0,
                            0,
                            D_L00_001B04B0_2d5160[index] + 0x10,
                            *(s32 *)D_L00_001B04B0_2d5160[index]);
    }
    if (*(s32 *)(toad_vars + 0x1C0) == 0) {
        *(s32 *)(toad_vars + 0x1C0) = (s32)hero.moby;
        qcopy(effect, &hero.motion.pos);
    }
    fast_dec_timer_s16(toad_vars + 0x276);
    index = *(s32 *)(toad_vars + 0x38);
    if (index != 0) {
        *(s16 *)(toad_vars + 0x276) =
            truncate_float_to_s32(scale_time(FUN_L00_00257be8_2d5160(180.0f, 240.0f)));
    }
    *(s32 *)(toad_vars + 0x38) = 0;
    if ((toad_moby[0x20] == 5 || toad_moby[0x20] == 8) && *(s16 *)(toad_vars + 0x276) != 0) {
        toad_moby[0x20] = 10;
        if (toad_moby[0x53] != 4)
            FUN_L00_00257880_2d5160(toad_moby, 4, 0, scale_game_frames(20));
    }
}
/* D_0015ED70 is declared sda above (crate_drop_bolts), but here retail loads it
   absolute: reach it as D_0015ED6C + 4, past whose size the assembler skips $gp. */
#define D_0015ED70_2d54c8 (((float *)&frame_time)[1])

float move_horny_toad_toward_position(unsigned char *moby,
                                      float         *target_position,

                                      float initial_travel_distance) {
    float          requested_movement_displacement[4] __attribute__((aligned(16)));
    unsigned char *moby_vars;
    float          desired_yaw, target_distance, z, movement_yaw, maximum_vertical_displacement,
        vertical_displacement;
    moby_vars = ((unsigned char *)MOBY(moby)->pvars);
    desired_yaw = fast_arc_tan(target_position[0] - MOBY(moby)->pos.x,
                               target_position[1] - MOBY(moby)->pos.y);
    z = MOBY(moby)->pos.z;
    target_distance = fast_vec_dist(moby + 0x10, target_position);
    if (2.0f < target_distance)
        desired_yaw = fast_add_rotations(desired_yaw, *(float *)(moby_vars + 0x1FC));
    if (moby[0x52] != moby[0x53])
        return target_distance;
    approach_angle(moby + 0x48,
                   desired_yaw,
                   moby_vars + 0x1F4,
                   D_0015ED70_2d54c8 * 12.566371f,
                   D_0015ED70_2d54c8 * 12.566371f,
                   frame_time * 12.566371f);
    advance_accelerated_scalar(&initial_travel_distance,
                               target_distance,
                               D_0015ED70_2d54c8 * 8.0f,
                               D_0015ED70_2d54c8 * 12.0f,
                               frame_time * 4.0f,
                               (float *)(moby_vars + 0x1F8));
    if (*(float *)(moby_vars + 0x1F8) < 0.0f)
        *(float *)(moby_vars + 0x1F8) = 0.0f;
    movement_yaw = MOBY(moby)->rot.z;
    *(float *)(moby_vars + 0x278) = movement_yaw;
    requested_movement_displacement[0] =
        FUN_001f9dc8_2d54c8(movement_yaw) * *(float *)(moby_vars + 0x1F8);
    requested_movement_displacement[1] =
        FUN_001f9de0_2d54c8(*(float *)(moby_vars + 0x278)) * *(float *)(moby_vars + 0x1F8);
    requested_movement_displacement[2] = 0.0f;
    requested_movement_displacement[2] = *(float *)(moby_vars + 0x270) - D_0015ED70_2d54c8 * 10.0f;
    FUN_L00_00258830_2d54c8(moby, requested_movement_displacement, 0x10, 0.5f, 0.5f, 0.0f);
    vertical_displacement = MOBY(moby)->pos.z - z;
    maximum_vertical_displacement = frame_time * 4.0f;
    *(float *)(moby_vars + 0x270) = vertical_displacement;
    if (maximum_vertical_displacement < vertical_displacement)
        *(float *)(moby_vars + 0x270) = maximum_vertical_displacement;
    if (moby[0x70] & 2)
        return target_distance;
    return 37.0f;
}

/* Suck cannon methods of the horny toad. */
extern int process_moby_suck_cannon_suction(void *) __asm__("FUN_L00_002db8f8");

/* Moby state 9 while FUN_L00_002db8f8 reports a hit, back to 5 when it stops. */
int process_suck_cannon_suction_and_update_moby_state(char *a) __asm__("FUN_L00_002d56e0");

int process_suck_cannon_suction_and_update_moby_state(char *moby) {
    int suction_result = process_moby_suck_cannon_suction(moby);
    if (suction_result == 0) {
        if (MOBY(moby)->state == 0x9)
            moby[0x20] = 0x5;
    } else {
        moby[0x20] = 0x9;
    }
    return suction_result;
}

extern int cancel_suck_cannon_suction(void *) __asm__("FUN_L00_002dbe20");

/* Runs FUN_L00_002dbe20 on the moby and sets its state to 9. */
int cancel_suck_cannon_suction_and_set_moby_state_9(char *moby) __asm__("FUN_L00_002d57d0");

int cancel_suck_cannon_suction_and_set_moby_state_9(char *moby) {
    int helper_result = cancel_suck_cannon_suction(moby);
    moby[0x20] = 0x9;
    return helper_result;
}

s32 get_moby_private_data_d0_address(T6 *moby) __asm__("FUN_L00_002d5800");

s32 get_moby_private_data_d0_address(T6 *moby) {
    s32 moby_vars_address;
    if (moby != 0) {
        moby_vars_address = moby->f;
        if (moby_vars_address != 0) {
            return moby_vars_address + 0xD0;
        }
        return 0;
    }
    return 0;
}

/* Kills a live moby: marks it in the level's spawn tables, starts its death sound and
 * clears its key out of the shared key table. */

void collect_moby_and_clear_path_keys(struct Moby *moby) __asm__("FUN_L00_002d5830");

void collect_moby_and_clear_path_keys(struct Moby *moby) {
    KeyVars *key_vars = (KeyVars *)moby->pvars;
    s32      i;
    if (moby->state != 1) {
        return;
    }
    D_L00_001BA5D0.killed[(s16)moby->save_id] = moby->mission + 2;
    if (moby->mission == 0xFF || (mission_state[moby->mission] != 0xFF &&
                                  D_0014C050[current_level_index][moby->mission] == 0xFF)) {
        D_L00_001BB230.killed[(s16)moby->save_id] = moby->mission + 2;
    }
    moby->state = 2;
    moby->command = 1;
    moby->glow_color = 0x80208020;
    allocate_voice_for_target_entry(0, 0, moby);
    if (key_vars->table == -1) {
        return;
    }
    for (i = 0; i < ((KeyRow *)D_L00_001B04B0_2d0538[key_vars->table])[0].count; i++) {
        KeyRow *row = &((KeyRow *)D_L00_001B04B0_2d0538[key_vars->table])[i];
        if (row[1].key == key_vars->key) {
            row[1].key = 0;
        }
    }
}

u8 D_L02_0015FC88[8] = {0};

u8 D_L04_0015FC88[8] = {0};

u8 D_L09_0015FC88[8] = {0};

u8 D_L12_0015FC88[8] = {0};

u8 D_L14_0015FC88[8] = {0};

u8 D_L16_0015FC88[8] = {0};

float D_L18_00161CF4 = 0.75f;
float D_L18_00161D08 = 2.0f;
float D_L18_00161D0C = 7.0f;
short D_L18_00161CE8 = 0x404;
f32   D_L18_00161CF8 = 4e+01f;
f32   D_L18_00161CFC = 14.0f;
f32   D_L18_00161D00 = 13.0f;
f32   D_L18_00161D04 = 14.0f;
f32   D_L18_00161D10 = 25.0f;
f32   D_L18_00161D14 = 14.0f;
