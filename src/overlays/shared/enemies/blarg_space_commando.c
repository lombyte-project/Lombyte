/* Shared moby class blarg_space_commando: its update, hit handling and beam attack. */
#include "types.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "sda.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/rendering/effect_texture.h"
#include "rnc/rendering/vu1_registers.h"
#include "rnc/game/interpolated_record.h"
#include "rnc/mobys/animation/blend_moby_animation.h"

/* Movement controller block at vars + 0xC0 (FUN_L01_0026d930 init, FUN_L00_00258b50 step). */
typedef struct {
    s32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    u8  pad10[0x14];
    f32 unk24;
    u8  pad28[0x10];
    s32 flags; /* 0x38 */
} MoveCtl;

typedef struct {
    u8  pad0[0x18];
    u8  unk18;
    u8  unk19;
    s16 oclass;
    u8  pad1C[0x14];
} ShotInfo;

/* pvars of blarg_space_commando_update */
typedef struct {
    u8           pad0[0x20];
    f32          unk20;
    s16          unk24;
    u8           pad26[2];
    u8           unk28;
    u8           unk29;
    u8           pad2A[6];
    f32          unk30;
    u8           pad34[0xC];
    Vec4         unk40;
    u8           pad50[8];
    u8           unk58;
    u8           pad59;
    u8           unk5A;
    u8           pad5B[5];
    Vec4         vel; /* 0x60 */
    u8           pad70[0x50];
    MoveCtl      ctl; /* 0xC0 */
    u8           padFC[0x64];
    struct Moby *target; /* 0x160 */
    s32          unk164;
    u8           pad168[8];
    Vec4         home; /* 0x170 */
    u8           pad180[8];
    s32          unk188;
    f32          turn_speed; /* 0x18C */
    u8           pad190[0xC];
    s32          timer; /* 0x19C */
    u8           pad1A0[4];
    s32          path;   /* 0x1A4 */
    s32          route;  /* 0x1A8 */
    s32          volume; /* 0x1AC */
    u8           pad1B0[0x50];
    Vec4         unk200;
    u8           pad210[0x210];
    u8           unk420[1];
} Moby1068Vars;

extern f32 D_L06_00161FA8 __attribute__((sda));

#include "rnc/overlay/shared/enemies/blarg_space_commando_globals.h"

extern s32   ranged_attack_cooldown_frames_gp __asm__("D_L06_00161FC0") __attribute__((sda));
extern f32   melee_attack_distance_threshold_gp __asm__("D_L06_00161FC4") __attribute__((sda));
extern f32   vehicle_melee_windup_animation_speed_gp __asm__("D_L06_00161FCC") __attribute__((sda));
extern Vec4f D_L06_00167540;
extern s32  *D_L06_001B0C30[];
extern f32   D_L06_0015F580[] MACRO_ADDR;

extern void moby_point_position(struct Moby *, s32, void *) __asm__("FUN_L00_0024f7c8");
extern void FUN_L00_00258278(struct Moby *, f32, f32 *, f32, f32, f32);
extern s32  FUN_L00_00258b50(struct Moby *, MoveCtl *, void *, void *, f32);
extern void
make_hit_descriptor(ShotInfo *, struct Moby *, s32, void *, f32) __asm__("FUN_L00_00259888");
extern void sweep_hit_segment(
    void *, void *, void *, void *, struct Moby *, ShotInfo *, s32) __asm__("FUN_L00_00259d08");
extern void FUN_L00_0025a120(struct Moby *);
extern void approach_angle(f32 *, f32, f32 *, f32, f32, f32) __asm__("FUN_L00_0025be00");
extern s32  step_moby_motion(struct Moby *, Vec4 *) __asm__("FUN_L00_0025c698");
extern void spawn_explosion(struct Moby *,
                            Vec4 *,
                            void *,
                            f32,
                            f32,
                            s32,
                            s32,
                            s32,
                            f32,
                            f32,
                            f32,
                            f32,
                            s32,
                            f32,
                            s32,
                            s32,
                            s32,
                            s32) __asm__("FUN_L00_0025e450");
extern s32  FUN_L00_00261b48(s32, void *, void *, void *);
extern void
resolve_point_path_segment_xy_clearance(s32, void *, void *, f32) __asm__("FUN_L00_00261d78");
extern void FUN_L00_00263ac8(struct Moby *, s32, void *, f32);
extern void
FUN_L00_00263fd8(struct Moby *, s32, void *, void *, s32, s32, f32, f32 *, f32 *, f32 *);
extern void FUN_L01_0026d930(MoveCtl *);
extern void FUN_L06_002feb40_u(struct Moby *) __asm__("FUN_L06_002feb40");
extern void FUN_L06_002ff100_u(struct Moby *) __asm__("FUN_L06_002ff100");

void blarg_space_commando_update(struct Moby *moby) __asm__("FUN_L06_002fdbd0");

