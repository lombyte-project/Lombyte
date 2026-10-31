/* Level 15 helicopter moby class: sphere-collision movement, steering, flocking and hits. */
#include "types.h"
#include "rnc/overlay/level_save_state.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/mobys/attach_manipulator.h"
#include "rnc/overlay/shared/mobys/count_active_moby_group_members.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "qcopy.h"
#include "rnc/overlay/collision.h"
#include "rnc/math/vector.h"
#include "eetypes.h"
#include "rnc/overlay/level_path.h"
#include "rnc/overlay/l15/mobys/helicopter_globals.h"
#include "rnc/overlay/quad.h"

extern char D_L15_001673C0[];

extern char D_0014C190[];

extern int D_L15_001BAAD0[];

extern char *D_L15_0015FFE4;

extern void FUN_L00_00257470(void *, int, int);

extern void spawn_explosion(void         *m,
                            void         *unused,
                            void         *pos,
                            float         fA,
                            float         fB,
                            int           n,
                            int           k8,
                            int           k9,
                            float         fC,
                            float         fD,
                            float         fE,
                            float         fF,
                            int           k10,
                            float         fG,
                            int           k11,
                            int           s16,
                            int           s18,
                            unsigned char flag) __asm__("FUN_L00_0025e450");

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");

extern float spring_turn(float *, float, float, float, float, float) __asm__("FUN_L00_00258110");

extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");

typedef union {
    u128  q;
    float f[4];
} SteeringVec_4f88;

typedef struct {
    char             pad[0x150];
    SteeringVec_4f88 delta;
    char             pad160[0x10];
    float            speed;
    float            pitch_velocity;
    float            yaw_velocity;
    float            facing_velocity;
    char             pad180[0x28];
    float            height;
} SteeringVars_4f88;

extern void FUN_001f9bf8_4f88(float, void *, void *) __asm__("FUN_001f9bf8");

extern void FUN_001f9a68_4f88(float, void *, void *) __asm__("FUN_001f9a68");

extern CollisionHit coll_hit __asm__("D_L15_00174440") NOT_SDA;
extern int          coll_sphere(void *, float, int, void *) __asm__("FUN_L00_001f0d60");

int move_moby_with_sphere_collision(struct Moby *moving_moby,
                                    u128        *delta,
                                    int          flags,
                                    float        step,
                                    float        radius) __asm__("FUN_L15_00247e90");

int move_moby_with_sphere_collision(

    struct Moby *moving_moby, u128 *delta, int flags, float step, float radius) {
    Vec4 start;
    Vec4 probe;
    int  pushed = 0;
    int  i;
    int  mask;

    qcopy(&start, &moving_moby->pos);
    fast_vec_add(&moving_moby->pos, &moving_moby->pos, delta);
    i = 0;
    do {
        mask = (flags << 1) & 0x20;
        qcopy(&probe, &moving_moby->pos);
        probe.f[2] += step;
        if (coll_sphere(&probe, radius, mask | 4, moving_moby) == 0) {
            break;
        }
        if (fast_arc_tan(coll_hit.normal_z, vector_length_xy(&coll_hit.normal_x)) > 0.5235988f) {
            qcopy(&moving_moby->pos, coll_hit.pad30);
            moving_moby->pos.z -= step;
            pushed |= 1;
        }
        i++;
    } while (i < 6);
    fast_vec_sub(delta, &moving_moby->pos, &start);
    return pushed;
}

void steer_helicopter(struct Moby      *moby,
                      SteeringVec_4f88 *goal,
                      SteeringVec_4f88 *look) __asm__("FUN_L15_002c4f88");

