/* Level 13 Blarg space commando moby class: damage, targeting, ground motion and its trail (points, quads and sparks). */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "qcopy.h"
#include "rnc/overlay/collision.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/game/interpolated_record.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/mobys/step_moby_motion.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/world/collision/collision_hit_material.h"
#include "rnc/world/collision/collision_line.h"
#include "qzero.h"
#include "rnc/rendering/effects/spawn_spark.h"
#include "rnc/rendering/vu1_registers.h"
#include "rnc/rendering/effect_texture.h"

extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");

extern CollisionHit coll_hit_1638 __asm__("D_L13_00174140");

extern void spawn_explosion(void *,
                            void *,
                            void *,
                            float,
                            float,
                            int,
                            int,
                            int,
                            float,
                            float,
                            float,
                            float,
                            int,
                            float,
                            int,
                            int,
                            int,
                            int) __asm__("FUN_L00_0025e450");

extern float fast_vec_dist(void *, void *);

extern char D_L13_001670C0[];

#include "rnc/overlay/l13/mobys/blarg_space_commando_globals.h"

extern int
fun_00213928(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_00213928");

extern void trooper_anim_update(struct Moby *, void *) __asm__("FUN_L00_0025d458");

extern void trooper_anim_finish(struct Moby *, void *) __asm__("FUN_L00_0025d538");

typedef struct {
    u8  pad0[0x18];
    u8  active;
    u8  valid;
    s16 oclass;
    u8  pad1C[0x14];
} Shot;

typedef struct {
    s32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    u8  pad10[0x14];
    f32 unk24;
    u8  pad28[0x18];
} Ctl;

typedef struct {
    u8           pad0[0x20];
    f32          unk20;
    s16          unk24;
    u8           pad26[2];
    u8           unk28;
    u8           pad29[7];
    f32          unk30;
    u8           pad34[0x24];
    u8           unk58;
    u8           pad59;
    u8           unk5A;
    u8           pad5B[5];
    Vec4         vel; /* 0x60 */
    u8           pad70[0x50];
    Ctl          ctl; /* 0xC0 */
    u8           pad100[0x60];
    struct Moby *target; /* 0x160 */
    s32          unk164;
    u8           pad168[8];
    Vec4         home; /* 0x170 */
    u8           pad180[4];
    s32          unk184;
    f32          turn; /* 0x188 */
    u8           pad18C[8];
    s32          timer; /* 0x194 */
    s32          path;  /* 0x198 */
    u8           pad19C[0xC];
    f32          unk1A8;
    u8           pad1AC[0x34];
    Vec4         unk1E0;
    u8           pad1F0[0x210];
    u8           unk400[1];
} Vars_309330;

extern int FUN_L00_00258b50(struct Moby *, void *, void *, void *, float);

extern int FUN_L00_00261b48(int, void *, void *, void *);

extern s32 rand(void);

extern void sweep_hit_segment(void *, void *, void *, void *, struct Moby *, Shot *, int) __asm__("FUN_L00_00259d08");

extern void FUN_L00_0025a120(void *);

extern void
approach_angle(float *, float, float *, float, float, float) __asm__("FUN_L00_0025be00");

extern void FUN_L00_0025e450_c(void *,
                               void *,
                               void *,
                               float,
                               float,
                               int,
                               int,
                               int,
                               float,
                               float,
                               float,
                               int,
                               float,
                               float,
                               int,
                               int,
                               int,
                               int) __asm__("FUN_L00_0025e450");

extern void
resolve_point_path_segment_xy_clearance(int, void *, void *, float) __asm__("FUN_L00_00261d78");

extern void FUN_L00_00263ac8(float, struct Moby *, int, void *);

extern void FUN_L01_0026d930(void *);

extern void FUN_L13_00308d10_c(struct Moby *) __asm__("FUN_L13_00308d10");

extern void FUN_L13_00309370_c(char *m) __asm__("FUN_L13_00309370");

extern void
func_L00_0025A8C0_30A9B0(void *, void *, int, void *, float) __asm__("FUN_L13_00265990");

extern float D_L13_00161E58_gp __asm__("D_L13_00161E58") __attribute__((sda));

extern float l13_blarg_space_commando_probe_trigger_distance_xy __asm__("D_L13_00161E74")
    __attribute__((sda));

extern float l13_blarg_space_commando_horizontal_speed_limit_base __asm__("D_L13_00161E78")
    __attribute__((sda));

extern void func_L00_002592B0_309330(struct Moby *, float, float *, float, float, float) __asm__("FUN_L13_00263fc8");

void blarg_space_commando_update(struct Moby *moby) __asm__("FUN_L13_00307e98");

void blarg_space_commando_update(struct Moby *moby) {
    enum {
        BLARG_SPACE_COMMANDO_INITIALIZE = 0,
        BLARG_SPACE_COMMANDO_FACE_TARGET_AND_SELECT_ACTION = 1,
        BLARG_SPACE_COMMANDO_ENTER_ACTION_SELECTION_AFTER_ANIMATION = 2,
        BLARG_SPACE_COMMANDO_SELECT_ACTION_AFTER_ANIMATION = 3,
        BLARG_SPACE_COMMANDO_FACE_TARGET_AND_WAIT_FOR_TIMER = 4,
        BLARG_SPACE_COMMANDO_FACE_TARGET_AND_PROBE_DURING_ANIMATION = 5,
        BLARG_SPACE_COMMANDO_RESUME_SAVED_STATE_AFTER_ANIMATION = 6,
        BLARG_SPACE_COMMANDO_APPROACH_TARGET_WITH_TURN_OFFSET = 7,
        BLARG_SPACE_COMMANDO_APPROACH_TARGET_UNTIL_BLOCKED_OR_NEAR = 11,
        BLARG_SPACE_COMMANDO_PROBE_DURING_ANIMATION_THEN_RESUME_APPROACH = 12,
        BLARG_SPACE_COMMANDO_RETURN_HOME = 13,
        BLARG_SPACE_COMMANDO_KNOCKBACK = 15,
        BLARG_SPACE_COMMANDO_DYING = 16
    };
    Vars_309330 *vars = (Vars_309330 *)moby->pvars;
    struct Moby *target;
    Vec4         point;
    Vec4         origin;
    Vec4         direction;
    Shot         shot;
    s32          blocked;
    s32          path_blocked;
    s32          hit;

    FUN_L13_00308d10_c(moby);
    target = vars->target;
    FUN_L00_00263ac8(2.5f, moby, 1, vars->unk400);
    if (moby->visible != 0 && fast_vec_dist(&moby->pos, D_L13_001670C0) < 28.0f) {
        FUN_L00_0025a120(moby);
        moby->shadow_range = 0x16;
    }
    switch (moby->state) {
    case BLARG_SPACE_COMMANDO_INITIALIZE:
        vars->unk20 = 2.0f;
        vars->unk28 = 1;
        vars->unk58 = 12;
        vars->unk5A = 8;
        vars->unk24 = 2;
        vars->unk30 = 1.5f;
        FUN_L01_0026d930(&vars->ctl);
        vars->ctl.unk8 = 2.0f;
        vars->ctl.unkC = 0.5f;
        vars->ctl.unk0 = truncate_float_to_s32(D_L13_00161E58_gp * 716.8f);
        vars->ctl.unk4 = D_L13_00161E58_gp * 0.7f;
        vars->ctl.unk24 = l13_blarg_space_commando_horizontal_speed_limit_base * frame_time *
                          random_float_between(0.95f, 1.05f);
        qcopy(&vars->home, &moby->pos);
        moby_point_position(moby, 0, &vars->unk1E0);
        moby->state = 1;
        break;
    case BLARG_SPACE_COMMANDO_FACE_TARGET_AND_SELECT_ACTION:
        approach_angle(&moby->rot.z,
                       fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y),
                       &vars->turn,
                       frame_time_sq * 6.2831855f,
                       frame_time_sq * 6.2831855f,
                       frame_time * 3.1415927f);
        if (vars->unk164 != 2) {
            f32 dist = vector_distance_xy(&moby->pos, &target->pos);
            if (l13_blarg_space_commando_probe_trigger_distance_xy < dist) {
                if (fast_difference_between_rotations(
                        moby->rot.z,
                        fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y)) <
                    0.2617994f) {
                    moby->state = 2;
                    if (moby->prev_seq != 3) {
                        blend_moby_animation(moby, 3, 0, scale_game_frames(10));
                    }
                } else {
                    moby->state = 7;
                    if (moby->prev_seq != 1) {
                        blend_moby_animation(moby, 1, 0, scale_game_frames(20));
                    }
                }
            } else {
                moby->state = 5;
                if (moby->prev_seq != 6) {
                    blend_moby_animation(moby, 6, 0, scale_game_frames(10));
                }
            }
        } else if (vars->unk184 != 0) {
            moby->state = 11;
            if (moby->prev_seq != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(20));
            }
        }
        break;
    case BLARG_SPACE_COMMANDO_ENTER_ACTION_SELECTION_AFTER_ANIMATION:
        if (moby->anim_flags & 2) {
            moby->state = 3;
            FUN_L13_00309370_c((char *)moby);
            if (moby->prev_seq != 5) {
                blend_moby_animation(moby, 5, 0, scale_game_frames(10));
            }
        }
        break;
    case BLARG_SPACE_COMMANDO_SELECT_ACTION_AFTER_ANIMATION:
        if (moby->anim_flags & 2) {
            f32 dist = vector_distance_xy(&moby->pos, &target->pos);
            if (vars->unk164 != 2 && dist < 15.0f) {
                moby->state = 4;
                if (moby->prev_seq != 4) {
                    blend_moby_animation(moby, 4, 0, scale_game_frames(20));
                }
                vars->timer = scale_game_frames(blarg_space_commando_face_target_wait_frames);
            } else {
                moby->state = 1;
                if (moby->prev_seq != 0) {
                    blend_moby_animation(moby, 0, 0, scale_game_frames(20));
                }
            }
        }
        break;
    case BLARG_SPACE_COMMANDO_FACE_TARGET_AND_WAIT_FOR_TIMER:
        approach_angle(&moby->rot.z,
                       fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y),
                       &vars->turn,
                       frame_time_sq * 6.2831855f,
                       frame_time_sq * 6.2831855f,
                       frame_time * 3.1415927f);
        if (fast_dec_timer(&vars->timer) != 0) {
            moby->state = 3;
            FUN_L13_00309370_c((char *)moby);
            if (moby->prev_seq != 5) {
                blend_moby_animation(moby, 5, 0, scale_game_frames(5));
            }
        } else if (vector_distance_xy(&moby->pos, &target->pos) <
                   l13_blarg_space_commando_probe_trigger_distance_xy) {
            moby_point_position(moby, 0, &vars->unk1E0);
            moby->state = 5;
            if (moby->prev_seq != 6) {
                blend_moby_animation(moby, 6, 0, scale_game_frames(10));
            }
        } else if (vars->unk184 != 0) {
            moby->state = 11;
            if (moby->prev_seq != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(20));
            }
        }
        break;
    case BLARG_SPACE_COMMANDO_FACE_TARGET_AND_PROBE_DURING_ANIMATION:
        approach_angle(&moby->rot.z,
                       fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y),
                       &vars->turn,
                       frame_time_sq * 6.2831855f,
                       frame_time_sq * 6.2831855f,
                       frame_time * 3.1415927f);
        moby_point_position(moby, 0, &point);
        qcopy(&origin, &moby->pos);
        origin.f[2] += 0.3f;
        {
            f32 frame = compute_interpolated_record_value(moby);
            if (frame >= 9.0f && frame <= 15.0f) {
                direction.f[0] = fast_cos(moby->rot.z);
                direction.f[1] = fast_sin(moby->rot.z);
                direction.f[2] = 1.0f;
                direction.f[3] = 5627.925f;
                func_L00_0025A8C0_30A9B0(&shot, moby, 1, &direction, 1.0f);
                shot.active = 0;
                shot.valid = 1;
                shot.oclass = moby->oclass;
                sweep_hit_segment(&point, &origin, &vars->unk1E0, &origin, moby, &shot, 5);
            }
        }
        qcopy(&vars->unk1E0, &point);
        if (moby->anim_flags & 2) {
            moby->state = 6;
            moby->command = 1;
            if (moby->prev_seq != 2) {
                blend_moby_animation(moby, 2, 0, scale_game_frames(10));
            }
        }
        break;
    case BLARG_SPACE_COMMANDO_RESUME_SAVED_STATE_AFTER_ANIMATION:
        if (moby->anim_flags & 2) {
            moby->state = moby->command;
            if (moby->prev_seq != 0) {
                blend_moby_animation(moby, 0, 0, scale_game_frames(20));
            }
        }
        break;
    case BLARG_SPACE_COMMANDO_APPROACH_TARGET_WITH_TURN_OFFSET:
        func_L00_002592B0_309330(moby,
                                 fast_add_rotations(fast_arc_tan(target->pos.x - moby->pos.x,
                                                                 target->pos.y - moby->pos.y),
                                                    vars->unk1A8),
                                 &vars->turn,
                                 0.05f,
                                 0.3f,
                                 frame_time * 6.2831855f);
        point.f[0] = 2.0f * fast_cos(moby->rot.z);
        point.f[1] = 2.0f * fast_sin(moby->rot.z);
        point.f[2] = 0.0f;
        if (FUN_L00_00258b50(moby, &vars->ctl, &point, &origin, 1.0f) & 2) {
            rand();
            vars->unk1A8 = convert_integer_to_float(-1) * 1.0995574f;
        } else {
            vars->unk1A8 = 0.0f;
        }
        resolve_point_path_segment_xy_clearance(vars->path, &moby->pos, &moby->pos, 0.75f);
        path_blocked = 0;
        if (FUN_L00_00261b48(vars->path, &moby->pos, &target->pos, &direction) != 0 &&
            fast_vec_dist(&moby->pos, &direction) < 0.8f) {
            path_blocked = 1;
        }
        {
            f32 dist = vector_distance_xy(&moby->pos, &target->pos);
            if (dist < 15.0f) {
                moby->state = 2;
                if (moby->prev_seq != 3) {
                    blend_moby_animation(moby, 3, 0, scale_game_frames(10));
                }
            } else if (path_blocked != 0) {
                moby->state = 2;
                if (moby->prev_seq != 3) {
                    blend_moby_animation(moby, 3, 0, scale_game_frames(10));
                }
            } else if (dist < l13_blarg_space_commando_probe_trigger_distance_xy) {
                moby->state = 5;
                if (moby->prev_seq != 6) {
                    blend_moby_animation(moby, 6, 0, scale_game_frames(10));
                }
            }
        }
        break;
    case BLARG_SPACE_COMMANDO_APPROACH_TARGET_UNTIL_BLOCKED_OR_NEAR:
        blocked = 0;
        func_L00_002592B0_309330(
            moby,
            fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y),
            &vars->turn,
            0.05f,
            0.3f,
            frame_time * 6.2831855f);
        point.f[0] = 2.0f * fast_cos(moby->rot.z);
        point.f[1] = 2.0f * fast_sin(moby->rot.z);
        point.f[2] = 0.0f;
        FUN_L00_00258b50(moby, &vars->ctl, &point, &origin, 1.0f);
        resolve_point_path_segment_xy_clearance(vars->path, &moby->pos, &moby->pos, 0.75f);
        if (FUN_L00_00261b48(vars->path, &moby->pos, &target->pos, &direction) != 0 &&
            fast_vec_dist(&moby->pos, &direction) < 0.8f) {
            blocked = 1;
        }
        if (vars->unk184 == 0) {
            moby->state = 13;
        } else if (blocked != 0) {
            moby->state = 12;
            if (moby->prev_seq != 6) {
                blend_moby_animation(moby, 6, 0, scale_game_frames(10));
            }
        } else if (vector_distance_xy(&moby->pos, &target->pos) <
                   l13_blarg_space_commando_probe_trigger_distance_xy) {
            moby_point_position(moby, 0, &vars->unk1E0);
            moby->state = 12;
            if (moby->prev_seq != 6) {
                blend_moby_animation(moby, 6, 0, scale_game_frames(10));
            }
        }
        break;
    case BLARG_SPACE_COMMANDO_PROBE_DURING_ANIMATION_THEN_RESUME_APPROACH:
        moby_point_position(moby, 0, &point);
        qcopy(&origin, &moby->pos);
        origin.f[2] += 1.5f;
        {
            f32 frame = compute_interpolated_record_value(moby);
            if (frame >= 9.0f && frame <= 15.0f) {
                direction.f[0] = fast_cos(moby->rot.z);
                direction.f[1] = fast_sin(moby->rot.z);
                direction.f[2] = 1.0f;
                direction.f[3] = 5627.925f;
                func_L00_0025A8C0_30A9B0(&shot, moby, 1, &direction, 1.0f);
                shot.active = 0;
                shot.valid = 1;
                shot.oclass = moby->oclass;
                sweep_hit_segment(&point, &origin, &vars->unk1E0, &origin, moby, &shot, 5);
            }
        }
        qcopy(&vars->unk1E0, &point);
        if (moby->anim_flags & 2) {
            moby->state = 11;
            if (moby->prev_seq != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(20));
            }
        }
        break;
    case BLARG_SPACE_COMMANDO_RETURN_HOME:
        func_L00_002592B0_309330(
            moby,
            fast_arc_tan(vars->home.f[0] - moby->pos.x, vars->home.f[1] - moby->pos.y),
            &vars->turn,
            0.05f,
            0.3f,
            frame_time * 6.2831855f);
        point.f[0] = 2.0f * fast_cos(moby->rot.z);
        point.f[1] = 2.0f * fast_sin(moby->rot.z);
        point.f[2] = 0.0f;
        if (fast_difference_between_rotations(
                moby->rot.z,
                fast_arc_tan(vars->home.f[0] - moby->pos.x, vars->home.f[1] - moby->pos.y)) <
            0.7853982f) {
            FUN_L00_00258b50(moby, &vars->ctl, &point, &origin, 1.0f);
            resolve_point_path_segment_xy_clearance(vars->path, &moby->pos, &moby->pos, 0.75f);
        }
        if (fast_vec_dist(&vars->home, &moby->pos) < 1.0f) {
            moby->state = 1;
            if (moby->prev_seq != 0) {
                blend_moby_animation(moby, 0, 0, scale_game_frames(20));
            }
        }
        break;
    case BLARG_SPACE_COMMANDO_KNOCKBACK:
        hit = step_moby_motion(moby, &vars->vel);
        if (hit & 0x40) {
            qzero(&vars->vel);
            moby->state = 1;
            if (moby->prev_seq != 0) {
                blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            }
        } else if (hit & 0x120) {
            mark_moby_for_removal(moby);
        }
        break;
    case BLARG_SPACE_COMMANDO_DYING:
        if (step_moby_motion(moby, &vars->vel) & 0x160) {
            FUN_L00_0025e450_c(moby,
                               &vars->vel,
                               0,
                               0.0f,
                               0.0f,
                               10,
                               3,
                               16,
                               4.0f,
                               2.0f,
                               9.0f,
                               -1,
                               1.0f,
                               15.0f,
                               1,
                               1,
                               -1,
                               0);
            mark_moby_for_removal(moby);
        }
        break;
    }
}

