/* Level 00 moby class veldin_robot_enemy: its update, damage and targeting, and its effect quad. */
#include "types.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/game/interpolated_record.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/mobys/animation/build_moby_bone_transform.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/mobys/test_path_segment_strict_crossing.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/overlay/shared/mobys/step_moby_motion.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/world/collision/collision_line.h"
#include "rnc/math/random/random_spherical_offset.h"
#include "rnc/overlay/shared/rendering/effects/beam_slots.h"
#include "rnc/overlay/shared/rendering/effects/particles.h"
#include "qcopy.h"
#include "rnc/overlay/collision.h"
#include "rnc/overlay/l00/mobys/veldin_robot_enemy_globals.h"

typedef union {
    u128 q;
    f32  f[4];
} ElectroidVec;

typedef struct {
    s32          count;
    s32          pad4[3];
    ElectroidVec pts[1];
} Path;

typedef struct ElectroidMoby ElectroidMoby;

typedef struct {
    u8             pad0[0x20];
    f32            health;
    s16            unk24;
    s16            unk26;
    u8             unk28;
    u8             unk29;
    u8             pad2A[0x38 - 0x2A];
    s32            unk38;
    u8             pad3C[0x58 - 0x3C];
    u8             unk58;
    u8             pad59;
    u8             unk5A;
    u8             pad5B[0x60 - 0x5B];
    u8             unk60[7];
    u8             unk67;
    u8             pad68[8];
    u8             unk70[0x10];
    f32            unk80;
    f32            unk84;
    f32            unk88;
    f32            unk8C;
    s32            unk90;
    s32            unk94;
    f32            unk98;
    u8             pad9C[0xAD - 0x9C];
    u8             unkAD;
    u8             padAE[0xBC - 0xAE];
    f32            unkBC;
    f32            unkC0;
    f32            unkC4;
    u8             padC8[8];
    ElectroidVec   targetPos;
    u8             padE0[0x110 - 0xE0];
    ElectroidMoby *target;
    s32            unk114;
    u8             pad118[0x120 - 0x118];
    u8             unk120[0x1A0 - 0x120];
    u128           spawnPos;
    u8             pad1B0[4];
    s32            unk1B4;
    s32            pathIdx;
    s32            unk1BC;
    u8             pad1C0[0x1D0 - 0x1C0];
    f32            turnVel;
    f32            speed;
    s16            timer;
    s16            unk1DA;
    s32            pathPt;
} ElectroidVars;

struct ElectroidMoby {
    u8             pad0[0x10];
    ElectroidVec   pos;
    u8             state;
    u8             pad21[0x31 - 0x21];
    u8             unk31;
    u8             pad32[2];
    u16            flags;
    u8             pad36[0x40 - 0x36];
    ElectroidVec   unk40;
    u8             pad50[2];
    u8             unk52;
    u8             anim;
    u8             pad54[0x70 - 0x54];
    u8             unk70;
    u8             pad71[0x78 - 0x71];
    ElectroidVars *vars;
    u8             pad7C[3];
    s8             unk7F;
    u8             pad80[0x94 - 0x80];
    s32            unk94;
    u8             pad98[0xA4 - 0x98];
    u8             unkA4;
    u8             padA5;
    u16            unkA6;
    u8             padA8[0xC0 - 0xA8];
    ElectroidVec   unkC0;
};

typedef struct {
    f32            dir[4];
    ElectroidMoby *moby;
    s32            flags;
    u8             unk88;
    u8             unk89;
    u16            unk8A;
    f32            unk8C;
    s32            unk90;
} ElectroidProbe;

f32                 veldin_robot_probe_length_gp __asm__("D_L00_00161BE4") = 5.0f;
s32                 veldin_robot_beam_hit_frames __asm__("D_L00_00161BE8") = 3;
f32                 veldin_robot_probe_heading_offset_gp __asm__("D_L00_00161BEC") = -0.15f;
extern Vec4         D_L00_00166DC0;
extern s32          D_001413D0_lui[] __asm__("D_001413D0") NOT_SDA;
extern CollisionHit coll_hit __asm__("D_L00_00173E40");
extern Path        *toad_paths[] __asm__("D_L00_001B04B0");
extern Vec4         zero_vector __asm__("D_L00_0015F580") MACRO_ADDR;