void steer_helicopter(struct Moby *moby, SteeringVec_4f88 *goal, SteeringVec_4f88 *look) {
    SteeringVec_4f88   target;
    SteeringVec_4f88   look_copy;
    SteeringVec_4f88   scaled;
    SteeringVec_4f88   origin;
    SteeringVec_4f88   delta;
    SteeringVec_4f88  *look_at = &look_copy;
    SteeringVec_4f88  *target_at = &target;
    SteeringVars_4f88 *helicopter_steering_vars = (SteeringVars_4f88 *)moby->pvars;
    float              distance;
    float              step;
    float              old_speed;
    float              limit;
    float              facing;
    float              angle;
    float              relative_heading_cosine;
    float              relative_heading_sine;
    float              yaw_target;
    float              pitch_target;
    float              dx;
    float              dy;
    float              threshold;
    float              speed_sq;
    float              twice_step;
    float              height;
    float              pos_x;
    float              pos_y;

    target.q = goal->q;
    look_copy.q = look->q;
    origin.q = *(u128 *)&moby->pos;
    distance = fast_vec_dist(&moby->pos, target_at);
    step = frame_time_sq * 10.0f;
    old_speed = helicopter_steering_vars->speed;
    speed_sq = old_speed * old_speed;
    twice_step = step + step;
    threshold = speed_sq / twice_step;
    if (distance <= threshold) {
        helicopter_steering_vars->speed = old_speed - step;
        if (helicopter_steering_vars->speed < 0.0f) {
            helicopter_steering_vars->speed = 0.0f;
        }
    } else {
        helicopter_steering_vars->speed = old_speed + step;
        limit = frame_time * 10.0f;
        if (limit < helicopter_steering_vars->speed) {
            helicopter_steering_vars->speed = limit;
        }
    }

    fast_vec_sub(&delta, target_at, &moby->pos);
    scaled.q = delta.q;
    FUN_001f9bf8_4f88(helicopter_steering_vars->speed * 0.1f, &scaled, &scaled);
    FUN_001f9a68_4f88(0.9f, &delta, &helicopter_steering_vars->delta);
    helicopter_steering_vars->delta.q = delta.q;
    add_vectors(&delta, &helicopter_steering_vars->delta, &scaled);
    move_moby_with_sphere_collision(moby, &helicopter_steering_vars->delta.q, 0, 1.0f, 2.0f);
    /* Preserve the retail height store and facing input access order. */
    height = *(volatile float *)&helicopter_steering_vars->height;
    pos_x = *(volatile float *)&moby->pos.x;
    *(volatile float *)&moby->pos.z = height;
    pos_y = *(volatile float *)&moby->pos.y;
    dy = *(volatile float *)&look_at->f[1];
    dx = *(volatile float *)&look_at->f[0];
    dy = dy - pos_y;
    dx = dx - pos_x;
    facing = fast_arc_tan(dx, dy);
    moby->rot.z = spring_turn(
        &helicopter_steering_vars->facing_velocity, moby->rot.z, facing, 0.01f, 0.3f, 0.1f);
    fast_vec_sub(&delta, &moby->pos, &origin);
    helicopter_steering_vars->delta.q = delta.q;
    angle =
        fast_arc_tan(helicopter_steering_vars->delta.f[0], helicopter_steering_vars->delta.f[1]);
    angle = fast_subtract_rotations(angle, moby->rot.z);
    relative_heading_cosine = fast_cos(angle);
    yaw_target = (helicopter_steering_vars->speed * 0.34906584f * relative_heading_cosine) /
                 (frame_time * 10.0f);
    relative_heading_sine = fast_sin(angle);
    pitch_target = (helicopter_steering_vars->speed * -0.34906584f * relative_heading_sine) /
                   (frame_time * 10.0f);
    moby->rot.y = spring_turn(&helicopter_steering_vars->yaw_velocity,
                              moby->rot.y,
                              yaw_target,
                              frame_time_sq * 0.5235988f,
                              frame_time_sq * 1.0471976f,
                              frame_time * 0.7853982f);
    moby->rot.x = spring_turn(&helicopter_steering_vars->pitch_velocity,
                              moby->rot.x,
                              pitch_target,
                              frame_time_sq * 1.0471976f,
                              frame_time_sq * 2.0943952f,
                              frame_time * 1.5707964f);
}

typedef struct {
    union {
        OvlQuad q;
        float   f[4];
    } pos;
    OvlQuad rest[4];
} Goal_52b8;

extern void normalize_vector_xyz(void *, void *, float) __asm__("FUN_001f9bf8");

extern void scale_vector_xyz_52b8(void *, void *, float) __asm__("FUN_001f9a68");

float compute_helicopter_flocking_steering_point(struct Moby *moby,
                                                 float       *out,
                                                 Goal_52b8    goal) __asm__("FUN_L15_002c52b8");

float compute_helicopter_flocking_steering_point(struct Moby *moby,
                                                 float       *flocking_steering_point,

                                                 Goal_52b8 goal) {
    float steering_offset[4];
    float goal_distance;
    float wsum;
    int   clear;
    float k;
    char *other_moby;
    goal_distance = vector_distance_xy(&moby->pos, &goal);
    clear = 1;
    clear_u64_value(flocking_steering_point);
    wsum = 0.0f;
    for (other_moby = D_L15_0015FFE4; other_moby != 0;
         other_moby = ((char *)MOBY(other_moby)->next)) {
        if (other_moby == moby)
            continue;
        if (MOBY(other_moby)->oclass != moby->oclass)
            continue;
        if ((u8)other_moby[0x20] == 1 || (u8)other_moby[0x20] == 0x40)
            continue;
        if (vector_distance_xy(other_moby + 0x10, &moby->pos) < 10.0f) {
            wsum += 6.0f;
            fast_vec_sub(steering_offset, &moby->pos, other_moby + 0x10);
            scale_vector_xyz_52b8(steering_offset, steering_offset, 6.0f);
            fast_vec_add(flocking_steering_point, flocking_steering_point, steering_offset);
        }
        if (vector_distance_xy(other_moby + 0x10, &goal) < goal_distance)
            clear = 0;
    }
    if (clear == 0) {
        fast_vec_sub(steering_offset, &moby->pos, &goal);
        if (goal_distance < 14.5f) {
            k = 15.0f;
        } else if (15.5f < goal_distance) {
            k = -15.0f;
        } else {
            k = 0.0f;
        }
        wsum += 10.0f;
        normalize_vector_xyz(steering_offset, steering_offset, k * 10.0f);
        fast_vec_add(flocking_steering_point, flocking_steering_point, steering_offset);
        scale_vector_xyz_52b8(flocking_steering_point, flocking_steering_point, 1.0f / wsum);
        fast_vec_add(flocking_steering_point, flocking_steering_point, &moby->pos);
    } else {
        fast_vec_sub(steering_offset, &moby->pos, &goal);
        if (goal_distance < 9.5f) {
            k = 10.0f;
        } else if (10.5f < goal_distance) {
            k = -10.0f;
        } else {
            k = 0.0f;
        }
        wsum += 10.0f;
        normalize_vector_xyz(steering_offset, steering_offset, k * 10.0f);
        fast_vec_add(flocking_steering_point, flocking_steering_point, steering_offset);
        scale_vector_xyz_52b8(flocking_steering_point, flocking_steering_point, 1.0f / wsum);
        fast_vec_add(flocking_steering_point, flocking_steering_point, &moby->pos);
    }
    if (vector_distance_xy(&moby->pos, flocking_steering_point) < 1.0f) {
        qcopy(flocking_steering_point, &moby->pos);
        return fast_arc_tan(goal.pos.f[0] - moby->pos.x, goal.pos.f[1] - moby->pos.y);
    }
    return fast_arc_tan(flocking_steering_point[0] - moby->pos.x,
                        flocking_steering_point[1] - moby->pos.y);
}