void blarg_space_commando_update(struct Moby *moby) {
    enum {
        BLARG_COMMANDO_STATE_INITIALIZE = 0,
        BLARG_COMMANDO_STATE_SELECT_ACTION = 1,
        BLARG_COMMANDO_STATE_RANGED_ATTACK_WINDUP = 2,
        BLARG_COMMANDO_STATE_RANGED_ATTACK_RECOVERY = 3,
        BLARG_COMMANDO_STATE_RANGED_ATTACK_COOLDOWN = 4,
        BLARG_COMMANDO_STATE_MELEE_ATTACK = 5,
        BLARG_COMMANDO_STATE_MELEE_RECOVERY = 6,
        BLARG_COMMANDO_STATE_CHASE_TARGET = 11,
        BLARG_COMMANDO_STATE_CHASE_MELEE_ATTACK = 12,
        BLARG_COMMANDO_STATE_RETURN_HOME = 13,
        BLARG_COMMANDO_STATE_APPROACH_ROUTE_DESTINATION = 14,
        BLARG_COMMANDO_STATE_KNOCKBACK = 15,
        BLARG_COMMANDO_STATE_DEATH_KNOCKBACK = 16
    };
    Moby1068Vars *vars = (Moby1068Vars *)moby->pvars;
    struct Moby  *target;
    s32          *route;
    Vec4          position_or_movement_vector;
    Vec4          movement_input_or_output_or_melee_origin;
    Vec4          movement_output_or_hit_vector_or_path_point;
    ShotInfo      shot;
    s32           near;
    s32           hit;
    f32           route_destination_distance;
    f32           angle;
    f32           frame;
    f32           cur_frame;

    FUN_L06_002feb40_u(moby);
    target = vars->target;
    FUN_L00_00263ac8(moby, 1, vars->unk420, 2.5f);
    if (moby->visible != 0 && fast_vec_dist(&moby->pos, &D_L06_00167540) < 27.0f) {
        FUN_L00_0025a120(moby);
        moby->shadow_range = 0x15;
    }
    moby->anim_speed = 1.0f;
    switch (moby->state) {
    case BLARG_COMMANDO_STATE_INITIALIZE:
        vars->unk28 = 1;
        vars->unk20 = 2.0f;
        vars->unk30 = 2.0f;
        vars->unk29 = 1;
        vars->unk24 = 2;
        vars->unk58 = truncate_float_to_s32(12.0f);
        vars->unk5A = truncate_float_to_s32(13.6f);
        FUN_L01_0026d930(&vars->ctl);
        vars->ctl.unk8 = 2.0f;
        vars->ctl.unkC = 0.5f;
        vars->ctl.unk0 = truncate_float_to_s32(D_L06_00161FA8 * 307.2f);
        vars->ctl.unk4 = D_L06_00161FA8 * 0.3f;
        vars->ctl.unk24 = movement_max_horizontal_speed_gp * frame_time;
        vars->ctl.flags |= 8;
        qcopy(&vars->home, &moby->pos);
        moby->state = 1;
        break;
    case BLARG_COMMANDO_STATE_SELECT_ACTION:
        if (vars->volume != -1 && vars->route != -1) {
            if (is_point_inside_clip_volume(&hero.motion.pos, vars->volume) != 0) {
                moby->state = 14;
                break;
            }
            if (vector_distance_xy(&moby->pos, &target->pos) > 7.0f) {
                break;
            }
        }
        approach_angle(&moby->rot.z,
                       fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y),
                       &vars->turn_speed,
                       frame_time_sq * 6.2831855f,
                       frame_time_sq * 6.2831855f,
                       frame_time * 3.1415927f);
        if (vars->unk164 != 2) {
            if (melee_attack_distance_threshold_gp < vector_distance_xy(&moby->pos, &target->pos)) {
                if (fast_difference_between_rotations(
                        moby->rot.z,
                        fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y)) <
                    0.17453292f) {
                    moby->state = 2;
                    if (moby->prev_seq != 3) {
                        blend_moby_animation(moby, 3, 0, scale_game_frames(10));
                    }
                }
            } else {
                moby->state = 5;
                if (moby->prev_seq != 6) {
                    blend_moby_animation(moby, 6, 0, scale_game_frames(10));
                }
            }
        } else if (vars->unk188 != 0) {
            moby->state = 11;
            if (moby->prev_seq != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(20));
            }
        }
        break;
    case BLARG_COMMANDO_STATE_APPROACH_ROUTE_DESTINATION:
        if (moby->prev_seq != 1) {
            blend_moby_animation(moby, 1, 0, scale_game_frames(12));
        }
        route = D_L06_001B0C30[vars->route];
        qcopy(&position_or_movement_vector, &route[*route * 4]);
        route_destination_distance = vector_distance_xy(&moby->pos, &position_or_movement_vector);
        if (route_destination_distance < 0.25f) {
            angle = fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y);
            FUN_L00_00258278(moby, angle, &vars->turn_speed, 0.05f, 0.3f, frame_time * 5.5850534f);
            if (fast_difference_between_rotations(angle, moby->rot.z) < 0.2617994f) {
                vars->turn_speed = 0.0f;
                vars->volume = -1;
                if (moby->prev_seq != 0) {
                    blend_moby_animation(moby, 0, 0, scale_game_frames(18));
                }
                moby->state = 1;
            }
        } else {
            FUN_L00_00258278(moby,
                             fast_arc_tan(position_or_movement_vector.f[0] - moby->pos.x,
                                          position_or_movement_vector.f[1] - moby->pos.y),
                             &vars->turn_speed,
                             0.025f,
                             0.3f,
                             frame_time * 5.5850534f);
            if (route_destination_distance > 2.0f) {
                route_destination_distance = 2.0f;
            }
            movement_input_or_output_or_melee_origin.f[0] =
                fast_cos(moby->rot.z) * route_destination_distance;
            movement_input_or_output_or_melee_origin.f[1] =
                fast_sin(moby->rot.z) * route_destination_distance;
            movement_input_or_output_or_melee_origin.f[2] = 0.0f;
            FUN_L00_00258b50(moby,
                             &vars->ctl,
                             &movement_input_or_output_or_melee_origin,
                             &movement_output_or_hit_vector_or_path_point,
                             1.0f);
        }
        break;
    case BLARG_COMMANDO_STATE_RANGED_ATTACK_WINDUP:
        approach_angle(&moby->rot.z,
                       fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y),
                       &vars->turn_speed,
                       frame_time_sq * 6.2831855f,
                       frame_time_sq * 6.2831855f,
                       frame_time * 3.1415927f);
        if (moby->anim_flags & 2) {
            FUN_L06_002ff100_u(moby);
            moby->state = 3;
            if (moby->prev_seq != 5) {
                blend_moby_animation(moby, 5, 0, scale_game_frames(10));
            }
        }
        break;
    case BLARG_COMMANDO_STATE_RANGED_ATTACK_RECOVERY:
        if (moby->anim_flags & 2) {
            if (vars->unk164 != 2) {
                moby->state = 4;
                if (moby->prev_seq != 4) {
                    blend_moby_animation(moby, 4, 0, scale_game_frames(20));
                }
                vars->timer = scale_game_frames(ranged_attack_cooldown_frames_gp);
            } else {
                moby->state = 1;
                if (moby->prev_seq != 0) {
                    blend_moby_animation(moby, 0, 0, scale_game_frames(20));
                }
            }
        }
        break;
    case BLARG_COMMANDO_STATE_RANGED_ATTACK_COOLDOWN:
        approach_angle(&moby->rot.z,
                       fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y),
                       &vars->turn_speed,
                       frame_time_sq * 6.2831855f,
                       frame_time_sq * 6.2831855f,
                       frame_time * 3.1415927f);
        if (fast_dec_timer(&vars->timer) != 0) {
            FUN_L06_002ff100_u(moby);
            moby->state = 3;
            if (moby->prev_seq != 5) {
                blend_moby_animation(moby, 5, 0, scale_game_frames(5));
            }
        } else if (vector_distance_xy(&moby->pos, &target->pos) <
                   melee_attack_distance_threshold_gp) {
            moby->state = 5;
            if (moby->prev_seq != 6) {
                blend_moby_animation(moby, 6, 0, scale_game_frames(10));
            }
        } else if (vars->unk188 != 0) {
            moby->state = 11;
            if (moby->prev_seq != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(20));
            }
        }
        break;
    case BLARG_COMMANDO_STATE_MELEE_ATTACK:
        approach_angle(&moby->rot.z,
                       fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y),
                       &vars->turn_speed,
                       frame_time_sq * 6.2831855f,
                       frame_time_sq * 6.2831855f,
                       frame_time * 3.1415927f);
        moby_point_position(moby, 0, &position_or_movement_vector);
        qcopy(&movement_input_or_output_or_melee_origin, &moby->pos);
        movement_input_or_output_or_melee_origin.f[2] += 0.3f;
        cur_frame = compute_interpolated_record_value(moby);
        if (hero.vehicle_mode == 1 && cur_frame < 9.0f) {
            moby->anim_speed = vehicle_melee_windup_animation_speed_gp;
        }
        if (cur_frame >= 9.0f && cur_frame <= 15.0f) {
            movement_output_or_hit_vector_or_path_point.f[0] = fast_cos(moby->rot.z);
            movement_output_or_hit_vector_or_path_point.f[1] = fast_sin(moby->rot.z);
            movement_output_or_hit_vector_or_path_point.f[2] = 1.0f;
            movement_output_or_hit_vector_or_path_point.f[3] = 5627.925f;
            make_hit_descriptor(&shot, moby, 1, &movement_output_or_hit_vector_or_path_point, 1.0f);
            shot.unk18 = 0;
            shot.unk19 = 1;
            shot.oclass = moby->oclass;
            sweep_hit_segment(&position_or_movement_vector,
                              &movement_input_or_output_or_melee_origin,
                              &vars->unk200,
                              &movement_input_or_output_or_melee_origin,
                              moby,
                              &shot,
                              5);
        }
        qcopy(&vars->unk200, &position_or_movement_vector);
        if (moby->anim_flags & 2) {
            moby->state = 6;
            moby->command = 1;
            if (moby->prev_seq != 2) {
                blend_moby_animation(moby, 2, 0, scale_game_frames(10));
            }
        }
        break;
    case BLARG_COMMANDO_STATE_MELEE_RECOVERY:
        if (moby->anim_flags & 2) {
            moby->state = moby->command;
            if (moby->prev_seq != 0) {
                blend_moby_animation(moby, 0, 0, scale_game_frames(20));
            }
        }
        break;
    case BLARG_COMMANDO_STATE_CHASE_TARGET:
        near = 0;
        FUN_L00_00258278(moby,
                         fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y),
                         &vars->turn_speed,
                         0.05f,
                         0.3f,
                         frame_time * 6.2831855f);
        position_or_movement_vector.f[0] = 2.0f * fast_cos(moby->rot.z);
        position_or_movement_vector.f[1] = 2.0f * fast_sin(moby->rot.z);
        position_or_movement_vector.f[2] = 0.0f;
        FUN_L00_00258b50(moby,
                         &vars->ctl,
                         &position_or_movement_vector,
                         &movement_input_or_output_or_melee_origin,
                         1.0f);
        resolve_point_path_segment_xy_clearance(vars->path, &moby->pos, &moby->pos, 0.75f);
        if (FUN_L00_00261b48(vars->path,
                             &moby->pos,
                             &target->pos,
                             &movement_output_or_hit_vector_or_path_point) != 0 &&
            fast_vec_dist(&moby->pos, &movement_output_or_hit_vector_or_path_point) < 0.8f) {
            near = 1;
        }
        if (vars->unk188 == 0) {
            moby->state = 13;
        } else if (near != 0) {
            moby->state = 12;
            if (moby->prev_seq != 6) {
                blend_moby_animation(moby, 6, 0, scale_game_frames(10));
            }
        } else if (vector_distance_xy(&moby->pos, &target->pos) <
                   melee_attack_distance_threshold_gp) {
            moby->state = 12;
            if (moby->prev_seq != 6) {
                blend_moby_animation(moby, 6, 0, scale_game_frames(10));
            }
        }
        break;
    case BLARG_COMMANDO_STATE_CHASE_MELEE_ATTACK:
        moby_point_position(moby, 0, &position_or_movement_vector);
        qcopy(&movement_input_or_output_or_melee_origin, &moby->pos);
        movement_input_or_output_or_melee_origin.f[2] += 1.5f;
        frame = compute_interpolated_record_value(moby);
        if (frame >= 9.0f && frame <= 15.0f) {
            movement_output_or_hit_vector_or_path_point.f[0] = fast_cos(moby->rot.z);
            movement_output_or_hit_vector_or_path_point.f[1] = fast_sin(moby->rot.z);
            movement_output_or_hit_vector_or_path_point.f[2] = 1.0f;
            movement_output_or_hit_vector_or_path_point.f[3] = 5627.925f;
            make_hit_descriptor(&shot, moby, 1, &movement_output_or_hit_vector_or_path_point, 1.0f);
            shot.unk18 = 0;
            shot.unk19 = 1;
            shot.oclass = moby->oclass;
            sweep_hit_segment(&position_or_movement_vector,
                              &movement_input_or_output_or_melee_origin,
                              &vars->unk200,
                              &movement_input_or_output_or_melee_origin,
                              moby,
                              &shot,
                              5);
        }
        qcopy(&vars->unk200, &position_or_movement_vector);
        if (moby->anim_flags & 2) {
            moby->state = 11;
            if (moby->prev_seq != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(20));
            }
        }
        break;
    case BLARG_COMMANDO_STATE_RETURN_HOME:
        FUN_L00_00258278(moby,
                         fast_arc_tan(vars->home.f[0] - moby->pos.x, vars->home.f[1] - moby->pos.y),
                         &vars->turn_speed,
                         0.05f,
                         0.3f,
                         frame_time * 6.2831855f);
        position_or_movement_vector.f[0] = 2.0f * fast_cos(moby->rot.z);
        position_or_movement_vector.f[1] = 2.0f * fast_sin(moby->rot.z);
        position_or_movement_vector.f[2] = 0.0f;
        if (fast_difference_between_rotations(
                moby->rot.z,
                fast_arc_tan(vars->home.f[0] - moby->pos.x, vars->home.f[1] - moby->pos.y)) <
            0.7853982f) {
            FUN_L00_00258b50(moby,
                             &vars->ctl,
                             &position_or_movement_vector,
                             &movement_input_or_output_or_melee_origin,
                             1.0f);
            resolve_point_path_segment_xy_clearance(vars->path, &moby->pos, &moby->pos, 0.75f);
        }
        if (fast_vec_dist(&vars->home, &moby->pos) < 1.0f) {
            moby->state = 1;
            if (moby->prev_seq != 0) {
                blend_moby_animation(moby, 0, 0, scale_game_frames(20));
            }
        }
        break;
    case BLARG_COMMANDO_STATE_KNOCKBACK:
        hit = step_moby_motion(moby, &vars->vel);
        if (hit & 0x40) {
            moby->state = 1;
            if (moby->prev_seq != 0) {
                blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            }
        } else if (hit & 0x120) {
            mark_moby_for_removal(moby);
        }
        break;
    case BLARG_COMMANDO_STATE_DEATH_KNOCKBACK:
        if (step_moby_motion(moby, &vars->vel) & 0x160) {
            s32 none;

            qcopy(&position_or_movement_vector, &moby->pos);
            position_or_movement_vector.f[2] += 1.0f;
            none = -1;
            spawn_explosion(moby,
                            &vars->unk40,
                            &position_or_movement_vector,
                            0.0f,
                            0.0f,
                            5,
                            2,
                            4,
                            2.0f,
                            1.0f,
                            9.0f,
                            1.0f,
                            -1,
                            15.0f,
                            1,
                            1,
                            none,
                            0);
            FUN_L00_00263fd8(moby,
                             0x6A6,
                             &moby->pos,
                             &moby->rot,
                             0,
                             0,
                             0.0f,
                             D_L06_0015F580,
                             D_L06_0015F580,
                             D_L06_0015F580);
            FUN_L00_00263fd8(moby,
                             0x6A7,
                             &moby->pos,
                             &moby->rot,
                             0,
                             0,
                             0.0f,
                             D_L06_0015F580,
                             D_L06_0015F580,
                             D_L06_0015F580);
            FUN_L00_00263fd8(moby,
                             0x6A8,
                             &moby->pos,
                             &moby->rot,
                             0,
                             0,
                             0.0f,
                             D_L06_0015F580,
                             D_L06_0015F580,
                             D_L06_0015F580);
            mark_moby_for_removal(moby);
        }
        break;
    }
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002FF000.c: func_L06_002FFF70), where it is exact; names translated to the US level program. */

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern char *FUN_L06_00300c60_c(int unused, char *pa, char *pb) __asm__("FUN_L06_00300c60");