struct WalkerVars_8d10 {
    u8    pad0[0x20];
    f32   health; /* 0x20 */
    u8    pad24[2];
    s16   stun_timer; /* 0x26 */
    u8    pad28[0x10];
    s32   init; /* 0x38: nonzero until the idle timer is first drawn */
    u8    pad3C[0x24];
    u8    anim[8];    /* 0x60 */
    f32   fall_speed; /* 0x68 */
    u8    pad6C[0xC];
    f32   knock_up;   /* 0x78 */
    f32   knock_back; /* 0x7C */
    s32   hit_sound;  /* 0x80 */
    u8    pad84[4];
    f32   hit_scale; /* 0x88 */
    u8    pad8C[0x24];
    f32   knock_min; /* 0xB0 */
    f32   knock_max; /* 0xB4 */
    u8    padB8[0x58];
    u8    flash[7];   /* 0x110 */
    u8    flash_time; /* 0x117 */
    u8    pad118[8];
    Vec4f target; /* 0x120 */
    u8    pad130[0x30];
    s32   start_frame; /* 0x160 */
    s32   mode;        /* 0x164 */
    u8    pad168[8];
    Vec4f home;       /* 0x170 */
    s32   path;       /* 0x180 */
    s32   idle_timer; /* 0x184 */
    u8    pad188[8];
    f32   range; /* 0x190 */
    u8    pad194[8];
    f32   near_range;  /* 0x19C */
    f32   far_range;   /* 0x1A0 */
    s32   target_moby; /* 0x1A4: -1: none */
};