extern void update_veldin_robot_damage_and_targeting() __asm__("FUN_L00_002e1678");
extern void FUN_L00_0025a120(ElectroidMoby *);
extern s32  scale_game_frames_s32(s32) __asm__("FUN_001f96f8");
extern void approach_angle(void *, f32, void *, f32, f32, f32) __asm__("FUN_L00_0025be00");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern f32  fast_vec_length(void *) __asm__("FUN_001f9af0");
extern f32  advance_accelerated_scalar(f32, f32, f32, f32, f32 *, f32 *) __asm__("FUN_00213f38");
extern void fast_vec_normalize(void *, void *, f32) __asm__("FUN_001f9bf8");
extern f32  fast_cos(f32) __asm__("FUN_001f9dc8");
extern f32  fast_sin(f32) __asm__("FUN_001f9de0");
extern s32  release_beam_slot_by_id(ElectroidMoby *) __asm__("FUN_L00_002e2040");
extern void
FUN_L00_00263fd8(ElectroidMoby *, s32, void *, void *, s32, s32, f32, void *, void *, void *);
extern void update_veldin_robot_effects() __asm__("FUN_L00_002e1aa8");

#define ELECTROID_TURN(m, v, ang, k)   \
    approach_angle(&(m)->unk40.f[2],   \
                   ang,                \
                   &(v)->turnVel,      \
                   frame_time_sq *(k), \
                   frame_time_sq *(k), \
                   frame_time * 12.566371f)
#define ELECTROID_ANIM(m, a, t) \
    if ((m)->anim != (a))       \
    blend_moby_animation((m), (a), 0, (t))

extern f32 frame_scale_sq_gp __asm__("D_0015ED64") __attribute__((sda));
void       veldin_robot_enemy_update(ElectroidMoby *moby) __asm__("FUN_L00_002e0b88");

