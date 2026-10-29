/* Shared moby classes: spawner, spawner_egg. */
#include "types.h"
#include "rnc/globals.h"
#include "asm.h"
#include "sda.h"

#include "qcopy.h"
#include "rnc/overlay/quad.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/data_labels.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"

/* pvars of the projectile moby FUN_L09_00307ba8 spawns */
typedef struct {
    OvlVec4 vel;     /* 0x00 */
    f32 spin;        /* 0x10 */
    s32 unk14;       /* 0x14 */
    void *owner;     /* 0x18 */
    s16 life;        /* 0x1C */
    s16 life_max;    /* 0x1E */
    f32 dist;        /* 0x20: distance from the hero at spawn */
    s32 unk24;       /* 0x24 */
    s32 unk28;       /* 0x28 */
} ShotVars;

extern OvlVec4 D_0013F3D0_307ba8 __asm__("D_0013F3D0");
extern struct Moby *create_moby(s32) __asm__("FUN_0020c4f8");
extern f32 vector_length_xy_307ba8(OvlVec4 *) __asm__("FUN_001f9b20");
extern f32 fast_arc_tan(f32, f32) __asm__("FUN_001f9e90");
extern f32 random_float_between(f32, f32) __asm__("FUN_002132a8");
extern f32 fast_vec_dist(OvlVec4 *, OvlVec4 *) __asm__("FUN_001f9b48");
extern u8 *spawn_source_attached_fading_effect_particle(f32, struct Moby *, u32, s32, s32) __asm__("FUN_L00_0026daa0");
extern void FUN_L00_00250df8_307ba8(struct Moby *) __asm__("FUN_L00_00250df8");

struct Moby *spawn_class_4ea_projectile(void *owner, OvlVec4 *pos, OvlVec4 *vel, s32 life) __asm__("FUN_L09_00307ba8");

struct Moby *spawn_class_4ea_projectile(void *owner, OvlVec4 *pos, OvlVec4 *vel, s32 life) {
    OvlVec4 position_copy;
    OvlVec4 velocity_copy;
    OvlVec4 *pp = &position_copy;
    OvlVec4 *vp = &velocity_copy;
    struct Moby *spawned_moby;
    ShotVars *projectile_vars;
    f32 spin_limit;
    s32 trail;

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
        spawned_moby->rot.y = -fast_arc_tan(vector_length_xy_307ba8(vp), vp->f[2]);
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
        spawn_source_attached_fading_effect_particle(400000.0f, spawned_moby, 0x2F7F4F4F, trail, -1);
        spawn_source_attached_fading_effect_particle(160000.0f, spawned_moby, 0x4F7F7F7F, trail, -1);
        FUN_L00_00250df8_307ba8(spawned_moby);
    }
    return spawned_moby;
}
/* Optional query block for collision_line: a swept probe along dir. */
typedef struct {
    OvlVec4 dir;                   /* 0x00: unit direction; z/w overwritten with 1 and a range */
    struct Moby *self;             /* 0x10: moby to ignore */
    s32 mask;                      /* 0x14: collision classes */
    u8 unk18;
    u8 unk19;
    s16 oclass;                    /* 0x1A */
    f32 radius;                    /* 0x1C */
    s32 unk20;
} CollisionProbe;