extern f32 D_L13_00161E58 __attribute__((sda));

#include "rnc/overlay/l13/mobys/blarg_space_commando_data/update_blarg_space_commando_damage_targeting_and_ground_motion.h"

#include "rnc/overlay/level_labels.h"

extern char D_L13_00160680[] MACRO_ADDR;

extern s32 D_001413D0_lui[] __asm__("D_001413D0") NOT_SDA;

void update_gemlik_trail(struct Moby *moby) __asm__("FUN_L13_003094f8");

extern void update_gemlik_trail(struct Moby *) __asm__("FUN_L13_003094f8");

extern f32 FUN_001f96b0_8d10(f32) __asm__("FUN_001f96b0");

extern void
trooper_start_knockback(struct Moby *, void *, s32, s32, s32, f32) __asm__("FUN_L00_0025c558");

extern void stop_moby_sound_8d10(struct Moby *, s32, s32) __asm__("FUN_L00_00257470");

extern s32
find_target_8d10(f32, struct Moby *, void *, s32, s32, s32 *, s32) __asm__("FUN_L00_0025ff38");

void update_blarg_space_commando_damage_targeting_and_ground_motion(struct Moby *moby) __asm__("FUN_L13_00308d10");

void update_blarg_space_commando_damage_targeting_and_ground_motion(struct Moby *moby) {
    enum {
        HIT_RESULT_NONE = 0,
        HIT_RESULT_DEATH = 1,
        HIT_RESULT_DYING_KNOCKBACK = 2,
        HIT_RESULT_STUN_KNOCKBACK = 3,
        HIT_RESULT_NO_REACTION = 11
    };
    enum {
        GROUND_MATERIAL_DESTROYS_WALKER = 0,
        GROUND_MATERIAL_TRIGGERS_WALKER_EXPLOSION = 1,
        GROUND_MATERIAL_EXPLODES_AND_REMOVES_WALKER = 3,
        GROUND_MATERIAL_KILLS_WALKER_WITH_EXPLOSION = 11,
        GROUND_MATERIAL_EXPLODES_WALKER = 13
    };
    struct WalkerVars_8d10 *vars = (struct WalkerVars_8d10 *)moby->pvars;
    char                   *hit;
    s32                     code;
    s32                     kind;
    f32                     damage;
    f32                     ground;
    s32                    *path;
    Vec4f                   push;

    moby->scale = moby->pclass->scale * D_L13_00161E58;
    update_gemlik_trail(moby);
    if (vars->init != 0) {
        vars->idle_timer =
            truncate_float_to_s32(FUN_001f96b0_8d10(random_float_between(180.0f, 240.0f)));
        vars->init = 0;
    }
    fast_dec_timer(&vars->idle_timer);
    damage = 0.0f;
    hit = find_moby_hit(moby, 0x330000, 0);
    code = fun_00213928(moby, hit, &vars->health, 0, &kind, &damage, 0, 4);
    if (kind != 1 && moby->state != 0x10) {
        if (hit != 0 && *(struct Moby **)(hit + 0x20) != 0 &&
            (*(struct Moby **)(hit + 0x20))->oclass == 0x4EE) {
            damage = 0.0f;
        }
        vars->health -= damage;
        if (vars->health <= 0.0f) {
            code = 1;
        }
        vars->hit_sound = 0x2CD;
        vars->hit_scale = 0.7f;
        switch (code) {
        case HIT_RESULT_NONE:
        case HIT_RESULT_NO_REACTION:
            break;
        case HIT_RESULT_STUN_KNOCKBACK:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
            moby->state = 0xF;
            vars->stun_timer = scale_game_frames(0x3C);
            vars->knock_up = blarg_space_commando_stun_horizontal_knockback_speed * frame_time;
            vars->knock_back = blarg_space_commando_stun_vertical_knockback_speed * frame_time;
            vars->knock_min = 8.0f;
            vars->knock_max = 15.0f;
            trooper_start_knockback(moby,
                                    vars->anim,
                                    7,
                                    1,
                                    0,
                                    fast_arc_tan(*(f32 *)(hit + 0x10), *(f32 *)(hit + 0x14)));
            vars->flash_time = 0xFA;
            break;
        case HIT_RESULT_DEATH:
        case HIT_RESULT_DYING_KNOCKBACK:
            moby->state = 0x10;
            moby->flags &= ~0x1000;
            vars->knock_up = blarg_space_commando_dying_horizontal_knockback_speed * frame_time;
            vars->knock_back = blarg_space_commando_dying_vertical_knockback_speed * frame_time;
            vars->knock_min = 9.0f;
            vars->knock_max = 17.0f;
            trooper_start_knockback(moby,
                                    vars->anim,
                                    8,
                                    1,
                                    0,
                                    fast_arc_tan(*(f32 *)(hit + 0x10), *(f32 *)(hit + 0x14)));
            vars->flash_time = 0xFA;
            stop_moby_sound_8d10(moby, 0, -1);
            break;
        }
        trooper_anim_update(moby, vars->flash);
    }
    moby->hit_slot = 0xFF;
    trooper_anim_finish(moby, vars->flash);
    if (vars->idle_timer != 0) {
        vars->range = vars->far_range;
    } else {
        vars->range = vars->near_range;
    }
    path = (s32 *)walker_paths[vars->path];
    if (find_target_8d10(vars->range, moby, &vars->target, 0, 0, path + 4, *path) != 2) {
        if (vars->range < vector_distance_xy(&vars->home, &vars->target) ||
            absolute_float(moby->pos.z - vars->target.z) > 3.0f) {
            vars->mode = 2;
        }
        if (vars->target_moby != -1 &&
            is_point_inside_clip_volume(&vars->target, vars->target_moby) == 0) {
            vars->mode = 2;
        } else {
            vars->target_moby = -1;
        }
    }
    if (vars->start_frame == 0) {
        vars->start_frame = (*(int *)&D_001413D0_lui);
    }
    ground = probe_ground_height(&moby->pos, 0, 0.5f);
    if (ground != 0.0f) {
        switch (collision_hit_material()) {
        /* These material IDs all destroy the walker; their material names are unknown. */
        case GROUND_MATERIAL_DESTROYS_WALKER:
        case GROUND_MATERIAL_TRIGGERS_WALKER_EXPLOSION:
        case GROUND_MATERIAL_EXPLODES_AND_REMOVES_WALKER:
        case GROUND_MATERIAL_KILLS_WALKER_WITH_EXPLOSION:
        case GROUND_MATERIAL_EXPLODES_WALKER:
            moby->flags &= ~0x1000;
            stop_moby_sound_8d10(moby, 0, -1);
            spawn_explosion(moby,
                            D_L13_00160680,
                            0,
                            0.0f,
                            0.0f,
                            10,
                            3,
                            16,
                            4.0f,
                            2.0f,
                            9.0f,
                            1.0f,
                            -1,
                            15.0f,
                            1,
                            1,
                            -1,
                            0);
            mark_moby_for_removal(moby);
            return;
        default:
            if (moby->state < 0xF) {
                if (fast_arc_tan(coll_hit_1638.normal_z,
                                 vector_length_xy(&coll_hit_1638.normal_x)) > 0.6981317f) {
                    fast_vec_normalize(&push, &coll_hit_1638.normal_x, 0.05f);
                    fast_vec_add(&moby->pos, &moby->pos, &push);
                }
                if (ground < moby->pos.z) {
                    vars->fall_speed -= frame_time_sq * 9.8f;
                    moby->pos.z += vars->fall_speed;
                    if (moby->pos.z < ground) {
                        vars->fall_speed = 0.0f;
                        moby->pos.z = ground;
                    }
                } else if (moby->pos.z < ground) {
                    moby->pos.z += 0.05f;
                }
            }
            break;
        }
    } else {
        if (moby->state < 0xF) {
            vars->fall_speed -= frame_time_sq * 9.8f;
            moby->pos.z += vars->fall_speed;
        }
    }
    if (moby->pos.x < 2.0f || moby->pos.x > 1021.0f || moby->pos.y < 2.0f ||
        moby->pos.y > 1021.0f || moby->pos.z < 2.0f || moby->pos.z > 1021.0f) {
        mark_moby_for_removal(moby);
    }
}

