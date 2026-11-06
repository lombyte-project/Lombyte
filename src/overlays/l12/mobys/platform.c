/* Level 12 moby class platform: the Hoven flying platform, its exhaust, steering and tilt. */
#include "types.h"
#include "rnc/overlay/shared/camera/level_camera.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/world/level_path.h"

#include "qcopy.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/mobys/count_active_moby_group_members.h"
#include "rnc/overlay/shared/rendering/effects/update_three_phase_effect_particle.h"

extern int          scale_game_frames(int);
typedef struct Moby Moby;
extern float        D_0015ED6C;
extern float        D_0015ED70;

#include "rnc/overlay/shared/world/level_globals.h"

/* Hoven engine exhaust: emits a flame and a smoke puff from each of the moby's two nozzles (the second one
 * mirrored across its local Y axis), with random jitter. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002E4C58), where it is exact; names translated to the US level program. */

extern void *func_L00_0026DEA0_x(void *, float, float, float, int, void *, float, int) __asm__("FUN_L00_0026d000");

float D_L12_00161920[4] __attribute__((section(".sdata"))) = {-1.5f, 0.8f, -0.5f, 0.0f};

void emit_platform_exhaust_effects(struct Moby *moby) __asm__("FUN_L12_002e3948");

void emit_platform_exhaust_effects(struct Moby *moby) {
    float emission_direction[4];
    float secondary_emission_direction[4];
    float emission_position[4];
    float mirrored_emission_position[4];
    float z[4];
    if (moby->visible == 0)
        return;
    clear_u64_value(z);
    emission_direction[0] = -random_float_between(0.0f, frame_time * 5.0f);
    emission_direction[1] = random_float_between(-(frame_time * 2.0f), frame_time * 2.0f);
    emission_direction[2] = random_float_between(-(frame_time * 2.0f), frame_time * 2.0f);
    transform_vector_by_basis(emission_direction, emission_direction, &moby->rot_row0);
    transform_vector_by_basis(emission_position, &D_L12_00161920, &moby->rot_row0);
    fast_vec_add(emission_position, emission_position, &moby->pos);
    secondary_emission_direction[0] = -random_float_between(0.0f, frame_time);
    secondary_emission_direction[1] = random_float_between(-frame_time, frame_time);
    secondary_emission_direction[2] = random_float_between(-frame_time, frame_time);
    transform_vector_by_basis(
        secondary_emission_direction, secondary_emission_direction, &moby->rot_row0);
    qcopy(mirrored_emission_position, &D_L12_00161920);
    mirrored_emission_position[1] = -mirrored_emission_position[1];
    transform_vector_by_basis(
        mirrored_emission_position, mirrored_emission_position, &moby->rot_row0);
    fast_vec_add(mirrored_emission_position, mirrored_emission_position, &moby->pos);
    FUN_L00_0026cbb0(emission_position,
                     emission_direction,
                     0x4F007FFF,
                     0x1FFFFFFF,
                     random_int_between(scale_game_frames(8), scale_game_frames(0x11)),
                     1,
                     10000.0f);
    func_L00_0026DEA0_x(emission_position,
                        0.05f,
                        1.01f,
                        1.03f,
                        6,
                        secondary_emission_direction,
                        30000.0f,
                        0x404040);
    FUN_L00_0026cbb0(mirrored_emission_position,
                     emission_direction,
                     0x4F007FFF,
                     0x1FFFFFFF,
                     random_int_between(scale_game_frames(8), scale_game_frames(0x11)),
                     1,
                     10000.0f);
    func_L00_0026DEA0_x(mirrored_emission_position,
                        0.05f,
                        1.01f,
                        1.03f,
                        6,
                        secondary_emission_direction,
                        30000.0f,
                        0x404040);
}

/* Steers the moby toward a target point, speeding up or braking, and turns it toward the given angles. */
extern float D_0015ED6C_k __asm__("D_0015ED6C");
extern float D_0015ED70_k __asm__("D_0015ED70");
extern float D_0015ED64_k __asm__("D_0015ED64");
extern void  appr_k(void *, float, float) __asm__("FUN_00213ed8");
extern float
spring_angle(float *, float, float *, int, float, float, float) __asm__("FUN_L00_0025bc98");