extern OvlVec4 D_0013F3D0_307d68 __asm__("D_0013F3D0");
extern OvlVec4 D_L09_00166F40_307d68 __asm__("D_L09_00166F40");
extern char D_L09_00174000[] __asm__("D_L09_00174000");
extern f32 distance_307d68(void *, void *) __asm__("FUN_001f9b48");
extern f32 vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern void set_vector_length_xy(void *, void *, f32) __asm__("FUN_001f9c48");
extern s32 allocate_voice_307d68(s32, s32, void *) __asm__("FUN_0022da68");
extern void mark_moby_for_removal_307d68(struct Moby *) __asm__("FUN_0020c828");
extern s32 tick_countdown_16_307d68(s16 *) __asm__("FUN_001f9770");
extern s32 collision_line_307d68(void *, void *, s32, void *, CollisionProbe *) __asm__("FUN_001efa68");
extern void reflect_vector(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern s32 random_int_between(s32, s32) __asm__("FUN_L00_00257b90");
extern void FUN_L00_0026dd20_307d68(void *, void *, u32, s32, f32) __asm__("FUN_L00_0026dd20");
extern void spawn_explosion(void *, void *, void *, f32, f32, s32, s32, s32, f32, f32,
                                    f32, s32, f32, f32, s32, s32, s32, s32) __asm__("FUN_L00_0025e450");
extern s32 collect_mobies_in_sphere(f32, void *, s32, void *, void *) __asm__("FUN_L00_001f2868");

extern OvlVec4 l09_projectile_impact_position __asm__("D_L09_00173FE0");
/*
 * Update of the shot FUN_L09_00307ba8 spawns (cf. FUN_L04_002af058): moves
 * along vel, plays its sound once it starts moving away from the hero within
 * 3 units, dies outside the level box, and on a hit (probing along the line
 * from the hero until the timer runs out) bursts into five sparks and a
 * flash, damages around the hit and is removed.
 */
void update_class_4ea_projectile(struct Moby *projectile_moby) __asm__("FUN_L09_00307d68");

void update_class_4ea_projectile(struct Moby *projectile_moby)
{
    OvlVec4 old;
    CollisionProbe pr;
    OvlVec4 push;
    OvlVec4 spark_scatter_vector;
    ShotVars *projectile_vars = (ShotVars *)projectile_moby->pvars;
    CollisionProbe *probe;
    f32 distance_to_hero;
    s32 i;

    distance_to_hero = distance_307d68(&projectile_moby->pos, &D_0013F3D0_307d68);
    qcopy(&old, &projectile_moby->pos);
    fast_vec_add(&projectile_moby->pos, &projectile_moby->pos, &projectile_vars->vel);
    if (projectile_vars->unk28 == 0 && projectile_vars->unk24 != 0 && projectile_vars->dist < distance_to_hero && distance_to_hero < 3.0f) {
        allocate_voice_307d68(0, 0, projectile_moby);
        projectile_vars->unk28 = 1;
    }
    if (distance_to_hero < projectile_vars->dist)
        projectile_vars->unk24 = 1;
    projectile_vars->dist = distance_to_hero;
    if (projectile_moby->pos.x < 2.0f || 1021.0f < projectile_moby->pos.x || projectile_moby->pos.y < 2.0f || 1021.0f < projectile_moby->pos.y
        || projectile_moby->pos.z < 2.0f || 1021.0f < projectile_moby->pos.z
        || 255.0f < vector_distance(&projectile_moby->pos, &D_L09_00166F40_307d68)) {
        mark_moby_for_removal_307d68(projectile_moby);
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
    if (tick_countdown_16_307d68(&projectile_vars->life) && projectile_moby->command == 0) {
        projectile_moby->command = 1;
        projectile_vars->life = projectile_vars->life_max / 4;
    } else if (projectile_vars->life <= 0 && projectile_moby->command != 0) {
        mark_moby_for_removal_307d68(projectile_moby);
        return;
    }
    if (collision_line_307d68(&old, &projectile_moby->pos, 0, projectile_vars->owner, probe) == 0)
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
            fast_vec_normalize(&push, &push, random_float_between(frame_time * 3.0f, frame_time * 6.0f));
            FUN_L00_0026dd20_307d68(&projectile_moby->pos, &push, 0x7F2F4F6F,
                                    random_int_between(scale_game_frames(10), scale_game_frames(15)), 60000.0f);
        }
    }
    spawn_explosion(projectile_moby, projectile_vars, 0, 0.0f, 0.0f, 4, 3, 6, 1.1f, 0.6f, 1.0f, -1, 0.9f, 0.0f, 0, 1, -1, 0);
    collect_mobies_in_sphere(1.1f, &projectile_moby->pos, 0x10, projectile_moby, probe);
    mark_moby_for_removal_307d68(projectile_moby);
}
extern s32 get_first_moby_in_group(struct Moby **, s32, s32, s32) __asm__("FUN_L00_002591d0");
extern void set_moby_animation(struct Moby *, s32, s32) __asm__("FUN_00212ed8");
extern struct Moby *get_moby_suck_cannon_record(struct Moby *) __asm__("FUN_L00_002db890");
extern void copy_hero_moby_field_38(struct Moby *) __asm__("FUN_L00_0025d1b8");
extern void FUN_L00_00250df8_30a778(struct Moby *) __asm__("FUN_L00_00250df8");
extern s16 *suck_get_aim_height(struct Moby *) __asm__("FUN_002141f8");
extern void debug_print(char *, s32, s32) __asm__("FUN_001e93b0");
extern char spawner_child_respawn_failure_format[] __asm__("D_L09_00209280");

struct Moby *respawn_spawner_child(struct Moby *owner, Vec4 *pos) __asm__("FUN_L09_0030a778");

