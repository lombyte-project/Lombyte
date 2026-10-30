/* Blarg trooper projectile (class 0xB8): its spawner and update, and the puff patterns spawned along a moby's segment table. */
#include "types.h"
#include "rnc/globals.h"
#include "asm.h"
#include "eetypes.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/overlay/quad.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"

#include "qcopy.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/world/collision/collision_line.h"

/* Builds two direction vectors from a table and relative to a moby, then hands them to the L04 effect spawner. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B0068.c: func_L04_002C5938), where it is exact; names translated to the US level program. */

extern char moby_segment_puff_local_endpoint_table[] __asm__("D_L04_001D2C90");
extern void spawn_smoke_puff_trail_with_endpoint_bursts(
    void *, void *, void *, int, float, float) __asm__("FUN_L04_002c41e0");

extern short moby_segment_puff_scale_multiplier __asm__("D_L04_001618B0") __attribute__((sda));
void spawn_moby_table_segment_puffs(char *moby, int i, int j, int k) __asm__("FUN_L04_002c45b8");

void spawn_moby_table_segment_puffs(char *moby, int i, int j, int k) {
    float first_world_position[4], second_world_position[4];
    char *pi, *pj;
    pi = moby_segment_puff_local_endpoint_table + i * 16;
    qcopy(first_world_position, pi);
    first_world_position[3] = 1.0f;
    transform_vector_by_basis(first_world_position, first_world_position, moby + 0xC0);
    fast_vec_add(first_world_position, first_world_position, moby + 0x10);
    pj = moby_segment_puff_local_endpoint_table + j * 16;
    qcopy(second_world_position, pj);
    second_world_position[3] = 1.0f;
    transform_vector_by_basis(second_world_position, second_world_position, moby + 0xC0);
    fast_vec_add(second_world_position, second_world_position, moby + 0x10);
    spawn_smoke_puff_trail_with_endpoint_bursts(
        moby,
        first_world_position,
        second_world_position,
        k,
        *(float *)(pi + 0xC) * *(float *)&moby_segment_puff_scale_multiplier,
        *(float *)(pj + 0xC) * *(float *)&moby_segment_puff_scale_multiplier);
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B0068.c: func_L04_002C5A50), where it is exact; names translated to the US level program. */

extern void
spawn_moby_table_segment_puffs(char *a, int i, int j, int k) __asm__("FUN_L04_002c45b8");

extern short D_L04_001618C0 __attribute__((sda));
extern short D_L04_001618C4 __attribute__((sda));
extern short D_L04_001618C8 __attribute__((sda));
extern short D_L04_001618CC __attribute__((sda));
extern short D_L04_001618D0 __attribute__((sda));
extern short D_L04_001618D4 __attribute__((sda));
extern short D_L04_001618D8 __attribute__((sda));
extern short D_L04_001618DC __attribute__((sda));
void         spawn_moby_table_puff_pattern(char *moby) __asm__("FUN_L04_002c46d0");

void spawn_moby_table_puff_pattern(char *moby) {
    spawn_moby_table_segment_puffs(moby, 0, 1, *(int *)&D_L04_001618C0);
    spawn_moby_table_segment_puffs(moby, 2, 3, *(int *)&D_L04_001618C4);
    spawn_moby_table_segment_puffs(moby, 4, 5, *(int *)&D_L04_001618C8);
    spawn_moby_table_segment_puffs(moby, 5, 6, *(int *)&D_L04_001618CC);
    spawn_moby_table_segment_puffs(moby, 6, 7, *(int *)&D_L04_001618D0);
    spawn_moby_table_segment_puffs(moby, 8, 9, *(int *)&D_L04_001618C8);
    spawn_moby_table_segment_puffs(moby, 9, 10, *(int *)&D_L04_001618CC);
    spawn_moby_table_segment_puffs(moby, 10, 11, *(int *)&D_L04_001618D0);
    spawn_moby_table_segment_puffs(moby, 12, 13, *(int *)&D_L04_001618D4);
    spawn_moby_table_segment_puffs(moby, 14, 15, *(int *)&D_L04_001618D4);
    spawn_moby_table_segment_puffs(moby, 16, 17, *(int *)&D_L04_001618D8);
    spawn_moby_table_segment_puffs(moby, 20, 21, *(int *)&D_L04_001618D8);
    spawn_moby_table_segment_puffs(moby, 18, 19, *(int *)&D_L04_001618DC);
    spawn_moby_table_segment_puffs(moby, 22, 23, *(int *)&D_L04_001618DC);
}

/* sda.h here, after the unit's empty MACRO_ADDR: D_0015ED6C needs the real one */
#include "sda.h"

