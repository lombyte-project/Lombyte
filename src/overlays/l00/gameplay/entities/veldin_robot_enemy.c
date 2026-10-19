/* Level 00 moby classes: veldin_robot_enemy. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/overlay/level_globals.h"
#include "rnc/gameplay/state/usage_stats.h"

typedef struct {
    u8 pad0[0x20];
    u8 state;
    u8 pad21[0xF];
    u8 alpha;
    u8 pad31[0x47];
    s32 *vars;
} HelpMoby;

typedef struct {
    u8 pad0[0x20];
    u8 state;
} HelpTarget;

s32 is_point_inside_clip_volume(s32 arg0, s32 arg1) __asm__("FUN_00214720");
extern void help_request(s32, s32) __asm__("FUN_L00_00203908");
extern u16 D_00141850[];
extern u16 D_00141968[];
extern u8 *D_L00_0015FFD8 __attribute__((sda));

void FUN_L00_002e0988(HelpMoby *m) {
    s32 *vars;
    s32 *p;
    s32 i;
    HelpTarget *t;
    struct Hero *g;

    vars = m->vars;
    if (m->state == 0) {
        m->state = 1;
        m->alpha = 0xFF;
    }
    if (D_00141968[0x18 / 2] == 0) {
        help_request(3, 3);
    }
    if (D_00141968[0x3A0 / 2] == 0) {
        if (is_point_inside_clip_volume((s32)(&hero.motion.pos), vars[4]) != 0) {
            help_request(0x4E2A, 0x74);
        }
    }
    if (D_00141968[0x48 / 2] == 0) {
        if (is_point_inside_clip_volume((s32)(&hero.motion.pos), vars[0]) != 0 && D_00141850[0] == 0) {
            help_request(5, 9);
        }
    }
    if (D_00141968[0x40 / 2] == 0 && usage_stats.stat[0].count == 0) {
        if (is_point_inside_clip_volume((s32)(&hero.motion.pos), vars[1]) != 0) {
            help_request(4, 8);
        }
    }
    if (D_00141968[0] == 0) {
        if (is_point_inside_clip_volume((s32)(&hero.motion.pos), vars[2]) != 0) {
            help_request(0, 0);
        }
    }
    if (D_00141968[0x8 / 2] == 0 && D_00141968[0x10 / 2] == 0) {
        g = &hero;
        p = vars + 5;
        for (i = 0; i < 4; i++) {
            if (*p != -1) {
                t = (HelpTarget *)(D_L00_0015FFD8 + (*p << 8));
                if (t == 0 || t->state == 0xFE || t->state == 0xFD) {
                    if (g->health.hp < 4) {
                        help_request(1, 1);
                    } else {
                        help_request(2, 2);
                    }
                }
            }
            p++;
        }
    }
}
#include "eetypes.h"
#include "sda.h"
#include "qcopy.h"
#include "rnc/math/vector.h"
#include "rnc/overlay/collision.h"

typedef union {
    u128 q;
    f32 f[4];
} ElectroidVec;

typedef struct {
    s32 count;
    s32 pad4[3];
    ElectroidVec pts[1];
} Path;

typedef struct ElectroidMoby ElectroidMoby;

typedef struct {
    u8 pad0[0x20];
    f32 health;
    s16 unk24;
    s16 unk26;
    u8 unk28;
    u8 unk29;
    u8 pad2A[0x38 - 0x2A];
    s32 unk38;
    u8 pad3C[0x58 - 0x3C];
    u8 unk58;
    u8 pad59;
    u8 unk5A;
    u8 pad5B[0x60 - 0x5B];
    u8 unk60[7];
    u8 unk67;
    u8 pad68[8];
    u8 unk70[0x10];
    f32 unk80;
    f32 unk84;
    f32 unk88;
    f32 unk8C;
    s32 unk90;
    s32 unk94;
    f32 unk98;
    u8 pad9C[0xAD - 0x9C];
    u8 unkAD;
    u8 padAE[0xBC - 0xAE];
    f32 unkBC;
    f32 unkC0;
    f32 unkC4;
    u8 padC8[8];
    ElectroidVec targetPos;
    u8 padE0[0x110 - 0xE0];
    ElectroidMoby *target;
    s32 unk114;
    u8 pad118[0x120 - 0x118];
    u8 unk120[0x1A0 - 0x120];
    u128 spawnPos;
    u8 pad1B0[4];
    s32 unk1B4;
    s32 pathIdx;
    s32 unk1BC;
    u8 pad1C0[0x1D0 - 0x1C0];
    f32 turnVel;
    f32 speed;
    s16 timer;
    s16 unk1DA;
    s32 pathPt;
} ElectroidVars;

struct ElectroidMoby {
    u8 pad0[0x10];
    ElectroidVec pos;
    u8 state;
    u8 pad21[0x31 - 0x21];
    u8 unk31;
    u8 pad32[2];
    u16 flags;
    u8 pad36[0x40 - 0x36];
    ElectroidVec unk40;
    u8 pad50[2];
    u8 unk52;
    u8 anim;
    u8 pad54[0x70 - 0x54];
    u8 unk70;
    u8 pad71[0x78 - 0x71];
    ElectroidVars *vars;
    u8 pad7C[3];
    s8 unk7F;
    u8 pad80[0x94 - 0x80];
    s32 unk94;
    u8 pad98[0xA4 - 0x98];
    u8 unkA4;
    u8 padA5;
    u16 unkA6;
    u8 padA8[0xC0 - 0xA8];
    ElectroidVec unkC0;
};

typedef struct {
    f32 dir[4];
    ElectroidMoby *moby;
    s32 flags;
    u8 unk88;
    u8 unk89;
    u16 unk8A;
    f32 unk8C;
    s32 unk90;
} ElectroidProbe;

extern f32 D_L00_00161BE4 __attribute__((sda));
s32 D_L00_00161BE8 = 3;
extern f32 D_L00_00161BEC __attribute__((sda));
extern Vec4 D_L00_00166DC0;
extern s32 D_001413D0_lui[] __asm__("D_001413D0") NOT_SDA;
extern CollisionHit coll_hit __asm__("D_L00_00173E40");
extern Path *D_L00_001B04B0[];
extern s32 D_L00_0015F580[] MACRO_ADDR;

extern void FUN_L00_002e1678();
extern void FUN_L00_00263b38(void *, f32);
extern void update_joint_animation(ElectroidMoby *, void *, s32, f32, f32) __asm__("FUN_L00_002628d8");
extern void FUN_L00_0025a120(ElectroidMoby *);
extern f32 FUN_001f9b48(void *, void *);
extern f32 vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern s32 random_integer_below(s32) __asm__("FUN_00213260");
extern void blend_moby_animation(ElectroidMoby *, s32, s32, s32) __asm__("FUN_00212f90");
extern void mark_moby_for_removal(ElectroidMoby *) __asm__("FUN_0020c828");
extern s32 scale_game_frames_s32(s32) __asm__("FUN_001f96f8");
extern f32 fast_arc_tan(f32, f32) __asm__("FUN_001f9e90");
extern void approach_angle(void *, f32, void *, f32, f32, f32) __asm__("FUN_L00_0025be00");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern f32 fast_vec_length(void *) __asm__("FUN_001f9af0");
extern f32 advance_accelerated_scalar(f32, f32, f32, f32, f32 *, f32 *) __asm__("FUN_00213f38");
extern void fast_vec_normalize(void *, void *, f32) __asm__("FUN_001f9bf8");
extern s32 FUN_L00_002e1f28(ElectroidMoby *, s32, s32);
extern f32 fast_add_rotations(f32, f32) __asm__("FUN_001fa580");
extern f32 fast_subtract_rotations(f32, f32) __asm__("FUN_001fa5c8");
extern void build_moby_bone_transform(ElectroidMoby *, s32, void *) __asm__("FUN_0020cca8");
extern f32 fast_cos(f32) __asm__("FUN_001f9dc8");
extern f32 fast_sin(f32) __asm__("FUN_001f9de0");
extern f32 compute_interpolated_record_value(ElectroidMoby *) __asm__("FUN_0020c9e0");
extern s32 collision_line(void *, void *, s32, ElectroidMoby *, ElectroidProbe *) __asm__("FUN_001efa68");
extern s32 FUN_L00_002e2040_c(ElectroidMoby *) __asm__("FUN_L00_002e2040");
extern s32 FUN_L00_002e2190(ElectroidMoby *, void *, void *);
extern s32 mine_fall(ElectroidMoby *, void *) __asm__("FUN_L00_0025c698");
extern void cart_burst(ElectroidMoby *, void *, s32, f32, f32) __asm__("FUN_L00_0025f090");
extern void FUN_L00_00263e30(ElectroidMoby *, s32, s32, s32, s32, s32, s32);
extern void FUN_L00_00263fd8(ElectroidMoby *, s32, void *, void *, s32, s32, f32, void *, void *,
                             void *);
extern void FUN_L00_002e1aa8();

#define ELECTROID_TURN(m, v, ang, k)                                                               \
    approach_angle(&(m)->unk40.f[2], ang, &(v)->turnVel, frame_time_sq *(k), frame_time_sq *(k),       \
                     frame_time * 12.566371f)
#define ELECTROID_ANIM(m, a, t)                                                                    \
    if ((m)->anim != (a))                                                                          \
    blend_moby_animation((m), (a), 0, (t))

extern f32 frame_scale_sq_gp __asm__("D_0015ED64") __attribute__((sda));
void veldin_robot_enemy_update(ElectroidMoby *moby) __asm__("FUN_L00_002e0b88");
void veldin_robot_enemy_update(ElectroidMoby *moby) {
    enum {
        VELDIN_ROBOT_STATE_INITIALIZE = 0,
        VELDIN_ROBOT_STATE_WAIT_TO_FOLLOW_PATH = 1,
        VELDIN_ROBOT_STATE_FOLLOW_PATH = 2,
        VELDIN_ROBOT_STATE_FACE_TARGET = 3,
        VELDIN_ROBOT_STATE_WAIT_TO_START_COLLISION_PROBES = 4,
        VELDIN_ROBOT_STATE_PROCESS_COLLISION_PROBES = 5,
        VELDIN_ROBOT_STATE_WAIT_TO_RETURN_TO_FACE_TARGET = 6,
        VELDIN_ROBOT_STATE_DAMAGE_KNOCKBACK = 7,
        VELDIN_ROBOT_STATE_DEATH_KNOCKBACK = 8
    };
    ElectroidVars *vars;
    ElectroidMoby *target;
    Path *path;
    ElectroidVec delta;
    ElectroidVec dir;
    ElectroidVec mtx[4];
    ElectroidProbe query;
    ElectroidVec probe;
    f32 vel;
    s32 hit;

    vars = moby->vars;
    FUN_L00_002e1678(moby);
    target = vars->target;
    FUN_L00_00263b38(vars->unk120, 2.5f);
    if (*(u8 *)0x15EDB7 != 0) {
        update_joint_animation(moby, vars->unk120, 2, frame_scale_sq_gp * 0.03f, frame_scale_sq_gp * 0.3f);
    }
    if (moby->unk31 != 0 && FUN_001f9b48(&moby->pos, &D_L00_00166DC0) < 27.0f) {
        FUN_L00_0025a120(moby);
        moby->unk7F = 0x15;
    }
    switch (moby->state) {
    case VELDIN_ROBOT_STATE_INITIALIZE:
        qcopy(&vars->spawnPos, &moby->pos);
        vars->unk29 = 0;
        vars->health = 2.0f;
        vars->unk28 = 1;
        vars->unk24 = 2;
        vars->unk58 = truncate_float_to_s32(14.0f);
        vars->unk5A = truncate_float_to_s32(8.0f);
        vars->pathPt = 1;
        if (random_integer_below(2) != 0) {
            moby->flags |= 0x8000;
        }
        if (vars->pathIdx >= 0) {
            if (vars->unk1B4 == -1) {
                mark_moby_for_removal(moby);
                return;
            }
            qcopy(&moby->pos, &D_L00_001B04B0[vars->pathIdx]->pts[0]);
            moby->state = 1;
            ELECTROID_ANIM(moby, 1, 5);
        } else {
            moby->state = 3;
            ELECTROID_ANIM(moby, 1, 5);
        }
        break;
    case VELDIN_ROBOT_STATE_WAIT_TO_FOLLOW_PATH:
        if (moby->unk70 & 2) {
            if (moby->anim == 1 && random_integer_below(4) == 0) {
                ELECTROID_ANIM(moby, 2, scale_game_frames_s32(20));
            } else {
                ELECTROID_ANIM(moby, 1, scale_game_frames_s32(20));
            }
        }
        if (vars->unk114 != 2) {
            moby->state = 2;
            ELECTROID_ANIM(moby, 9, scale_game_frames_s32(10));
        }
        break;
    case VELDIN_ROBOT_STATE_FOLLOW_PATH:
        vel = 0.0f;
        path = D_L00_001B04B0[vars->pathIdx];
        qcopy(&delta, &path->pts[vars->pathPt]);
        ELECTROID_TURN(moby, vars,
                  fast_arc_tan(delta.f[0] - moby->pos.f[0], delta.f[1] - moby->pos.f[1]),
                  12.566371f);
        subtract_vector_xyz(&dir, &delta, &moby->pos);
        advance_accelerated_scalar(FUN_001f9b48(&moby->pos, &path->pts[path->count - 1]),
                                   frame_time_sq * 8.0f, frame_time_sq * 16.0f, frame_time * 12.0f, &vel,
                                   &vars->speed);
        if (fast_vec_length(&dir) < vars->speed + vars->speed) {
            vars->pathPt++;
            if (vars->pathPt == path->count) {
                moby->state = 3;
                ELECTROID_ANIM(moby, 1, scale_game_frames_s32(10));
            }
        }
        fast_vec_normalize(&dir, &dir, vars->speed);
        add_vector_xyz(&moby->pos, &moby->pos, &dir);
        break;
    case VELDIN_ROBOT_STATE_FACE_TARGET:
        ELECTROID_TURN(
            moby, vars,
            fast_arc_tan(target->pos.f[0] - moby->pos.f[0], target->pos.f[1] - moby->pos.f[1]),
            12.566371f);
        if (vector_distance(&moby->pos, &hero.motion.pos) < 20.0f) {
            ELECTROID_ANIM(moby, 4, scale_game_frames_s32(20));
        } else if (moby->unk70 & 2) {
            ELECTROID_ANIM(moby, random_integer_below(2) + 1, scale_game_frames_s32(20));
        }
        if (vars->unk114 != 2 && FUN_L00_002e1f28(moby, 0, 0) != 0) {
            moby->state = 4;
            ELECTROID_ANIM(moby, 5, scale_game_frames_s32(10));
        }
        break;
    case VELDIN_ROBOT_STATE_WAIT_TO_START_COLLISION_PROBES:
        ELECTROID_TURN(
            moby, vars,
            fast_arc_tan(target->pos.f[0] - moby->pos.f[0], target->pos.f[1] - moby->pos.f[1]),
            12.566371f);
        if (moby->unk70 & 2) {
            vars->timer = 0;
            moby->state = 5;
            ELECTROID_ANIM(moby, 6, scale_game_frames_s32(10));
        }
        break;
    case VELDIN_ROBOT_STATE_PROCESS_COLLISION_PROBES: {
        f32 ang;

        ang = fast_arc_tan(vars->targetPos.f[0] - moby->pos.f[0], vars->targetPos.f[1] - moby->pos.f[1]);
        if (moby->flags & 0x8000) {
            ang = fast_add_rotations(ang, D_L00_00161BEC);
        } else {
            ang = fast_subtract_rotations(ang, D_L00_00161BEC);
        }
        ELECTROID_TURN(moby, vars, ang, 1.5707964f);
        build_moby_bone_transform(moby, 0, mtx);
        qcopy(&delta, &moby->unkC0);
        delta.f[2] = 0.0f;
        fast_vec_normalize(&delta, &delta, D_L00_00161BE4);
        add_vector_xyz(&dir, &mtx[3], &delta);
        hit = 0;
        query.flags = 0x10000;
        query.moby = moby;
        query.unk90 = 1;
        query.unk8C = 1.0f;
        query.dir[0] = fast_cos(moby->unk40.f[2]);
        query.dir[1] = fast_sin(moby->unk40.f[2]);
        query.dir[3] = 5627.925f;
        query.unk89 = 1;
        query.unk8A = moby->unkA6;
        query.dir[2] = 1.0f;
        query.unk88 = 0;
        if (moby->unk52 == moby->anim && compute_interpolated_record_value(moby) > 1.0f) {
            hit = 1;
            query.flags |= 1;
        }
        qcopy(&probe, &moby->pos);
        probe.f[2] += 0.5f;
        if (collision_line(&mtx[3], &probe, 0, moby, &query) != 0 && hit != 0 &&
            coll_hit.moby == (*(s32 *)&D_001413D0_lui)) {
            FUN_L00_002e2040_c(moby);
            moby->state = 6;
            ELECTROID_ANIM(moby, 7, scale_game_frames_s32(10));
        }
        if (collision_line(&mtx[3], &dir, 0, moby, &query) != 0) {
            qcopy(&dir, &coll_hit.point);
            if (hit != 0 && coll_hit.moby == (*(s32 *)&D_001413D0_lui)) {
                FUN_L00_002e2040_c(moby);
                moby->state = 6;
                ELECTROID_ANIM(moby, 7, scale_game_frames_s32(10));
            }
        }
        FUN_L00_002e2190(moby, mtx, &dir);
        if (moby->unk70 & 2) {
            if (vars->timer++ == D_L00_00161BE8) {
                FUN_L00_002e2040_c(moby);
                moby->state = 6;
                ELECTROID_ANIM(moby, 12, scale_game_frames_s32(10));
            }
        }
        break;
    }
    case VELDIN_ROBOT_STATE_WAIT_TO_RETURN_TO_FACE_TARGET:
        ELECTROID_TURN(
            moby, vars,
            fast_arc_tan(target->pos.f[0] - moby->pos.f[0], target->pos.f[1] - moby->pos.f[1]),
            12.566371f);
        if (moby->unk70 & 2) {
            moby->state = 3;
            ELECTROID_ANIM(moby, 1, scale_game_frames_s32(10));
        }
        break;
    case VELDIN_ROBOT_STATE_DAMAGE_KNOCKBACK:
        if (mine_fall(moby, vars->unk70) & 0x140) {
            moby->state = 3;
            ELECTROID_ANIM(moby, 1, scale_game_frames_s32(10));
        }
        break;
    case VELDIN_ROBOT_STATE_DEATH_KNOCKBACK:
        if (mine_fall(moby, vars->unk70) & 0x140) {
            cart_burst(moby, &moby->pos, -1, 0.75f, 13.0f);
            FUN_L00_00263e30(moby, 0x7AB, 1, 0x7AB, 1, 4, 0);
            FUN_L00_00263fd8(moby, 0x78D, &moby->pos, &moby->unk40, 0, 0, 0.0f, D_L00_0015F580,
                             D_L00_0015F580, D_L00_0015F580);
            FUN_L00_00263fd8(moby, 0x78E, &moby->pos, &moby->unk40, 0, 0, 0.0f, D_L00_0015F580,
                             D_L00_0015F580, D_L00_0015F580);
            FUN_L00_00263fd8(moby, 0x78F, &moby->pos, &moby->unk40, 0, 0, 0.0f, D_L00_0015F580,
                             D_L00_0015F580, D_L00_0015F580);
            mark_moby_for_removal(moby);
            return;
        }
        break;
    }
    FUN_L00_002e1aa8(moby);
}
#include "eetypes.h"
#include "qcopy.h"

typedef struct {
    u8 pad0[0x10];
    ElectroidVec pos;
} ElectroidHit;

extern s16 scale_game_frames(s32) __asm__("FUN_001f96f8");
extern int fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern f32 vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern void FUN_L00_00257470(ElectroidMoby *, s32, s32);
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern ElectroidHit *find_moby_hit(ElectroidMoby *, s32, s32) __asm__("FUN_L00_0025a420");
extern void hit_reaction(ElectroidMoby *, ElectroidHit *, void *, s32, s32 *, f32 *, s32, s32) __asm__("FUN_L00_0025a478");
extern void trooper_knockback_arc(ElectroidVec *, f32 *, f32 *, f32 *) __asm__("FUN_L00_0025ab48");
extern void trooper_start_knockback(ElectroidMoby *, void *, s32, s32, s32, f32) __asm__("FUN_L00_0025c558");
extern void trooper_anim_update(ElectroidMoby *, void *) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(ElectroidMoby *, void *) __asm__("FUN_L00_0025d538");
extern s32 FUN_L00_0025fcb8(ElectroidMoby *, f32, void *);
extern void FUN_L00_0025ff38(ElectroidMoby *, void *, s32, s32, void *, s32, f32);
extern void FUN_L00_002e2040(ElectroidMoby *);
extern f32 D_L00_00161BD0 __attribute__((sda));
extern f32 D_L00_00161BD4 __attribute__((sda));
extern f32 D_L00_00161BD8 __attribute__((sda));
extern f32 D_L00_00161BDC __attribute__((sda));
extern f32 D_L00_00161BE0 __attribute__((sda));

void FUN_L00_002e1678(ElectroidMoby *m) {
    ElectroidVars *vars;
    void *anim;
    ElectroidHit *coll;
    Path *a;
    ElectroidMoby *t;
    ElectroidVec v;
    struct Hero *g;
    s32 hit;
    f32 dmg;
    f32 angle;
    f32 unused;
    f32 range;

    vars = m->vars;
    if (m->state == 0) {
        return;
    }
    if (vars->unk38 != 0) {
        vars->unk38 = 0;
        vars->unk1DA = scale_game_frames(random_int_between(0xB4, 0x12C));
    }
    fast_dec_timer_s16(&vars->unk1DA);
    anim = vars->unk60;
    dmg = 0.0f;
    coll = find_moby_hit(m, 0x330000, 0);
    hit_reaction(m, coll, &vars->health, 0, &hit, &dmg, 0, 4);
    if (hit != 1 && m->state != 8 && dmg != 0.0f) {
        vars->health -= dmg;
        vars->unk90 = 0x200;
        vars->unk80 = D_L00_00161BD4 * frame_time_sq;
        vars->unk84 = D_L00_00161BD8 * frame_time_sq;
        vars->unk88 = D_L00_00161BDC * frame_time;
        vars->unk8C = D_L00_00161BE0 * frame_time;
        vars->unkAD = 0;
        vars->unk94 = 9;
        vars->unkBC = frame_time + frame_time;
        vars->unk98 = 0.5f;
        if (vars->health <= 0.0f) {
            vars->unk94 = 0x29;
            m->flags &= ~0x1000;
            v.q = coll->pos.q;
            trooper_knockback_arc(&v, &angle, &vars->unk88, &vars->unk8C);
            trooper_start_knockback(m, vars->unk70, 0xB, 1, 0, angle);
            vars->unkC0 = 7.0f;
            vars->unkC4 = 16.0f;
            m->state = 8;
            m->unk94 = 0;
            vars->unk67 = 0x78;
            trooper_anim_update(m, anim);
            FUN_L00_00257470(m, 0, -1);
        } else {
            vars->unk88 = D_L00_00161BDC * frame_time * 0.35f;
            vars->unk8C = D_L00_00161BE0 * frame_time * 0.5f;
            v.q = coll->pos.q;
            trooper_knockback_arc(&v, &unused, &vars->unk88, &vars->unk8C);
            trooper_start_knockback(m, vars->unk70, 8, 1, 0, fast_arc_tan(coll->pos.f[0], coll->pos.f[1]));
            vars->unkC4 = vars->unkC0 = -1.0f;
            m->state = 7;
            vars->unk67 = 0xFA;
            vars->unk26 = scale_game_frames(0x3C);
            trooper_anim_update(m, anim);
        }
        FUN_L00_002e2040(m);
    }
    m->unkA4 = 0xFF;
    trooper_anim_finish(m, anim);
    if (m->state == 1) {
        a = D_L00_001B04B0[vars->unk1B4];
        FUN_L00_0025ff38(m, &vars->targetPos, 0, 0, a->pts, a->count, 64.0f);
    } else if (m->state != 5) {
        range = D_L00_00161BD0;
        if (vars->unk1DA != 0) {
            range = 37.0f;
        }
        if (FUN_L00_0025fcb8(m, range, &vars->targetPos) != 2 &&
            range < vector_distance(&m->pos, &vars->targetPos)) {
            vars->unk114 = 2;
        }
    }
    if (vars->target == 0) {
        g = &hero;
        vars->target = ((ElectroidMoby *)g->moby);
        qcopy(&vars->targetPos, &g->motion.pos);
    }
    if (vars->unk1BC >= 0) {
        t = (ElectroidMoby *)(D_L00_0015FFD8 + (vars->unk1BC << 8));
        if (t == 0 || t->state == 0xFE || t->state == 0xFD) {
            vars->unk1BC = -1;
        } else {
            range = vector_distance(&m->pos, &t->pos);
            if (range < vector_distance(&m->pos, &vars->target->pos)) {
                vars->target = t;
                qcopy(&vars->targetPos, &t->pos);
                vars->unk114 = 1;
            }
        }
    }
}
typedef struct {
    u8 pad0[0x10];
    f32 unk10;
    u8 pad14[2];
    s16 unk16;
    s32 color;
} SparkTail;

typedef struct {
    u8 pad0[0xA];
    s16 life;
    f32 unkC;
    u8 pad10[0x10];
    SparkTail tail;
} SparkPart;

typedef struct {
    u8 pad0[0x1C0];
    f32 pos[4];
} SparkVars;

typedef struct {
    u8 pad0[0x78];
    SparkVars *vars;
} SparkMoby;

extern void moby_point_position(void *, s32, void *) __asm__("FUN_L00_0024f7c8");
extern void random_spherical_offset(void *out, f32 a, f32 b) __asm__("FUN_00213358");
extern SparkPart *FUN_L00_00274948(void *, void *, s32, void *);
extern f32 random_float_between(f32, f32) __asm__("FUN_002132a8");
extern f32 convert_integer_to_float(s32) __asm__("FUN_001fa6c0");
extern void FUN_L00_002e1c78();
void enqueue_callback_list_1(s32 arg0, s32 arg1) __asm__("FUN_001f4600");

void FUN_L00_002e1aa8(SparkMoby *m) {
    SparkVars *vars;
    SparkPart *p;
    SparkTail *t;
    f32 *pos;
    f32 vel[4];
    s32 i;
    s32 life;

    vars = m->vars;
    pos = vars->pos;
    moby_point_position(m, 1, pos);
    vars->pos[2] -= 0.333f;
    random_spherical_offset(vel, 0.005f, 0.03f);
    p = FUN_L00_00274948(pos, vel, 0x7F, m);
    if (p != 0) {
        p->unkC = random_float_between(6000.0f, 32000.0f);
    }
    for (i = 0; i < 3; i++) {
        p = FUN_L00_00274948(vars->pos, D_L00_0015F580, 0x7F, m);
        if (p != 0) {
            t = &p->tail;
            if (i == 2 && random_integer_below(8) == 0) {
                p->unkC = 180000.0f;
            } else {
                p->unkC = random_float_between(80000.0f, 120000.0f);
            }
            life = scale_game_frames_s32(2);
            p->life = life;
            t->unk10 = 1.0f / convert_integer_to_float((s16)life);
            t->color = 0x7F7F7F;
            t->unk16 = 3;
        }
    }
    enqueue_callback_list_1((s32)FUN_L00_002e1c78, (s32)m);
}
typedef union {
    u128 q;
    f32 f[4];
} GlowVec;

typedef struct {
    GlowVec corner[4];
    u32 color[4];
    s32 unk50;
    s32 unk54;
    s32 unk58;
    f32 unk5C;
    f32 unk60;
    s32 unk64;
    f32 unk68;
    f32 unk6C;
    s64 unk70;
    s64 tex;
    s64 unk80;
    s64 unk88;
} GlowQuad;

typedef struct {
    u8 pad0[0x1C0];
    GlowVec pos;
} GlowVars;

typedef struct {
    u8 pad0[0x78];
    GlowVars *vars;
} GlowMoby;

extern GlowVec glow_origin __asm__("D_L00_00166DC0");
extern GlowVec D_L00_001E6CA0[4];
extern void fast_vec_scale(f32, void *, void *) __asm__("FUN_001f9a68");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern void fast_draw_quad_real(void *, void *, s32) __asm__("FUN_001f7d30");
extern void fast_vec_cross(void *, void *, void *) __asm__("FUN_001f9ad8");
extern s64 get_effect_texture(s32) __asm__("FUN_001f44b8");

void FUN_L00_002e1c78(GlowMoby *m) {
    GlowQuad quad;
    GlowVec mat[4];
    GlowVec *src;
    GlowVec *dst;
    s32 i;
    f32 one;

    qcopy(&mat[3], &m->vars->pos);
    one = 1.0f;
    mat[3].f[3] = one;
    subtract_vector_xyz(&mat[0], &glow_origin, &mat[3]);
    fast_vec_normalize(&mat[0], &mat[0], one);
    fast_vec_cross(&mat[1], &mat[0], (GlowVec *)&hero.gravity_dir);
    fast_vec_normalize(&mat[1], &mat[1], -1.0f);
    fast_vec_cross(&mat[2], &mat[1], &mat[0]);
    quad.tex = get_effect_texture(0xB);
    quad.unk88 = 0x8000000048L;
    quad.unk80 = 0xFF9000000260L;
    quad.unk70 = 5;
    quad.color[3] = 0x80FFFFFF;
    quad.color[2] = 0x80FFFFFF;
    quad.color[1] = 0x80FFFFFF;
    quad.unk50 = 0;
    quad.unk54 = 0;
    quad.unk58 = 0;
    quad.unk5C = one;
    quad.unk60 = one;
    quad.color[0] = 0x80FFFFFF;
    quad.unk64 = 0;
    quad.unk68 = one;
    quad.unk6C = one;
    src = D_L00_001E6CA0;
    dst = quad.corner;
    for (i = 0; i < 4; i++) {
        fast_vec_scale(0.2f, dst++, src++);
    }
    fast_draw_quad_real(&quad, mat, 0);
}
typedef struct {
    u8 pad0[0x20];
    u8 state;
    u8 pad21[0xF];
    u8 alpha;
} SlotMoby;

extern s32 D_L00_00161BF8[] MACRO_ADDR;
extern s32 D_L00_00161C88[] MACRO_ADDR;
extern void FUN_L00_002e20f8(s32);
extern void FUN_L00_002e2250(s32, s32);
extern void FUN_L00_002e2af0();
void enqueue_callback_list_1(s32 arg0, s32 arg1) __asm__("FUN_001f4600");

void FUN_L00_002e1df0(SlotMoby *m) {
    enum {
        SLOT_CONTROLLER_STATE_INITIALIZE = 0,
        SLOT_CONTROLLER_STATE_PROCESS_SLOTS = 1
    };
    s32 i;
    s32 j;
    s32 n;
    s32 st;

    switch (m->state) {
    case SLOT_CONTROLLER_STATE_INITIALIZE:
        for (j = 0; j < 3; j++) {
            D_L00_00161BF8[j] = 0;
            D_L00_00161C88[j] = 0;
        }
        m->state = 1;
        m->alpha = 0xFF;
        break;
    case SLOT_CONTROLLER_STATE_PROCESS_SLOTS:
        n = 0;
        for (i = 0; i < 3; i++) {
            st = D_L00_00161BF8[i];
            if (st == 1) {
            } else if (st == 4) {
                FUN_L00_002e20f8(i);
            } else if (st != 0) {
                n++;
                FUN_L00_002e2250(D_L00_00161C88[i], i);
                D_L00_00161BF8[i] = 4;
            }
        }
        if (n != 0) {
            enqueue_callback_list_1((s32)FUN_L00_002e2af0, (s32)m);
        }
        break;
    }
}
typedef struct {
    u8 pad0[0x20];
    u8 state;
    u8 pad21[0xF];
    u8 unk30;
} FlockMoby;

typedef struct {
    u8 pad0[0x30];
    s32 phase;
    s32 level;
    u8 pad38[0x178 - 0x38];
    void *mobys[4];
} FlockGame;

typedef struct {
    u8 pad0[4];
    s32 unk4;
    u8 pad8[2];
    u8 unkA;
    u8 unkB;
} FlockTail;

typedef struct {
    u8 pad0[3];
    u8 unk3;
    u8 pad4[4];
    u8 unk8;
    u8 pad9;
    s16 unkA;
    u8 padC[0x20 - 0xC];
    FlockTail tail;
} FlockPart;

extern FlockGame D_L00_0016C860;

extern void cutscene_set_camera_target(void *) __asm__("FUN_L00_002637f8");
extern void moby_point_position(void *, s32, void *) __asm__("FUN_L00_0024f7c8");
extern void draw_trail(void *, void *, void *, f32, f32) __asm__("FUN_L00_00263b70");
extern FlockPart *FUN_L00_0026d000(void *, s32, void *, s32, f32, f32, f32, f32);

void FUN_L00_002e3800(FlockMoby *moby) {
    enum {
        FLOCK_EFFECT_STATE_INITIALIZE = 0,
        FLOCK_EFFECT_STATE_PROCESS_PHASE_EFFECTS = 1
    };
    f32 pos[4];
    void *src;
    FlockPart *part;
    s32 color;
    s32 v;
    s32 i;
    s32 phase;
    s32 idx;
    FlockTail *tail;

    switch (moby->state) {
    case FLOCK_EFFECT_STATE_INITIALIZE:
        moby->unk30 = 0xFF;
        moby->state = 1;
        break;
    case FLOCK_EFFECT_STATE_PROCESS_PHASE_EFFECTS:
        if ((D_L00_0015F5C4 == 2 && D_L00_0016C860.phase == 2 &&
             scale_game_frames_s32(0x618) <= D_L00_0016C860.level) ||
            (D_L00_0015F5C4 == 2 && D_L00_0016C860.phase == 3)) {
            phase = D_L00_0016C860.phase;
            idx = -1;
            if (phase == 2) {
                idx = 3;
            } else if (phase == 3) {
                idx = 2;
            }
            cutscene_set_camera_target(D_L00_0016C860.mobys[idx]);
        }
        if (D_L00_0015F5C4 == 2 && D_L00_0016C860.phase == 4) {
            src = D_L00_0016C860.mobys[3];
            if (src != 0 && scale_game_frames_s32(150) <= D_L00_0016C860.level &&
                D_L00_0016C860.level <= scale_game_frames_s32(220)) {
                moby_point_position(src, 0, pos);
                draw_trail(pos, pos, D_L00_0015F580, 120000.0f, 50000.0f);
                for (i = 0; i < 10; i++) {
                    color = (random_int_between(0x40, 0x80) << 24) | 0x787878;
                    v = random_int_between(0, 4);
                    part = FUN_L00_0026d000(pos, random_integer_below(2) == 0 ? v : -v,
                                            D_L00_0015F580, color, 0.75f, 1.0f, 1.03f, 120000.0f);
                    if (part != 0) {
                        part->unk3 = 0x44;
                        tail = &part->tail;
                        part->unk8 = random_int_between(0, 0xFF);
                        part->unkA = scale_game_frames_s32(60);
                        tail->unk4 = 2;
                        tail->unkA = color >> 24;
                        tail->unkB = part->unkA;
                    }
                }
            }
        }
        break;
    }
}
