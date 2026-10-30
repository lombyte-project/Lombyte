/* Shared moby class bomb (0x221): the bomb a spawner drops, which lands and revives one of the spawner's pooled children. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/collision.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/mobys/get_moby_target_info.h"
#include "rnc/mobys/animation/set_moby_animation.h"
#include "rnc/overlay/shared/hero/copy_hero_moby_field_38.h"
#include "rnc/overlay/shared/weapons/suckcannon.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/world/collision/collision_hit_material.h"
#include "rnc/world/collision/collision_line.h"
#include "qcopy.h"

/* Class header fields the spawner copies into a fresh moby. */
typedef struct {
    u8  pad0[0xF];
    u8  always_draw; /* 0x0F */
    s32 unk10;       /* 0x10 */
    u8  pad14[0x10];
    f32 scale; /* 0x24 */
    u8  pad28[0x18];
    s32 unk40; /* 0x40 */
    u16 flags; /* 0x44 */
} SpawnClass;

/* Spawner pvars. */
typedef struct {
    u8  pad0[0x83];
    u8  options; /* 0x83: bit 1 = spawn at a tenth of the class scale */
    u8  pad84[2];
    u8  live; /* 0x86: children alive */
    u8  pad87[0x61];
    s32 slots[18]; /* 0xE8: moby indices of the pooled children, -1 = empty */
} Spawner;

typedef struct {
    f32 timer;    /* 0x00 */
    s16 duration; /* 0x04 */
    u8  pad6[0x2A];
    s32 unk30;
    s32 unk34;
} SpawnSound;

typedef struct {
    u8  pad0[0x68];
    s16 unk68;
    u8  pad6A[2];
    s32 unk6C;
    u8  pad70[0x24];
    s32 unk94;
} SpawnTarget;

extern struct Moby *D_L03_0015FFD8_gp __asm__("D_L03_0015FFD8") __attribute__((sda));
extern struct Moby *D_L03_0015FFE4;
extern s32          D_L03_0015F5CC;
extern char         spawner_spawn_failure_format[] __asm__("D_L03_001E2FA0");

extern void  debug_print() __asm__("FUN_001e93b0");
extern float normalize_angle(float) __asm__("FUN_L00_0025e310");

extern int   coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern int   FUN_0022da68(int, int, void *);
extern float FUN_002132a8(float, float);
extern void  jet_animate(void *, float) __asm__("FUN_L00_00259fe8");
extern void  FUN_0020c828(void *);
extern CollisionHit D_L03_00173F40 NOT_SDA;

/* Spawner: revives the first free pooled child (of class oclass, -1 = any) at a copy of at, counts the live children, returns the revived one or NULL. */

struct Moby *revive_spawner_pooled_child(struct Moby *spawner_moby,
                                         Vec4        *at,
                                         s32          oclass) __asm__("FUN_L03_002bec60");

struct Moby *revive_spawner_pooled_child(struct Moby *spawner_moby, Vec4 *at, s32 oclass) {
    Vec4         pos;
    Vec4        *spawn_position_source;
    Spawner     *spawner_data;
    struct Moby *current_moby;
    struct Moby *spawned;
    SpawnClass  *spawned_moby_class;
    SpawnTarget *target;
    SpawnSound  *sound;
    s32          found;
    s32          i;
    s32          pooled_child_moby_index;
    u16          flags;