/* Moby class 1440 */
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
    Path          *path;
    ElectroidVec   delta;
    ElectroidVec   dir;
    ElectroidVec   mtx[4];
    ElectroidProbe query;
    ElectroidVec   probe;
    f32            path_step_distance;
    s32            hit;

    vars = moby->vars;
    update_veldin_robot_damage_and_targeting(moby);
    target = vars->target;
    manipulator_set_scale(vars->unk120, 2.5f);
    if (*(u8 *)0x15EDB7 != 0) {
        update_joint_animation(
            moby, vars->unk120, 2, frame_scale_sq_gp * 0.03f, frame_scale_sq_gp * 0.3f);
    }
    if (moby->unk31 != 0 && fast_vec_dist(&moby->pos, &D_L00_00166DC0) < 27.0f) {
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
            qcopy(&moby->pos, &toad_paths[vars->pathIdx]->pts[0]);
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
        path_step_distance = 0.0f;
        path = toad_paths[vars->pathIdx];
        qcopy(&delta, &path->pts[vars->pathPt]);
        ELECTROID_TURN(moby,
                       vars,
                       fast_arc_tan(delta.f[0] - moby->pos.f[0], delta.f[1] - moby->pos.f[1]),
                       12.566371f);
        fast_vec_sub(&dir, &delta, &moby->pos);
        advance_accelerated_scalar(fast_vec_dist(&moby->pos, &path->pts[path->count - 1]),
                                   frame_time_sq * 8.0f,
                                   frame_time_sq * 16.0f,
                                   frame_time * 12.0f,
                                   &path_step_distance,
                                   &vars->speed);
        if (fast_vec_length(&dir) < vars->speed + vars->speed) {
            vars->pathPt++;
            if (vars->pathPt == path->count) {
                moby->state = 3;
                ELECTROID_ANIM(moby, 1, scale_game_frames_s32(10));
            }
        }
        fast_vec_normalize(&dir, &dir, vars->speed);
        fast_vec_add(&moby->pos, &moby->pos, &dir);
        break;
    case VELDIN_ROBOT_STATE_FACE_TARGET:
        ELECTROID_TURN(
            moby,
            vars,
            fast_arc_tan(target->pos.f[0] - moby->pos.f[0], target->pos.f[1] - moby->pos.f[1]),
            12.566371f);
        if (vector_distance_xy(&moby->pos, &hero.motion.pos) < 20.0f) {
            ELECTROID_ANIM(moby, 4, scale_game_frames_s32(20));
        } else if (moby->unk70 & 2) {
            ELECTROID_ANIM(moby, random_integer_below(2) + 1, scale_game_frames_s32(20));
        }
        if (vars->unk114 != 2 && register_beam_slot(moby, 0, 0) != 0) {
            moby->state = 4;
            ELECTROID_ANIM(moby, 5, scale_game_frames_s32(10));
        }
        break;
    case VELDIN_ROBOT_STATE_WAIT_TO_START_COLLISION_PROBES:
        ELECTROID_TURN(
            moby,
            vars,
            fast_arc_tan(target->pos.f[0] - moby->pos.f[0], target->pos.f[1] - moby->pos.f[1]),
            12.566371f);
        if (moby->unk70 & 2) {
            vars->timer = 0;
            moby->state = 5;
            ELECTROID_ANIM(moby, 6, scale_game_frames_s32(10));
        }
        break;
    case VELDIN_ROBOT_STATE_PROCESS_COLLISION_PROBES: {
        f32 desired_heading;

        desired_heading = fast_arc_tan(vars->targetPos.f[0] - moby->pos.f[0],
                                       vars->targetPos.f[1] - moby->pos.f[1]);
        if (moby->flags & 0x8000) {
            desired_heading =
                fast_add_rotations(desired_heading, veldin_robot_probe_heading_offset_gp);
        } else {
            desired_heading =
                fast_subtract_rotations(desired_heading, veldin_robot_probe_heading_offset_gp);
        }
        ELECTROID_TURN(moby, vars, desired_heading, 1.5707964f);
        build_moby_bone_transform(moby, 0, mtx);
        qcopy(&delta, &moby->unkC0);
        delta.f[2] = 0.0f;
        fast_vec_normalize(&delta, &delta, veldin_robot_probe_length_gp);
        fast_vec_add(&dir, &mtx[3], &delta);
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
            release_beam_slot_by_id(moby);
            moby->state = 6;
            ELECTROID_ANIM(moby, 7, scale_game_frames_s32(10));
        }
        if (collision_line(&mtx[3], &dir, 0, moby, &query) != 0) {
            qcopy(&dir, &coll_hit.point);
            if (hit != 0 && coll_hit.moby == (*(s32 *)&D_001413D0_lui)) {
                release_beam_slot_by_id(moby);
                moby->state = 6;
                ELECTROID_ANIM(moby, 7, scale_game_frames_s32(10));
            }
        }
        set_beam_origin_transform_and_endpoint(moby, mtx, &dir);
        if (moby->unk70 & 2) {
            if (vars->timer++ == veldin_robot_beam_hit_frames) {
                release_beam_slot_by_id(moby);
                moby->state = 6;
                ELECTROID_ANIM(moby, 12, scale_game_frames_s32(10));
            }
        }
        break;
    }
    case VELDIN_ROBOT_STATE_WAIT_TO_RETURN_TO_FACE_TARGET:
        ELECTROID_TURN(
            moby,
            vars,
            fast_arc_tan(target->pos.f[0] - moby->pos.f[0], target->pos.f[1] - moby->pos.f[1]),
            12.566371f);
        if (moby->unk70 & 2) {
            moby->state = 3;
            ELECTROID_ANIM(moby, 1, scale_game_frames_s32(10));
        }
        break;
    case VELDIN_ROBOT_STATE_DAMAGE_KNOCKBACK:
        if (step_moby_motion(moby, vars->unk70) & 0x140) {
            moby->state = 3;
            ELECTROID_ANIM(moby, 1, scale_game_frames_s32(10));
        }
        break;
    case VELDIN_ROBOT_STATE_DEATH_KNOCKBACK:
        if (step_moby_motion(moby, vars->unk70) & 0x140) {
            cart_burst(moby, &moby->pos, -1, 0.75f, 13.0f);
            spawn_moby_break_fragments(moby, 0x7AB, 1, 0x7AB, 1, 4, 0);
            FUN_L00_00263fd8(moby,
                             0x78D,
                             &moby->pos,
                             &moby->unk40,
                             0,
                             0,
                             0.0f,
                             &zero_vector,
                             &zero_vector,
                             &zero_vector);
            FUN_L00_00263fd8(moby,
                             0x78E,
                             &moby->pos,
                             &moby->unk40,
                             0,
                             0,
                             0.0f,
                             &zero_vector,
                             &zero_vector,
                             &zero_vector);
            FUN_L00_00263fd8(moby,
                             0x78F,
                             &moby->pos,
                             &moby->unk40,
                             0,
                             0,
                             0.0f,
                             &zero_vector,
                             &zero_vector,
                             &zero_vector);
            mark_moby_for_removal(moby);
            return;
        }
        break;
    }
    update_veldin_robot_effects(moby);
}