typedef struct {
    u8           pad0[0x20];
    f32          health;
    s16          unk24;
    u8           pad26[2];
    u8           unk28;
    u8           pad29[5];
    u8           unk2E;
    u8           pad2F;
    f32          unk30;
    u8           pad34[0x24];
    u8           unk58;
    u8           unk59;
    u8           unk5A;
    u8           pad5B[0x75];
    u8           turret_yaw_joint[0x40];
    u8           turret_pitch_joint[0x50];
    struct Moby *pilot;
    struct Moby *shot;
    s16          fire_timer;
    s16          recoil_timer;
    f32          bob_angle;
    f32          speed;
    f32          pitch_velocity;
    f32          yaw_velocity;
    f32          facing_velocity;
    f32          turret_yaw;
    f32          turret_yaw_velocity;
    f32          turret_pitch;
    f32          turret_pitch_velocity;
    s32          wait_group;
    s32          area_path;
    f32          wake_range;
    f32          chase_range;
    f32          range;
    s32          unk1A4;
    f32          height;
    s32          entry_path;
    s32          trigger_volume;
} HelicopterVars;

typedef struct {
    Vec4         pos;
    u8           pad10[0x30];
    struct Moby *target;
    s32          status;
    u8           pad48[8];
} HelicopterGoal;

extern LevelPath *D_L15_001B0AB0[];

extern LevelSaveState D_L15_001BABD0;

float helicopter_bob_period_gp __asm__("D_L15_00161A08") = 2.0f;

float helicopter_bob_amplitude_gp __asm__("D_L15_00161A0C") = 0.17f;

extern struct Moby *create_class_55b_moby_from_source(struct Moby *) __asm__("FUN_L12_00308670");

extern void start_blarg_commander_knockback(void *, struct Moby *) __asm__("FUN_L12_00308708");

void process_helicopter_hits(struct Moby *moby) __asm__("FUN_L15_002c6900");

void process_helicopter_hits(struct Moby *m) __asm__("FUN_L15_002c6900");

extern void select_moby_target_goal(void *, float *, float) __asm__("FUN_L02_00261c80");

extern int FUN_L00_0025ff38(float, void *, void *, int, int, void *, int);

extern void FUN_L00_0025df68(void *, void *, void *, int *, float *, int, float, float, float);

extern struct Moby *spawn_homing_spark_projectile(
    void *, void *, struct Moby *, int, float) __asm__("FUN_L03_002d5208");

extern void FUN_L00_0025a120(void *);

extern void FUN_L00_001fff28(void *, int, float);

extern int point_in_path_xy(void *, void *, int) __asm__("FUN_L00_00259740");

extern void detach_manipulator(void *moby, void *manip) __asm__("FUN_0020cb88");

extern void advance_moby_animation(void *) __asm__("FUN_0020d580");

extern float scale_time(float) __asm__("FUN_001f96b0");

void helicopter_update(struct Moby *moby) __asm__("FUN_L15_002c5630");