    spawn_position_source = &pos;
    pos.q = at->q;
    if (spawner_moby == NULL || (spawner_data = (Spawner *)spawner_moby->pvars) == NULL) {
        return NULL;
    }
    spawner_data->live = 0;
    found = 0;
    spawned = NULL;
    for (i = 0; i < 18; i++) {
        pooled_child_moby_index = spawner_data->slots[i];
        if (pooled_child_moby_index == -1) {
            continue;
        }
        current_moby = &D_L03_0015FFD8_gp[pooled_child_moby_index];
        if (current_moby != NULL && current_moby->state != 0xFE && current_moby->state != 0xFD) {
            spawner_data->live++;
            continue;
        }
        if (oclass != -1 && oclass != current_moby->oclass) {
            continue;
        }
        if (found) {
            continue;
        }
        spawner_data->live++;
        found = 1;
        qcopy(&current_moby->pos, spawn_position_source);
        current_moby->pos.w = 0.0f;
        current_moby->state = 0;
        current_moby->command = 0;
        spawned_moby_class = (SpawnClass *)current_moby->pclass;
        flags = spawned_moby_class->flags;
        current_moby->flags = flags;
        current_moby->scale = spawned_moby_class->scale;
        current_moby->update_dist = spawner_moby->update_dist;
        current_moby->draw_dist = spawner_moby->draw_dist;
        current_moby->visible = found;
        if (spawned_moby_class->unk40 != 0) {
            current_moby->flags = flags | 0x10;
        }
        if (((SpawnClass *)current_moby->pclass)->always_draw != 0) {
            current_moby->flags |= 0x400;
        }
        current_moby->unk71 = 0xFF;
        current_moby->occlusion = 0x7F80;
        current_moby->unk72 = 0xFF;
        current_moby->hit_slot = 0xFF;
        if (spawner_data->options & 2) {
            current_moby->scale = ((SpawnClass *)current_moby->pclass)->scale * 0.1f;
        }
        set_moby_animation(current_moby, 0, 0);
        target = get_moby_suck_cannon_record(current_moby);
        if (target != NULL) {
            target->unk94 = 0;
            target->unk68 = 0;
            target->unk6C = 0;
        }
        if (spawner_moby->unkB4 < current_moby->unkB4) {
            current_moby->unkB4 = spawner_moby->unkB4;
        }
        if (current_moby->unkB4 <= 0) {
            current_moby->unkB4 = 1;
        }
        spawner_moby->unkB4 -= current_moby->unkB4;
        if (spawner_moby->unkB4 < 0) {
            spawner_moby->unkB4 = 1;
        }
        current_moby->collision = ((SpawnClass *)current_moby->pclass)->unk10;
        copy_hero_moby_field_38(current_moby);
        update_entity(current_moby);
        spawned = current_moby;
        sound = get_moby_target_info(spawned);
        if (sound != NULL) {
            sound->unk34 = 0;
            sound->unk30 = 0;
            sound->timer = sound->duration;
        }
    }
    for (current_moby = D_L03_0015FFE4; current_moby != NULL; current_moby = current_moby->next) {
        if (current_moby->oclass == 0x221 && current_moby->state != 0xFE &&
            current_moby->state != 0xFD) {
            spawner_data->live++;
        }
    }
    if (spawned == NULL) {
        debug_print(spawner_spawn_failure_format,
                    D_L03_0015F5CC,
                    (s16)spawner_moby->save_id,
                    spawner_moby->oclass);
    }
    return spawned;
}

/* Drifts and bounces off walls (state 0), lands and spawns its child moby (1), then shrinks while the child grows (2). */

