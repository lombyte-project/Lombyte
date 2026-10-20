/* Shared moby classes: blarg_space_commando. */
/* Mixed file: L06 moby updates and spawners; 003021d8 draws the level timer as text. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "qcopy.h"
#include "sda.h"
#include "rnc/math/vector.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/gameplay/hero.h"

/* Movement controller block at vars + 0xC0 (FUN_L01_0026d930 init, FUN_L00_00258b50 step). */
typedef struct {
    s32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    u8 pad10[0x14];
    f32 unk24;
    u8 pad28[0x10];
    s32 flags;           /* 0x38 */
} MoveCtl;

typedef struct {
    u8 pad0[0x18];
    u8 unk18;
    u8 unk19;
    s16 oclass;
    u8 pad1C[0x14];
} ShotInfo;

/* pvars of blarg_space_commando_update */
typedef struct {
    u8 pad0[0x20];
    f32 unk20;
    s16 unk24;
    u8 pad26[2];
    u8 unk28;
    u8 unk29;
    u8 pad2A[6];
    f32 unk30;
    u8 pad34[0xC];
    Vec4 unk40;
    u8 pad50[8];
    u8 unk58;
    u8 pad59;
    u8 unk5A;
    u8 pad5B[5];
    Vec4 vel;            /* 0x60 */
    u8 pad70[0x50];
    MoveCtl ctl;         /* 0xC0 */
    u8 padFC[0x64];
    struct Moby *target; /* 0x160 */
    s32 unk164;
    u8 pad168[8];
    Vec4 home;           /* 0x170 */
    u8 pad180[8];
    s32 unk188;
    f32 turn_speed;      /* 0x18C */
    u8 pad190[0xC];
    s32 timer;           /* 0x19C */
    u8 pad1A0[4];
    s32 path;            /* 0x1A4 */
    s32 route;           /* 0x1A8 */
    s32 volume;          /* 0x1AC */
    u8 pad1B0[0x50];
    Vec4 unk200;
    u8 pad210[0x210];
    u8 unk420[1];
} Moby1068Vars;

extern f32 D_L06_00161FA8 __attribute__((sda));
extern s32 D_L06_00161FC0 __attribute__((sda));
extern f32 D_L06_00161FC4 __attribute__((sda));
extern f32 D_L06_00161FC8 __attribute__((sda));
extern f32 D_L06_00161FCC __attribute__((sda));
extern Vec4f D_L06_00167540;
extern s32 *D_L06_001B0C30[];
extern f32 D_L06_0015F580[] MACRO_ADDR;

extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
extern s32 fast_dec_timer(s32 *) __asm__("FUN_001f9740");
extern f32 FUN_001f9b48(void *, void *);
extern f32 vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern f32 fast_cos(f32) __asm__("FUN_001f9dc8");
extern f32 fast_sin(f32) __asm__("FUN_001f9de0");
extern f32 fast_arc_tan(f32, f32) __asm__("FUN_001f9e90");
extern f32 fast_difference_between_rotations(f32, f32) __asm__("FUN_001fa688");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern void mark_moby_for_removal(struct Moby *) __asm__("FUN_0020c828");
extern f32 compute_interpolated_record_value(struct Moby *) __asm__("FUN_0020c9e0");
extern void blend_moby_animation(struct Moby *, s32, s32, s32) __asm__("FUN_00212f90");
extern s32 is_point_inside_clip_volume(void *, s32) __asm__("FUN_00214720");
extern void moby_point_position(struct Moby *, s32, void *) __asm__("FUN_L00_0024f7c8");
extern void FUN_L00_00258278(struct Moby *, f32, f32 *, f32, f32, f32);
extern s32 FUN_L00_00258b50(struct Moby *, MoveCtl *, void *, void *, f32);
extern void make_hit_descriptor(ShotInfo *, struct Moby *, s32, void *, f32) __asm__("FUN_L00_00259888");
extern void sweep_hit_segment(void *, void *, void *, void *, struct Moby *, ShotInfo *, s32) __asm__("FUN_L00_00259d08");
extern void FUN_L00_0025a120(struct Moby *);
extern void approach_angle(f32 *, f32, f32 *, f32, f32, f32) __asm__("FUN_L00_0025be00");
extern s32 mine_fall(struct Moby *, Vec4 *) __asm__("FUN_L00_0025c698");
extern void spawn_explosion(struct Moby *, Vec4 *, void *, f32, f32, s32, s32, s32, f32, f32, f32, f32, s32,
                             f32, s32, s32, s32, s32) __asm__("FUN_L00_0025e450");