/* Moby class 233 */
void helicopter_update(struct Moby *moby) {
    enum {
        HELICOPTER_INITIALIZE = 0,
        HELICOPTER_WAIT_FOR_ACTIVATION = 1,
        HELICOPTER_FOLLOW_ENTRY_PATH = 2,
        HELICOPTER_APPROACH_AREA_PATH = 3,
        HELICOPTER_HOLD_POSITION_WAIT_FOR_TARGET = 4,
        HELICOPTER_ENGAGE_AND_FIRE = 5,
        HELICOPTER_RECOIL = 6,
        HELICOPTER_DEATH_EFFECTS = 7,
        HELICOPTER_DEAD_WAIT_FOR_RESET = 0x40
    };
    Vec4            previous_pos;
    HelicopterGoal  goal;
    Vec4            move;
    Vec4            point;
    Vec4            aim;
    Vec4            target_pos;
    Vec4            turret_pos;
    Vec4            local_dir;
    Vec4            world_dir;
    int             segment;
    float           fraction;
    struct Moby    *target;
    float           distance;
    HelicopterVars *helicopter_instance_vars;
    float           length;
    float           angle;
    float           spin;

    previous_pos.q = *(u128 *)&moby->pos;
    helicopter_instance_vars = (HelicopterVars *)moby->pvars;
    if (helicopter_instance_vars->pilot == 0) {
        helicopter_instance_vars->pilot = create_class_55b_moby_from_source(moby);
    }
    if (moby->state != 0 && moby->state != 0x40 && helicopter_instance_vars->unk1A4 != 0 &&
        hero.vehicle_mode != 2) {
        moby->collision = 0;
        moby->flags = (moby->flags | 0x41) & 0xEFFF;
        helicopter_instance_vars->unk58 = 0;
        if (helicopter_instance_vars->pilot != 0)
            helicopter_instance_vars->pilot->flags |= 0x41;
        moby->state = 1;
        return;
    }
    process_helicopter_hits(moby);
    goal.status = 0;
    if (moby->state == 1) {
        helicopter_instance_vars->range = helicopter_instance_vars->wake_range;
        goto near;
    }
    if (helicopter_instance_vars->area_path == -1) {
        helicopter_instance_vars->range = helicopter_instance_vars->chase_range;
    near:
        select_moby_target_goal(moby, goal.pos.f, helicopter_instance_vars->range);
    } else {
        LevelPath *path;
        helicopter_instance_vars->range = helicopter_instance_vars->chase_range;
        path = D_L15_001B0AB0[helicopter_instance_vars->area_path];
        FUN_L00_0025ff38(
            helicopter_instance_vars->chase_range, moby, &goal, 0, 0, path->points, path->count);
    }
    if (helicopter_instance_vars->range < fast_vec_dist(&moby->pos, &goal))
        goal.status = 2;
    if (goal.target == 0)
        goal.target = hero.moby;
    target = goal.target;
    if (moby->state != 0 && helicopter_instance_vars->unk2E != 1) {
        if (helicopter_instance_vars->pilot != 0)
            mark_moby_for_removal(helicopter_instance_vars->pilot);
        mark_moby_for_removal(moby);
        return;
    }
    switch (moby->state) {
    case HELICOPTER_INITIALIZE:
        moby->pos.z += 7.5f;
        moby->shadow_range = 0x20;
        helicopter_instance_vars->height = moby->pos.z;
        helicopter_instance_vars->health = 3.0f;
        helicopter_instance_vars->unk24 = truncate_float_to_s32(3.0f);
        helicopter_instance_vars->unk28 = 2;
        helicopter_instance_vars->unk58 = 0x19;
        helicopter_instance_vars->unk30 = 1.0f;
        helicopter_instance_vars->unk2E = 1;
        helicopter_instance_vars->unk59 = 1;
        helicopter_instance_vars->unk5A = 6;
        moby->update_dist = 0xFF;
        moby->draw_dist = 0x80;
        if (helicopter_instance_vars->entry_path != -1) {
            LevelPath *path;
            int        i;
            path = D_L15_001B0AB0[helicopter_instance_vars->entry_path];
            for (i = 0; i < path->count - 2; i++) {
                path->points[i].f[3] = fast_vec_dist(&path->points[i], &path->points[i + 1]);
            }
            *(u128 *)&moby->pos = path->points[0].q;
        }
        moby->state = 1;
        helicopter_instance_vars->speed = 0.0f;
        helicopter_instance_vars->pitch_velocity = 0.0f;
        helicopter_instance_vars->yaw_velocity = 0.0f;
        helicopter_instance_vars->facing_velocity = 0.0f;
        helicopter_instance_vars->chase_range = 60.0f;
        attach_manipulator(moby, 0, helicopter_instance_vars->turret_yaw_joint);
        attach_manipulator(moby, 1, helicopter_instance_vars->turret_pitch_joint);
        moby->scale *= 0.5f;
        if (helicopter_instance_vars->pilot != 0) {
            helicopter_instance_vars->pilot->update_dist = 0;
            helicopter_instance_vars->pilot->flags &= 0xFFBE;
            helicopter_instance_vars->pilot->scale *= 0.5f;
        }
        moby->collision = 0;
        moby->flags = (moby->flags | 0x41) & 0xEFFF;
        helicopter_instance_vars->unk58 = 0;
        if (helicopter_instance_vars->pilot != 0)
            helicopter_instance_vars->pilot->flags |= 0x41;
        break;
    case HELICOPTER_WAIT_FOR_ACTIVATION:
        if (helicopter_instance_vars->unk1A4 != 0 && hero.vehicle_mode != 2) {
            moby->collision = 0;
            moby->flags = (moby->flags | 0x41) & 0xEFFF;
            helicopter_instance_vars->unk58 = 0;
            if (helicopter_instance_vars->pilot != 0)
                helicopter_instance_vars->pilot->flags |= 0x41;
            return;
        }
        if (helicopter_instance_vars->wait_group != -1 ||
            (is_point_inside_clip_volume(&hero.motion.pos,
                                         helicopter_instance_vars->trigger_volume) &&
             goal.status != 2)) {
            if (helicopter_instance_vars->wait_group != -1 &&
                count_active_moby_group_members(helicopter_instance_vars->wait_group, 0x40))
                break;
            moby->collision = moby->pclass->collision;
            moby->flags = (moby->flags & 0xFFBE) | 0x1000;
            helicopter_instance_vars->unk58 = 0x19;
            if (helicopter_instance_vars->entry_path != -1)
                moby->state = 2;
            else
                moby->state = 3;
        } else {
            if (helicopter_instance_vars->trigger_volume != -1)
                break;
            if (goal.status == 2)
                break;
            moby->collision = moby->pclass->collision;
            moby->flags = (moby->flags & 0xFFBE) | 0x1000;
            helicopter_instance_vars->unk58 = 0x19;
            if (helicopter_instance_vars->entry_path != -1)
                moby->state = 2;
            else
                moby->state = 3;
        }
        if (helicopter_instance_vars->pilot != 0)
            helicopter_instance_vars->pilot->flags &= 0xFFBE;
        break;
    case HELICOPTER_FOLLOW_ENTRY_PATH: {
        LevelPath *path;
        path = D_L15_001B0AB0[helicopter_instance_vars->entry_path];
        FUN_L00_0025df68(path, &moby->pos, &point, &segment, &fraction, 0, 999.0f, 5.0f, 0.0f);
        distance = convert_integer_to_float(segment) * path->points[0].f[3] + fraction + 2.0f;
        if (fast_vec_dist(&moby->pos, &path->points[path->count - 1]) <=
            helicopter_instance_vars->speed * helicopter_instance_vars->speed /
                (frame_time_sq * 10.0f + frame_time_sq * 10.0f)) {
            helicopter_instance_vars->speed -= frame_time_sq * 10.0f;
            if (helicopter_instance_vars->speed < 0.0f)
                helicopter_instance_vars->speed = 0.0f;
        } else {
            helicopter_instance_vars->speed += frame_time_sq * 10.0f;
            if (frame_time * 10.0f < helicopter_instance_vars->speed)
                helicopter_instance_vars->speed = frame_time * 10.0f;
        }
        segment = truncate_float_to_s32(distance / path->points[0].f[3]);
        fraction = (distance - convert_integer_to_float(segment) * path->points[0].f[3]) /
                   path->points[0].f[3];
        if (segment >= path->count - 1) {
            segment = path->count - 2;
            fraction = 1.0f;
        }
        fast_vec_sub(&point, &path->points[segment + 1], &path->points[segment]);
        fast_vec_scale(&point, &point, fraction);
        fast_vec_add(&point, &point, &path->points[segment]);
        if (vector_distance_xy(&path->points[path->count - 1], &moby->pos) < 1.0f ||
            point_in_path_xy(&moby->pos,
                             D_L15_001B0AB0[helicopter_instance_vars->area_path]->points,
                             D_L15_001B0AB0[helicopter_instance_vars->area_path]->count)) {
            if (goal.status != 2) {
                moby->state = 5;
                helicopter_instance_vars->fire_timer = 0;
            } else {
                moby->state = 4;
            }
        }
        if (segment + 1 < path->count - 8) {
            angle = fast_arc_tan(path->points[segment + 1].f[0] - moby->pos.x,
                                 path->points[segment + 1].f[1] - moby->pos.y);
            moby->rot.z = spring_turn(
                &helicopter_instance_vars->facing_velocity, moby->rot.z, angle, 0.01f, 0.3f, 0.1f);
        } else {
            angle = fast_arc_tan(goal.pos.f[0] - moby->pos.x, goal.pos.f[1] - moby->pos.y);
            moby->rot.z = spring_turn(
                &helicopter_instance_vars->facing_velocity, moby->rot.z, angle, 0.01f, 0.3f, 0.1f);
        }
        fast_vec_sub(&move, &point, &moby->pos);
        length = fast_vec_length(&move);
        if (helicopter_instance_vars->speed < length)
            length = helicopter_instance_vars->speed;
        fast_vec_normalize(&move, &move, length);
        fast_vec_add(&moby->pos, &moby->pos, &move);
        {
            float heading;
            float tilt_y;
            float tilt_x;
            heading = fast_subtract_rotations(fast_arc_tan(move.f[0], move.f[1]), moby->rot.z);
            tilt_y = helicopter_instance_vars->speed * 0.34906584f * fast_cos(heading) /
                     (frame_time * 10.0f);
            tilt_x = helicopter_instance_vars->speed * -0.34906584f * fast_sin(heading) /
                     (frame_time * 10.0f);
            moby->rot.y = spring_turn(&helicopter_instance_vars->yaw_velocity,
                                      moby->rot.y,
                                      tilt_y,
                                      frame_time_sq * 0.5235988f,
                                      frame_time_sq * 1.0471976f,
                                      frame_time * 0.7853982f);
            moby->rot.x = spring_turn(&helicopter_instance_vars->pitch_velocity,
                                      moby->rot.x,
                                      tilt_x,
                                      frame_time_sq * 1.0471976f,
                                      frame_time_sq * 2.0943952f,
                                      frame_time * 1.5707964f);
        }
        break;
    }
    case HELICOPTER_APPROACH_AREA_PATH: {
        LevelPath *path;
        int        i;
        float      best = 9999.999f;
        path = D_L15_001B0AB0[helicopter_instance_vars->area_path];
        move.q = *(u128 *)&target->pos;
        i = 0;
        if (i < path->count) {
            Vec4 *path_point = path->points;
            Vec4 *area_path_point_source = path_point;
            do {
                if (vector_distance_xy(path_point, &moby->pos) < best)
                    move.q = area_path_point_source->q;
                i++;
                area_path_point_source++;
                path_point++;
            } while (i < path->count);
        }
        move.f[2] = target->pos.z + 7.5f;
        point.q = *(u128 *)&target->pos;
        steer_helicopter(moby, (SteeringVec_4f88 *)&move, (SteeringVec_4f88 *)&point);
        if (vector_distance_xy(&move, &moby->pos) < 1.0f ||
            point_in_path_xy(&moby->pos, path->points, path->count)) {
            if (goal.status != 2) {
                moby->state = 5;
                helicopter_instance_vars->fire_timer = 0;
            } else {
                moby->state = 4;
            }
        }
        break;
    }
    case HELICOPTER_HOLD_POSITION_WAIT_FOR_TARGET: {
        LevelPath *path;
        path = D_L15_001B0AB0[helicopter_instance_vars->area_path];
        move.q = *(u128 *)&moby->pos;
        point.q = *(u128 *)&target->pos;
        steer_helicopter(moby, (SteeringVec_4f88 *)&move, (SteeringVec_4f88 *)&point);
        if (goal.status == 2)
            break;
        if (point_in_path_xy(&moby->pos, path->points, path->count)) {
            moby->state = 5;
            helicopter_instance_vars->fire_timer = 0;
        } else {
            moby->state = 3;
        }
        break;
    }
    case HELICOPTER_ENGAGE_AND_FIRE:
        moby_point_position(moby, 0, &point);
        if (goal.status != 2) {
            compute_helicopter_flocking_steering_point(moby, move.f, *(Goal_52b8 *)&goal);
            aim.q = *(u128 *)&target->pos;
            steer_helicopter(moby, (SteeringVec_4f88 *)&move, (SteeringVec_4f88 *)&aim);
            if (fast_dec_timer_s16(&helicopter_instance_vars->fire_timer) &&
                helicopter_instance_vars->shot == 0) {
                helicopter_instance_vars->fire_timer = scale_game_frames(0x3C);
                qcopy(&target_pos, &goal);
                fast_vec_sub(&aim, &target_pos, &point);
                set_vector_length_xy(&aim, &aim, frame_time * 6.0f);
                aim.i[2] = 0;
                helicopter_instance_vars->shot = spawn_homing_spark_projectile(
                    &aim, &point, moby, scale_game_frames(0xF0), goal.pos.f[2]);
            }
        } else {
            aim.q = *(u128 *)&moby->pos;
            target_pos.q = *(u128 *)&target->pos;
            steer_helicopter(moby, (SteeringVec_4f88 *)&aim, (SteeringVec_4f88 *)&target_pos);
            moby->state = 4;
        }
        break;
    case HELICOPTER_RECOIL:
        if (fast_dec_timer_s16(&helicopter_instance_vars->recoil_timer)) {
            moby->state = moby->command;
            break;
        }
        helicopter_instance_vars->speed -= frame_time_sq * 4.0f;
        if (helicopter_instance_vars->speed < 0.0f)
            helicopter_instance_vars->speed = 0.0f;
        move.f[0] = fast_cos(moby->rot.z) * -helicopter_instance_vars->speed;
        move.f[1] = fast_sin(moby->rot.z) * -helicopter_instance_vars->speed;
        move.f[2] = 0.0f;
        fast_vec_add(&moby->pos, &moby->pos, &move);
        {
            float heading;
            float tilt_y;
            float tilt_x;
            heading = fast_subtract_rotations(fast_arc_tan(move.f[0], move.f[1]), moby->rot.z);
            tilt_y = helicopter_instance_vars->speed * 0.34906584f * fast_cos(heading) /
                     (frame_time * 10.0f);
            tilt_x = helicopter_instance_vars->speed * -0.34906584f * fast_sin(heading) /
                     (frame_time * 10.0f);
            moby->rot.y = spring_turn(&helicopter_instance_vars->yaw_velocity,
                                      moby->rot.y,
                                      tilt_y,
                                      frame_time_sq * 0.5235988f,
                                      frame_time_sq * 1.0471976f,
                                      frame_time * 0.7853982f);
            moby->rot.x = spring_turn(&helicopter_instance_vars->pitch_velocity,
                                      moby->rot.x,
                                      tilt_x,
                                      frame_time_sq * 1.0471976f,
                                      frame_time_sq * 2.0943952f,
                                      frame_time * 1.5707964f);
        }
        break;
    case HELICOPTER_DEATH_EFFECTS: {
        int i;
        move.q = 0;
        spin = random_angle_radians();
        moby_point_position(moby, 4, &point);
        for (i = 0; i < 4; i++) {
            aim.f[0] = fast_cos(spin) * 1.5f;
            aim.f[1] = fast_sin(spin) * 1.5f;
            aim.i[2] = 0;
            spin = fast_add_rotations(spin, 1.5707964f);
            add_vectors(&target_pos, &aim, &point);
        }
        moby_point_position(moby, 5, &aim);
        if (helicopter_instance_vars->pilot != 0) {
            start_blarg_commander_knockback(moby, helicopter_instance_vars->pilot);
            helicopter_instance_vars->pilot = 0;
        }
        spawn_explosion(moby,
                        &move,
                        &moby->pos,
                        0.0f,
                        0.0f,
                        0x14,
                        3,
                        4,
                        4.0f,
                        2.0f,
                        100000.0f,
                        3.0f,
                        2,
                        15.0f,
                        1,
                        1,
                        -1,
                        0);
        FUN_L00_00257470(moby, 0x200, -1);
        detach_manipulator(moby, helicopter_instance_vars->turret_yaw_joint);
        detach_manipulator(moby, helicopter_instance_vars->turret_pitch_joint);
        moby->state = 0x40;
        return;
    }
    case HELICOPTER_DEAD_WAIT_FOR_RESET:
        moby->collision = 0;
        moby->flags = (moby->flags | 0x41) & 0xEFFF;
        helicopter_instance_vars->unk58 = 0;
        if (helicopter_instance_vars->pilot != 0)
            helicopter_instance_vars->pilot->flags |= 0x41;
        D_L15_001BABD0.cleared[(short)moby->save_id] = 0;
        l15_save_state.cleared[(short)moby->save_id] = 0;
        *(int *)(D_0014C190 + (((short)moby->save_id >> 5) * 4 + (current_level_index << 8))) &=
            ~(1 << (moby->save_id & 0x1F));
        D_L15_001BAAD0[(short)moby->save_id >> 5] &= ~(1 << (moby->save_id & 0x1F));
        if (hero.vehicle_mode != 2) {
            moby->pos.z = helicopter_instance_vars->height - 7.5f;
            moby->scale *= 2.0f;
            moby->state = 0;
        }
        return;
    }
    if (moby->visible != 0 && fast_vec_dist(&moby->pos, D_L15_001673C0) < 40.0f) {
        FUN_L00_0025a120(moby);
    }
    if (moby->state != 1) {
        float climb = absolute_float(moby->pos.z - previous_pos.f[2]);
        float limit = frame_time * 10.0f;
        if (limit < climb) {
            if (previous_pos.f[2] < moby->pos.z)
                moby->pos.z = previous_pos.f[2] + limit;
            else
                moby->pos.z = previous_pos.f[2] - limit;
        }
    }
    {
        float z = moby->pos.z -
                  helicopter_bob_amplitude_gp * fast_cos(helicopter_instance_vars->bob_angle);
        float period = helicopter_bob_period_gp * 60.0f;
        moby->pos.z = z;
        helicopter_instance_vars->bob_angle = fast_add_rotations(
            helicopter_instance_vars->bob_angle, 6.2831855f / scale_time(period));
    }
    moby->pos.z += helicopter_bob_amplitude_gp * fast_cos(helicopter_instance_vars->bob_angle);
    if (helicopter_instance_vars->pilot != 0) {
        *(u128 *)&helicopter_instance_vars->pilot->pos = *(u128 *)&moby->pos;
        *(u128 *)&helicopter_instance_vars->pilot->rot = *(u128 *)&moby->rot;
        advance_moby_animation(helicopter_instance_vars->pilot);
        update_entity(helicopter_instance_vars->pilot);
        helicopter_instance_vars->pilot->flags |= 6;
    }
    if (helicopter_instance_vars->shot == 0 || helicopter_instance_vars->shot->oclass != 0x350 ||
        helicopter_instance_vars->shot->state == 0xFE ||
        helicopter_instance_vars->shot->state == 0xFD) {
        moby_point_position(moby, 0, &turret_pos);
        fast_vec_sub(&world_dir, &goal.pos, &turret_pos);
        local_dir.q = world_dir.q;
        transpose_rotation_matrix(&move, &moby->rot_row0);
        transform_vector(&local_dir, &local_dir, &move);
        angle = fast_arc_tan(local_dir.f[0], local_dir.f[1]);
        helicopter_instance_vars->turret_yaw =
            spring_turn(&helicopter_instance_vars->turret_yaw_velocity,
                        helicopter_instance_vars->turret_yaw,
                        angle,
                        frame_time_sq * 6.2831855f,
                        frame_time_sq * 12.566371f,
                        frame_time * 6.2831855f);
        angle = fast_arc_tan(vector_length_xy(&local_dir), local_dir.f[2]);
        helicopter_instance_vars->turret_pitch =
            spring_turn(&helicopter_instance_vars->turret_pitch_velocity,
                        helicopter_instance_vars->turret_pitch,
                        -angle,
                        frame_time_sq * 3.1415927f,
                        frame_time_sq * 6.2831855f,
                        frame_time * 1.5707964f);
        FUN_L00_001fff28(helicopter_instance_vars->turret_yaw_joint + 0x10,
                         2,
                         helicopter_instance_vars->turret_yaw);
        FUN_L00_001fff28(helicopter_instance_vars->turret_pitch_joint + 0x10,
                         2,
                         helicopter_instance_vars->turret_pitch);
        helicopter_instance_vars->shot = 0;
    } else {
        moby_point_position(moby, 0, &turret_pos);
        fast_vec_sub(&world_dir, &helicopter_instance_vars->shot->pos, &turret_pos);
        local_dir.q = world_dir.q;
        transpose_rotation_matrix(&move, &moby->rot_row0);
        transform_vector(&local_dir, &local_dir, &move);
        angle = fast_arc_tan(local_dir.f[0], local_dir.f[1]);
        helicopter_instance_vars->turret_yaw =
            spring_turn(&helicopter_instance_vars->turret_yaw_velocity,
                        helicopter_instance_vars->turret_yaw,
                        angle,
                        frame_time_sq * 6.2831855f,
                        frame_time_sq * 12.566371f,
                        frame_time * 6.2831855f);
        angle = fast_arc_tan(vector_length_xy(&local_dir), local_dir.f[2]);
        helicopter_instance_vars->turret_pitch =
            spring_turn(&helicopter_instance_vars->turret_pitch_velocity,
                        helicopter_instance_vars->turret_pitch,
                        -angle,
                        frame_time_sq * 3.1415927f,
                        frame_time_sq * 6.2831855f,
                        frame_time * 1.5707964f);
        FUN_L00_001fff28(helicopter_instance_vars->turret_yaw_joint + 0x10,
                         2,
                         helicopter_instance_vars->turret_yaw);
        FUN_L00_001fff28(helicopter_instance_vars->turret_pitch_joint + 0x10,
                         2,
                         helicopter_instance_vars->turret_pitch);
    }
}