extern char gemlik_trail_direction_source[] __asm__("D_L13_00174180");

extern float gemlik_trail_spread_degrees_gp __asm__("D_L13_00161E80") __attribute__((sda));

int D_L13_00161E84 = 150;

void initialize_gemlik_trail(struct Moby *moby) __asm__("FUN_L13_00309370");

void initialize_gemlik_trail(struct Moby *moby) {
    char *moby_vars = (char *)moby->pvars;
    float point_offset[4];
    int   i;
    probe_ground_height(&moby->pos, 0, 0.5f);
    fast_vec_normalize(moby_vars + 0x200, gemlik_trail_direction_source, 1.0f);
    moby_point_position(moby, 0, moby_vars + 0x1F0);
    for (i = 0; i < 16; i++) {
        float a = (convert_integer_to_float(i) * 0.0625f - 0.5f) *
                  (gemlik_trail_spread_degrees_gp * DEG_TO_RAD);
        point_offset[0] = fast_cos(fast_add_rotations(a, moby->rot.z)) * 0.5f;
        point_offset[1] = fast_sin(fast_add_rotations(a, moby->rot.z)) * 0.5f;
        point_offset[2] = 0.0f;
        fast_vec_add(moby_vars + 0x210 + i * 16, moby_vars + 0x1F0, point_offset);
        if (i == 0 || i == 15) {
            *(int *)(moby_vars + 0x21C + i * 16) = 0;
        } else {
            *(float *)(moby_vars + 0x21C + i * 16) = 1.0f;
        }
    }
    *(float *)(moby_vars + 0x1FC) = scale_game_frames(D_L13_00161E84);
}