extern s32 FUN_L00_00261b48(s32, void *, void *, void *);
extern void FUN_L00_00261d78(s32, void *, void *, f32);
extern void FUN_L00_00263ac8(struct Moby *, s32, void *, f32);
extern void FUN_L00_00263fd8(struct Moby *, s32, void *, void *, s32, s32, f32, f32 *, f32 *, f32 *);
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
    struct Moby *target;
    s32 *route;
    Vec4 a;
    Vec4 b;
    Vec4 c;
    ShotInfo shot;
    s32 near;
    s32 hit;
    f32 dist;
    f32 angle;
    f32 frame;
    f32 cur_frame;

    FUN_L06_002feb40_u(moby);
    target = vars->target;
    FUN_L00_00263ac8(moby, 1, vars->unk420, 2.5f);
    if (moby->visible != 0 && FUN_001f9b48(&moby->pos, &D_L06_00167540) < 27.0f) {
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
        vars->ctl.unk24 = D_L06_00161FC8 * frame_time;
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
            if (vector_distance(&moby->pos, &target->pos) > 7.0f) {
                break;
            }
        }
        approach_angle(&moby->rot.z, fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y),
                         &vars->turn_speed, frame_time_sq * 6.2831855f, frame_time_sq * 6.2831855f,
                         frame_time * 3.1415927f);
        if (vars->unk164 != 2) {
            if (D_L06_00161FC4 < vector_distance(&moby->pos, &target->pos)) {
                if (fast_difference_between_rotations(
                        moby->rot.z, fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y)) <
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
        qcopy(&a, &route[*route * 4]);
        dist = vector_distance(&moby->pos, &a);
        if (dist < 0.25f) {
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
            FUN_L00_00258278(moby, fast_arc_tan(a.f[0] - moby->pos.x, a.f[1] - moby->pos.y), &vars->turn_speed,
                             0.025f, 0.3f, frame_time * 5.5850534f);
            if (dist > 2.0f) {
                dist = 2.0f;
            }
            b.f[0] = fast_cos(moby->rot.z) * dist;
            b.f[1] = fast_sin(moby->rot.z) * dist;
            b.f[2] = 0.0f;
            FUN_L00_00258b50(moby, &vars->ctl, &b, &c, 1.0f);
        }
        break;
    case BLARG_COMMANDO_STATE_RANGED_ATTACK_WINDUP:
        approach_angle(&moby->rot.z, fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y),
                         &vars->turn_speed, frame_time_sq * 6.2831855f, frame_time_sq * 6.2831855f,
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
                vars->timer = scale_game_frames(D_L06_00161FC0);
            } else {
                moby->state = 1;
                if (moby->prev_seq != 0) {
                    blend_moby_animation(moby, 0, 0, scale_game_frames(20));
                }
            }
        }
        break;
    case BLARG_COMMANDO_STATE_RANGED_ATTACK_COOLDOWN:
        approach_angle(&moby->rot.z, fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y),
                         &vars->turn_speed, frame_time_sq * 6.2831855f, frame_time_sq * 6.2831855f,
                         frame_time * 3.1415927f);
        if (fast_dec_timer(&vars->timer) != 0) {
            FUN_L06_002ff100_u(moby);
            moby->state = 3;
            if (moby->prev_seq != 5) {
                blend_moby_animation(moby, 5, 0, scale_game_frames(5));
            }
        } else if (vector_distance(&moby->pos, &target->pos) < D_L06_00161FC4) {
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
        approach_angle(&moby->rot.z, fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y),
                         &vars->turn_speed, frame_time_sq * 6.2831855f, frame_time_sq * 6.2831855f,
                         frame_time * 3.1415927f);
        moby_point_position(moby, 0, &a);
        qcopy(&b, &moby->pos);
        b.f[2] += 0.3f;
        cur_frame = compute_interpolated_record_value(moby);
        if (hero.vehicle_mode == 1 && cur_frame < 9.0f) {
            moby->anim_speed = D_L06_00161FCC;
        }
        if (cur_frame >= 9.0f && cur_frame <= 15.0f) {
            c.f[0] = fast_cos(moby->rot.z);
            c.f[1] = fast_sin(moby->rot.z);
            c.f[2] = 1.0f;
            c.f[3] = 5627.925f;
            make_hit_descriptor(&shot, moby, 1, &c, 1.0f);
            shot.unk18 = 0;
            shot.unk19 = 1;
            shot.oclass = moby->oclass;
            sweep_hit_segment(&a, &b, &vars->unk200, &b, moby, &shot, 5);
        }
        qcopy(&vars->unk200, &a);
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
        FUN_L00_00258278(moby, fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y), &vars->turn_speed, 0.05f, 0.3f,
                         frame_time * 6.2831855f);
        a.f[0] = 2.0f * fast_cos(moby->rot.z);
        a.f[1] = 2.0f * fast_sin(moby->rot.z);
        a.f[2] = 0.0f;
        FUN_L00_00258b50(moby, &vars->ctl, &a, &b, 1.0f);
        FUN_L00_00261d78(vars->path, &moby->pos, &moby->pos, 0.75f);
        if (FUN_L00_00261b48(vars->path, &moby->pos, &target->pos, &c) != 0 &&
            FUN_001f9b48(&moby->pos, &c) < 0.8f) {
            near = 1;
        }
        if (vars->unk188 == 0) {
            moby->state = 13;
        } else if (near != 0) {
            moby->state = 12;
            if (moby->prev_seq != 6) {
                blend_moby_animation(moby, 6, 0, scale_game_frames(10));
            }
        } else if (vector_distance(&moby->pos, &target->pos) < D_L06_00161FC4) {
            moby->state = 12;
            if (moby->prev_seq != 6) {
                blend_moby_animation(moby, 6, 0, scale_game_frames(10));
            }
        }
        break;
    case BLARG_COMMANDO_STATE_CHASE_MELEE_ATTACK:
        moby_point_position(moby, 0, &a);
        qcopy(&b, &moby->pos);
        b.f[2] += 1.5f;
        frame = compute_interpolated_record_value(moby);
        if (frame >= 9.0f && frame <= 15.0f) {
            c.f[0] = fast_cos(moby->rot.z);
            c.f[1] = fast_sin(moby->rot.z);
            c.f[2] = 1.0f;
            c.f[3] = 5627.925f;
            make_hit_descriptor(&shot, moby, 1, &c, 1.0f);
            shot.unk18 = 0;
            shot.unk19 = 1;
            shot.oclass = moby->oclass;
            sweep_hit_segment(&a, &b, &vars->unk200, &b, moby, &shot, 5);
        }
        qcopy(&vars->unk200, &a);
        if (moby->anim_flags & 2) {
            moby->state = 11;
            if (moby->prev_seq != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(20));
            }
        }
        break;
    case BLARG_COMMANDO_STATE_RETURN_HOME:
        FUN_L00_00258278(moby, fast_arc_tan(vars->home.f[0] - moby->pos.x, vars->home.f[1] - moby->pos.y), &vars->turn_speed, 0.05f, 0.3f,
                         frame_time * 6.2831855f);
        a.f[0] = 2.0f * fast_cos(moby->rot.z);
        a.f[1] = 2.0f * fast_sin(moby->rot.z);
        a.f[2] = 0.0f;
        if (fast_difference_between_rotations(
                moby->rot.z, fast_arc_tan(vars->home.f[0] - moby->pos.x, vars->home.f[1] - moby->pos.y)) <
            0.7853982f) {
            FUN_L00_00258b50(moby, &vars->ctl, &a, &b, 1.0f);
            FUN_L00_00261d78(vars->path, &moby->pos, &moby->pos, 0.75f);
        }
        if (FUN_001f9b48(&vars->home, &moby->pos) < 1.0f) {
            moby->state = 1;
            if (moby->prev_seq != 0) {
                blend_moby_animation(moby, 0, 0, scale_game_frames(20));
            }
        }
        break;
    case BLARG_COMMANDO_STATE_KNOCKBACK:
        hit = mine_fall(moby, &vars->vel);
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
        if (mine_fall(moby, &vars->vel) & 0x160) {
            s32 none;

            qcopy(&a, &moby->pos);
            a.f[2] += 1.0f;
            none = -1;
            spawn_explosion(moby, &vars->unk40, &a, 0.0f, 0.0f, 5, 2, 4, 2.0f, 1.0f, 9.0f, 1.0f, -1, 15.0f, 1, 1, none, 0);
            FUN_L00_00263fd8(moby, 0x6A6, &moby->pos, &moby->rot, 0, 0, 0.0f, D_L06_0015F580, D_L06_0015F580,
                             D_L06_0015F580);
            FUN_L00_00263fd8(moby, 0x6A7, &moby->pos, &moby->rot, 0, 0, 0.0f, D_L06_0015F580, D_L06_0015F580,
                             D_L06_0015F580);
            FUN_L00_00263fd8(moby, 0x6A8, &moby->pos, &moby->rot, 0, 0, 0.0f, D_L06_0015F580, D_L06_0015F580,
                             D_L06_0015F580);
            mark_moby_for_removal(moby);
        }
        break;
    }
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002FF000.c: func_L06_002FFF70), where it is exact; names translated to the US level program. */

