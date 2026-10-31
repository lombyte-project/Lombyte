/* Level 13 homing rocket moby class (0x52) and its spawn helper. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/overlay/collision.h"
#include "rnc/overlay/l13/mobys/projectile_moby.h"
#include "rnc/overlay/quad.h"

extern char *create_moby(int) __asm__("FUN_0020c4f8");

extern char *spawn_source_attached_fading_effect_particle(char *, int, int, int, float) __asm__("FUN_L00_0026daa0");

unsigned char *spawn_level_13_homing_rocket(char *owner_moby,
                                            char *pos,
                                            char *target,
                                            char *initial_rotation,
                                            int   arg,
                                            float scale,
                                            float max_speed) __asm__("FUN_L13_002c13b0");

unsigned char *spawn_level_13_homing_rocket(char *owner_moby,
                                            char *rocket_spawn_position,
                                            char *target,
                                            char *initial_rotation,
                                            int   initial_expiry_countdown,
                                            float scale,

                                            float max_speed) {
    unsigned char *moby = (unsigned char *)create_moby(0x52);
    if (moby != 0) {
        char *homing_rocket_vars = ((char *)MOBY(moby)->pvars);
        moby[0x30] = 0xFF;
        MOBY(moby)->draw_dist = 0xFF;
        moby[0x31] = 1;
        moby[0x20] = 0;
        *(char **)(homing_rocket_vars + 0x24) = target;
        *(char **)(homing_rocket_vars + 0x20) = owner_moby;
        qcopy(moby + 0x10, rocket_spawn_position);
        qcopy(moby + 0x40, initial_rotation);
        {
            float *sp = &scale;
            *(float *)(homing_rocket_vars + 0x2C) = *sp;
        }
        *(float *)(homing_rocket_vars + 0x38) = max_speed;
        *(int *)(homing_rocket_vars + 0x28) = initial_expiry_countdown;
        *(int *)(homing_rocket_vars + 0x30) = 0;
        *(int *)(homing_rocket_vars + 0x34) = 0;
        qcopy(homing_rocket_vars, ((char *)MOBY(owner_moby)->pvars) + 0xC0);
        if (moby[0x53] != 1) {
            blend_moby_animation(moby, 1, 0, 10);
        }
        fast_vec_add(moby + 0x10, moby + 0x10, homing_rocket_vars);
        if (*(char **)(homing_rocket_vars + 0x24) != 0) {
            qcopy(homing_rocket_vars + 0x10, *(char **)(homing_rocket_vars + 0x24) + 0x10);
        }
        MOBY(moby)->scale = *(float *)(((char *)MOBY(moby)->pclass) + 0x24);
        update_entity(moby);
        spawn_source_attached_fading_effect_particle(
            (char *)moby, 0x60808080, initial_expiry_countdown, 0, 500000.0f);
    }
    return moby;
}

typedef struct Rocket {
    ProjectileVec4  vel;
    ProjectileVec4  tpos;
    ProjectileMoby *owner;
    ProjectileMoby *target;
    int             timer;
    float           speed;
    float           yawv;
    float           pitchv;
    float           maxspeed;
} Rocket;

typedef struct Hit_2C27B0 {
    char            pad0[0x20];
    ProjectileMoby *owner;
} Hit_2C27B0;

typedef struct Veh_2C27B0 {
    char  pad0[0x64];
    float f64;
    char  pad68[0x28];
    float f90;
    float f94;
} Veh_2C27B0;

typedef struct Tex {
    char           pad0[0x5C];
    unsigned char *p5C;
} Tex;

extern ProjectileMoby *D_L13_00174158 NOT_SDA;

extern Tex D_L13_001B2380;

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");

extern char *
spawn_burst(void *pos, void *dir, int c, int d, int n, int k, float f) __asm__("FUN_L00_0026cbb0");

extern char D_0013F350[];

extern f32 random_angle_radians(void) __asm__("FUN_00213308");

extern f32 random_float_between(f32, f32) __asm__("FUN_002132a8");

extern float D_0015ED6C;

extern float
approach_angle(float *, float *, float, float, float, float) __asm__("FUN_L00_0025be00");

extern int collision_line(void *, void *, int, void *, int) __asm__("FUN_001efa68");

extern int coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");

extern unsigned char *FUN_L00_002712b8(void  *pos,
                                       float *vec,
                                       int    s,
                                       int    a,
                                       int    col,
                                       int    n,
                                       float  x,
                                       float  y,
                                       float  z,
                                       float  w,
                                       float  pw);

extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");

extern void build_look_at_matrix(void *out, void *dir, void *axis, f32 ang) __asm__("FUN_00214890");

extern void build_spherical_offset_c(void *, f32, f32, f32) __asm__("func_00214DB0");

extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");

extern void func_L00_0025F4A8_alt(void *,
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

void level_13_homing_rocket_update(ProjectileMoby *rocket_moby) __asm__("FUN_L13_002c1528");

void level_13_homing_rocket_update(ProjectileMoby *m) __asm__("FUN_L13_002c1528");

/* Moby class 82 */
void level_13_homing_rocket_update(ProjectileMoby *rocket_moby) {
    Rocket        *rocket_data = rocket_moby->data;
    ProjectileVec4 previous_rocket_position;
    ProjectileVec4 hitpos;
    ProjectileVec4 scratch_vector;
    ProjectileVec4 particle_initial_velocity;
    ProjectileVec4 particle_direction_scratch;
    Hit_2C27B0    *hit;
    float          trail_segment_start_fraction;
    float          yaw;
    float          pitch;

    qcopy(&previous_rocket_position, &rocket_moby->pos);
    rocket_moby->scale += (rocket_moby->cls->scale - rocket_moby->scale) * 0.1f;
    hit = (Hit_2C27B0 *)find_moby_hit(rocket_moby, 0xA30000, 0);
    if (hit != 0 && hit->owner != 0 && hit->owner->cls_id != 0x52) {
        func_L00_0025F4A8_alt(rocket_moby,
                              rocket_data,
                              0,
                              0.0f,
                              0.0f,
                              5,
                              2,
                              7,
                              0.0f,
                              0.0f,
                              0.0f,
                              1.0f,
                              1,
                              0.0f,
                              0,
                              1,
                              -1,
                              0);
        mark_moby_for_removal(rocket_moby);
        return;
    }
    *(OvlQuad *)&particle_direction_scratch = 0;
    particle_direction_scratch.x = random_float_between(-1.0f, 1.0f);
    particle_direction_scratch.y = random_float_between(-1.0f, 1.0f);
    particle_direction_scratch.z = random_float_between(-1.0f, 1.0f);
    *(OvlQuad *)&particle_initial_velocity = *(OvlQuad *)&particle_direction_scratch;
    fast_vec_normalize(&particle_initial_velocity,
                       &particle_initial_velocity,
                       random_float_between(0.1f, 0.2f) * D_0015ED6C);
    fast_vec_normalize(
        &scratch_vector, rocket_data, -random_float_between(D_0015ED6C * 0.1f, D_0015ED6C));
    fast_vec_add(&particle_initial_velocity, &particle_initial_velocity, &scratch_vector);
    FUN_L00_002712b8(&rocket_moby->pos,
                     (float *)&particle_initial_velocity,
                     scale_game_frames(6),
                     0x7F,
                     0xB0B0B0,
                     3,
                     80000.0f,
                     1000.0f,
                     1.0f,
                     -0.0004f,
                     0.0f);
    for (trail_segment_start_fraction = 0.0f; trail_segment_start_fraction < 1.0f;
         trail_segment_start_fraction += 0.33333334f) {
        unsigned char *particle;
        fast_vec_normalize(&scratch_vector,
                           rocket_data,
                           random_float_between(trail_segment_start_fraction,
                                                trail_segment_start_fraction + 0.33333334f) *
                               rocket_data->speed);
        fast_vec_add(&scratch_vector, &scratch_vector, &rocket_moby->pos);
        particle = FUN_L00_002712b8(&scratch_vector,
                                    (float *)&particle_initial_velocity,
                                    scale_game_frames(0x3C),
                                    0x7F,
                                    0x606060,
                                    3,
                                    80000.0f,
                                    1000.0f,
                                    1.0f,
                                    -0.0002f,
                                    0.0f);
        if (particle != 0) {
            Tex *trail_particle_header_source = &D_L13_001B2380;
            particle[2] = *trail_particle_header_source->p5C;
            particle[3] = 0x44;
        }
        *(OvlQuad *)&particle_direction_scratch = 0;
        particle_direction_scratch.z = 0.02f;
        transform_vector_by_basis(
            &particle_direction_scratch, &particle_direction_scratch, &rocket_moby->mat);
        build_look_at_matrix(&particle_direction_scratch,
                             &particle_direction_scratch,
                             &rocket_moby->mat,
                             random_angle_radians());
        spawn_burst(&scratch_vector,
                    &particle_direction_scratch,
                    0x4F007FFF,
                    0x1FFFFFFF,
                    scale_game_frames(5),
                    1,
                    40000.0f);
    }
    if (rocket_data->target != 0 && rocket_data->target->state != 0xFE &&
        rocket_data->target->state != 0xFD &&
        fast_vec_dist(&rocket_data->tpos, &rocket_data->target->pos) < 3.0f) {
        ProjectileHero *hero_state = (ProjectileHero *)(D_0013F350);
        if (hero_state->veh != 0 && hero_state->veh->cls_id == 0x45 &&
            rocket_data->owner->state != 5) {
            Veh_2C27B0 *vd = hero_state->veh->data;
            float       vehicle_lead_yaw = fast_add_rotations(hero_state->veh->rot.z, vd->f90);
            float       vehicle_lead_pitch = fast_add_rotations(hero_state->veh->rot.y, vd->f94);
            build_spherical_offset_c(&scratch_vector,
                                     fast_vec_dist(&rocket_moby->pos, &hero_state->veh->pos) /
                                         (rocket_data->speed + 24.0f) * vd->f64,
                                     vehicle_lead_yaw,
                                     -vehicle_lead_pitch);
            fast_vec_add(&scratch_vector, &scratch_vector, &hero_state->veh->pos);
            yaw = fast_arc_tan(scratch_vector.x - rocket_moby->pos.x,
                               scratch_vector.y - rocket_moby->pos.y);
            pitch = -fast_arc_tan(vector_distance_xy(&rocket_moby->pos, &scratch_vector),
                                  scratch_vector.z - rocket_moby->pos.z);
        } else {
            yaw = fast_arc_tan(rocket_data->target->pos.x - rocket_moby->pos.x,
                               rocket_data->target->pos.y - rocket_moby->pos.y);
            pitch = -fast_arc_tan(vector_distance_xy(&rocket_moby->pos, &rocket_data->target->pos),
                                  rocket_data->target->pos.z - rocket_moby->pos.z);
        }
        if (fast_difference_between_rotations(yaw, rocket_moby->rot.z) < 1.0471976f) {
            rocket_data->speed += (rocket_data->maxspeed - rocket_data->speed) * 0.05f;
        }
        approach_angle(&rocket_moby->rot.z,
                       &rocket_data->yawv,
                       yaw,
                       frame_time_sq * 12.566371f,
                       frame_time_sq * 12.566371f,
                       D_0015ED6C * 25.132742f);
        approach_angle(&rocket_moby->rot.y,
                       &rocket_data->pitchv,
                       pitch,
                       frame_time_sq * 12.566371f,
                       frame_time_sq * 12.566371f,
                       D_0015ED6C * 25.132742f);
        qcopy(&rocket_data->tpos, &rocket_data->target->pos);
    } else {
        approach_angle(&rocket_moby->rot.z,
                       &rocket_data->yawv,
                       rocket_moby->rot.z,
                       frame_time_sq * 12.566371f,
                       frame_time_sq * 12.566371f,
                       D_0015ED6C * 25.132742f);
        approach_angle(&rocket_moby->rot.y,
                       &rocket_data->pitchv,
                       rocket_moby->rot.y,
                       frame_time_sq * 12.566371f,
                       frame_time_sq * 12.566371f,
                       D_0015ED6C * 25.132742f);
        rocket_data->target = 0;
    }
    if (scale_game_frames(0x78) < rocket_data->timer) {
        build_spherical_offset_c(
            rocket_data, rocket_data->speed, rocket_moby->rot.z, -rocket_moby->rot.y);
    } else {
        float k = convert_integer_to_float(rocket_data->timer) /
                  convert_integer_to_float(scale_game_frames(0x78));
        if (1.0f < k) {
            k = 1.0f;
        } else if (k < 0.0f) {
            k = 0.0f;
        }
        k = k * k;
        build_spherical_offset_c(
            &scratch_vector, rocket_data->speed * k, rocket_moby->rot.z, -rocket_moby->rot.y);
        fast_vec_scale(rocket_data, rocket_data, 1.0f - k);
        fast_vec_add(rocket_data, rocket_data, &scratch_vector);
    }
    fast_vec_add(&rocket_moby->pos, &rocket_moby->pos, rocket_data);
    if (rocket_moby->pos.x < 2.0f || 1021.0f < rocket_moby->pos.x || rocket_moby->pos.y < 2.0f ||
        1021.0f < rocket_moby->pos.y || rocket_moby->pos.z < 2.0f || 1021.0f < rocket_moby->pos.z) {
        mark_moby_for_removal(rocket_moby);
        return;
    }
    moby_point_position(rocket_moby, 1, &hitpos);
    if (collision_line(&previous_rocket_position, &hitpos, 0, rocket_moby, 0) != 0) {
        ProjectileMoby *o = D_L13_00174158;
        if (o == 0 ||
            (o != rocket_data->owner && o->cls_id != 0x504 && o->cls_id != 0x534 &&
             o->cls_id != 0x535 && o->cls_id != 0x536 && o->cls_id != 0x537 && o->cls_id != 0x538 &&
             o->cls_id != 0x539 && o->cls_id != 0x53A && o->cls_id != 0x52 && o->cls_id != 0x53 &&
             o->cls_id != 0x191 && o->cls_id != 0x184 && o->cls_id != 0x185 && o->cls_id != 0x186 &&
             o->cls_id != 0x187 && o->cls_id != 0x188 && o->cls_id != 0x189 && o->cls_id != 0x18A &&
             o->cls_id != 0x18B && o->cls_id != 0x18C && o->cls_id != 0x18F &&
             o->cls_id != 0x190)) {
            func_L00_0025F4A8_alt(rocket_moby,
                                  rocket_data,
                                  0,
                                  5.0f,
                                  10.0f,
                                  0x14,
                                  5,
                                  0x10,
                                  7.0f,
                                  4.0f,
                                  1.0f,
                                  1.0f,
                                  0,
                                  15.0f,
                                  1,
                                  1,
                                  -1,
                                  0);
            mark_moby_for_removal(rocket_moby);
            return;
        }
    } else if (coll_sphere(1.5f, &rocket_moby->pos, 0, rocket_moby) != 0) {
        ProjectileMoby *o = D_L13_00174158;
        if (o == 0 ||
            (o != rocket_data->owner && o->cls_id != 0x504 && o->cls_id != 0x534 &&
             o->cls_id != 0x535 && o->cls_id != 0x536 && o->cls_id != 0x537 && o->cls_id != 0x538 &&
             o->cls_id != 0x539 && o->cls_id != 0x53A && o->cls_id != 0x52 && o->cls_id != 0x53 &&
             o->cls_id != 0x191 && o->cls_id != 0x184 && o->cls_id != 0x185 && o->cls_id != 0x186 &&
             o->cls_id != 0x187 && o->cls_id != 0x188 && o->cls_id != 0x189 && o->cls_id != 0x18A &&
             o->cls_id != 0x18B && o->cls_id != 0x18C && o->cls_id != 0x18F &&
             o->cls_id != 0x190)) {
            func_L00_0025F4A8_alt(rocket_moby,
                                  rocket_data,
                                  0,
                                  5.0f,
                                  10.0f,
                                  0x14,
                                  5,
                                  0x10,
                                  7.0f,
                                  4.0f,
                                  1.0f,
                                  1.0f,
                                  0,
                                  15.0f,
                                  1,
                                  1,
                                  -1,
                                  0);
            mark_moby_for_removal(rocket_moby);
            return;
        }
    }
    if (fast_dec_timer(&rocket_data->timer) != 0 ||
        (rocket_moby->b31 == 0 && rocket_data->owner != 0 && rocket_data->owner->b31 != 0)) {
        mark_moby_for_removal(rocket_moby);
    }
}

extern ProjectileMoby *D_L13_00174158;

extern float random_float_between(float, float) __asm__("FUN_002132a8");

extern float random_angle_radians(void) __asm__("FUN_00213308");