typedef struct {
    u8           pad0[0x10];
    ElectroidVec pos;
} ElectroidHit;

extern s16  scale_game_frames(s32) __asm__("FUN_001f96f8");
extern void FUN_L00_00257470(ElectroidMoby *, s32, s32);
extern void
hit_reaction(ElectroidMoby *, ElectroidHit *, void *, s32, s32 *, f32 *, s32, s32) __asm__("FUN_L00_0025a478");
extern void trooper_knockback_arc(ElectroidVec *, f32 *, f32 *, f32 *) __asm__("FUN_L00_0025ab48");
extern void
trooper_start_knockback(ElectroidMoby *, void *, s32, s32, s32, f32) __asm__("FUN_L00_0025c558");
extern void trooper_anim_update(ElectroidMoby *, void *) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(ElectroidMoby *, void *) __asm__("FUN_L00_0025d538");
extern s32  select_moby_combat_target(ElectroidMoby *, f32, void *) __asm__("FUN_L00_0025fcb8");
extern void FUN_L00_0025ff38(ElectroidMoby *, void *, s32, s32, void *, s32, f32);
f32         veldin_robot_default_target_range_gp __asm__("D_L00_00161BD0") = 24.0f;
f32         veldin_robot_knockback_xy_scale_gp __asm__("D_L00_00161BDC") = 1e+01f;
f32         veldin_robot_knockback_z_scale_gp __asm__("D_L00_00161BE0") = 9.5f;