struct Moby *respawn_spawner_child(struct Moby *owner, Vec4 *pos)
{
    struct Moby *spawned_moby;
    struct Moby *other;
    u8 *owner_vars;
    s16 *data;
    s32 left;
    Vec4 pos_copy;
    Vec4 *pos_ptr;
    struct Moby *result;

    pos_ptr = &pos_copy;
    pos_copy.q = pos->q;

    if (owner == 0)
        return 0;
    owner_vars = owner->pvars;
    spawned_moby = 0;
    result = 0;
    if (get_first_moby_in_group(&spawned_moby, *(s32 *)(owner_vars + 0x94), 1, 1) == 0) {
        owner_vars[0x91] = owner_vars[0x91] + 1;
        qcopy_nc(&spawned_moby->pos, pos_ptr);
        spawned_moby->pos.w = 0.0f;
        spawned_moby->state = 0;
        spawned_moby->command = 0;
        spawned_moby->flags = spawned_moby->pclass->flags;
        spawned_moby->scale = spawned_moby->pclass->scale;
        spawned_moby->update_dist = 0xff;
        spawned_moby->draw_dist = 0xff;
        spawned_moby->visible = 1;
        if (*(s32 *)((u8 *)spawned_moby->pclass + 0x40) != 0)
            spawned_moby->flags |= 0x10;
        if (*((u8 *)spawned_moby->pclass + 0x0f) != 0)
            spawned_moby->flags |= 0x400;
        spawned_moby->occlusion = 0x7f80;
        spawned_moby->unk71 = 0xff;
        spawned_moby->unk72 = 0xff;
        spawned_moby->hit_slot = 0xff;
        if (owner_vars[0x93] != 0)
            spawned_moby->scale = spawned_moby->pclass->scale * 0.1f;
        set_moby_animation(spawned_moby, 0, 0);
        other = get_moby_suck_cannon_record(spawned_moby);
        if (other != 0) {
            *(volatile s32 *)((u8 *)other + 0x6c) = 0;
            *(volatile s32 *)((u8 *)other + 0x94) = 0;
            *(volatile s16 *)((u8 *)other + 0x68) = 0;
        }
        if (owner->unkB4 < spawned_moby->unkB4)
            spawned_moby->unkB4 = owner->unkB4;
        if (spawned_moby->unkB4 < 1)
            spawned_moby->unkB4 = 1;
        left = (u16)owner->unkB4 - (u16)spawned_moby->unkB4;
        owner->unkB4 = left;
        if ((s16)left < 0)
            owner->unkB4 = 1;
        {
            struct Moby *target = ((struct Moby *volatile *)&spawned_moby)[0];
            target->collision = target->pclass->collision;
        }
        copy_hero_moby_field_38(spawned_moby);
        FUN_L00_00250df8_30a778(spawned_moby);
        result = spawned_moby;
        data = suck_get_aim_height(result);
        if (data != 0) {
            *(f32 *)((u8 *)data + 0x34) = 0.0f;
            *(f32 *)((u8 *)data + 0x30) = 0.0f;
            *(f32 *)data = (f32)data[2];
        }
    } else {
        debug_print(spawner_child_respawn_failure_format, (s16)owner->save_id, D_L09_0015F5CC);
    }
    return result;
}
extern char *FUN_L00_0025a420_c(void *, int, int) __asm__("FUN_L00_0025a420");
extern int FUN_L00_0025a478_c(void *, void *, void *, int, int *, float *, int,
                              int) __asm__("FUN_L00_0025a478");
extern void FUN_L00_0025d458_c(void *, void *) __asm__("FUN_L00_0025d458");
extern void FUN_L00_0025d538_c(void *, void *) __asm__("FUN_L00_0025d538");
extern void FUN_L00_00257470_c(void *, int, int) __asm__("FUN_L00_00257470");

void process_spawner_hit_record(unsigned char *moby, char *spawner_vars) __asm__("FUN_L09_0030a9e8");