extern float approach_value(float *, float, float) __asm__("FUN_00213ed8");

void spawn_point_table_sparks(struct Moby *moby) __asm__("FUN_L13_00309bc8");

void draw_point_table_quads(struct Moby *moby) __asm__("FUN_L13_003098d0");

extern float gemlik_trail_point_radial_speed_gp __asm__("D_L13_00161E7C") __attribute__((sda));

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) TrailPoint_94f8;

typedef struct {
    TrailPoint_94f8 vector;
    float           p, q;
    unsigned char   a, b;
    unsigned short  c;
} __attribute__((aligned(16))) TrailHit_94f8;

typedef struct {
    char            pad[0x1f0];
    TrailPoint_94f8 anchor;
    TrailPoint_94f8 direction;
    TrailPoint_94f8 point[17];
} TrailVars_94f8;

void update_gemlik_trail(struct Moby *moby) {
    TrailVars_94f8 *data = *(TrailVars_94f8 **)&moby->pvars;
    TrailPoint_94f8 next;
    union {
        struct {
            TrailPoint_94f8 delta;
            TrailPoint_94f8 scaled;
        } vectors;
        TrailHit_94f8 hit;
    } work;
    TrailPoint_94f8 upper, lower;
    int             timer, i, j, clamp_count;
    char           *clamp_cursor;
    float           prior_height, dot;

    timer = truncate_float_to_s32(data->anchor.w);
    fast_dec_timer(&timer);
    data->anchor.w = (float)timer;
    if (!timer)
        return;

    if (timer == 20) {
        for (clamp_cursor = (char *)data, clamp_count = 15; clamp_count >= 0;
             clamp_count--, clamp_cursor += 16) {
            if (*(float *)(clamp_cursor + 0x21c) >= 1.0f)
                *(float *)(clamp_cursor + 0x21c) = 0.98f;
        }
    }

    i = 15;
    do {
        fast_vec_sub(&next, &data->point[15 - i], &data->anchor);
        next.z = 0.0f;
        fast_vec_normalize(&next, &next, gemlik_trail_point_radial_speed_gp * frame_time);
        fast_vec_add(&next, &data->point[15 - i], &next);
        fast_vec_sub(&work.vectors.delta, &next, &moby->pos);
        dot = fast_vec_dot(&work.vectors.delta, &data->direction);
        fast_vec_normalize(&work.vectors.scaled, &data->direction, -dot + 0.35f);
        fast_vec_add(&work.vectors.delta, &work.vectors.delta, &work.vectors.scaled);
        prior_height = work.vectors.scaled.z;
        qcopy(&upper, &data->point[15 - i]);
        upper.z = upper.z + 1.0f;
        qcopy(&lower, &data->point[15 - i]);
        lower.z = lower.z - 3.0f;
        if (collision_line(&upper, &lower, 2, 0, 0) != 0 && prior_height < next.z) {
            approach_value(&next.z, coll_hit_1638.point.f[2] + 0.35f, frame_time * 4.0f);
        }
        if (1.0f <= data->point[15 - i].w && collision_line(&data->point[15 - i], &next, 2, 0, 0)) {
            next.w = 0.98f;
        } else {
            if (next.w < 1.0f)
                approach_value(&next.w, 0.0f, frame_time + frame_time);
        }
        qcopy(&data->point[15 - i], &next);
        i--;
    } while (i >= 0);

    j = 0;
    do {
        if (data->point[j].w >= 0.98f && data->point[j + 1].w >= 0.98f) {
            next.x = fast_cos(moby->rot.z);
            next.y = fast_sin(moby->rot.z);
            next.z = 1.0f;
            next.w = 5627.925f;
            make_hit_descriptor(&work.hit, moby, 0x10001, 1.0f, &next);
            work.hit.c = (unsigned short)moby->oclass;
            work.hit.a = 0;
            work.hit.b = 1;
            collision_line(&data->point[j], &data->point[j + 1], 0, moby, &work.hit);
        }
        j++;
    } while (j < 15);
    add_draw_callback((void *)draw_point_table_quads, moby);
    spawn_point_table_sparks(moby);
}