void update_veldin_robot_damage_and_targeting(ElectroidMoby *moby) {
    ElectroidVars *vars;
    void          *anim;
    ElectroidHit  *coll;
    Path          *path;
    ElectroidMoby *candidate_target;
    ElectroidVec   hit_position;
    struct Hero   *hero_ptr;
    s32            hit;
    f32            dmg;
    f32            angle;
    f32            unused;
    f32            range;

    vars = moby->vars;
    if (moby->state == 0) {
        return;
    }
    if (vars->unk38 != 0) {
        vars->unk38 = 0;
        vars->unk1DA = scale_game_frames(random_int_between(0xB4, 0x12C));
    }
    fast_dec_timer_s16(&vars->unk1DA);
    anim = vars->unk60;
    dmg = 0.0f;
    coll = find_moby_hit(moby, 0x330000, 0);
    hit_reaction(moby, coll, &vars->health, 0, &hit, &dmg, 0, 4);
    if (hit != 1 && moby->state != 8 && dmg != 0.0f) {
        vars->health -= dmg;
        vars->unk90 = 0x200;
        vars->unk80 = veldin_robot_knockback_gravity_gp * frame_time_sq;
        vars->unk84 = veldin_robot_knockback_xy_speed_step_gp * frame_time_sq;
        vars->unk88 = veldin_robot_knockback_xy_scale_gp * frame_time;
        vars->unk8C = veldin_robot_knockback_z_scale_gp * frame_time;
        vars->unkAD = 0;
        vars->unk94 = 9;
        vars->unkBC = frame_time + frame_time;
        vars->unk98 = 0.5f;
        if (vars->health <= 0.0f) {
            vars->unk94 = 0x29;
            moby->flags &= ~0x1000;
            hit_position.q = coll->pos.q;
            trooper_knockback_arc(&hit_position, &angle, &vars->unk88, &vars->unk8C);
            trooper_start_knockback(moby, vars->unk70, 0xB, 1, 0, angle);
            vars->unkC0 = 7.0f;
            vars->unkC4 = 16.0f;
            moby->state = 8;
            moby->unk94 = 0;
            vars->unk67 = 0x78;
            trooper_anim_update(moby, anim);
            FUN_L00_00257470(moby, 0, -1);
        } else {
            vars->unk88 = veldin_robot_knockback_xy_scale_gp * frame_time * 0.35f;
            vars->unk8C = veldin_robot_knockback_z_scale_gp * frame_time * 0.5f;
            hit_position.q = coll->pos.q;
            trooper_knockback_arc(&hit_position, &unused, &vars->unk88, &vars->unk8C);
            trooper_start_knockback(
                moby, vars->unk70, 8, 1, 0, fast_arc_tan(coll->pos.f[0], coll->pos.f[1]));
            vars->unkC4 = vars->unkC0 = -1.0f;
            moby->state = 7;
            vars->unk67 = 0xFA;
            vars->unk26 = scale_game_frames(0x3C);
            trooper_anim_update(moby, anim);
        }
        release_beam_slot_by_id(moby);
    }
    moby->unkA4 = 0xFF;
    trooper_anim_finish(moby, anim);
    if (moby->state == 1) {
        path = toad_paths[vars->unk1B4];
        FUN_L00_0025ff38(moby, &vars->targetPos, 0, 0, path->pts, path->count, 64.0f);
    } else if (moby->state != 5) {
        range = veldin_robot_default_target_range_gp;
        if (vars->unk1DA != 0) {
            range = 37.0f;
        }
        if (select_moby_combat_target(moby, range, &vars->targetPos) != 2 &&
            range < vector_distance_xy(&moby->pos, &vars->targetPos)) {
            vars->unk114 = 2;
        }
    }
    if (vars->target == 0) {
        hero_ptr = &hero;
        vars->target = ((ElectroidMoby *)hero_ptr->moby);
        qcopy(&vars->targetPos, &hero_ptr->motion.pos);
    }
    if (vars->unk1BC >= 0) {
        candidate_target = (ElectroidMoby *)(moby_table + (vars->unk1BC << 8));
        if (candidate_target == 0 || candidate_target->state == 0xFE ||
            candidate_target->state == 0xFD) {
            vars->unk1BC = -1;
        } else {
            range = vector_distance_xy(&moby->pos, &candidate_target->pos);
            if (range < vector_distance_xy(&moby->pos, &vars->target->pos)) {
                vars->target = candidate_target;
                qcopy(&vars->targetPos, &candidate_target->pos);
                vars->unk114 = 1;
            }
        }
    }
}

typedef struct {
    u8  pad0[0x10];
    f32 unk10;
    u8  pad14[2];
    s16 unk16;
    s32 color;
} SparkTail;

typedef struct {
    u8        pad0[0xA];
    s16       life;
    f32       unkC;
    u8        pad10[0x10];
    SparkTail tail;
} SparkPart;

typedef struct {
    u8  pad0[0x1C0];
    f32 pos[4];
} SparkVars;

typedef struct {
    u8         pad0[0x78];
    SparkVars *vars;
} SparkMoby;