#include "rnc/data_labels.h"

extern f32           D_0015ED6C;
extern float         FUN_L04_001f1978(float);
extern float         randf_sym(float lo, float hi) __asm__("FUN_L00_00257c48");
extern int           FUN_L00_0025ff38(float, char *, void *, int, int, void *, int);
extern int           fun_00213928(void *, void *, void *, int, int *, float *, int, int);
extern int          *D_L06_001B0C30[];
extern int           current_level_index_gp __asm__("D_0015ED84") __attribute__((sda));
extern float         D_L06_00161FA8 __attribute__((sda));
extern unsigned char D_0013D408[];
extern void          FUN_L00_00257470(void *, int, int);
extern void
trooper_start_knockback(void *, void *, float, int, int, int) __asm__("FUN_L00_0025c558");
extern void trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern void queue_dialogue_message(int, int) __asm__("FUN_L00_00263d40");
void        update_blarg_space_commando_beam(void *owner_moby) __asm__("FUN_L06_002ff2c8");

void        update_blarg_space_commando_beam(void *arg) __asm__("FUN_L06_002ff2c8");
extern void allocate_voice_for_bank_entry(int, int, int) __asm__("FUN_0022db10");

void process_blarg_space_commando_hits(char *moby) __asm__("FUN_L06_002feb40");