int blarg_space_commando_face_target_wait_frames = 0x5a;

/* GS quad packet for draw_geometry_quad: four corners, their colours and
   texture coordinates, then the giftag and register words. */
typedef struct {
    f32 pos[4][4];
    u32 col[4];
    f32 uv[4][2];
    u64 tag[4];
} QuadPacket;

s32 D_L13_00161E88 = 0;
s32 D_L13_00161E8C = 2;
s32 D_L13_00161E90 = 0;
s32 D_L13_00161E94 = 1;
s32 D_L13_00161E98 = 0x80;
s32 D_L13_00161E9C = 0xb;
s32 D_L13_00161EA0 = 0x40ff;
s32 D_L13_00161EA4 = 0x600040ff;
s32 D_L13_00161EA8 = 0xcccc;
s32 D_L13_00161EAC = 0x6000cccc;

/* Scrolling texture coordinates of the four corners. */
f32 D_L13_001D9B70[4][2] = {
    {0.0f, 1.0f},
    {0.0f, 0.0f},
    {1.0f, 1.0f},
    {1.0f, 0.0f},
};

extern u32  fast_tween_color(f32, s32, s32) __asm__("FUN_001fa6e0");
extern void draw_geometry_quad(void *, void *, s32) __asm__("FUN_001f7d30");