extern int
hit_reaction(void *, void *, void *, int, void *, void *, int, int) __asm__("FUN_L00_0025a478");

extern void trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");

extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");

void process_helicopter_hits(struct Moby *moby) {
    int   hit_reaction_status;
    float fv;
    int   hit_reaction_code;
    char *helicopter_vars;
    char *hit_record;
    float hit_away_heading, relative_hit_heading, recoil_rotation_y, recoil_rotation_x;
    helicopter_vars = (char *)moby->pvars;
    fv = 0.0f;
    hit_record = find_moby_hit(moby, 0x330000, 0);
    hit_reaction_code =
        hit_reaction(moby, hit_record, helicopter_vars + 0x20, 0, &hit_reaction_status, &fv, 0, 4);
    if (hit_reaction_status != 1 && moby->state != 7 && moby->state != 0x40) {
        float remaining_health = *(float *)(helicopter_vars + 0x20) - fv;
        *(float *)(helicopter_vars + 0x20) = remaining_health;
        if (hit_reaction_code == 1 || remaining_health <= 0.0f) {
            moby->state = 7;
        } else if (hit_reaction_code != 0xB) {
            *(unsigned char *)(helicopter_vars + 0x67) = 0xB4;
            trooper_anim_update(moby, (short *)(helicopter_vars + 0x60));
            if (*(float *)(hit_record + 0x1C) == 5627.925f && moby->state != 6) {
                hit_away_heading = fast_arc_tan(moby->pos.x - *(float *)(hit_record + 0),
                                                moby->pos.y - *(float *)(hit_record + 4));
                relative_hit_heading = fast_subtract_rotations(hit_away_heading, moby->rot.z);
                recoil_rotation_y = fast_cos(relative_hit_heading) * 0.08726646f;
                recoil_rotation_x = fast_sin(relative_hit_heading) * -0.34906584f;
                if (absolute_float(moby->rot.x) < absolute_float(recoil_rotation_x)) {
                    moby->rot.x = recoil_rotation_x;
                    *(float *)(helicopter_vars + 0x174) = 0.0f;
                }
                if (absolute_float(moby->rot.y) < absolute_float(recoil_rotation_y)) {
                    moby->rot.y = recoil_rotation_y;
                    *(float *)(helicopter_vars + 0x178) = 0.0f;
                }
                *(float *)(helicopter_vars + 0x170) = frame_time * 4.0f;
                *(short *)(helicopter_vars + 0x16A) = scale_game_frames(0x3C);
                moby->command = moby->state;
                moby->state = 6;
            }
        }
    }
    moby->hit_slot = 0xFF;
    trooper_anim_finish(moby, helicopter_vars + 0x60);
}

extern float spring_turn(float *vel, float cur, float target, float k, float d, float max) __asm__("FUN_L00_00258110");
