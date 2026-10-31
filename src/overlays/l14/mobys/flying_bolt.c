/* Level 14 flying bolt moby class and its spawn helper. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/world/collision/collision_line.h"
#include "rnc/math/random.h"

extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");

extern char *create_moby(int);

extern char *create_moby(int) __asm__("FUN_0020c4f8");

char *spawn_flying_bolt(void  *spawn_position,
                        float *launch_vector,
                        int    collision_ignore_moby,
                        int    collision_mode) __asm__("FUN_L14_002eccf0");

char *spawn_flying_bolt(void  *spawn_position,
                        float *launch_vector,
                        int    collision_ignore_moby,

                        int collision_mode) {
    char *projectile_moby = create_moby(0x2A9);
    if (projectile_moby) {
        char *flying_bolt_vars;
        *(int *)(projectile_moby + 0x40) = 0;
        MOBY(projectile_moby)->update_dist = 0xFF;
        MOBY(projectile_moby)->draw_dist = 0x7F;
        projectile_moby[0x31] = 1;
        flying_bolt_vars = ((char *)MOBY(projectile_moby)->pvars);
        MOBY(projectile_moby)->rot.y =
            -fast_arc_tan(vector_length_xy(launch_vector), launch_vector[2]);
        MOBY(projectile_moby)->rot.z = fast_arc_tan(launch_vector[0], launch_vector[1]);
        qcopy(projectile_moby + 0x10, spawn_position);
        qcopy(flying_bolt_vars + 0x10, spawn_position);
        qcopy(flying_bolt_vars, launch_vector);
        blend_moby_animation(projectile_moby, 1, 0, scale_game_frames(5));
        MOBY(projectile_moby)->flags = 0x208;
        *(int *)(flying_bolt_vars + 0x20) = collision_ignore_moby;
        *(int *)(flying_bolt_vars + 0x24) = collision_mode;
        projectile_moby[0x23] = 0x10;
        update_entity(projectile_moby);
    }
    return projectile_moby;
}

typedef struct {
    Vec4  vel;    /* 0x00 */
    Vec4  origin; /* 0x10 */
    void *ignore; /* 0x20: collision owner to skip */
    s32   solid;  /* 0x24 */
} L14BoltVars;

typedef struct {
    u8 pad0[0x18];
    u8 flag18;
    u8 flag19;
    u8 pad1A[0x16];
} L14BoltSweep;

extern int FUN_L00_00257b90(int, int);

extern void FUN_L00_00259888(void *, struct Moby *, s32, void *, f32);

extern void spawn_projectile_expiry_particle(void *, void *) __asm__("FUN_L07_0029a2a0");

extern char *FUN_L00_0026dd20(char *, char *, int c, int d, float f);

extern struct {
    u8           pad0[0x18];
    struct Moby *moby; /* 0x18 */
    u8           pad1C[4];
    Vec4         pos; /* 0x20 */
} D_L14_00174540;

extern char bolt_impact_reflection_normal[] __asm__("D_L14_00174580");

void flying_bolt_update(struct Moby *projectile_moby) __asm__("FUN_L14_002ece00");

/* Moby class 681 */
void flying_bolt_update(struct Moby *projectile_moby) {
    L14BoltVars *bolt_vars = (L14BoltVars *)projectile_moby->pvars;
    Vec4         collision_line_start;
    Vec4         target;
    L14BoltSweep sweep;
    Vec4         aim;
    Vec4         particle_launch_vector;
    Vec4         spark;
    void        *ignore;
    struct Moby *hit;
    int          pass;
    int          tries;
    int          i;
    int          j;

    qcopy(&collision_line_start, &projectile_moby->pos);
    fast_vec_add(&projectile_moby->pos, &projectile_moby->pos, &bolt_vars->vel);
    qcopy(&target, &projectile_moby->pos);
    ignore = bolt_vars->ignore;
    qcopy(&aim, &bolt_vars->vel);
    aim.f[2] = 0.0f;
    set_vector_length_xy(&aim, &aim, 1.0f);
    tries = 0;
    aim.f[2] = 1.0f;
    FUN_L00_00259888(&sweep, projectile_moby, 0x10001, &aim, 1.0f);
    sweep.flag18 = 1;
    sweep.flag19 = 1;
    pass = bolt_vars->solid == 0;
    if (vector_distance_xy(&projectile_moby->pos, &bolt_vars->origin) > 30.0f) {
        if (projectile_moby->visible != 0) {
            for (i = 0; i < 3; i++) {
                spark.q = 0;
                spark.f[0] = random_float_between(-1.0f, 1.0f);
                spark.f[1] = random_float_between(-1.0f, 1.0f);
                spark.f[2] = random_float_between(-1.0f, 1.0f);
                particle_launch_vector.q = spark.q;
                fast_vec_normalize(&particle_launch_vector,
                                   &particle_launch_vector,
                                   fast_vec_length(&spark) * 0.1f);
                fast_vec_add(&particle_launch_vector, &spark, &particle_launch_vector);
                fast_vec_normalize(
                    &particle_launch_vector,
                    &particle_launch_vector,
                    random_float_between(frame_time + frame_time, frame_time * 4.0f));
                spawn_projectile_expiry_particle(&projectile_moby->pos, &particle_launch_vector);
            }
        }
        mark_moby_for_removal(projectile_moby);
        return;
    }
    while (tries < 3 && collision_line(&collision_line_start, &target, pass, ignore, 0) != 0) {
        hit = D_L14_00174540.moby;
        tries++;
        if (hit != 0 && hit->oclass == 0xFC) {
            qcopy(&collision_line_start, &D_L14_00174540.pos);
            ignore = hit;
            continue;
        }
        collision_line(&collision_line_start, &target, pass, ignore, &sweep);
        qcopy(&projectile_moby->pos, &D_L14_00174540.pos);
        if (projectile_moby->visible != 0) {
            for (j = 0; j < 5; j++) {
                spark.q = 0;
                spark.f[0] = random_float_between(-1.0f, 1.0f);
                spark.f[1] = random_float_between(-1.0f, 1.0f);
                spark.f[2] = random_float_between(-1.0f, 1.0f);
                particle_launch_vector.q = spark.q;
                reflect_vector(&spark, &bolt_vars->vel, bolt_impact_reflection_normal);
                fast_vec_normalize(&particle_launch_vector,
                                   &particle_launch_vector,
                                   fast_vec_length(&spark) * 0.5f);
                fast_vec_add(&particle_launch_vector, &spark, &particle_launch_vector);
                fast_vec_normalize(&particle_launch_vector,
                                   &particle_launch_vector,
                                   random_float_between(frame_time * 3.0f, frame_time * 6.0f));
                FUN_L00_0026dd20((char *)&projectile_moby->pos,
                                 (char *)&particle_launch_vector,
                                 0x7F2F4F6F,
                                 FUN_L00_00257b90(scale_game_frames(10), scale_game_frames(15)),
                                 30000.0f);
            }
        }
        allocate_voice_for_target_entry(0, 0, projectile_moby);
        mark_moby_for_removal(projectile_moby);
        return;
    }
    if (projectile_moby->pos.x < 2.0f || projectile_moby->pos.x > 1021.0f ||
        projectile_moby->pos.y < 2.0f || projectile_moby->pos.y > 1021.0f ||
        projectile_moby->pos.z < 2.0f || projectile_moby->pos.z > 1021.0f) {
        mark_moby_for_removal(projectile_moby);
    }
}

extern float vector_length_xy(void *);