void FUN_L03_002c4718(struct Moby *moby) {
    float       *vars;
    Vec4        *pos;
    struct Moby *other;
    Vec4         old_pos;
    Vec4         next_pos;
    Vec4         delta;
    int          old_disable, old_owner_disable, old_collision, old_owner_collision;
    float        angle;
    float        distance;

    if (moby == NULL)
        return;
    vars = (float *)moby->pvars;
    if (vars == NULL)
        return;
    pos = (Vec4 *)&moby->pos;
    fast_dec_timer(vars + 8);
    qcopy(&old_pos, pos);
    switch (moby->state) {
    case 0: {
        moby->scale += (moby->pclass->scale - moby->scale) * (frame_scale * 0.05f);
        fast_vec_add(pos, pos, vars);
        fast_vec_normalize(&next_pos, vars, 0.2f);
        fast_vec_add(&next_pos, &next_pos, pos);
        old_disable = moby->collision_disable;
        old_collision = moby->collision;
        old_owner_disable = (*(struct Moby **)(vars + 4))->collision_disable;
        old_owner_collision = (*(struct Moby **)(vars + 4))->collision;
        moby->collision_disable = -1;
        (*(struct Moby **)(vars + 4))->collision_disable = -1;
        moby->collision = 0;
        (*(struct Moby **)(vars + 4))->collision = 0;
        if (collision_line(&old_pos, &next_pos, 4, moby, 0)) {
            fast_vec_sub(&next_pos, &D_L03_00173F40.point, &next_pos);
            fast_vec_normalize(&next_pos, &next_pos, 0.215f);
            fast_vec_add(pos, pos, &next_pos);
            if (D_L03_00173F40.moby == 0) {
                reflect_vector(vars, vars, &D_L03_00173F40.normal_x);
                angle = vector_length_xy(&D_L03_00173F40.normal_x);
                if (fast_arc_tan(D_L03_00173F40.normal_z, angle) < 0.6981317f &&
                    collision_hit_material() == -1) {
                    fast_vec_scale(vars, vars, 0.5f);
                    if (fast_vec_length(vars) < 0.025f) {
                        moby->state = 1;
                        goto first_collision_timer;
                    }
                }
            } else if (((struct Moby *)D_L03_00173F40.moby)->oclass != 0x221) {
                reflect_vector(vars, vars, &D_L03_00173F40.normal_x);
                angle = vector_length_xy(&D_L03_00173F40.normal_x);
                if (fast_arc_tan(D_L03_00173F40.normal_z, angle) < 0.6981317f &&
                    collision_hit_material() == -1)
                    fast_vec_scale(vars, vars, 1.2f);
            } else {
                vars[2] *= FUN_002132a8(0.9f, 1.1f);
            }
        first_collision_timer:
            if (*(int *)(vars + 8) == 0) {
                FUN_0022da68(0, 0, moby);
                *(int *)(vars + 8) = 12;
            }
        }
        if (coll_sphere(0.2f, (Vec4 *)&moby->pos, 4, moby)) {
            fast_vec_sub(&next_pos, &D_L03_00173F40.point, &D_L03_00173F40.pad30);
            fast_vec_normalize(&next_pos, &next_pos, 0.015f);
            fast_vec_add((Vec4 *)&moby->pos, &D_L03_00173F40.pad30, &next_pos);
            if (D_L03_00173F40.moby == 0) {
                reflect_vector(vars, vars, &D_L03_00173F40.normal_x);
                angle = vector_length_xy(&D_L03_00173F40.normal_x);
                if (fast_arc_tan(D_L03_00173F40.normal_z, angle) < 0.6981317f &&
                    collision_hit_material() == -1) {
                    fast_vec_scale(vars, vars, 0.5f);
                    if (fast_vec_length(vars) < 0.025f)
                        moby->state = 1;
                }
            } else if (((struct Moby *)D_L03_00173F40.moby)->oclass != 0x221) {
                reflect_vector(vars, vars, &D_L03_00173F40.normal_x);
                angle = vector_length_xy(&D_L03_00173F40.normal_x);
                if (fast_arc_tan(D_L03_00173F40.normal_z, angle) < 0.6981317f &&
                    collision_hit_material() == -1)
                    fast_vec_scale(vars, vars, 1.2f);
            } else {
                vars[2] *= FUN_002132a8(0.9f, 1.1f);
            }
            if (*(int *)(vars + 8) == 0) {
                FUN_0022da68(0, 0, moby);
                *(int *)(vars + 8) = 12;
            }
        }
        for (other = D_L03_0015FFE4; other != NULL; other = other->next) {
            if (other == moby || (int)other == 0 || other->state == 0xfe || other->state == 0xfd)
                continue;
            fast_vec_sub(&delta, (Vec4 *)&moby->pos, &other->pos);
            delta.f[2] = 0.0f;
            distance = fast_vec_length(&delta);
            if (distance < 1.8f) {
                fast_vec_scale(&delta, &delta, (1.8f - distance) * (frame_scale * -0.5f + 1.0f));
                fast_vec_add((Vec4 *)&moby->pos, (Vec4 *)&moby->pos, &delta);
            }
        }
        if (fast_dec_timer_s16(vars + 5))
            moby->state = 2;
        moby->rot.x = normalize_angle(vars[0]);
        moby->rot.y = normalize_angle(vars[1]);
        moby->collision_disable = old_disable;
        (*(struct Moby **)(vars + 4))->collision_disable = old_owner_disable;
        moby->collision = old_collision;
        (*(struct Moby **)(vars + 4))->collision = old_owner_collision;
        vars[2] -= frame_time_sq * 9.8f;
        moby->rot.x += frame_scale * 0.01f;
        moby->rot.y += frame_scale * 0.02f;
        break;
    }
    case 1: {
        float height = probe_ground_height(pos, 0, 0.5f);
        moby->pos.w = 0.0f;
        moby->pos.z = height + 0.2f;
        next_pos.q = *(u128 *)pos;
        *(struct Moby **)(vars + 6) =
            revive_spawner_pooled_child(*(struct Moby **)(vars + 4), &next_pos, -1);
        moby->state = 2;
        break;
    }
    case 2: {
        struct Moby *other;
        float        rate = frame_scale * 0.05f;
        moby->scale -= moby->scale * rate;
        other = *(struct Moby **)(vars + 6);
        if (other != NULL && (*(int *)(vars + 7) & 4))
            other->scale += (other->pclass->scale - other->scale) * rate;
        if (moby->scale < moby->pclass->scale * (frame_scale * 0.05f)) {
            moby->scale = 0.0001f;
            other = *(struct Moby **)(vars + 6);
            if (other == NULL || other->state == 0xfe || other->state == 0xfd ||
                !(*(int *)(vars + 7) & 4) || other->pclass->scale - 0.01f <= other->scale)
                goto remove_moby;
        }
    }
    }
    jet_animate(moby, moby->scale * 0.3f / moby->pclass->scale);
    if (moby->pos.z < 5.0f || moby->pos.z > 500.0f) {
    remove_moby:
        FUN_0020c828(moby);
        return;
    }
    if (moby->pos.x < 5.0f)
        moby->pos.x = 5.0f;
    /* Fakematch: the do-while only weights moby's uses for the register allocator, so moby stays in $18 like retail. */
    if (moby->pos.x > 1018.0f)
        do {
            moby->pos.x = 1018.0f;
        } while (0);
    if (moby->pos.y < 5.0f)
        moby->pos.y = 5.0f;
    if (moby->pos.y > 1018.0f)
        moby->pos.y = 1018.0f;
    if (moby->pos.z < 5.0f)
        moby->pos.z = 5.0f;
    if (moby->pos.z > 1018.0f)
        moby->pos.z = 1018.0f;
    return;
}