void process_blarg_space_commando_hits(char *moby) {
    enum {
        HIT_RESPONSE_WITHOUT_KNOCKBACK = 0,
        FATAL_HIT_RESPONSE = 1,
        FRAGMENTING_FATAL_HIT_RESPONSE = 2,
        NONFATAL_KNOCKBACK_HIT_RESPONSE = 3,
        TIMED_NONFATAL_KNOCKBACK_HIT_RESPONSE = 4,
        RECOVERABLE_KNOCKBACK_HIT_RESPONSE = 5,
        TIMED_RECOVERABLE_KNOCKBACK_HIT_RESPONSE = 6,
        SCALED_DURATION_KNOCKBACK_HIT_RESPONSE = 7,
        TIMED_KNOCKBACK_HIT_RESPONSE = 8,
        FRAME_SCALED_KNOCKBACK_HIT_RESPONSE = 9,
        SIXTY_FRAME_KNOCKBACK_HIT_RESPONSE = 10,
        HIT_RESPONSE_PRESERVING_MOBY_STATE = 11
    };
    extern unsigned char D_0013D408[];
    char                *vars;
    char                *hit_record;
    char                *fragment;
    int                 *route_record;
    int                  hit_response_index;
    int                  k;
    int                  bits;
    float                vv[4];
    float                fragment_velocity[4];
    int                  hit;
    float                dt;
    float                remaining_health;
    float                lo, hi, spd, minimum_fragment_speed, pull;

    vars = ((char *)MOBY(moby)->pvars);
    MOBY(moby)->scale = *(float *)(((char *)MOBY(moby)->pclass) + 0x24) * D_L06_00161FA8;
    update_blarg_space_commando_beam(moby);
    if (*(int *)(vars + 0x38) != 0) {
        *(int *)(vars + 0x188) =
            truncate_float_to_s32(FUN_L04_001f1978(random_float_between(180.0f, 240.0f)));
        *(int *)(vars + 0x38) = 0;
    }
    fast_dec_timer((s32 *)(vars + 0x188));
    *(int *)&dt = 0;
    hit_record = find_moby_hit(moby, 0x330000, 0);
    hit_response_index = fun_00213928(moby, hit_record, vars + 0x20, 0, &hit, &dt, 0, 4);
    if (hit != 1 && (unsigned char)moby[0x20] != 0x10) {
        if (current_level_index_gp == 0xA && hit_record != 0 &&
            *(char **)(hit_record + 0x20) != 0 &&
            *(short *)(*(char **)(hit_record + 0x20) + 0xA6) == 0x31A && D_0013D408[0x11] == 0) {
            D_0013D408[0x11] = 1;
            allocate_voice_for_bank_entry(1, 0, 0);
            queue_dialogue_message(0x53D6, -1);
        }
        remaining_health = *(float *)(vars + 0x20) - dt;
        *(float *)(vars + 0x20) = remaining_health;
        if (remaining_health <= 0.0f)
            hit_response_index = 1;
        *(int *)(vars + 0x80) = 0x1F4;
        *(float *)(vars + 0x88) = 0.5f;
        switch (hit_response_index) {
        case NONFATAL_KNOCKBACK_HIT_RESPONSE:
        case TIMED_NONFATAL_KNOCKBACK_HIT_RESPONSE:
        case RECOVERABLE_KNOCKBACK_HIT_RESPONSE:
        case TIMED_RECOVERABLE_KNOCKBACK_HIT_RESPONSE:
        case SCALED_DURATION_KNOCKBACK_HIT_RESPONSE:
        case TIMED_KNOCKBACK_HIT_RESPONSE:
        case FRAME_SCALED_KNOCKBACK_HIT_RESPONSE:
        case SIXTY_FRAME_KNOCKBACK_HIT_RESPONSE: {
            char *q;
            int   knockback_frames;
            float b, a;
            moby[0x20] = 0xF;
            q = vars + 0x60;
            knockback_frames = scale_game_frames(0x3C);
            a = nonfatal_knockback_horizontal_speed_gp * D_0015ED6C;
            b = nonfatal_knockback_vertical_speed_gp * D_0015ED6C;
            *(short *)(vars + 0x26) = knockback_frames;
            *(float *)(vars + 0x7C) = b;
            *(float *)(vars + 0x78) = a;
            *(float *)(vars + 0xB0) = 8.0f;
            *(float *)(vars + 0xB4) = 15.0f;
            trooper_start_knockback(
                moby,
                q,
                fast_arc_tan(*(float *)(hit_record + 0x10), *(float *)(hit_record + 0x14)),
                7,
                1,
                0);
            *(unsigned char *)(vars + 0x117) = 0xFA;
            break;
        }
        case FATAL_HIT_RESPONSE:
        case FRAGMENTING_FATAL_HIT_RESPONSE: {
            char *q;
            float a, b;
            moby[0x20] = 0x10;
            q = vars + 0x60;
            a = fatal_knockback_horizontal_speed_gp * D_0015ED6C;
            b = fatal_knockback_vertical_speed_gp * D_0015ED6C;
            MOBY(moby)->flags &= 0xEFFF;
            *(float *)(vars + 0x78) = a;
            *(float *)(vars + 0x7C) = b;
            *(float *)(vars + 0xB0) = 9.0f;
            *(float *)(vars + 0xB4) = 18.0f;
            bits = 0;
            lo = *(float *)&bits;
            bits = 0x3E4CCCCD;
            hi = *(float *)&bits;
            a = fast_arc_tan(*(float *)(hit_record + 0x10), *(float *)(hit_record + 0x14));
            bits = 0x40400000;
            minimum_fragment_speed = *(float *)&bits;
            trooper_start_knockback(moby, q, a, 9, 1, 0);
            bits = 0x40E00000;
            pull = *(float *)&bits;
            for (k = 0; k < 10; k++) {
                float fragment_scatter_angle;
                qcopy(vv, moby + 0x10);
                vv[2] = vv[2] + 2.5f;
                vv[0] = vv[0] + randf_sym(lo, hi);
                vv[1] = vv[1] + randf_sym(lo, hi);
                vv[2] = vv[2] + randf_sym(lo, hi);
                fragment_scatter_angle = random_angle_radians();
                spd = random_float_between(minimum_fragment_speed, 5.7f) * D_0015ED6C;
                fragment_velocity[0] = fast_cos(fragment_scatter_angle) * spd;
                fragment_velocity[1] = fast_sin(fragment_scatter_angle) * spd;
                fragment_velocity[2] = lo;
                fragment_velocity[0] =
                    fragment_velocity[0] + fast_cos(fast_arc_tan(*(float *)(hit_record + 0x10),
                                                                 *(float *)(hit_record + 0x14))) *
                                               (D_0015ED6C * pull);
                fragment_velocity[1] =
                    fragment_velocity[1] + fast_sin(fast_arc_tan(*(float *)(hit_record + 0x10),
                                                                 *(float *)(hit_record + 0x14))) *
                                               (D_0015ED6C * pull);
                fragment_velocity[2] =
                    random_float_between(minimum_fragment_speed, 8.5f) * D_0015ED6C;
                fragment = FUN_L06_00300c60_c((int)moby, (char *)vv, (char *)fragment_velocity);
                if (fragment != 0) {
                    *(float *)(fragment + 0x2C) = *(float *)(fragment + 0x2C) * 0.4f;
                }
            }
            *(unsigned char *)(vars + 0x117) = 0xFA;
            FUN_L00_00257470(moby, 0, -1);
            break;
        }
        case HIT_RESPONSE_WITHOUT_KNOCKBACK:
        case HIT_RESPONSE_PRESERVING_MOBY_STATE:
        default:
            break;
        }
        trooper_anim_update(moby, (short *)(vars + 0x110));
    }
    MOBY(moby)->hit_slot = 0xFF;
    trooper_anim_finish(moby, vars + 0x110);
    if (*(int *)(vars + 0x188) != 0) {
        *(float *)(vars + 0x198) = base_home_distance_limit_gp + 6.0f;
    } else {
        *(float *)(vars + 0x198) = base_home_distance_limit_gp;
    }
    {
        char *q = vars + 0x120;
        route_record = D_L06_001B0C30[*(int *)(vars + 0x180)];
        if (FUN_L00_0025ff38(
                *(float *)(vars + 0x198), moby, q, 0, 0, route_record + 4, *route_record) != 2) {
            if (*(float *)(vars + 0x198) < vector_distance_xy(vars + 0x170, vars + 0x120)) {
                *(int *)(vars + 0x164) = 2;
            } else if (3.0f < absolute_float(MOBY(moby)->pos.z - *(float *)(vars + 0x128))) {
                *(int *)(vars + 0x164) = 2;
            }
        }
    }
    if (*(int *)(vars + 0x160) == 0) {
        *(int *)(vars + 0x160) = *(int *)(((char *)&D_001413D0));
    }
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002FF000.c: func_L06_00300530), where it is exact; names translated to the US level program. */