#include "rnc/overlay/l12/mobys/platform_globals.h"

float platform_tilt_approach_divisor_gp __asm__("D_L12_00161918") = 15.0f;
void  steer_platform_toward_target(char  *moby,
                                   float *target_angles,
                                   void  *target,
                                   int    unused,
                                   int    brake) __asm__("FUN_L12_002e3c08");

typedef struct PlatformVars {
    u8  pad_0[0x40];
    f32 velocity[4];
    u8  pad_50[0x80];
    f32 max_speed;
    f32 acceleration;
    u8  pad_D8[0x20];
    f32 yaw_velocity;
} PlatformVars;

void steer_platform_toward_target(

    char *moby, float *target_angles, void *target, int unused, int brake) {
    PlatformVars *moby_vars = (PlatformVars *)MOBY(moby)->pvars;
    float         target_offset[4];
    float         target_distance;
    float         speed;
    float         acc;
    float         yaw_angular_velocity_limit = 1.9198622f;
    fast_vec_sub(target_offset, target, moby + 0x10);
    target_distance = fast_vec_length(target_offset);
    speed = fast_vec_length(moby_vars->velocity);
    if (brake) {
        if (speed == 0.0f)
            speed = D_0015ED6C_k * 0.01f;
        acc = moby_vars->acceleration * D_0015ED70_k;
        if (acc < speed * speed / (target_distance + target_distance))
            speed -= acc;
        else if (speed < moby_vars->max_speed * D_0015ED6C_k)
            speed += acc;
    } else {
        if (speed < moby_vars->max_speed * D_0015ED6C_k)
            speed += moby_vars->acceleration * D_0015ED70_k;
        else if (moby_vars->max_speed * D_0015ED6C_k < speed)
            speed -= moby_vars->acceleration * D_0015ED70_k;
    }
    fast_vec_normalize(moby_vars->velocity, target_offset, speed);
    appr_k(moby + 0x40,
           target_angles[0],
           fast_difference_between_rotations(target_angles[0], MOBY(moby)->rot.x) /
               platform_tilt_approach_divisor_gp);
    appr_k(moby + 0x44,
           target_angles[1],
           fast_difference_between_rotations(target_angles[1], MOBY(moby)->rot.y) /
               platform_tilt_approach_divisor_gp);
    spring_angle((float *)&MOBY(moby)->rot.z,
                 target_angles[2],
                 &moby_vars->yaw_velocity,
                 0,
                 D_0015ED64_k * 0.005f,
                 D_0015ED64_k * 0.3f,
                 D_0015ED6C_k * yaw_angular_velocity_limit);
    fast_vec_add(moby + 0x10, moby + 0x10, moby_vars->velocity);
}
/* Projects a moby's offset from a reference point into a 2D pair, or zeroes it. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002E5138), where it is exact; names translated to the US level program. */

extern char D_0013E533[];

float platform_tilt_offset_divisor_gp __asm__("D_L12_00161914") = 8.0f;
void  compute_platform_target_tilt(struct Moby *moby,
                                   float       *out,
                                   int          on) __asm__("FUN_L12_002e3e28");

void compute_platform_target_tilt(struct Moby *moby, float *target_tilt_angles, int on) {
    float inverse_yaw_angles[4];
    float reference_offset[4];
    float inverse_yaw_matrix[4];
    float pad[12];
    float offset_scale_divisor;
    if (on) {
        fast_vec_sub(reference_offset, D_0013E533 + 0xE9D, &moby->pos);
        clear_u64_value(inverse_yaw_angles);
        inverse_yaw_angles[2] = -moby->rot.z;
        euler_rotation_matrix(inverse_yaw_matrix, inverse_yaw_angles);
        transform_vector(reference_offset, reference_offset, inverse_yaw_matrix);
        offset_scale_divisor = platform_tilt_offset_divisor_gp;
        target_tilt_angles[1] = reference_offset[0] / offset_scale_divisor;
        target_tilt_angles[0] = -reference_offset[1] / offset_scale_divisor;
    } else {
        target_tilt_angles[0] = 0;
        target_tilt_angles[1] = 0;
    }
}
/* Update of the Hoven flying platform (class 240): follows or eases along its path, bobs, falls, and reports its motion. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002E51E8), where it is exact; names translated to the US level program. */