void process_spawner_hit_record(unsigned char *moby, char *spawner_vars) {
    char *moby_record;
    int record_processing_status;
    float remaining_value;

    moby_record = FUN_L00_0025a420_c(moby, 0xC30000, 0);
    FUN_L00_0025a478_c(moby, moby_record, spawner_vars + 0x20, 0, &record_processing_status, 0, 0, 4);
    if (record_processing_status >= 2) {
        remaining_value = *(float *)(spawner_vars + 0x20) - *(float *)(moby_record + 0x2C);
        *(float *)(spawner_vars + 0x20) = remaining_value;
        if (0.0f < remaining_value) {
            spawner_vars[0x67] = 0x5A;
            FUN_L00_0025d458_c(moby, spawner_vars + 0x60);
        } else {
            FUN_L00_00257470_c(moby, 0, -1);
            moby[0x20] = 2;
            spawner_vars[0x67] = 0x78;
            FUN_L00_0025d458_c(moby, spawner_vars + 0x60);
        }
    }
    moby[0xA4] = 0xFF;
    FUN_L00_0025d538_c(moby, spawner_vars + 0x60);
}
/* Counts the live level-0x75E mobys among the 20 slots at +0xB0 and returns the last slot not counted. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C6B30.c: func_L09_0030BEE8), where it is exact; names translated to the US level program. */

extern int count_active_moby_group_members(int, int) __asm__("FUN_L01_0026e008");

int refresh_spawner_population_and_find_free_slot(int unused, char *spawner_vars) __asm__("FUN_L09_0030aae0");