extern char D_L06_00174600_lui[] __asm__("D_L06_00174600") NOT_SDA;

extern float beam_spread_degrees_gp __asm__("D_L06_00161FD4") __attribute__((sda));
extern int   beam_duration_frames_gp __asm__("D_L06_00161FD8") __attribute__((sda));
void         initialize_blarg_space_commando_beam(char *moby) __asm__("FUN_L06_002ff100");

void initialize_blarg_space_commando_beam(char *moby) {
    char *vars = ((char *)MOBY(moby)->pvars);
    float point_offset[4];
    int   i;
    probe_ground_height(moby + 0x10, 0, 0.5f);
    fast_vec_normalize(vars + 0x220, D_L06_00174600_lui, 1.0f);
    moby_point_position(moby, 0, vars + 0x210);
    *(float *)(vars + 0x218) = MOBY(moby)->pos.z;
    for (i = 0; i < 16; i++) {
        float angle_offset = convert_integer_to_float(i) * 0.0625f - 0.5f;
        angle_offset = angle_offset * (beam_spread_degrees_gp * 0.017453292f);
        point_offset[0] = fast_cos(fast_add_rotations(angle_offset, MOBY(moby)->rot.z)) * 0.2f;
        point_offset[1] = fast_sin(fast_add_rotations(angle_offset, MOBY(moby)->rot.z)) * 0.2f;
        point_offset[2] = 0.15f;
        fast_vec_add((float *)(vars + 0x230 + i * 16), (float *)(vars + 0x210), point_offset);
        if (i == 0 || i == 15) {
            float *endpoint_color_blend_factor = (float *)(vars + 0x23C + i * 16);
            *endpoint_color_blend_factor = *endpoint_color_blend_factor = 0.0f;
        } else {
            *(float *)(vars + 0x23C + i * 16) = 1.0f;
        }
    }
    *(float *)(vars + 0x21C) = (float)scale_game_frames(beam_duration_frames_gp);
}
/* Shared moby update: steps the timer at 0x21C, spaces 16 entries, and runs the pair checks. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002FF000.c: func_L06_003006F8), where it is exact; names translated to the US level program. */

