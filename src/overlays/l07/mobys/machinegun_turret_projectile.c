/* Level 07 moby class 0x370: machinegun turret projectile, flying straight until it hits. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "qcopy.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/mobys/animation/blend_moby_animation.h"

/* spawns a moby of class 0x370 at a position moving along a direction */
/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_0030C918), where it is exact; names translated to the US level program. */
extern char *create_moby(int) __asm__("FUN_0020c4f8");
extern void  update_entity(void *) __asm__("FUN_L00_00250df8");
extern void
make_hit_descriptor(char *arg, int a, int b, void *src, float scale) __asm__("FUN_L00_00259888");
unsigned char *spawn_machinegun_turret_projectile(void  *spawn_position,
                                                  float *initial_velocity,
                                                  int    collision_ignore_moby,
                                                  int solid_collision) __asm__("FUN_L07_0030b538");
extern int     allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");
extern void    mark_moby_for_removal(void *) __asm__("FUN_0020c828");

/* Pvars of a flying bolt. */
typedef struct {
    u8    pad0[0x30];
    Vec4  vel;    /* 0x30 */
    Vec4  origin; /* 0x40 */
    void *ignore; /* 0x50: collision owner to skip */
    s32   solid;  /* 0x54 */
} BoltVars;

extern f32   random_float_between(f32, f32) __asm__("FUN_002132a8");
extern int   random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern int   collision_line(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern void  spawn_projectile_expiry_particle(void *, void *) __asm__("FUN_L07_0029a2a0");
extern void  reflect_vector(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern char *FUN_L00_0026dd20(char *, char *, int c, int d, float f);
extern int   allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");
extern Vec4  D_L07_00173EE0;
extern char  projectile_impact_reflection_normal[] __asm__("D_L07_00173F00");

/* Bolt update: flies on, sparks out at the end of its range, bursts on impact, removes it out of bounds. */
void         machinegun_turret_projectile_update(struct Moby *moby) __asm__("FUN_L07_0030b6a8");
void         mark_moby_for_removal(void *obj) __asm__("FUN_0020c828");
extern float random_float_between(float, float) __asm__("FUN_002132a8");
s32          allocate_voice_for_target_entry(s32   entry_index,
                                             s32   flags,
                                             void *target) __asm__("FUN_0022da68");

unsigned char *spawn_machinegun_turret_projectile(void  *spawn_position,
                                                  float *initial_velocity,
                                                  int    collision_ignore_moby,

                                                  int solid_collision) {
    unsigned char *spawned_moby = create_moby(0x370);
    if (spawned_moby) {
        char *moby_data;
        float V[4];
        spawned_moby[0x30] = 0xFF;
        MOBY(spawned_moby)->draw_dist = 0x7F;
        spawned_moby[0x31] = 1;
        *(int *)(spawned_moby + 0x40) = 0;
        moby_data = ((char *)MOBY(spawned_moby)->pvars);
        MOBY(spawned_moby)->rot.y =
            -fast_arc_tan(vector_length_xy(initial_velocity), initial_velocity[2]);
        MOBY(spawned_moby)->rot.z = fast_arc_tan(initial_velocity[0], initial_velocity[1]);
        qcopy(spawned_moby + 0x10, spawn_position);
        qcopy(moby_data + 0x40, spawn_position);
        qcopy(moby_data + 0x30, initial_velocity);
        qcopy(V, moby_data + 0x30);
        V[2] = 0.0f;
        set_vector_length_xy(V, V, 1.0f);
        V[2] = 1.0f;
        make_hit_descriptor(moby_data, (int)spawned_moby, 0x10001, V, 1.0f);
        moby_data[0x18] = 1;
        moby_data[0x19] = 1;
        blend_moby_animation(spawned_moby, 1, 0, scale_game_frames(5));
        MOBY(spawned_moby)->flags = 0x208;
        *(int *)(moby_data + 0x50) = collision_ignore_moby;
        *(int *)(moby_data + 0x54) = solid_collision;
        spawned_moby[0x23] = 0x10;
        update_entity(spawned_moby);
    }
    return spawned_moby;
}

/* Moby class 880 */
void machinegun_turret_projectile_update(struct Moby *moby) {
    BoltVars *bolt_vars = (BoltVars *)moby->pvars;
    Vec4      previous_projectile_position;
    Vec4      particle_motion_vector;
    Vec4      spark;
    int       pass;
    int       i;
    int       j;

    qcopy(&previous_projectile_position, &moby->pos);
    fast_vec_add(&moby->pos, &moby->pos, &bolt_vars->vel);
    pass = bolt_vars->solid == 0;
    if (vector_distance_xy(&moby->pos, &bolt_vars->origin) > 30.0f) {
        if (moby->visible != 0) {
            for (i = 0; i < 3; i++) {
                spark.q = 0;
                spark.f[0] = random_float_between(-1.0f, 1.0f);
                spark.f[1] = random_float_between(-1.0f, 1.0f);
                spark.f[2] = random_float_between(-1.0f, 1.0f);
                particle_motion_vector.q = spark.q;
                fast_vec_normalize(&particle_motion_vector,
                                   &particle_motion_vector,
                                   fast_vec_length(&spark) * 0.1f);
                fast_vec_add(&particle_motion_vector, &spark, &particle_motion_vector);
                fast_vec_normalize(
                    &particle_motion_vector,
                    &particle_motion_vector,
                    random_float_between(frame_time + frame_time, frame_time * 4.0f));
                spawn_projectile_expiry_particle(&moby->pos, &particle_motion_vector);
            }
        }
        mark_moby_for_removal(moby);
        return;
    }
    if ((moby->visible != 0 || vector_distance_xy(&moby->pos, &hero.motion.pos) < 30.0f) &&
        collision_line(
            &previous_projectile_position, &moby->pos, pass, bolt_vars->ignore, bolt_vars) != 0) {
        qcopy(&moby->pos, &D_L07_00173EE0);
        if (moby->visible != 0) {
            for (j = 0; j < 5; j++) {
                spark.q = 0;
                spark.f[0] = random_float_between(-1.0f, 1.0f);
                spark.f[1] = random_float_between(-1.0f, 1.0f);
                spark.f[2] = random_float_between(-1.0f, 1.0f);
                particle_motion_vector.q = spark.q;
                reflect_vector(&spark, &bolt_vars->vel, projectile_impact_reflection_normal);
                fast_vec_normalize(&particle_motion_vector,
                                   &particle_motion_vector,
                                   fast_vec_length(&spark) * 0.5f);
                fast_vec_add(&particle_motion_vector, &spark, &particle_motion_vector);
                fast_vec_normalize(&particle_motion_vector,
                                   &particle_motion_vector,
                                   random_float_between(frame_time * 3.0f, frame_time * 6.0f));
                FUN_L00_0026dd20((char *)&moby->pos,
                                 (char *)&particle_motion_vector,
                                 0x7F2F4F6F,
                                 random_int_between(scale_game_frames(10), scale_game_frames(15)),
                                 30000.0f);
            }
        }
        allocate_voice_for_target_entry(0, 0, moby);
        mark_moby_for_removal(moby);
        return;
    }
    if (moby->pos.x < 2.0f || moby->pos.x > 1021.0f || moby->pos.y < 2.0f ||
        moby->pos.y > 1021.0f || moby->pos.z < 2.0f || moby->pos.z > 1021.0f) {
        mark_moby_for_removal(moby);
    }
}