typedef struct {
    float x, y, z, w;
} Node;

typedef struct {
    int  n;
    int  pad4[3];
    Node nodes[1];
} Path;

typedef struct {
    char pad0[0x10];
    int  f10;
} Class_2E51E8;

typedef struct {
    int         idx;
    signed char dir;
    char        pad5[0xB];
    Path       *path;
    int         pathIdx;
} Walk;

typedef struct {
    char           pad0[0x3E];
    unsigned short f3E;
    float          vel[4];
    char           pad50[0x10];
    char           f60[0x40];
    Walk           walk;
    char           padB8[0x18];
    float          target_speed;
    float          acceleration;
    float          path_oscillation_phase;
    int            movement_blocking_group_id;
    int            timer;
    int            kind;
    float          tumble_rotation_x_step;
    float          tumble_rotation_y_step;
    int            height_reference_moby_index;
    char           padF4[8];
    int            movement_blocking_moby_index;
} Data_2E51E8;

extern int  is_hero_on_moby(void *) __asm__("FUN_L01_00277fb8");
extern int  FUN_L08_002025d0(void *, int, int, void *, int);
extern void interpolate_vector(void *, void *, void *, float) __asm__("FUN_L00_0025b6b8");
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
                            int,
                            float,
                            float,
                            int,
                            int,
                            int,
                            int) __asm__("FUN_L00_0025e450");
extern void update_entity_matrix(char *, void *, void *, void *) __asm__("FUN_L00_00260738");
extern void func_L12_002E4F18_2E51E8(void *, void *, void *, int, int) __asm__("FUN_L12_002e3c08");

void platform_update(Moby *moby) __asm__("FUN_L12_002e3ed8");