typedef struct Hero Hero;

typedef struct Moby Moby;

typedef struct {
    float frames, y, z, w;
} FanPt;

typedef struct {
    char  pad0[0x210];
    FanPt base;    /* 0x210; w = timer */
    FanPt dir;     /* 0x220 */
    FanPt pts[16]; /* 0x230 */
} FanData;

extern float approach_value(float *, float, float);
extern int   collision_line(void *, void *, int, void *, void *);
extern f32   beam_point_horizontal_speed_gp __asm__("D_L06_00161FD0") __attribute__((sda));
void         draw_blarg_space_commando_beam(char *moby) __asm__("FUN_L06_002ff680");

void spawn_blarg_space_commando_beam_sparks(char *moby) __asm__("FUN_L06_002ff978");

extern void add_draw_callback(void *, void *) __asm__("FUN_001f4600");

void update_blarg_space_commando_beam(void *owner_moby) {
    char    *moby = owner_moby;
    FanData *data = ((FanData *)MOBY(moby)->pvars);
    int      frames;
    float    point[4];
    float    relative[4];
    ShotInfo shot;
    float    step;
    float    alignment;
    int      i, j, next, hit_type;
    int      remaining;

    frames = truncate_float_to_s32(data->base.w);
    fast_dec_timer((s32 *)&frames);
    data->base.w = (float)frames;
    if (frames == 0) {
        return;
    }
    if (frames == 20) {
        char *weights = (char *)data;
        for (remaining = 15; remaining >= 0; remaining--) {
            if (1.0f <= *(float *)(weights + 0x23C)) {
                *(float *)(weights + 0x23C) = 0.98f;
            }
            weights += 0x10;
        }
    }

    for (i = 0; i < 16; i++) {
        char *entry;
        if (hero.vehicle_mode == 1) {
            step = (beam_point_horizontal_speed_gp * frame_time) * 0.5f;
        } else {
            step = beam_point_horizontal_speed_gp * frame_time;
        }
        fast_vec_sub(point, &data->pts[i], &data->base);
        point[2] = 0.0f;
        fast_vec_normalize(point, point, step);
        fast_vec_add(point, (float *)&data->pts[i], point);
        fast_vec_sub(relative, point, moby + 0x10);
        alignment = fast_vec_dot(relative, &data->dir);
        if (0.4f < alignment) {
            point[2] = point[2] - (frame_time + frame_time);
        } else if (alignment < 0.3f) {
            point[2] = point[2] + (frame_time + frame_time);
        }
        entry = (char *)data + i * 16;
        if (1.0f <= *(float *)(entry + 0x23C) && collision_line(&data->pts[i], point, 2, 0, 0)) {
            point[3] = 0.98f;
        } else if (point[3] < 1.0f) {
            approach_value(&point[3], 0.0f, frame_time + frame_time);
        }
        qcopy(&data->pts[i], point);
    }

    j = 0;
    do {
        char *entry = (char *)data + j * 16;
        char *next_entry;
        next = j + 1;
        if (0.6f <= *(float *)(entry + 0x23C)) {
            next_entry = (char *)data + next * 16;
            if (0.6f <= *(float *)(next_entry + 0x23C)) {
                point[0] = fast_cos(MOBY(moby)->rot.z);
                point[1] = fast_sin(MOBY(moby)->rot.z);
                point[2] = 1.0f;
                point[3] = 5627.925f;
                hit_type = 0x10000;
                if (1.0f <= *(float *)(entry + 0x23C) && 1.0f <= *(float *)(next_entry + 0x23C)) {
                    hit_type = 0x10001;
                }
                make_hit_descriptor(&shot, (struct Moby *)moby, hit_type, point, 1.0f);
                shot.oclass = MOBY(moby)->oclass;
                shot.unk18 = 0;
                shot.unk19 = 1;
                collision_line(&data->pts[j], &data->pts[j + 1], 0, moby, &shot);
            }
        }
        j = next;
    } while (j < 15);

    add_draw_callback((void *)draw_blarg_space_commando_beam, moby);
    spawn_blarg_space_commando_beam_sparks(moby);
}