/* Draws the beam as 15 textured quads between consecutive points of the
   moby's point list (data + 0x210), each twice: once with the odd corners
   pulled toward the centre (data + 0x1F0) and once raised by 0.25. */
void draw_point_table_quads(struct Moby *moby) __asm__("FUN_L13_003098d0");

void draw_point_table_quads(struct Moby *moby) {
    char      *data = (char *)moby->pvars;
    QuadPacket pk[2];
    f32        identity_matrix[4][4];
    f32        corner_inset_offset[4];
    s32        i;
    s32        j;
    s32        source_point_byte_offset;
    f32       *corner_position;
    u64        regs;

    vu1_add_g_sregister(0x47, 0x5380B);
    i = 0;
    pk[0].tag[1] = get_effect_texture(D_L13_00161E9C);
    pk[1].tag[1] = get_effect_texture(D_L13_00161E9C);
    regs = D_L13_00161E88;
    regs |= (u64)D_L13_00161E8C << 2;
    regs |= (u64)D_L13_00161E90 << 4;
    regs |= (u64)D_L13_00161E94 << 6;
    regs |= (u64)D_L13_00161E98 << 32;
    /* Store order is load-bearing: it sets which register keeps regs. */
    pk[1].tag[2] = 0x0000FF9000000260ULL;
    pk[0].tag[2] = 0x0000FF9000000260ULL;
    pk[1].tag[0] = 0;
    pk[0].tag[0] = 0;
    pk[1].tag[3] = regs;
    pk[0].tag[3] = regs;
    set_identity_matrix(identity_matrix);
    do {
        D_L13_001D9B70[1][1] = D_L13_001D9B70[3][1];
        D_L13_001D9B70[3][1] = random_float_between(0.0f, 0.2f);
        for (j = 0; j < 4; j++) {
            corner_position = pk[0].pos[j];
            source_point_byte_offset = (i + j / 2) * 16;
            /* &data[...] (not data + off + ...) keeps retail's off + data add. */
            qcopy(corner_position, &data[source_point_byte_offset + 0x210]);
            /* The colour call in both arms: retail merges the two calls and
               leaves the address add in each arm. */
            if (j & 1) {
                fast_vec_sub(corner_inset_offset, corner_position, data + 0x1F0);
                fast_vec_normalize(corner_inset_offset, corner_inset_offset, 0.5f);
                fast_vec_sub(corner_position, corner_position, corner_inset_offset);
                corner_position[2] -= 0.125f;
                pk[0].col[j] = fast_tween_color(*(f32 *)(data + source_point_byte_offset + 0x21C),
                                                D_L13_00161EA8,
                                                D_L13_00161EAC);
            } else {
                pk[0].col[j] = fast_tween_color(*(f32 *)(data + source_point_byte_offset + 0x21C),
                                                D_L13_00161EA0,
                                                D_L13_00161EA4);
            }
            pk[0].uv[j][0] = D_L13_001D9B70[j][0];
            pk[0].uv[j][1] = D_L13_001D9B70[j][1];
            pk[1].uv[j][0] = D_L13_001D9B70[j][0];
            pk[1].uv[j][1] = D_L13_001D9B70[j][1];
        }
        draw_geometry_quad(pk, identity_matrix, 0);
        for (j = 0; j < 4; j++) {
            corner_position = pk[0].pos[j];
            if (j & 1) {
                corner_position[2] += 0.25f;
            }
        }
        i++;
        draw_geometry_quad(pk, identity_matrix, 0);
    } while (i < 15);
    vu1_add_g_sregister(0x47, 0x5360B);
}