/* Moby classes 240, 664 */
void platform_update(Moby *moby) {
    enum {
        PLATFORM_INITIALIZE = 0,
        PLATFORM_OSCILLATE_BETWEEN_PATH_ENDPOINTS = 2,
        PLATFORM_WAIT_TO_RESUME_OSCILLATION = 3,
        PLATFORM_OSCILLATE_BETWEEN_PATH_ENDPOINTS_FIXED_PITCH = 4,
        PLATFORM_WAIT_TO_RESUME_FIXED_PITCH_OSCILLATION = 5,
        PLATFORM_ACCELERATE_UPWARD_UNTIL_NOT_VISIBLE = 6,
        PLATFORM_FOLLOW_WAYPOINTS = 7,
        PLATFORM_FALL_AND_TUMBLE = 8,
        PLATFORM_TRACK_REFERENCED_MOBY_HEIGHT = 9
    };
    enum {
        PLATFORM_KIND_FOLLOW_WAYPOINTS = 0,
        PLATFORM_KIND_OSCILLATE_BETWEEN_PATH_ENDPOINTS = 1,
        PLATFORM_KIND_OSCILLATE_BETWEEN_PATH_ENDPOINTS_FIXED_PITCH = 2,
        PLATFORM_KIND_DISABLE_DRAWING_AND_COLLISION = 3,
        PLATFORM_KIND_TRACK_REFERENCED_MOBY_HEIGHT = 4
    };
    enum {
        PLATFORM_DRAWING_AND_COLLISION_DISABLED = 1
    };
    float        oldPos[4];
    float        oldRot[4];
    float        delta[4];
    float        angles[4];
    float        goal[4];
    float        to_goal[4];
    Data_2E51E8 *platform_vars;

    platform_vars = (*(Data_2E51E8 **)&moby->pvars);
    qcopy(oldPos, (*(float(*)[4]) & moby->pos));
    qcopy(oldRot, (*(float(*)[4]) & moby->rot));
    if (moby->state != PLATFORM_INITIALIZE && platform_vars->movement_blocking_moby_index != -1 &&
        (signed char)(*(Moby **)&D_L12_0015FFD8_p)[platform_vars->movement_blocking_moby_index]
                .state >= 0) {
        emit_platform_exhaust_effects((char *)moby);
        return;
    }
    if (moby->state != PLATFORM_INITIALIZE && platform_vars->movement_blocking_group_id != -1 &&
        count_active_moby_group_members(platform_vars->movement_blocking_group_id, -1) != 0) {
        emit_platform_exhaust_effects((char *)moby);
        return;
    }
    switch (moby->state) {
    case PLATFORM_INITIALIZE:
        switch (platform_vars->kind) {
        case PLATFORM_KIND_FOLLOW_WAYPOINTS:
            (*(int *)&moby->collision) = (*(Class_2E51E8 **)&moby->pclass)->f10;
            platform_vars->walk.path = ((Path *)l12_paths[platform_vars->walk.pathIdx]);
            platform_vars->walk.idx = 0;
            platform_vars->walk.dir = 1;
            if (platform_vars->target_speed == 0.0f)
                platform_vars->target_speed = 6.0f;
            if (platform_vars->acceleration == 0.0f)
                platform_vars->acceleration = 6.0f;
            platform_vars->f3E |= 9;
            clear_u64_value(platform_vars->vel);
            moby->state = PLATFORM_FOLLOW_WAYPOINTS;
            break;
        case PLATFORM_KIND_OSCILLATE_BETWEEN_PATH_ENDPOINTS:
            (*(int *)&moby->collision) = (*(Class_2E51E8 **)&moby->pclass)->f10;
            platform_vars->walk.path = ((Path *)l12_paths[platform_vars->walk.pathIdx]);
            platform_vars->walk.idx = 0;
            platform_vars->walk.dir = 1;
            if (platform_vars->acceleration == 0.0f)
                platform_vars->acceleration = 2.0f;
            if (platform_vars->target_speed == 0.0f)
                platform_vars->target_speed = 4.0f;
            platform_vars->path_oscillation_phase = random_angle_radians();
            platform_vars->f3E |= 9;
            clear_u64_value(platform_vars->vel);
            moby->state = PLATFORM_OSCILLATE_BETWEEN_PATH_ENDPOINTS;
            break;
        case PLATFORM_KIND_OSCILLATE_BETWEEN_PATH_ENDPOINTS_FIXED_PITCH:
            (*(int *)&moby->collision) = (*(Class_2E51E8 **)&moby->pclass)->f10;
            (*(float(*)[4]) & moby->rot)[1] = -1.5707964f;
            (*(float(*)[4]) & moby->rot)[2] = (*(float(*)[4]) & moby->rot)[2] + 3.1415927f;
            platform_vars->walk.path = ((Path *)l12_paths[platform_vars->walk.pathIdx]);
            platform_vars->walk.idx = 0;
            platform_vars->walk.dir = 1;
            if (platform_vars->acceleration == 0.0f)
                platform_vars->acceleration = 8.0f;
            if (platform_vars->target_speed == 0.0f)
                platform_vars->target_speed = 8.0f;
            clear_u64_value(platform_vars->vel);
            moby->state = PLATFORM_OSCILLATE_BETWEEN_PATH_ENDPOINTS_FIXED_PITCH;
            break;
        case PLATFORM_KIND_DISABLE_DRAWING_AND_COLLISION:
            moby->draw_dist = 0;
            moby->state = PLATFORM_DRAWING_AND_COLLISION_DISABLED;
            (*(int *)&moby->collision) = 0;
            break;
        case PLATFORM_KIND_TRACK_REFERENCED_MOBY_HEIGHT:
            moby->state = PLATFORM_TRACK_REFERENCED_MOBY_HEIGHT;
            platform_vars->acceleration = 4.0f;
            platform_vars->target_speed = 4.0f;
            platform_vars->f3E &= 0xFFFE;
            break;
        }
        break;
    case PLATFORM_FOLLOW_WAYPOINTS: {
        Walk *w = &platform_vars->walk;
        Path *path;
        int   next_waypoint_index;
        emit_platform_exhaust_effects((char *)moby);
        path = w->path;
        next_waypoint_index = (w->idx + path->n + w->dir) % path->n;
        qcopy(goal, &path->nodes[next_waypoint_index]);
        if (fast_vec_dist((*(float(*)[4]) & moby->pos), &path->nodes[next_waypoint_index]) < 0.5f) {
            platform_vars->walk.idx = next_waypoint_index;
            if (platform_vars->walk.path->nodes[next_waypoint_index].w != -1.0f) {
                platform_vars->target_speed =
                    platform_vars->walk.path->nodes[next_waypoint_index].w;
            }
        }
        fast_vec_sub(to_goal, goal, (*(float(*)[4]) & moby->pos));
        angles[2] = fast_arc_tan(to_goal[0], to_goal[1]);
        if (is_hero_on_moby(moby)) {
            compute_platform_target_tilt((char *)moby, angles, 1);
            func_L12_002E4F18_2E51E8(moby, angles, goal, 1, 0);
        } else {
            compute_platform_target_tilt((char *)moby, angles, 0);
            func_L12_002E4F18_2E51E8(moby, angles, goal, 0, 0);
        }
        break;
    }
    case PLATFORM_OSCILLATE_BETWEEN_PATH_ENDPOINTS: {
        Path *path = platform_vars->walk.path;
        float swing;
        platform_vars->acceleration = 0.0f;
        platform_vars->path_oscillation_phase =
            fast_add_rotations(platform_vars->path_oscillation_phase, D_0015ED6C * 1.2217305f);
        swing = fast_sin(platform_vars->path_oscillation_phase);
        if (platform_vars->path_oscillation_phase > 1.5707964f &&
            platform_vars->path_oscillation_phase < 3.1415927f)
            swing = 2.0f - swing;
        if (platform_vars->path_oscillation_phase > -3.1415927f &&
            platform_vars->path_oscillation_phase < -1.5707964f)
            swing = 2.0f - swing;
        swing = swing * 0.5f;
        swing = swing + 0.5f;
        if (swing > 1.0f)
            swing = swing - 2.0f;
        swing = absolute_float(swing);
        *(u128 *)angles = *(u128 *)&path->nodes[0];
        *(u128 *)goal = *(u128 *)&path->nodes[1];
        interpolate_vector((*(float(*)[4]) & moby->pos), angles, goal, swing);
        angles[2] = (*(float(*)[4]) & moby->rot)[2];
        if (is_hero_on_moby(moby)) {
            compute_platform_target_tilt((char *)moby, angles, 1);
            func_L12_002E4F18_2E51E8(moby, angles, (*(float(*)[4]) & moby->pos), 1, 1);
        } else {
            compute_platform_target_tilt((char *)moby, angles, 0);
            func_L12_002E4F18_2E51E8(moby, angles, (*(float(*)[4]) & moby->pos), 0, 1);
        }
        break;
    }
    case PLATFORM_OSCILLATE_BETWEEN_PATH_ENDPOINTS_FIXED_PITCH: {
        Path *path = platform_vars->walk.path;
        float swing;
        platform_vars->path_oscillation_phase =
            fast_add_rotations(platform_vars->path_oscillation_phase, D_0015ED6C * 1.2217305f);
        swing = fast_sin(platform_vars->path_oscillation_phase);
        if (platform_vars->path_oscillation_phase > 1.5707964f &&
            platform_vars->path_oscillation_phase < 3.1415927f)
            swing = 2.0f - swing;
        if (platform_vars->path_oscillation_phase > -3.1415927f &&
            platform_vars->path_oscillation_phase < -1.5707964f)
            swing = 2.0f - swing;
        swing = swing * 0.5f;
        swing = swing + 0.5f;
        if (swing > 1.0f)
            swing = swing - 2.0f;
        swing = absolute_float(swing);
        *(u128 *)angles = *(u128 *)&path->nodes[0];
        *(u128 *)goal = *(u128 *)&path->nodes[1];
        interpolate_vector((*(float(*)[4]) & moby->pos), angles, goal, swing);
        (*(float(*)[4]) & moby->rot)[1] = -1.5707964f;
        break;
    }
    case PLATFORM_WAIT_TO_RESUME_OSCILLATION:
        angles[2] = (*(float(*)[4]) & moby->rot)[2];
        if (fast_dec_timer(&platform_vars->timer)) {
            if (platform_vars->walk.dir == 1) {
                platform_vars->walk.dir = 2;
            } else {
                platform_vars->walk.dir = 1;
            }
            moby->state = PLATFORM_OSCILLATE_BETWEEN_PATH_ENDPOINTS;
        }
        if (is_hero_on_moby(moby)) {
            compute_platform_target_tilt((char *)moby, angles, 1);
            func_L12_002E4F18_2E51E8(moby, angles, (*(float(*)[4]) & moby->pos), 1, 1);
        } else {
            compute_platform_target_tilt((char *)moby, angles, 0);
            func_L12_002E4F18_2E51E8(moby, angles, (*(float(*)[4]) & moby->pos), 0, 1);
        }
        break;
    case PLATFORM_WAIT_TO_RESUME_FIXED_PITCH_OSCILLATION:
        if (fast_dec_timer(&platform_vars->timer)) {
            if (platform_vars->walk.dir == 1) {
                platform_vars->walk.dir = 2;
            } else {
                platform_vars->walk.dir = 1;
            }
            moby->state = PLATFORM_OSCILLATE_BETWEEN_PATH_ENDPOINTS_FIXED_PITCH;
        }
        if (platform_vars->walk.dir == 2) {
            emit_platform_exhaust_effects((char *)moby);
        }
        qcopy(angles, (*(float(*)[4]) & moby->rot));
        func_L12_002E4F18_2E51E8(moby, angles, (*(float(*)[4]) & moby->pos), 0, 1);
        break;
    case PLATFORM_ACCELERATE_UPWARD_UNTIL_NOT_VISIBLE:
        if (fast_vec_length(platform_vars->vel) < D_0015ED6C * 6.0f) {
            platform_vars->vel[2] = platform_vars->vel[2] + D_0015ED70 * 6.0f;
        }
        fast_vec_add(
            (*(float(*)[4]) & moby->pos), (*(float(*)[4]) & moby->pos), platform_vars->vel);
        if (moby->visible == 0) {
            mark_moby_for_removal(moby);
        }
        break;
    case PLATFORM_FALL_AND_TUMBLE:
        fast_vec_add(
            (*(float(*)[4]) & moby->pos), (*(float(*)[4]) & moby->pos), platform_vars->vel);
        if (FUN_L08_002025d0((*(float(*)[4]) & moby->pos), 0x600, 0, moby, 0) &&
            platform_vars->walk.path == 0) {
            spawn_explosion(moby,
                            platform_vars->vel,
                            0,
                            2.0f,
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
            return;
        }
        platform_vars->vel[2] = platform_vars->vel[2] - D_0015ED70 * 7.0f;
        (*(float(*)[4]) & moby->rot)[0] = fast_add_rotations(platform_vars->tumble_rotation_x_step,
                                                             (*(float(*)[4]) & moby->rot)[0]);
        (*(float(*)[4]) & moby->rot)[1] = fast_add_rotations(platform_vars->tumble_rotation_y_step,
                                                             (*(float(*)[4]) & moby->rot)[1]);
        if (platform_vars->walk.path != 0 &&
            vector_distance_xy((*(float(*)[4]) & moby->pos), &platform_vars->walk.path->nodes[0]) <
                0.5f) {
            moby->state = PLATFORM_INITIALIZE;
        }
        if ((*(float(*)[4]) & moby->pos)[2] < 0.0f) {
            mark_moby_for_removal(moby);
        }
        break;
    case PLATFORM_TRACK_REFERENCED_MOBY_HEIGHT: {
        Moby *height_reference_moby = (Moby *)((char *)(*(Moby **)&D_L12_0015FFD8_p) +
                                               (platform_vars->height_reference_moby_index << 8));
        qcopy(angles, (*(float(*)[4]) & moby->pos));
        angles[2] = (*(float(*)[4]) & height_reference_moby->pos)[2];
        clear_u64_value(goal);
        goal[2] = (*(float(*)[4]) & moby->rot)[2];
        if (is_hero_on_moby(moby)) {
            angles[2] = angles[2] + 0.2f;
            compute_platform_target_tilt((char *)moby, goal, 1);
            func_L12_002E4F18_2E51E8(moby, goal, angles, 1, 1);
        } else {
            angles[2] = angles[2] + 0.4f;
            compute_platform_target_tilt((char *)moby, goal, 0);
            func_L12_002E4F18_2E51E8(moby, goal, angles, 0, 1);
        }
        break;
    }
    }
    fast_vec_sub(delta, (*(float(*)[4]) & moby->pos), oldPos);
    update_entity_matrix(platform_vars->f60, delta, oldRot, (*(float(*)[4]) & moby->rot));
}