extern char *FUN_L00_0025a420(void *, int, int);
extern char *FUN_L06_00300c60_c(int unused, char *pa, char *pb) __asm__("FUN_L06_00300c60");
#include "rnc/data_labels.h"
extern f32 D_0015ED6C;
extern f32 fast_arc_tan(f32, f32) __asm__("func_001F9E90");
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin_c2(f32) __asm__("func_001F9DE0");
extern f32 random_angle_radians_c(void) __asm__("func_00213308");
extern f32 random_float_between_c(f32, f32) __asm__("func_002132A8");
extern f32 vector_distance_xy_alt(void *, void *) __asm__("FUN_001f9b80");
extern float FUN_L04_001f1978(float);
extern float FUN_L00_00257c48(float lo, float hi);
extern int FUN_L00_0025ff38(float, char *, void *, int, int, void *, int);
extern int fun_00213928(void *, void *, void *, int, int *, float *, int, int);
extern int *D_L06_001B0C30[];
extern s32 scale_game_frames_c(s32) __asm__("func_001F96F8");
extern s32 tick_countdown_32_alt(s32 *) __asm__("FUN_001f9740");
extern s32 truncate_float_to_s32_c(f32) __asm__("func_001FA6D0");
extern int current_level_index_gp __asm__("D_0015ED84") __attribute__((sda));
extern float D_L06_00161FA8 __attribute__((sda));
extern float D_L06_00161FAC __attribute__((sda));
extern float D_L06_00161FB0 __attribute__((sda));
extern float D_L06_00161FB4 __attribute__((sda));
extern float D_L06_00161FB8 __attribute__((sda));
extern float D_L06_00161FBC __attribute__((sda));
extern unsigned char D_0013D408[];
extern void FUN_L00_00257470(void *, int, int);
extern void trooper_start_knockback(void *, void *, float, int, int, int) __asm__("FUN_L00_0025c558");
extern void trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern void FUN_L00_00263d40(int, int);
void FUN_L06_002ff2c8(void *arg);
extern void allocate_voice_for_bank_entry(int, int, int) __asm__("FUN_0022db10");
float absolute_float(float input) __asm__("func_001F99C0");