/* Spawns a burst of effects for each pair of ready entries in the moby's table. */

extern float scale_time(float) __asm__("FUN_001f96b0");
int          D_L13_00161EB0 = 0x800040ff;
int          D_L13_00161EB4 = 0x4000cc80;
extern short point_spark_first_vector_w __asm__("D_L13_00161EB8") __attribute__((sda));
extern short point_spark_second_vector_w __asm__("D_L13_00161EBC") __attribute__((sda));
extern short point_spark_frame_count_range_start __asm__("D_L13_00161EC0") __attribute__((sda));
extern short point_spark_frame_count_range_end __asm__("D_L13_00161EC4") __attribute__((sda));
extern short point_spark_second_frame_count_range_start __asm__("D_L13_00161EC8")
    __attribute__((sda));
extern short point_spark_second_frame_count_range_end __asm__("D_L13_00161ECC")
    __attribute__((sda));
extern short point_spark_third_frame_count_range_start __asm__("D_L13_00161ED0")
    __attribute__((sda));
extern short point_spark_third_frame_count_range_end __asm__("D_L13_00161ED4") __attribute__((sda));
extern short point_spark_xy_multiplier_range_start __asm__("D_L13_00161ED8") __attribute__((sda));
extern short point_spark_xy_multiplier_range_end __asm__("D_L13_00161EDC") __attribute__((sda));
extern short point_spark_z_range_start __asm__("D_L13_00161EE0") __attribute__((sda));
extern short point_spark_z_range_end __asm__("D_L13_00161EE4") __attribute__((sda));
extern short point_spark_spawn_random_threshold __asm__("D_L13_00161EE8") __attribute__((sda));
int          D_L13_00161EEC = 24;

void spawn_point_table_sparks(struct Moby *moby) __asm__("FUN_L13_00309bc8");

void spawn_point_table_sparks(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    int   i, next_point_index;
    for (i = 0; i < 15; i = next_point_index) {
        float first_vector_angle_radians, second_vector_angle_radians;
        int   frame_count, second_frame_count, third_frame_count;
        int   spawn_position[4];
        float first_vector[4], second_vector[4];
        char *source_point_position;
        int   source_point_byte_offset = i * 16;
        next_point_index = i + 1;
        if (*(float *)(data + source_point_byte_offset + 0x21c) < 0.99f &&
            *(float *)(data - (-(next_point_index * 16)) + 0x21c) < 0.99f) {
            continue;
        }
        if (*(float *)&point_spark_spawn_random_threshold < random_float_between(0.0f, 1.0f)) {
            continue;
        }
        first_vector_angle_radians = random_angle_radians();
        second_vector_angle_radians = random_angle_radians();
        first_vector[0] = fast_cos(first_vector_angle_radians) *
                          random_float_between(*(float *)&point_spark_xy_multiplier_range_start,
                                               *(float *)&point_spark_xy_multiplier_range_end);
        first_vector[1] = fast_sin(first_vector_angle_radians) *
                          random_float_between(*(float *)&point_spark_xy_multiplier_range_start,
                                               *(float *)&point_spark_xy_multiplier_range_end);
        first_vector[2] = 0;
        second_vector[0] = fast_cos(second_vector_angle_radians) *
                           random_float_between(*(float *)&point_spark_xy_multiplier_range_start,
                                                *(float *)&point_spark_xy_multiplier_range_end);
        second_vector[1] = fast_sin(second_vector_angle_radians) *
                           random_float_between(*(float *)&point_spark_xy_multiplier_range_start,
                                                *(float *)&point_spark_xy_multiplier_range_end);
        second_vector[2] = 0;
        first_vector[2] = random_float_between(*(float *)&point_spark_z_range_start,
                                               *(float *)&point_spark_z_range_end);
        second_vector[2] = random_float_between(*(float *)&point_spark_z_range_start,
                                                *(float *)&point_spark_z_range_end);
        frame_count = truncate_float_to_s32(
            scale_time(random_float_between(*(float *)&point_spark_frame_count_range_start,
                                            *(float *)&point_spark_frame_count_range_end)));
        second_frame_count = truncate_float_to_s32(
            scale_time(random_float_between(*(float *)&point_spark_second_frame_count_range_start,
                                            *(float *)&point_spark_second_frame_count_range_end)));
        third_frame_count = truncate_float_to_s32(
            scale_time(random_float_between(*(float *)&point_spark_third_frame_count_range_start,
                                            *(float *)&point_spark_third_frame_count_range_end)));
        fast_vec_scale(first_vector, first_vector, 1.0f / (float)frame_count);
        first_vector[3] = *(float *)&point_spark_first_vector_w;
        fast_vec_scale(second_vector, second_vector, 1.0f / (float)third_frame_count);
        second_vector[3] = *(float *)&point_spark_second_vector_w;
        source_point_position = data + (source_point_byte_offset + 0x210);
        lerp_vector(spawn_position,
                    source_point_position,
                    source_point_position,
                    random_float_between(0.0f, 1.0f));
        spawn_spark(spawn_position,
                    first_vector,
                    second_vector,
                    D_L13_00161EB0,
                    D_L13_00161EB4,
                    frame_count,
                    second_frame_count,
                    third_frame_count,
                    D_L13_00161EEC);
    }
}