/* GS quad packet for draw_geometry_quad: four corners, their colours and
   texture coordinates, then the giftag and register words. */
typedef struct {
    f32 pos[4][4];
    u32 col[4];
    f32 uv[4][2];
    u64 tag[4];
} QuadPacket_f680;

extern s32 beam_effect_texture_index_gp __asm__("D_L06_00161FF0") __attribute__((sda));
extern s32 beam_outer_corner_color_at_zero_weight_gp __asm__("D_L06_00161FF4") __attribute__((sda));
extern s32 beam_outer_corner_color_at_unit_weight_gp __asm__("D_L06_00161FF8") __attribute__((sda));
extern s32 beam_inset_corner_color_at_zero_weight_gp __asm__("D_L06_00161FFC") __attribute__((sda));
extern s32 beam_inset_corner_color_at_unit_weight_gp __asm__("D_L06_00162000") __attribute__((sda));
/* Scrolling texture coordinates of the four corners. */
extern f32 beam_quad_corner_uvs[4][2] __asm__("D_L06_001F2E90");

extern u32  fast_tween_color(f32, s32, s32) __asm__("FUN_001fa6e0");
extern void draw_geometry_quad(void *, void *, s32) __asm__("FUN_001f7d30");

/* Draws the beam as 15 textured quads between consecutive points of the
   moby's point list (data + 0x230), each twice: once with the odd corners
   pulled toward the centre (data + 0x210) and once raised by 0.25. */
void draw_blarg_space_commando_beam(char *moby) {
    char           *data = ((char *)MOBY(moby)->pvars);
    QuadPacket_f680 pk[2];
    f32             quad_identity_matrix[4][4];
    f32             corner_inset_vector[4];
    s32             i;
    s32             j;
    s32             source_point_byte_offset;
    f32            *corner_position;
    u64             regs;

    vu1_add_g_sregister(0x47, 0x5380B);
    i = 0;
    pk[0].tag[1] = get_effect_texture(beam_effect_texture_index_gp);
    pk[1].tag[1] = get_effect_texture(beam_effect_texture_index_gp);
    regs = beam_blend_minuend_selector_gp;
    regs |= (u64)beam_blend_subtrahend_selector_gp << 2;
    regs |= (u64)beam_blend_alpha_multiplier_selector_gp << 4;
    regs |= (u64)beam_blend_addend_selector_gp << 6;
    regs |= (u64)beam_blend_fixed_alpha_gp << 32;
    /* Store order is load-bearing: it sets which register keeps regs. */
    pk[1].tag[2] = 0x0000FF9000000260ULL;
    pk[0].tag[2] = 0x0000FF9000000260ULL;
    pk[1].tag[0] = 0;
    pk[0].tag[0] = 0;
    pk[1].tag[3] = regs;
    pk[0].tag[3] = regs;
    set_identity_matrix(quad_identity_matrix);
    do {
        beam_quad_corner_uvs[1][1] = beam_quad_corner_uvs[3][1];
        beam_quad_corner_uvs[3][1] = random_float_between(0.0f, 0.2f);
        for (j = 0; j < 4; j++) {
            corner_position = pk[0].pos[j];
            source_point_byte_offset = (i + j / 2) * 16;
            /* &data[...] (not data + off + ...) keeps retail's off + data add. */
            qcopy(corner_position, &data[source_point_byte_offset + 0x230]);
            /* The colour call in both arms: retail merges the two calls and
               leaves the address add in each arm. */
            if (j & 1) {
                fast_vec_sub(corner_inset_vector, corner_position, data + 0x210);
                fast_vec_normalize(corner_inset_vector, corner_inset_vector, 0.5f);
                fast_vec_sub(corner_position, corner_position, corner_inset_vector);
                corner_position[2] -= 0.125f;
                pk[0].col[j] = fast_tween_color(*(f32 *)(data + source_point_byte_offset + 0x23C),
                                                beam_inset_corner_color_at_zero_weight_gp,
                                                beam_inset_corner_color_at_unit_weight_gp);
            } else {
                pk[0].col[j] = fast_tween_color(*(f32 *)(data + source_point_byte_offset + 0x23C),
                                                beam_outer_corner_color_at_zero_weight_gp,
                                                beam_outer_corner_color_at_unit_weight_gp);
            }
            pk[0].uv[j][0] = beam_quad_corner_uvs[j][0];
            pk[0].uv[j][1] = beam_quad_corner_uvs[j][1];
            pk[1].uv[j][0] = beam_quad_corner_uvs[j][0];
            pk[1].uv[j][1] = beam_quad_corner_uvs[j][1];
        }
        draw_geometry_quad(pk, quad_identity_matrix, 0);
        for (j = 0; j < 4; j++) {
            corner_position = pk[0].pos[j];
            if (j & 1) {
                corner_position[2] += 0.25f;
            }
        }
        i++;
        draw_geometry_quad(pk, quad_identity_matrix, 0);
    } while (i < 15);
    vu1_add_g_sregister(0x47, 0x5360B);
}

/* Spawns a burst of effects for each pair of ready entries in the moby's table. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002FF000.c: func_L06_00300DA8), where it is exact; names translated to the US level program. */

extern char *
spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");
extern float scale_time(float) __asm__("FUN_001f96b0");
extern float beam_spark_first_vector_w_gp __asm__("D_L06_0016200C") __attribute__((sda));
extern float beam_spark_second_vector_w_gp __asm__("D_L06_00162010") __attribute__((sda));
extern float beam_spark_frame_count_range_start_gp __asm__("D_L06_00162014") __attribute__((sda));
extern float beam_spark_frame_count_range_end_gp __asm__("D_L06_00162018") __attribute__((sda));
extern float beam_spark_second_frame_count_range_start_gp __asm__("D_L06_0016201C")
    __attribute__((sda));
extern float beam_spark_second_frame_count_range_end_gp __asm__("D_L06_00162020")
    __attribute__((sda));
extern float beam_spark_third_frame_count_range_start_gp __asm__("D_L06_00162024")
    __attribute__((sda));
extern float beam_spark_third_frame_count_range_end_gp __asm__("D_L06_00162028")
    __attribute__((sda));
extern float beam_spark_horizontal_scale_range_start_gp __asm__("D_L06_0016202C")
    __attribute__((sda));
extern float beam_spark_horizontal_scale_range_end_gp __asm__("D_L06_00162030")
    __attribute__((sda));