void FUN_L06_002feb40(char *moby) {
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
    char *vars;
    char *hit_record;
    char *fragment;
    int *route_record;
    int r;
    int k;
    int bits;
    float vv[4];
    float vec2[4];
    int hit;
    float dt;
    float remaining_health;
    float lo, hi, spd, off, pull;

    vars = *(char **)(moby + 0x78);
    *(float *)(moby + 0x2C) = *(float *)(*(char **)(moby + 0x24) + 0x24) * D_L06_00161FA8;
    FUN_L06_002ff2c8(moby);
    if (*(int *)(vars + 0x38) != 0) {
        *(int *)(vars + 0x188) = truncate_float_to_s32_c(FUN_L04_001f1978(random_float_between_c(180.0f, 240.0f)));
        *(int *)(vars + 0x38) = 0;
    }
    tick_countdown_32_alt((s32 *)(vars + 0x188));
    *(int *)&dt = 0;
    hit_record = FUN_L00_0025a420(moby, 0x330000, 0);
    r = fun_00213928(moby, hit_record, vars + 0x20, 0, &hit, &dt, 0, 4);
    if (hit != 1 && (unsigned char)moby[0x20] != 0x10) {
        if (current_level_index_gp == 0xA && hit_record != 0 && *(char **)(hit_record + 0x20) != 0 &&
            *(short *)(*(char **)(hit_record + 0x20) + 0xA6) == 0x31A && D_0013D408[0x11] == 0) {
            D_0013D408[0x11] = 1;
            allocate_voice_for_bank_entry(1, 0, 0);
            FUN_L00_00263d40(0x53D6, -1);
        }
        remaining_health = *(float *)(vars + 0x20) - dt;
        *(float *)(vars + 0x20) = remaining_health;
        if (remaining_health <= 0.0f) r = 1;
        *(int *)(vars + 0x80) = 0x1F4;
        *(float *)(vars + 0x88) = 0.5f;
        switch (r) {
        case NONFATAL_KNOCKBACK_HIT_RESPONSE:
        case TIMED_NONFATAL_KNOCKBACK_HIT_RESPONSE:
        case RECOVERABLE_KNOCKBACK_HIT_RESPONSE:
        case TIMED_RECOVERABLE_KNOCKBACK_HIT_RESPONSE:
        case SCALED_DURATION_KNOCKBACK_HIT_RESPONSE:
        case TIMED_KNOCKBACK_HIT_RESPONSE:
        case FRAME_SCALED_KNOCKBACK_HIT_RESPONSE:
        case SIXTY_FRAME_KNOCKBACK_HIT_RESPONSE:
        {
            char *q;
            int knockback_frames;
            float b, a;
            moby[0x20] = 0xF;
            q = vars + 0x60;
            knockback_frames = scale_game_frames_c(0x3C);
            a = D_L06_00161FB0 * D_0015ED6C;
            b = D_L06_00161FB4 * D_0015ED6C;
            *(short *)(vars + 0x26) = knockback_frames;
            *(float *)(vars + 0x7C) = b;
            *(float *)(vars + 0x78) = a;
            *(float *)(vars + 0xB0) = 8.0f;
            *(float *)(vars + 0xB4) = 15.0f;
            trooper_start_knockback(moby, q, fast_arc_tan(*(float *)(hit_record + 0x10), *(float *)(hit_record + 0x14)), 7, 1, 0);
            *(unsigned char *)(vars + 0x117) = 0xFA;
            break;
        }
        case FATAL_HIT_RESPONSE:
        case FRAGMENTING_FATAL_HIT_RESPONSE:
        {
            char *q;
            float a, b;
            moby[0x20] = 0x10;
            q = vars + 0x60;
            a = D_L06_00161FB8 * D_0015ED6C;
            b = D_L06_00161FBC * D_0015ED6C;
            *(unsigned short *)(moby + 0x34) &= 0xEFFF;
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
            off = *(float *)&bits;
            trooper_start_knockback(moby, q, a, 9, 1, 0);
            bits = 0x40E00000;
            pull = *(float *)&bits;
            for (k = 0; k < 10; k++) {
                float ang;
                qcopy(vv, moby + 0x10);
                vv[2] = vv[2] + 2.5f;
                vv[0] = vv[0] + FUN_L00_00257c48(lo, hi);
                vv[1] = vv[1] + FUN_L00_00257c48(lo, hi);
                vv[2] = vv[2] + FUN_L00_00257c48(lo, hi);
                ang = random_angle_radians_c();
                spd = random_float_between_c(off, 5.7f) * D_0015ED6C;
                vec2[0] = fast_cos(ang) * spd;
                vec2[1] = fast_sin_c2(ang) * spd;
                vec2[2] = lo;
                vec2[0] = vec2[0] + fast_cos(fast_arc_tan(*(float *)(hit_record + 0x10), *(float *)(hit_record + 0x14))) * (D_0015ED6C * pull);
                vec2[1] = vec2[1] + fast_sin_c2(fast_arc_tan(*(float *)(hit_record + 0x10), *(float *)(hit_record + 0x14))) * (D_0015ED6C * pull);
                vec2[2] = random_float_between_c(off, 8.5f) * D_0015ED6C;
                fragment = FUN_L06_00300c60_c((int)moby, (char *)vv, (char *)vec2);
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
    *(unsigned char *)(moby + 0xA4) = 0xFF;
    trooper_anim_finish(moby, vars + 0x110);
    if (*(int *)(vars + 0x188) != 0) {
        *(float *)(vars + 0x198) = D_L06_00161FAC + 6.0f;
    } else {
        *(float *)(vars + 0x198) = D_L06_00161FAC;
    }
    {
    char *q = vars + 0x120;
    route_record = D_L06_001B0C30[*(int *)(vars + 0x180)];
    if (FUN_L00_0025ff38(*(float *)(vars + 0x198), moby, q, 0, 0, route_record + 4, *route_record) != 2) {
        if (*(float *)(vars + 0x198) < vector_distance_xy_alt(vars + 0x170, vars + 0x120)) {
            *(int *)(vars + 0x164) = 2;
        } else if (3.0f < absolute_float(*(float *)(moby + 0x18) - *(float *)(vars + 0x128))) {
            *(int *)(vars + 0x164) = 2;
        }
    }
    }
    if (*(int *)(vars + 0x160) == 0) {
        *(int *)(vars + 0x160) = *(int *)(((char *)&D_001413D0));
    }
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002FF000.c: func_L06_00300530), where it is exact; names translated to the US level program. */

extern char D_L06_00174600_d[] __asm__("D_L06_00174600") __attribute__((section(".data")));
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern f32 fast_add_rotations(f32, f32) __asm__("func_001FA580");
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin_c(f32) __asm__("func_001F9DE0");
extern float probe_ground_height(void *, int, float) __asm__("func_00213508");
extern void add_vector_xyz_cf(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");

extern float D_L06_00161FD4 __attribute__((sda));
extern int D_L06_00161FD8 __attribute__((sda));
void FUN_L06_002ff100(char *moby) {
    char *vars = *(char **)(moby + 0x78);
    float v[4];
    int i;
    probe_ground_height(moby + 0x10, 0, 0.5f);
    normalize_vector_xyz(vars + 0x220, D_L06_00174600_d, 1.0f);
    add_vector_xyz_cf(moby, 0, vars + 0x210);
    *(float *)(vars + 0x218) = *(float *)(moby + 0x18);
    for (i = 0; i < 16; i++) {
        float angle_offset = convert_integer_to_float(i) * 0.0625f - 0.5f;
        angle_offset = angle_offset * (D_L06_00161FD4 * 0.017453292f);
        v[0] = fast_cos(fast_add_rotations(angle_offset, *(float *)(moby + 0x48))) * 0.2f;
        v[1] = fast_sin_c(fast_add_rotations(angle_offset, *(float *)(moby + 0x48))) * 0.2f;
        v[2] = 0.15f;
        add_vector_xyz((float *)(vars + 0x230 + i * 16), (float *)(vars + 0x210), v);
        if (i == 0 || i == 15) {
            float *w = (float *)(vars + 0x23C + i * 16);
            *w = *w = 0.0f;
        } else {
            *(float *)(vars + 0x23C + i * 16) = 1.0f;
        }
    }
    *(float *)(vars + 0x21C) = (float)scale_game_frames(D_L06_00161FD8);
}
/* Shared moby update: steps the timer at 0x21C, spaces 16 entries, and runs the pair checks. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002FF000.c: func_L06_003006F8), where it is exact; names translated to the US level program. */

typedef struct Hero Hero;

typedef struct Moby Moby;

typedef struct { float frames, y, z, w; } FanPt;

typedef struct {
    char pad0[0x210];
    FanPt base;   /* 0x210; w = timer */
    FanPt dir;    /* 0x220 */
    FanPt pts[16]; /* 0x230 */
} FanData;

extern f32 fast_vec_dot(void *a, void *b) __asm__("func_001F9AB0");
extern float approach_value(float *, float, float);
extern int collision_line(void *, void *, int, void *, void *);
extern f32 D_L06_00161FD0 __attribute__((sda));
void FUN_L06_002ff680(char *moby);
void FUN_L06_002ff978(char *moby);
extern void enqueue_callback_list_1(void *, void *) __asm__("FUN_001f4600");
extern void fast_vec_normalize(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");

void FUN_L06_002ff2c8(void *arg) {
    char *moby = arg;
    FanData *data = *(FanData **)(moby + 0x78);
    int frames;
    float point[4];
    float relative[4];
    ShotInfo shot;
    float step;
    float alignment;
    int i, j, next, hit_type;
    int remaining;

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
            step = (D_L06_00161FD0 * frame_time) * 0.5f;
        } else {
            step = D_L06_00161FD0 * frame_time;
        }
        subtract_vector_xyz(point, &data->pts[i], &data->base);
        point[2] = 0.0f;
        fast_vec_normalize(point, point, step);
        add_vector_xyz(point, (float *)&data->pts[i], point);
        subtract_vector_xyz(relative, point, moby + 0x10);
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
                point[0] = fast_cos(*(float *)(moby + 0x48));
                point[1] = fast_sin(*(float *)(moby + 0x48));
                point[2] = 1.0f;
                point[3] = 5627.925f;
                hit_type = 0x10000;
                if (1.0f <= *(float *)(entry + 0x23C) && 1.0f <= *(float *)(next_entry + 0x23C)) {
                    hit_type = 0x10001;
                }
                make_hit_descriptor(&shot, (struct Moby *)moby, hit_type, point, 1.0f);
                shot.oclass = *(s16 *)(moby + 0xA6);
                shot.unk18 = 0;
                shot.unk19 = 1;
                collision_line(&data->pts[j], &data->pts[j + 1], 0, moby, &shot);
            }
        }
        j = next;
    } while (j < 15);

    enqueue_callback_list_1((void *)FUN_L06_002ff680, moby);
    FUN_L06_002ff978(moby);
}

/* GS quad packet for draw_geometry_quad: four corners, their colours and
   texture coordinates, then the giftag and register words. */
typedef struct {
    f32 pos[4][4];
    u32 col[4];
    f32 uv[4][2];
    u64 tag[4];
} QuadPacket_f680;

extern s32 D_L06_00161FDC __attribute__((sda));
extern s32 D_L06_00161FE0 __attribute__((sda));
extern s32 D_L06_00161FE4 __attribute__((sda));
extern s32 D_L06_00161FE8 __attribute__((sda));
extern s32 D_L06_00161FEC __attribute__((sda));
extern s32 D_L06_00161FF0 __attribute__((sda));
extern s32 D_L06_00161FF4 __attribute__((sda));
extern s32 D_L06_00161FF8 __attribute__((sda));
extern s32 D_L06_00161FFC __attribute__((sda));
extern s32 D_L06_00162000 __attribute__((sda));
/* Scrolling texture coordinates of the four corners. */
extern f32 D_L06_001F2E90[4][2];

extern void vu1_add_gs_register(s32, s64) __asm__("FUN_00233980");
extern u64 get_effect_texture(s32) __asm__("FUN_001f44b8");
extern void set_identity_matrix(void *) __asm__("FUN_001f9fc8");
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern u32 fast_tween_color(f32, s32, s32) __asm__("FUN_001fa6e0");
extern void draw_geometry_quad(void *, void *, s32) __asm__("FUN_001f7d30");

/* Draws the beam as 15 textured quads between consecutive points of the
   moby's point list (data + 0x230), each twice: once with the odd corners
   pulled toward the centre (data + 0x210) and once raised by 0.25. */
void FUN_L06_002ff680(char *moby) {
    char *data = *(char **)(moby + 0x78);
    QuadPacket_f680 pk[2];
    f32 mat[4][4];
    f32 corner_inset_vector[4];
    s32 i;
    s32 j;
    s32 off;
    f32 *v;
    u64 regs;

    vu1_add_gs_register(0x47, 0x5380B);
    i = 0;
    pk[0].tag[1] = get_effect_texture(D_L06_00161FF0);
    pk[1].tag[1] = get_effect_texture(D_L06_00161FF0);
    regs = D_L06_00161FDC;
    regs |= (u64)D_L06_00161FE0 << 2;
    regs |= (u64)D_L06_00161FE4 << 4;
    regs |= (u64)D_L06_00161FE8 << 6;
    regs |= (u64)D_L06_00161FEC << 32;
    /* Store order is load-bearing: it sets which register keeps regs. */
    pk[1].tag[2] = 0x0000FF9000000260ULL;
    pk[0].tag[2] = 0x0000FF9000000260ULL;
    pk[1].tag[0] = 0;
    pk[0].tag[0] = 0;
    pk[1].tag[3] = regs;
    pk[0].tag[3] = regs;
    set_identity_matrix(mat);
    do {
        D_L06_001F2E90[1][1] = D_L06_001F2E90[3][1];
        D_L06_001F2E90[3][1] = random_float_between(0.0f, 0.2f);
        for (j = 0; j < 4; j++) {
            v = pk[0].pos[j];
            off = (i + j / 2) * 16;
            /* &data[...] (not data + off + ...) keeps retail's off + data add. */
            qcopy(v, &data[off + 0x230]);
            /* The colour call in both arms: retail merges the two calls and
               leaves the address add in each arm. */
            if (j & 1) {
                subtract_vector_xyz(corner_inset_vector, v, data + 0x210);
                normalize_vector_xyz(corner_inset_vector, corner_inset_vector, 0.5f);
                subtract_vector_xyz(v, v, corner_inset_vector);
                v[2] -= 0.125f;
                pk[0].col[j] = fast_tween_color(*(f32 *)(data + off + 0x23C), D_L06_00161FFC,
                                                D_L06_00162000);
            } else {
                pk[0].col[j] = fast_tween_color(*(f32 *)(data + off + 0x23C), D_L06_00161FF4,
                                                D_L06_00161FF8);
            }
            pk[0].uv[j][0] = D_L06_001F2E90[j][0];
            pk[0].uv[j][1] = D_L06_001F2E90[j][1];
            pk[1].uv[j][0] = D_L06_001F2E90[j][0];
            pk[1].uv[j][1] = D_L06_001F2E90[j][1];
        }
        draw_geometry_quad(pk, mat, 0);
        for (j = 0; j < 4; j++) {
            v = pk[0].pos[j];
            if (j & 1) {
                v[2] += 0.25f;
            }
        }
        i++;
        draw_geometry_quad(pk, mat, 0);
    } while (i < 15);
    vu1_add_gs_register(0x47, 0x5360B);
}

/* Spawns a burst of effects for each pair of ready entries in the moby's table. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002FF000.c: func_L06_00300DA8), where it is exact; names translated to the US level program. */

extern char *spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern float fast_cos(float) __asm__("func_001F9DC8");
extern float fast_sin(float) __asm__("func_001F9DE0");
extern float random_angle_radians(void) __asm__("FUN_00213308");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern int D_L06_00162004 __attribute__((sda));
extern int D_L06_00162008 __attribute__((sda));
extern float D_L06_0016200C __attribute__((sda));
extern float D_L06_00162010 __attribute__((sda));
extern float D_L06_00162014 __attribute__((sda));
extern float D_L06_00162018 __attribute__((sda));
extern float D_L06_0016201C __attribute__((sda));
extern float D_L06_00162020 __attribute__((sda));
extern float D_L06_00162024 __attribute__((sda));
extern float D_L06_00162028 __attribute__((sda));
extern float D_L06_0016202C __attribute__((sda));
extern float D_L06_00162030 __attribute__((sda));
extern float D_L06_00162034 __attribute__((sda));
extern float D_L06_00162038 __attribute__((sda));
extern float D_L06_0016203C __attribute__((sda));
extern int D_L06_00162040 __attribute__((sda));
extern void lerp_vector(void *, void *, void *, float) __asm__("FUN_001f9a40");
extern void scale_vector_xyz(void *out, void *a, f32 s) __asm__("FUN_001f9a68");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

void FUN_L06_002ff978(char *moby) {
    char *data = *(char **)(moby + 0x78);
    int i, next_point_index;
    for (i = 0; i < 15; i = next_point_index) {
        float f21, f22;
        int frame_count, second_frame_count, third_frame_count;
        int spark_position[4];
        float v[4], w[4];
        char *spark_source_point;
        int off = i * 16;
        next_point_index = i + 1;
        if (*(float *)(data + off + 0x23C) < 0.99f &&
            *(float *)(data - (-(next_point_index * 16)) + 0x23C) < 0.99f) {
            continue;
        }
        if (D_L06_0016203C < random_float_between(0.0f, 1.0f)) {
            continue;
        }
        f21 = random_angle_radians();
        f22 = random_angle_radians();
        v[0] = fast_cos(f21) *
               random_float_between(D_L06_0016202C, D_L06_00162030);
        v[1] = fast_sin(f21) *
               random_float_between(D_L06_0016202C, D_L06_00162030);
        v[2] = 0;
        w[0] = fast_cos(f22) *
               random_float_between(D_L06_0016202C, D_L06_00162030);
        w[1] = fast_sin(f22) *
               random_float_between(D_L06_0016202C, D_L06_00162030);
        w[2] = 0;
        v[2] = random_float_between(D_L06_00162034, D_L06_00162038);
        w[2] = random_float_between(D_L06_00162034, D_L06_00162038);
        frame_count = func_001FA898_r(multiply_global_factor_ed64(
            random_float_between(D_L06_00162014, D_L06_00162018)));
        second_frame_count = func_001FA898_r(multiply_global_factor_ed64(
            random_float_between(D_L06_0016201C, D_L06_00162020)));
        third_frame_count = func_001FA898_r(multiply_global_factor_ed64(
            random_float_between(D_L06_00162024, D_L06_00162028)));
        scale_vector_xyz(v, v, 1.0f / (float)frame_count);
        v[3] = D_L06_0016200C;
        scale_vector_xyz(w, w, 1.0f / (float)third_frame_count);
        w[3] = D_L06_00162010;
        spark_source_point = data + (off + 0x230);
        lerp_vector(spark_position, spark_source_point, spark_source_point, random_float_between(0.0f, 1.0f));
        spawn_spark(spark_position, v, w, D_L06_00162004, D_L06_00162008, frame_count, second_frame_count, third_frame_count,
                     D_L06_00162040);
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/vendor_002FF000.c: func_L06_00301FE8), where it is exact; names translated to the US level program. */

extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern f32 D_L06_00162080_gp __asm__("D_L06_00162080") __attribute__((sda));
extern void fast_vec_add(float *, float *, float *) __asm__("FUN_001f9a10");

void FUN_L06_00300b90(struct Moby *moby) {
    char *data;
    float vertical_velocity_decrement = frame_time_sq * 15.0f;
    moby->scale =
        moby->pclass->scale * *(float *)&D_L06_00162080_gp;
    data = (char *)moby->pvars;
    *(float *)(data + 8) -= vertical_velocity_decrement;
    fast_vec_add(&moby->pos.x, &moby->pos.x, (float *)data);
    fast_vec_add(&moby->rot.x, &moby->rot.x, (float *)(data + 0x10));
    moby->rot.x = fast_add_rotations(moby->rot.x, *(float *)(data + 0x10));
    moby->rot.y = fast_add_rotations(moby->rot.y, *(float *)(data + 0x14));
    moby->rot.z = fast_add_rotations(moby->rot.z, *(float *)(data + 0x18));
    if (--moby->command == 0) {
        mark_moby_for_removal(moby);
    }
}

#include "eetypes.h"
extern unsigned char *create_moby(int) __asm__("FUN_0020c4f8");
extern void upd_a(unsigned char *) __asm__("FUN_L00_00250df8");
extern float D_L06_00162080_gp __asm__("D_L06_00162080") __attribute__((sda));
extern int ftoi_300c60(float) __asm__("FUN_001fa6d0");
extern float scale_time(float) __asm__("FUN_001f96b0");

/* Spawns one fragment of the break-apart at pos, flying along vel with a
 * random spin. */
unsigned char *FUN_L06_00300c60(char *m, float *pos, float *vel) {
    u128 position_copy, v;
    u128 *pp = &position_copy;
    u128 *pv = &v;
    unsigned char *fragment_moby;
    char *fragment_vars;

    *pp = *(u128 *)pos;
    *pv = *(u128 *)vel;
    fragment_moby = create_moby(ftoi_300c60(random_float_between(1085.0f, 1089.0f)));

    if (fragment_moby != 0) {
        fragment_vars = *(char **)(fragment_moby + 0x78);
        *(float *)(fragment_moby + 0x2C) = *(float *)(*(char **)(fragment_moby + 0x24) + 0x24) * D_L06_00162080_gp;
        fragment_moby[0x30] = 0xFF;
        *(short *)(fragment_moby + 0x32) = 0x7E;
        fragment_moby[0x31] = 1;
        qcopy(fragment_moby + 0x10, pp);
        qcopy(fragment_vars, pv);
        *(float *)(fragment_vars + 0x10) = random_float_between(-360.0f, 360.0f) * DEG_TO_RAD * frame_time;
        *(float *)(fragment_vars + 0x14) = random_float_between(-360.0f, 360.0f) * DEG_TO_RAD * frame_time;
        *(float *)(fragment_vars + 0x18) = random_float_between(-360.0f, 360.0f) * DEG_TO_RAD * frame_time;
        fragment_moby[0xBC] = ftoi_300c60(scale_time(random_float_between(60.0f, 120.0f)));
        upd_a(fragment_moby);
    }
    return fragment_moby;
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002FF000.c: func_L06_00303630), where it is exact; names translated to the US level program. */

extern int level_frame_count __asm__("D_L06_0015F5CC");
extern short D_L06_001620C8 __attribute__((sda));
extern short D_L06_001620CC __attribute__((sda));
extern short D_L06_001620D0 __attribute__((sda));
extern short D_L06_001620D4 __attribute__((sda));
extern short D_L06_001620D8 __attribute__((sda));
extern short D_L06_001620DC __attribute__((sda));
extern short D_L06_001620E0 __attribute__((sda));
extern short D_L06_001620E4 __attribute__((sda));
extern short D_L06_001620E8 __attribute__((sda));
extern void draw_ui_frame(int, int, int, int, int) __asm__("FUN_001f5f18");
extern float func_001FA888(int) __asm__("FUN_001fa6c0");
extern int fast_tween_color_local(int, int, float) __asm__("FUN_001fa6e0");
extern int scale_ticks(int) __asm__("FUN_001f96f8");
extern int random_integer_below(int) __asm__("FUN_00213260");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");
extern void font_print_center(int a, int b, int c, int d, int e) __asm__("FUN_001f6af0");
extern void font_print_center_large_001f6c20(int a, int b, int c, int d, int e) __asm__("FUN_001f6c20");

#define W(x) (*(int *)&(x))

/* Draws the level timer as "H:MM:SS.t" text with a pulsing colour. */
void FUN_L06_003021d8(char *moby) {
    char buf[8];
    char *data = *(char **)(moby + 0x78);
    float v;
    int col;

    draw_ui_frame(W(D_L06_001620D8) + W(D_L06_001620CC), W(D_L06_001620DC) + W(D_L06_001620CC),
                W(D_L06_001620E0) + W(D_L06_001620C8), W(D_L06_001620E4) + W(D_L06_001620C8),
                W(D_L06_001620E8));
    v = fast_sin(func_001FA888(level_frame_count % 60) / 60.0f * 6.28318f) * 3.0f + 0.5f;
    if (v > 1.0f) {
        v = 1.0f;
    } else if (v < 0.0f) {
        v = 0.0f;
    }
    col = fast_tween_color_local(W(D_L06_001620D0), W(D_L06_001620D4), v);
    buf[0] = 0x30;
    buf[1] = *(int *)(data + 0xB4) / scale_ticks(0xE10) + 0x30;
    buf[2] = 0x3A;
    buf[3] = *(int *)(data + 0xB4) % scale_ticks(0xE10) / scale_ticks(0x258) + 0x30;
    buf[4] = *(int *)(data + 0xB4) % scale_ticks(0x258) / scale_ticks(0x3C) + 0x30;
    buf[5] = 0x3A;
    buf[6] = *(int *)(data + 0xB4) % scale_ticks(0x3C) / scale_ticks(6) + 0x30;
    buf[7] = func_001FA898_r((float)random_integer_below(10)) + 0x30;
    font_print_center_large_001f6c20(W(D_L06_001620C8), W(D_L06_001620CC), col, (int)buf, 8);
}

f32 D_L10_001DA10C[1] __attribute__((section(".data"))) = {0};
