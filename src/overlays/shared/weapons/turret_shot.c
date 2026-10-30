/* Turret shot (class 0x4EA): the shot the turret and gun mobys fire, and the function that spawns it. */

#include "types.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/overlay/quad.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/world/collision/collision_line.h"

/* pvars of the projectile moby FUN_L09_00307ba8 spawns */
typedef struct {
    OvlVec4 vel;      /* 0x00 */
    f32     spin;     /* 0x10 */
    s32     unk14;    /* 0x14 */
    void   *owner;    /* 0x18 */
    s16     life;     /* 0x1C */
    s16     life_max; /* 0x1E */
    f32     dist;     /* 0x20: distance from the hero at spawn */
    s32     unk24;    /* 0x24 */
    s32     unk28;    /* 0x28 */
} ShotVars;

/* Optional query block for collision_line: a swept probe along dir. */
typedef struct {
    OvlVec4      dir;  /* 0x00: unit direction; z/w overwritten with 1 and a range */
    struct Moby *self; /* 0x10: moby to ignore */
    s32          mask; /* 0x14: collision classes */
    u8           unk18;
    u8           unk19;
    s16          oclass; /* 0x1A */
    f32          radius; /* 0x1C */
    s32          unk20;
} CollisionProbe;

extern u8 *spawn_source_attached_fading_effect_particle(f32, struct Moby *, u32, s32, s32) __asm__("FUN_L00_0026daa0");
extern void FUN_L00_0026dd20_307d68(void *, void *, u32, s32, f32) __asm__("FUN_L00_0026dd20");
extern void spawn_explosion(void *,
                            void *,
                            void *,
                            f32,
                            f32,
                            s32,
                            s32,
                            s32,
                            f32,
                            f32,
                            f32,
                            s32,
                            f32,
                            f32,
                            s32,
                            s32,
                            s32,
                            s32) __asm__("FUN_L00_0025e450");
extern s32  collect_mobies_in_sphere(f32, void *, s32, void *, void *) __asm__("FUN_L00_001f2868");

extern OvlVec4 D_0013F3D0_307ba8 __asm__("D_0013F3D0");
extern OvlVec4 D_0013F3D0_307d68 __asm__("D_0013F3D0");
extern OvlVec4 D_L09_00166F40_307d68 __asm__("D_L09_00166F40");
extern char    D_L09_00174000[] __asm__("D_L09_00174000");
extern OvlVec4 l09_projectile_impact_position __asm__("D_L09_00173FE0");

struct Moby *
spawn_turret_shot(void *owner, OvlVec4 *pos, OvlVec4 *vel, s32 life) __asm__("FUN_L09_00307ba8");

struct Moby *spawn_turret_shot(void *owner, OvlVec4 *pos, OvlVec4 *vel, s32 life) {
    OvlVec4      position_copy;
    OvlVec4      velocity_copy;
    OvlVec4     *pp = &position_copy;
    OvlVec4     *vp = &velocity_copy;
    struct Moby *spawned_moby;
    ShotVars    *projectile_vars;
    f32          spin_limit;
    s32          trail;

    position_copy.q = pos->q;
    velocity_copy.q = vel->q;
    spawned_moby = create_moby(0x4EA);
    if (spawned_moby != 0) {
        projectile_vars = (ShotVars *)spawned_moby->pvars;
        projectile_vars->owner = owner;
        spawned_moby->rot.x = 0.0f;
        spawned_moby->scale *= 4.0f;
        spawned_moby->update_dist = 0xFF;
        spawned_moby->draw_dist = 0xFF;
        spawned_moby->visible = 1;
        spawned_moby->rot.y = -fast_arc_tan(vector_length_xy(vp), vp->f[2]);
        spawned_moby->rot.z = fast_arc_tan(velocity_copy.f[0], vp->f[1]);
        qcopy(&spawned_moby->pos, pp);
        qcopy(&projectile_vars->vel, vp);
        spin_limit = frame_time * 1.5707964f;
        projectile_vars->spin = random_float_between(-spin_limit, spin_limit);
        projectile_vars->unk14 = 0;
        projectile_vars->life = life;
        projectile_vars->life_max = life;
        projectile_vars->dist = fast_vec_dist((OvlVec4 *)&spawned_moby->pos, &D_0013F3D0_307ba8);
        projectile_vars->unk24 = 0;
        projectile_vars->unk28 = 0;
        trail = life * 5 / 4;
        spawned_moby->flags |= 0x200;
        spawned_moby->alpha = 0x40;
        spawn_source_attached_fading_effect_particle(
            400000.0f, spawned_moby, 0x2F7F4F4F, trail, -1);
        spawn_source_attached_fading_effect_particle(
            160000.0f, spawned_moby, 0x4F7F7F7F, trail, -1);
        update_entity(spawned_moby);
    }
    return spawned_moby;
}