/* Vars of the class 0xB8 moby spawned by FUN_L04_002aee30. */
typedef struct {
    Vec4f        dir;  /* 0x00: travel direction (also gives the yaw) */
    f32          spin; /* 0x10: random turn rate, +-90 degrees per frame unit */
    s32          unk14;
    struct Moby *owner;     /* 0x18: the moby that fired it */
    s16          time;      /* 0x1C: frames left */
    s16          time_max;  /* 0x1E: lifetime */
    f32          hero_dist; /* 0x20: distance to the hero at spawn */
    s32          unk24;
    s32          unk28;
} ShotVars;

extern void spawn_source_attached_fading_effect_particle(
    struct Moby *, int color, int life, int, float size) __asm__("FUN_L00_0026daa0");

extern f32 frame_time_macro __asm__("D_0015ED6C") MACRO_ADDR;
/*
 * Spawns a class 0xB8 shot for `owner` at `pos` heading along `dir`, living
 * `life` frames, with two glows (bluish, or reddish for an owner of class
 * 0xFF, which also keeps full scale). Returns the new moby or 0.
 */
struct Moby *spawn_blarg_trooper_projectile(struct Moby *owner,
                                            void        *pos,
                                            void        *dir,
                                            int          life) __asm__("FUN_L04_002aee30");

struct Moby *spawn_blarg_trooper_projectile(struct Moby *owner, void *pos, void *dir, int life) {
    struct Moby *spawned_moby;
    ShotVars    *vars;
    OvlQuad      pos_q;
    OvlQuad      dir_q;
    Vec4f       *position_copy = (Vec4f *)&pos_q;
    Vec4f       *direction_copy = (Vec4f *)&dir_q;

    pos_q = *(OvlQuad *)pos;
    dir_q = *(OvlQuad *)dir;
    spawned_moby = create_moby(0xB8);
    if (spawned_moby != 0) {
        vars = (ShotVars *)spawned_moby->pvars;
        vars->owner = owner;
        spawned_moby->update_dist = 0xFF;
        spawned_moby->draw_dist = 0xFF;
        spawned_moby->visible = 1;
        if (owner->oclass != 0xFF)
            spawned_moby->scale = spawned_moby->scale * 0.5f;
        spawned_moby->rot.x = 0.0f;
        spawned_moby->rot.y = 0.0f;
        spawned_moby->rot.z = fast_arc_tan(direction_copy->x, direction_copy->y);
        qcopy(&spawned_moby->pos, position_copy);
        qcopy(&vars->dir, direction_copy);
        vars->spin =
            random_float_between(-(frame_time_macro * 1.5707964f), frame_time_macro * 1.5707964f);
        vars->unk14 = 0;
        vars->time = life;
        vars->time_max = life;
        vars->hero_dist = fast_vec_dist(&spawned_moby->pos, &hero.motion.pos);
        vars->unk24 = 0;
        vars->unk28 = 0;
        if (owner->oclass == 0xFF) {
            spawn_source_attached_fading_effect_particle(
                spawned_moby, 0x2F7F4F4F, life * 5 / 4, -1, 100000.0f);
            spawn_source_attached_fading_effect_particle(
                spawned_moby, 0x4F7F7F7F, life * 5 / 4, -1, 40000.0f);
        } else {
            spawn_source_attached_fading_effect_particle(
                spawned_moby, 0x2F4F7F7F, life * 5 / 4, -1, 100000.0f);
            spawn_source_attached_fading_effect_particle(
                spawned_moby, 0x4F6F7F7F, life * 5 / 4, -1, 40000.0f);
        }
        update_entity(spawned_moby);
    }
    return spawned_moby;
}

/* Optional query block for collision_line: a swept probe along dir. */
typedef struct {
    union {
        u128 q;
        struct {
            f32 x, y, z, w;
        } v;
    } dir;             /* 0x00: unit direction; z/w overwritten with 1 and a range */
    struct Moby *self; /* 0x10: moby to ignore */
    s32          mask; /* 0x14: collision classes */
    u8           unk18;
    u8           unk19;
    s16          oclass; /* 0x1A */
    f32          radius; /* 0x1C */
    s32          unk20;
} CollisionProbe;

extern f32  D_L04_00166F40[]; /* arena centre */
extern u128 D_L04_00173FE0;   /* hit position of the last collision_line */
extern u128 coll_hit_normal[] __asm__("D_L04_00174000"); /* normal of the last collision_line hit */
extern void FUN_L00_0026dd20(void *, void *, u32, s32, f32);

/*
 * Update of the class 0xB8 shot (spawned by FUN_L04_002aee30): moves along
 * its direction, plays its sound once it starts moving away from the hero
 * within 3 units, dies off the arena, and on a hit (probing for its owner's
 * targets until the timer runs out) bursts into five sparks and is removed.
 */
void update_blarg_trooper_projectile(struct Moby *moby) __asm__("FUN_L04_002af058");