extern float beam_spark_vector_z_range_start_gp __asm__("D_L06_00162034") __attribute__((sda));
extern float beam_spark_vector_z_range_end_gp __asm__("D_L06_00162038") __attribute__((sda));
extern float beam_spark_spawn_probability_gp __asm__("D_L06_0016203C") __attribute__((sda));

void spawn_blarg_space_commando_beam_sparks(char *moby) {
    char *data = ((char *)MOBY(moby)->pvars);
    int   i, next_point_index;
    for (i = 0; i < 15; i = next_point_index) {
        float first_vector_angle, second_vector_angle;
        int   frame_count, second_frame_count, third_frame_count;
        int   spark_position[4];
        float first_spark_vector[4], second_spark_vector[4];
        char *spark_source_point;
        int   current_point_byte_offset = i * 16;
        next_point_index = i + 1;
        if (*(float *)(data + current_point_byte_offset + 0x23C) < 0.99f &&
            *(float *)(data - (-(next_point_index * 16)) + 0x23C) < 0.99f) {
            continue;
        }
        if (beam_spark_spawn_probability_gp < random_float_between(0.0f, 1.0f)) {
            continue;
        }
        first_vector_angle = random_angle_radians();
        second_vector_angle = random_angle_radians();
        first_spark_vector[0] = fast_cos(first_vector_angle) *
                                random_float_between(beam_spark_horizontal_scale_range_start_gp,
                                                     beam_spark_horizontal_scale_range_end_gp);
        first_spark_vector[1] = fast_sin(first_vector_angle) *
                                random_float_between(beam_spark_horizontal_scale_range_start_gp,
                                                     beam_spark_horizontal_scale_range_end_gp);
        first_spark_vector[2] = 0;
        second_spark_vector[0] = fast_cos(second_vector_angle) *
                                 random_float_between(beam_spark_horizontal_scale_range_start_gp,
                                                      beam_spark_horizontal_scale_range_end_gp);
        second_spark_vector[1] = fast_sin(second_vector_angle) *
                                 random_float_between(beam_spark_horizontal_scale_range_start_gp,
                                                      beam_spark_horizontal_scale_range_end_gp);
        second_spark_vector[2] = 0;
        first_spark_vector[2] = random_float_between(beam_spark_vector_z_range_start_gp,
                                                     beam_spark_vector_z_range_end_gp);
        second_spark_vector[2] = random_float_between(beam_spark_vector_z_range_start_gp,
                                                      beam_spark_vector_z_range_end_gp);
        frame_count = truncate_float_to_s32(scale_time(random_float_between(
            beam_spark_frame_count_range_start_gp, beam_spark_frame_count_range_end_gp)));
        second_frame_count = truncate_float_to_s32(
            scale_time(random_float_between(beam_spark_second_frame_count_range_start_gp,
                                            beam_spark_second_frame_count_range_end_gp)));
        third_frame_count = truncate_float_to_s32(
            scale_time(random_float_between(beam_spark_third_frame_count_range_start_gp,
                                            beam_spark_third_frame_count_range_end_gp)));
        fast_vec_scale(first_spark_vector, first_spark_vector, 1.0f / (float)frame_count);
        first_spark_vector[3] = beam_spark_first_vector_w_gp;
        fast_vec_scale(second_spark_vector, second_spark_vector, 1.0f / (float)third_frame_count);
        second_spark_vector[3] = beam_spark_second_vector_w_gp;
        spark_source_point = data + (current_point_byte_offset + 0x230);
        lerp_vector(spark_position,
                    spark_source_point,
                    spark_source_point,
                    random_float_between(0.0f, 1.0f));
        spawn_spark(spark_position,
                    first_spark_vector,
                    second_spark_vector,
                    beam_spark_initial_word_gp,
                    beam_spark_second_word_gp,
                    frame_count,
                    second_frame_count,
                    third_frame_count,
                    beam_spark_lookup_index_and_flags_gp);
    }
}

f32 D_L10_001DA10C[1] __attribute__((section(".data"))) = {0};

f32   D_L10_00161CB8 = 2.5f;
s32   D_L10_00161CD0 = 0x5a;
f32   D_L10_00161CD4 = 4.0f;
f32   D_L10_00161CDC = 0.125f;
float D_L10_00161CE4 = 45.0f;
int   D_L10_00161CE8 = 0x96;
f32   D_L10_00161CE0 = 6.0f;
s32   D_L10_00161D00 = 0xb;
s32   D_L10_00161D04 = 0;
s32   D_L10_00161D08 = 0x600040ff;
s32   D_L10_00161D0C = 0;
s32   D_L10_00161D10 = 0x6000cccc;

f32 D_L10_001DA0F0[4][2] = {
    {0.5f, 0.5f},
    {0.5f, 0.0f},
    {0.5f, 0.5f},
    {0.5f, 0.0f},
};

float D_L10_00161D1C = 0.25f;
float D_L10_00161D20 = 0.1f;
float D_L10_00161D24 = 1e+01f;
float D_L10_00161D28 = 2e+01f;
float D_L10_00161D2C = 1e+01f;
float D_L10_00161D30 = 2e+01f;
float D_L10_00161D34 = 1e+01f;
float D_L10_00161D38 = 2e+01f;
float D_L10_00161D3C = 0.25f;
float D_L10_00161D40 = 0.75f;
float D_L10_00161D44 = -0.065f;
float D_L10_00161D48 = 0.125f;
float D_L10_00161D4C = 0.25f;

s32 ranged_attack_cooldown_frames_gp __asm__("D_L06_00161FC0") = 0x5a;
int beam_duration_frames_gp __asm__("D_L06_00161FD8") = 0x96;
s32 beam_effect_texture_index_gp __asm__("D_L06_00161FF0") = 0xb;
s32 beam_outer_corner_color_at_zero_weight_gp __asm__("D_L06_00161FF4") = 0;
s32 beam_outer_corner_color_at_unit_weight_gp __asm__("D_L06_00161FF8") = 0x600040ff;
s32 beam_inset_corner_color_at_zero_weight_gp __asm__("D_L06_00161FFC") = 0;
s32 beam_inset_corner_color_at_unit_weight_gp __asm__("D_L06_00162000") = 0x6000cccc;

f32 beam_quad_corner_uvs[4][2] __asm__("D_L06_001F2E90") = {
    {0.5f, 0.5f},
    {0.5f, 0.0f},
    {0.5f, 0.5f},
    {0.5f, 0.0f},
};