/*
 * Update of the shot FUN_L09_00307ba8 spawns (cf. FUN_L04_002af058): moves
 * along vel, plays its sound once it starts moving away from the hero within
 * 3 units, dies outside the level box, and on a hit (probing along the line
 * from the hero until the timer runs out) bursts into five sparks and a
 * flash, damages around the hit and is removed.
 */
void update_turret_shot(struct Moby *projectile_moby) __asm__("FUN_L09_00307d68");

void update_turret_shot(struct Moby *projectile_moby) {
    OvlVec4         old;
    CollisionProbe  pr;
    OvlVec4         push;
    OvlVec4         spark_scatter_vector;
    ShotVars       *projectile_vars = (ShotVars *)projectile_moby->pvars;
    CollisionProbe *probe;
    f32             distance_to_hero;
    s32             i;

    distance_to_hero = fast_vec_dist(&projectile_moby->pos, &D_0013F3D0_307d68);
    qcopy(&old, &projectile_moby->pos);
    fast_vec_add(&projectile_moby->pos, &projectile_moby->pos, &projectile_vars->vel);
    if (projectile_vars->unk28 == 0 && projectile_vars->unk24 != 0 &&
        projectile_vars->dist < distance_to_hero && distance_to_hero < 3.0f) {
        allocate_voice_for_target_entry(0, 0, projectile_moby);
        projectile_vars->unk28 = 1;
    }
    if (distance_to_hero < projectile_vars->dist)
        projectile_vars->unk24 = 1;
    projectile_vars->dist = distance_to_hero;
    if (projectile_moby->pos.x < 2.0f || 1021.0f < projectile_moby->pos.x ||
        projectile_moby->pos.y < 2.0f || 1021.0f < projectile_moby->pos.y ||
        projectile_moby->pos.z < 2.0f || 1021.0f < projectile_moby->pos.z ||
        255.0f < vector_distance_xy(&projectile_moby->pos, &D_L09_00166F40_307d68)) {
        mark_moby_for_removal(projectile_moby);
        return;
    }

    probe = 0;
    if (projectile_moby->command == 0) {
        pr.mask = 0x70001;
        pr.radius = 3.0f;
        pr.self = projectile_moby;
        pr.unk20 = 1;
        qcopy(&pr.dir, &projectile_vars->vel);
        fast_vec_sub(&pr.dir, &projectile_moby->pos, &D_0013F3D0_307d68);
        set_vector_length_xy(&pr.dir, &pr.dir, 1.0f);
        probe = &pr;
        pr.dir.f[2] = 1.0f;
        pr.dir.f[3] = 5627.9248f;
        pr.unk18 = 1;
        pr.unk19 = 3;
        pr.oclass = projectile_moby->oclass;
    }
    if (fast_dec_timer_s16(&projectile_vars->life) && projectile_moby->command == 0) {
        projectile_moby->command = 1;
        projectile_vars->life = projectile_vars->life_max / 4;
    } else if (projectile_vars->life <= 0 && projectile_moby->command != 0) {
        mark_moby_for_removal(projectile_moby);
        return;
    }
    if (collision_line(&old, &projectile_moby->pos, 0, projectile_vars->owner, probe) == 0)
        return;

    qcopy(&projectile_moby->pos, &l09_projectile_impact_position);
    if (projectile_moby->command == 0) {
        for (i = 0; i < 5; i++) {
            spark_scatter_vector.q = 0;
            spark_scatter_vector.f[0] = random_float_between(-1.0f, 1.0f);
            spark_scatter_vector.f[1] = random_float_between(-1.0f, 1.0f);
            spark_scatter_vector.f[2] = random_float_between(-1.0f, 1.0f);
            push.q = spark_scatter_vector.q;
            reflect_vector(&spark_scatter_vector, projectile_vars, D_L09_00174000);
            fast_vec_normalize(&push, &push, fast_vec_length(&spark_scatter_vector) * 0.5f);
            fast_vec_add(&push, &spark_scatter_vector, &push);
            fast_vec_normalize(
                &push, &push, random_float_between(frame_time * 3.0f, frame_time * 6.0f));
            FUN_L00_0026dd20_307d68(
                &projectile_moby->pos,
                &push,
                0x7F2F4F6F,
                random_int_between(scale_game_frames(10), scale_game_frames(15)),
                60000.0f);
        }
    }
    spawn_explosion(projectile_moby,
                    projectile_vars,
                    0,
                    0.0f,
                    0.0f,
                    4,
                    3,
                    6,
                    1.1f,
                    0.6f,
                    1.0f,
                    -1,
                    0.9f,
                    0.0f,
                    0,
                    1,
                    -1,
                    0);
    collect_mobies_in_sphere(1.1f, &projectile_moby->pos, 0x10, projectile_moby, probe);
    mark_moby_for_removal(projectile_moby);
}