int refresh_spawner_population_and_find_free_slot(int unused, char *spawner_vars) {
    int last = -1;
    int i;
    char **slot;
    spawner_vars[0x91] = count_active_moby_group_members(*(int *)(spawner_vars + 0x94), -1);
    for (i = 0, slot = (char **)(spawner_vars + 0xB0); i < 20; i++, slot++) {
        char *slot_moby = *slot;
        if (slot_moby != 0 && *(short *)(slot_moby + 0xA6) == 0x75E && (unsigned char)slot_moby[0x20] != 0xFE &&
            (unsigned char)slot_moby[0x20] != 0xFD) {
            spawner_vars[0x91]++;
        } else {
            last = i;
        }
    }
    return last;
}
extern void FUN_L09_0030a9e8_30ab80(char *, char *) __asm__("FUN_L09_0030a9e8");
extern void mark_moby_for_removal_30ab80(void *) __asm__("FUN_0020c828");
extern int FUN_L00_002591d0_30ab80(int *, int, int, int) __asm__("FUN_L00_002591d0");
extern int tick_countdown_16_30ab80(void *) __asm__("FUN_001f9770");
extern int random_integer_below(int) __asm__("FUN_00213260");
extern int activate_effect_l08(void *, void *, float, void *, int, void *, int) __asm__("FUN_L00_0025ff38");
extern int refresh_spawner_population_and_find_free_slot(int, char *) __asm__("FUN_L09_0030aae0");
extern float distance_30ab80(void *, void *) __asm__("FUN_001f9b48");
extern void clamp_vector_length(void *, void *, float) __asm__("FUN_001f9c90");
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern void generate_random_scaled_vector2(float *, float) __asm__("FUN_L09_00277758");
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern float FUN_00213508_30ab80(void *, int, float) __asm__("FUN_00213508");
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void add_random_offset_to_vector_xyz(float *, float) __asm__("FUN_L00_0025f8e0");
extern float FUN_L00_0025abf0_30ab80(void *, void *, void *, float, float) __asm__("FUN_L00_0025abf0");
extern char *spawn_spawner_egg(int, void *, void *, unsigned char, unsigned char) __asm__("FUN_L09_0030b218");
extern void FUN_L00_0025e450_30ab80(void *, void *, void *, float, float, int, int, int, float, float, float, float, int,
                                    float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern void *FUN_L00_00263fd8_30ab80(char *, int, void *, void *, int, int, float, float *, float *,
                                     float *) __asm__("FUN_L00_00263fd8");

typedef struct {
    char pad0[0xB0];
    char *slots[4];
} SpawnerVars;

extern char *spawner_target_region_records __asm__("D_L09_001600EC") __attribute__((sda));
extern float D_L09_0015F580[] MACRO_ADDR;
extern f32 frame_time_macro __asm__("D_0015ED6C") MACRO_ADDR;
extern f32 frame_time_sq_macro __asm__("D_0015ED70") MACRO_ADDR;
/* Spawner update (moby class 0x75D): state 0 clears the slot list, state 1 picks a target and lobs a
 * 0x75E moby towards it, state 2 explodes. Ported from rac1-decomp (src/overlays/shared/vendor_002C6B30.c:
 * func_L09_0030BF88), where it is exact; this build has no 0.85 limit check before a spawn. */
void spawner_update(char *moby) __asm__("FUN_L09_0030ab80");
void spawner_update(char *moby)
{
    enum {
        SPAWNER_STATE_INITIALIZE = 0,
        SPAWNER_STATE_SPAWN = 1,
        SPAWNER_STATE_EXPLODE = 2
    };
    char *spawner_vars = *(char **)(moby + 0x78);
    float target[4];
    float launch_velocity[4];
    float spawn_position[4];
    float offset[4];

    FUN_L09_0030a9e8_30ab80(moby, spawner_vars);
    switch ((unsigned char)moby[0x20]) {
    case SPAWNER_STATE_INITIALIZE: {
        int moby_to_remove = 0;
        while (FUN_L00_002591d0_30ab80(&moby_to_remove, *(int *)(spawner_vars + 0x94), 0, 0) == 0) {
            mark_moby_for_removal_30ab80((void *)moby_to_remove);
        }
        moby[0x20] = 1;
        return;
    }
    case SPAWNER_STATE_SPAWN: {
        FUN_L09_0030a9e8_30ab80(moby, spawner_vars);
        if (!tick_countdown_16_30ab80(spawner_vars + 0x8E))
            return;
        if (random_integer_below(7) == 0) {
            char *excluded = 0;
            int excluded_count = 0;
            char *region = 0;
            int region_count = 0;
            if (*(int *)(spawner_vars + 0x84) != -1) {
                excluded = spawner_vars + 0x84;
                excluded_count = 1;
            }
            if (*(int *)(spawner_vars + 0x88) != -1) {
                char *q = D_L09_001B0630[*(int *)(spawner_vars + 0x88)];
                region_count = *(int *)q;
                region = q + 0x10;
            }
            activate_effect_l08(moby, spawner_vars + 0x100, *(float *)(spawner_vars + 0x80), excluded, excluded_count, region, region_count);
        } else {
            char *target_moby = *(char **)(spawner_vars + 0x140);
            if (target_moby != 0 && (unsigned char)target_moby[0x20] != 0xFE && (unsigned char)target_moby[0x20] != 0xFD) {
                qcopy(spawner_vars + 0x100, target_moby + 0x10);
            } else {
                *(int *)(spawner_vars + 0x140) = 0;
                *(int *)(spawner_vars + 0x144) = 2;
            }
        }
        if (*(int *)(spawner_vars + 0x144) == 2)
            return;
        {
        int last = refresh_spawner_population_and_find_free_slot((int)moby, spawner_vars);
        char *sel;
        if ((unsigned char)spawner_vars[0x91] >= (unsigned char)spawner_vars[0x90])
            return;
        if (last == -1)
            return;
        sel = 0;
        if (*(unsigned char *)(spawner_vars + 0x92) != 0) {
            int i;
            float best = 100000.0f;
            for (i = 0; i < 4; i++) {
                int idx = *(int *)(spawner_vars + 0xA0 + i * 4);
                if (idx != -1) {
                    char *q = spawner_target_region_records + (idx << 7);
                    float candidate_distance = distance_30ab80(D_0013E533 + 0xE9D, q + 0x30);
                    if (candidate_distance < best) {
                        sel = q;
                        best = candidate_distance;
                    }
                }
            }
            if (sel == 0) {
                fast_vec_sub(target, spawner_vars + 0x100, moby + 0x10);
                fast_vec_scale(target, target, 0.5f);
                if (3.0f < *(float *)(spawner_vars + 0x80))
                    clamp_vector_length(target, target, 3.0f);
                if (fast_vec_length(target) < 3.0f)
                    fast_vec_normalize(target, target, 3.0f);
                fast_vec_add(target, target, moby + 0x10);
            } else {
                qcopy(target, sel + 0x30);
            }
        } else {
            int start = random_int_between(0, 3);
            int i;
            for (i = 0; i < 4; i++) {
                int k = (i + start) % 4;
                int idx = *(int *)(spawner_vars + (k << 2) + 0xA0);
                if (idx != -1) {
                    sel = spawner_target_region_records + (idx << 7);
                    break;
                }
            }
            if (sel == 0) {
                float target_offset_scale = *(float *)(spawner_vars + 0x80);
                if (target_offset_scale < 3.0f)
                    target_offset_scale = 3.0f;
                generate_random_scaled_vector2(target, target_offset_scale);
                fast_vec_add(target, target, moby + 0x10);
            } else {
                qcopy(target, sel + 0x30);
            }
        }
        if (sel != 0) {
            fast_vec_scale(launch_velocity, sel, random_float_between(-1.0f, 1.0f));
            fast_vec_add(target, target, launch_velocity);
            fast_vec_scale(launch_velocity, sel + 0x10, random_float_between(-1.0f, 1.0f));
            fast_vec_add(target, target, launch_velocity);
        }
        {
            float ground = FUN_00213508_30ab80(target, 0, 0.5f);
            if (ground != 0.0f)
                target[2] = ground;
        }
        fast_vec_sub(launch_velocity, target, moby + 0x10);
        {
            float target_distance = fast_vec_length(launch_velocity);
            float speed = target_distance * (frame_time_macro * 0.6f);
            fast_vec_normalize(launch_velocity, launch_velocity, speed);
            moby_point_position(moby, 0, spawn_position);
            fast_vec_normalize(offset, launch_velocity, 0.05f);
            add_random_offset_to_vector_xyz(offset, 0.025f);
            fast_vec_add(spawn_position, spawn_position, offset);
            launch_velocity[2] = FUN_L00_0025abf0_30ab80(spawn_position, target, 0, speed, -(frame_time_sq_macro * 9.8f));
        }
        ((SpawnerVars *)spawner_vars)->slots[last] =
            spawn_spawner_egg((int)moby, spawn_position, launch_velocity, (unsigned char)spawner_vars[0x93], (unsigned char)last);
        *(short *)(spawner_vars + 0x8E) = scale_game_frames(*(short *)(spawner_vars + 0x8C));
        spawner_vars[0x91] = (char)(spawner_vars[0x91] + 1);
        }
        return;
    }
    case SPAWNER_STATE_EXPLODE:
        FUN_L00_0025e450_30ab80(moby, spawner_vars + 0x70, 0, 0.0f, 0.0f, 30, 8, 20, 8.0f, 4.0f, 18.0f, 1.0f, 1, 15.0f, 1, 1, -1, 0);
        FUN_L00_00263fd8_30ab80(moby, 0x6C5, moby + 0x10, moby + 0x40, 0, 0, 0.0f, D_L09_0015F580,
                                D_L09_0015F580, D_L09_0015F580);
        FUN_L00_00263fd8_30ab80(moby, 0x6C6, moby + 0x10, moby + 0x40, 0, 0, 0.0f, D_L09_0015F580,
                                D_L09_0015F580, D_L09_0015F580);
        FUN_L00_00263fd8_30ab80(moby, 0x6C7, moby + 0x10, moby + 0x40, 0, 0, 0.0f, D_L09_0015F580,
                                D_L09_0015F580, D_L09_0015F580);
        mark_moby_for_removal_30ab80(moby);
        return;
    default:
        return;
    }
}
#include "qcopy.h"

/* Spawns a moby of class 0x75E and initialises its data. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C6B30.c: func_L09_0030C658), where it is exact; names translated to the US level program. */

extern char *func_0020D348_m(int);
extern void update_entity(void *) __asm__("FUN_L00_00250df8");

char *spawn_spawner_egg(int owner, void *initial_position, void *initial_velocity, unsigned char grow_held_moby, unsigned char owner_slot_index) {
    char *spawned_moby = (char *)func_0020D348_m(0x75E);
    if (spawned_moby) {
        char *moby_vars = *(char **)(spawned_moby + 0x78);
        *(int *)(moby_vars + 0x10) = owner;
        qcopy(spawned_moby + 0x10, initial_position);
        qcopy(moby_vars, initial_velocity);
        *(float *)(spawned_moby + 0x40) = random_float_between(-3.1415927f, 3.1415927f);
        *(float *)(spawned_moby + 0x44) = random_float_between(-3.1415927f, 3.1415927f);
        *(int *)(moby_vars + 0x14) = 0;
        *(short *)(moby_vars + 0x18) = scale_game_frames(500);
        *(unsigned char *)(spawned_moby + 0x30) = 0xFF;
        *(short *)(spawned_moby + 0x32) = 0x40;
        spawned_moby[0x31] = 1;
        spawned_moby[0x20] = 0;
        *(float *)(spawned_moby + 0x2C) = *(float *)(*(char **)(spawned_moby + 0x24) + 0x24) * 0.3f;
        moby_vars[0x1C] = owner_slot_index;
        moby_vars[0x1A] = grow_held_moby;
        moby_vars[0x1B] = 0;
        update_entity(spawned_moby);
    }
    return spawned_moby;
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C6B30.c: func_L09_0030C790), where it is exact; names translated to the US level program. */

void mark_moby_for_removal(struct Obj *obj) __asm__("FUN_0020c828");

void remove_spawner_egg(void *moby, char *data) __asm__("FUN_L09_0030b350");

void remove_spawner_egg(void *moby, char *data) {
    char *other = *(char **)(data + 0x10);
    if (other != 0 && *(short *)(other + 0xA6) == 0x75D && (unsigned char)other[0x20] != 0xFE &&
        (unsigned char)other[0x20] != 0xFD) {
        int index = (unsigned char)data[0x1C];
        char *base = *(char **)(other + 0x78);
        base += index * 4;
        *(int *)(base + 0xB0) = 0;
    }
    mark_moby_for_removal(moby);
}
/* Bubble update: drifts, bounces off level geometry and other bubbles, settles on the ground, then shrinks away. */
#include "rnc/math/vector.h"
#include "rnc/overlay/collision.h"
#include "rnc/gameplay/entities/moby.h"


/* pvars of the class 0x75E bubble FUN_L09_0030b218 spawns */
typedef struct {
    Vec4 vel;              /* 0x00 */
    struct Moby *owner;    /* 0x10: class 0x75D spawner; its pvars hold the bubble list at 0xB0 */
    struct Moby *held;     /* 0x14: from FUN_L09_0030a778 when the bubble settles */
    s16 life;              /* 0x18 */
    u8 unk1A;
    u8 sound_delay;        /* 0x1B */
    u8 slot;               /* 0x1C: index in the owner's bubble list */
} BubbleVars;

typedef struct {
    u8 pad0[0xB0];
    struct Moby *bubbles[20];
} BubbleOwnerVars;

extern CollisionHit D_L09_00173FC0;

extern s32 collision_line(void *, void *, s32, struct Moby *, s32) __asm__("FUN_001efa68");
extern s32 collision_hit_material(void) __asm__("FUN_001f0b58");
extern s32 fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern f32 vector_length_xy(void *) __asm__("FUN_001f9b20");
extern f32 probe_ground_height(void *, s32, f32) __asm__("FUN_00213508");
extern s32 allocate_voice_for_target_entry(s32, s32, struct Moby *) __asm__("FUN_0022da68");
extern s32 coll_sphere(f32, void *, s32, struct Moby *) __asm__("FUN_L00_001f0d60");
extern void FUN_L00_001fefc8(void *);
extern void jet_animate(struct Moby *, f32) __asm__("FUN_L00_00259fe8");
extern f32 normalize_angle(f32) __asm__("FUN_L00_0025e310");
extern struct Moby *respawn_spawner_child(struct Moby *, Vec4 *) __asm__("FUN_L09_0030a778");

static inline void bubble_bounce(struct Moby *moby, BubbleVars *vars) {
    CollisionHit *hit = &D_L09_00173FC0;

    if (hit->moby == 0) {
        reflect_vector(&vars->vel, &vars->vel, &hit->normal_x);
        if (fast_arc_tan(hit->normal_z, vector_length_xy(&hit->normal_x)) < 0.6981317f && collision_hit_material() == -1) {
            fast_vec_scale(&vars->vel, &vars->vel, 0.5f);
            if (fast_vec_length(&vars->vel) < 0.025f) {
                moby->state = 1;
            }
        }
    } else if (((struct Moby *)hit->moby)->oclass != 0x75E) {
        reflect_vector(&vars->vel, &vars->vel, &hit->normal_x);
        if (fast_arc_tan(hit->normal_z, vector_length_xy(&hit->normal_x)) < 0.6981317f && collision_hit_material() == -1) {
            fast_vec_scale(&vars->vel, &vars->vel, 1.2f);
        }
    } else {
        vars->vel.f[2] *= random_float_between(0.9f, 1.1f);
    }
    if (vars->sound_delay == 0) {
        allocate_voice_for_target_entry(0, 0, moby);
        vars->sound_delay = 12;
    }
}

void spawner_egg_update(struct Moby *moby) __asm__("FUN_L09_0030b3b0");
void spawner_egg_update(struct Moby *moby) {
    enum {
        SPAWNER_EGG_STATE_MOVING = 0,
        SPAWNER_EGG_STATE_RESPAWN_CHILD = 1,
        SPAWNER_EGG_STATE_SHRINKING = 2
    };
    BubbleVars *vars = (BubbleVars *)moby->pvars;
    Vec4 previous_position;
    Vec4 step;
    Vec4 away;
    BubbleOwnerVars *owner;
    struct Moby *other;
    struct Moby *held;
    s32 i;
    f32 other_bubble_distance_xy;
    f32 rate;

    FUN_L00_001fefc8(&vars->sound_delay);
    qcopy(&previous_position, &moby->pos);
    switch (moby->state) {
    case SPAWNER_EGG_STATE_MOVING:
        moby->scale += (moby->pclass->scale - moby->scale) * (frame_scale * 0.05f);
        fast_vec_add(&moby->pos, &moby->pos, &vars->vel);
        fast_vec_normalize(&step, &vars->vel, 0.2f);
        fast_vec_add(&step, &step, &moby->pos);
        moby->collision = 0;
        vars->owner->collision = 0;
        if (collision_line(&previous_position, &step, 4, moby, 0) != 0) {
            fast_vec_sub(&step, &D_L09_00173FC0.point, &step);
            fast_vec_normalize(&step, &step, 0.215f);
            fast_vec_add(&moby->pos, &moby->pos, &step);
            bubble_bounce(moby, vars);
        }
        if (coll_sphere(0.2f, &moby->pos, 4, moby) != 0) {
            fast_vec_sub(&step, &D_L09_00173FC0.point, D_L09_00173FC0.pad30);
            fast_vec_normalize(&step, &step, 0.015f);
            fast_vec_add(&moby->pos, D_L09_00173FC0.pad30, &step);
            bubble_bounce(moby, vars);
        }
        owner = (BubbleOwnerVars *)vars->owner->pvars;
        for (i = 0; i < 20; i++) {
            other = owner->bubbles[i];
            if (other != moby && other != NULL && other->oclass == 0x75E && other->state != 0xFE &&
                other->state != 0xFD) {
                fast_vec_sub(&away, &moby->pos, &other->pos);
                away.f[2] = 0.0f;
                other_bubble_distance_xy = fast_vec_length(&away);
                if (other_bubble_distance_xy < 1.8f) {
                    fast_vec_scale(&away, &away, (1.8f - other_bubble_distance_xy) * (frame_scale * -0.5f + 1.0f));
                    fast_vec_add(&moby->pos, &moby->pos, &away);
                }
            }
        }
        if (fast_dec_timer_s16(&vars->life) != 0) {
            moby->state = 2;
        }
        moby->rot.x = normalize_angle(vars->vel.f[0]);
        moby->rot.y = normalize_angle(vars->vel.f[1]);
        moby->collision = moby->pclass->collision;
        vars->owner->collision = vars->owner->pclass->collision;
        vars->vel.f[2] -= frame_time_sq * 9.8f;
        moby->rot.x += frame_scale * 0.01f;
        moby->rot.y += frame_scale * 0.02f;
        break;
    case SPAWNER_EGG_STATE_RESPAWN_CHILD:
        moby->pos.z = probe_ground_height(&moby->pos, 0, 0.5f) + 0.2f;
        moby->pos.w = 0.0f;
        step.q = ((Vec4 *)&moby->pos)->q;
        vars->held = respawn_spawner_child(vars->owner, &step);
        moby->state = 2;
        break;
    case SPAWNER_EGG_STATE_SHRINKING:
        rate = frame_scale * 0.05f;
        moby->scale -= moby->scale * rate;
        if (vars->held != NULL && vars->unk1A != 0) {
            vars->held->scale += (vars->held->pclass->scale - vars->held->scale) * rate;
        }
        if (moby->scale < moby->pclass->scale * (frame_scale * 0.05f)) {
            moby->scale = 0.0001f;
            held = vars->held;
            if (held == NULL || held->state == 0xFE || held->state == 0xFD || vars->unk1A == 0 ||
                held->pclass->scale - 0.01f <= held->scale) {
                remove_spawner_egg(moby, (char *)vars);
            }
        }
        break;
    }
    jet_animate(moby, moby->scale * 0.3f / moby->pclass->scale);
    if (moby->pos.z < 5.0f || moby->pos.z > 500.0f) {
        remove_spawner_egg(moby, (char *)vars);
        return;
    }
    if (moby->pos.x < 5.0f) {
        moby->pos.x = 5.0f;
    }
    if (moby->pos.x > 1018.0f) {
        moby->pos.x = 1018.0f;
    }
    if (moby->pos.y < 5.0f) {
        moby->pos.y = 5.0f;
    }
    if (moby->pos.y > 1018.0f) {
        moby->pos.y = 1018.0f;
    }
    if (moby->pos.z < 5.0f) {
        moby->pos.z = 5.0f;
    }
    if (moby->pos.z > 1018.0f) {
        moby->pos.z = 1018.0f;
    }
}