extern void draw_veldin_robot_effect_quad() __asm__("FUN_L00_002e1c78");

void update_veldin_robot_effects(SparkMoby *moby) {
    SparkVars *vars;
    SparkPart *particle;
    SparkTail *particle_tail;
    f32       *pos;
    f32        particle_initial_velocity[4];
    s32        i;
    s32        life;

    vars = moby->vars;
    pos = vars->pos;
    moby_point_position(moby, 1, pos);
    vars->pos[2] -= 0.333f;
    random_spherical_offset(particle_initial_velocity, 0.005f, 0.03f);
    particle = spawn_mode_controlled_rotating_alpha_fade_particle(
        pos, particle_initial_velocity, 0x7F, moby);
    if (particle != 0) {
        particle->unkC = random_float_between(6000.0f, 32000.0f);
    }
    for (i = 0; i < 3; i++) {
        particle =
            spawn_mode_controlled_rotating_alpha_fade_particle(vars->pos, &zero_vector, 0x7F, moby);
        if (particle != 0) {
            particle_tail = &particle->tail;
            if (i == 2 && random_integer_below(8) == 0) {
                particle->unkC = 180000.0f;
            } else {
                particle->unkC = random_float_between(80000.0f, 120000.0f);
            }
            life = scale_game_frames_s32(2);
            particle->life = life;
            particle_tail->unk10 = 1.0f / convert_integer_to_float((s16)life);
            particle_tail->color = 0x7F7F7F;
            particle_tail->unk16 = 3;
        }
    }
    add_draw_callback((s32)draw_veldin_robot_effect_quad, (s32)moby);
}

typedef union {
    u128 q;
    f32  f[4];
} GlowVec;

typedef struct {
    GlowVec corner[4];
    u32     color[4];
    s32     unk50;
    s32     unk54;
    s32     unk58;
    f32     unk5C;
    f32     unk60;
    s32     unk64;
    f32     unk68;
    f32     unk6C;
    s64     unk70;
    s64     tex;
    s64     unk80;
    s64     unk88;
} GlowQuad;

typedef struct {
    u8      pad0[0x1C0];
    GlowVec pos;
} GlowVars;

typedef struct {
    u8        pad0[0x78];
    GlowVars *vars;
} GlowMoby;

extern GlowVec g_debug_camera __asm__("D_L00_00166DC0");
extern GlowVec l00_veldin_robot_effect_quad_template_corners[4] __asm__("D_L00_001E6CA0");
extern void    fast_vec_scale(f32, void *, void *) __asm__("FUN_001f9a68");
extern void    fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern void    draw_geometry_quad(void *, void *, s32) __asm__("FUN_001f7d30");
extern void    fast_vec_cross(void *, void *, void *) __asm__("FUN_001f9ad8");
extern s64     get_effect_texture(s32) __asm__("FUN_001f44b8");

void draw_veldin_robot_effect_quad(GlowMoby *moby) {
    GlowQuad quad;
    GlowVec  quad_transform[4];
    GlowVec *template_corner;
    GlowVec *quad_corner;
    s32      i;
    f32      one;

    qcopy(&quad_transform[3], &moby->vars->pos);
    one = 1.0f;
    quad_transform[3].f[3] = one;
    fast_vec_sub(&quad_transform[0], &g_debug_camera, &quad_transform[3]);
    fast_vec_normalize(&quad_transform[0], &quad_transform[0], one);
    fast_vec_cross(&quad_transform[1], &quad_transform[0], (GlowVec *)&hero.gravity_dir);
    fast_vec_normalize(&quad_transform[1], &quad_transform[1], -1.0f);
    fast_vec_cross(&quad_transform[2], &quad_transform[1], &quad_transform[0]);
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
    template_corner = l00_veldin_robot_effect_quad_template_corners;
    quad_corner = quad.corner;
    for (i = 0; i < 4; i++) {
        fast_vec_scale(0.2f, quad_corner++, template_corner++);
    }
    draw_geometry_quad(&quad, quad_transform, 0);
}