void update_blarg_trooper_projectile(struct Moby *moby) {
    Vec4            old;
    CollisionProbe  pr;
    Vec4            push;
    Vec4            random_vector;
    ShotVars       *v = (ShotVars *)moby->pvars;
    CollisionProbe *probe;
    f32             range;
    f32             hero_distance;
    s32             i;

    range = 64.0f;
    if (v->owner->oclass == 0xFF)
        range = 128.0f;
    if (current_level_index == 12)
        range = 100.0f;
    hero_distance = fast_vec_dist(&moby->pos, &hero.motion.pos);
    qcopy(&old, &moby->pos);
    fast_vec_add(&moby->pos, &moby->pos, &v->dir);
    if (v->unk28 == 0 && v->unk24 != 0 && v->hero_dist < hero_distance && hero_distance < 3.0f) {
        allocate_voice_for_target_entry(0, 0, moby);
        v->unk28 = 1;
    }
    if (hero_distance < v->hero_dist)
        v->unk24 = 1;
    v->hero_dist = hero_distance;
    if (moby->pos.x < 0.0f || moby->pos.y < 0.0f || moby->pos.z < 0.0f ||
        range < vector_distance_xy(&moby->pos, D_L04_00166F40)) {
        mark_moby_for_removal(moby);
        return;
    }

    probe = 0;
    if (moby->command == 0) {
        pr.self = moby;
        if (v->owner->oclass == 0xFF || v->owner->oclass == 0x4B1) {
            pr.mask = 0x50001;
            pr.radius = 3.0f;
        } else {
            pr.mask = 0x10001;
            pr.radius = 1.0f;
        }
        pr.unk20 = 1;
        qcopy(&pr.dir, &v->dir);
        set_vector_length_xy(&pr.dir, &pr.dir, 1.0f);
        probe = &pr;
        pr.dir.v.z = 1.0f;
        pr.dir.v.w = 5627.9248f;
        pr.unk19 = pr.unk18 = 1;
        pr.oclass = moby->oclass;
    }
    if (fast_dec_timer_s16(&v->time) && moby->command == 0) {
        moby->command = 1;
        v->time = v->time_max / 4;
    } else if (v->time <= 0 && moby->command != 0) {
        mark_moby_for_removal(moby);
        return;
    }
    if (collision_line(&old, &moby->pos, 0, v->owner, probe) == 0)
        return;

    qcopy(&moby->pos, &D_L04_00173FE0);
    if (moby->command == 0) {
        for (i = 0; i < 5; i++) {
            random_vector.q = 0;
            random_vector.f[0] = random_float_between(-1.0f, 1.0f);
            random_vector.f[1] = random_float_between(-1.0f, 1.0f);
            random_vector.f[2] = random_float_between(-1.0f, 1.0f);
            push.q = random_vector.q;
            reflect_vector(&random_vector, v, coll_hit_normal);
            fast_vec_normalize(&push, &push, fast_vec_length(&random_vector) * 0.5f);
            fast_vec_add(&push, &random_vector, &push);
            fast_vec_normalize(
                &push,
                &push,
                random_float_between(frame_time_macro * 3.0f, frame_time_macro * 6.0f));
            FUN_L00_0026dd20(&moby->pos,
                             &push,
                             0x7F2F4F6F,
                             random_int_between(scale_game_frames(10), scale_game_frames(15)),
                             30000.0f);
        }
    }
    mark_moby_for_removal(moby);
}

/* Ported from rac1-decomp (src/overlays/l04_eudora/vendor_002CB800.c: func_L04_002D48B8), where it is exact; names translated to the US level program. */

void *spawn_moby_attachment(char *source_moby, char *spawn_position) __asm__("FUN_L04_002ce080");

void *spawn_moby_attachment(char *source_moby, char *spawn_position) {
    char  position_copy[16];
    char *position_copy_ptr = position_copy;
    char *spawned_moby;
    *(OvlQuad *)position_copy = *(OvlQuad *)spawn_position;
    spawned_moby = (char *)create_moby(0x1F3);
    if (spawned_moby != 0) {
        MOBY(spawned_moby)->update_dist = 0xFF;
        MOBY(spawned_moby)->draw_dist = ((unsigned short)MOBY(source_moby)->draw_dist);
        spawned_moby[0x31] = 1;
        *(int *)(spawned_moby + 0x40) = 0;
        *(int *)(spawned_moby + 0x44) = 0;
        MOBY(spawned_moby)->rot.z = MOBY(source_moby)->rot.z;
        MOBY(spawned_moby)->spawn_frame = MOBY(source_moby)->spawn_frame;
        qcopy(spawned_moby + 0x10, position_copy_ptr);
        update_entity(spawned_moby);
    }
    return spawned_moby;
}

short D_L08_00161994 = 0;
short D_L08_001619B0 = 0xa;
short D_L08_001619B4 = 0xa;
short D_L08_001619B8 = 0xa;
short D_L08_001619BC = 0xa;
short D_L08_001619C0 = 8;
short D_L08_001619C4 = 0x14;
short D_L08_001619C8 = 8;
short D_L08_001619CC = 0x14;
