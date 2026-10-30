/* Shared moby class spawner_egg: the bubble a spawner lobs, which settles and releases a moby, and the function that spawns it. */

#include "types.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/weapons/suck_aim.h"
#include "rnc/mobys/animation/set_moby_animation.h"
#include "rnc/overlay/shared/hero/copy_hero_moby_field_38.h"
#include "rnc/overlay/shared/weapons/suckcannon.h"
#include "rnc/overlay/shared/math/interpolation/spring_turn.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/world/collision/collision_hit_material.h"
#include "rnc/world/collision/collision_line.h"
#include "rnc/overlay/collision.h"

/* pvars of the class 0x75E bubble FUN_L09_0030b218 spawns */
typedef struct {
    Vec4         vel;   /* 0x00 */
    struct Moby *owner; /* 0x10: class 0x75D spawner; its pvars hold the bubble list at 0xB0 */
    struct Moby *held;  /* 0x14: from FUN_L09_0030a778 when the bubble settles */
    s16          life;  /* 0x18 */
    u8           unk1A;
    u8           sound_delay; /* 0x1B */
    u8           slot;        /* 0x1C: index in the owner's bubble list */
} BubbleVars;

typedef struct {
    u8           pad0[0xB0];
    struct Moby *bubbles[20];
} BubbleOwnerVars;

extern void debug_print(char *, s32, s32) __asm__("FUN_001e93b0");
extern s32  coll_sphere(f32, void *, s32, struct Moby *) __asm__("FUN_L00_001f0d60");
extern void FUN_L00_001fefc8(void *);
extern void jet_animate(struct Moby *, f32) __asm__("FUN_L00_00259fe8");
extern f32  normalize_angle(f32) __asm__("FUN_L00_0025e310");

extern char         spawner_child_respawn_failure_format[] __asm__("D_L09_00209280");
extern CollisionHit D_L09_00173FC0;

static inline void bubble_bounce(struct Moby *moby, BubbleVars *vars) {
    CollisionHit *hit = &D_L09_00173FC0;

    if (hit->moby == 0) {
        reflect_vector(&vars->vel, &vars->vel, &hit->normal_x);
        if (fast_arc_tan(hit->normal_z, vector_length_xy(&hit->normal_x)) < 0.6981317f &&
            collision_hit_material() == -1) {
            fast_vec_scale(&vars->vel, &vars->vel, 0.5f);
            if (fast_vec_length(&vars->vel) < 0.025f) {
                moby->state = 1;
            }
        }
    } else if (((struct Moby *)hit->moby)->oclass != 0x75E) {
        reflect_vector(&vars->vel, &vars->vel, &hit->normal_x);
        if (fast_arc_tan(hit->normal_z, vector_length_xy(&hit->normal_x)) < 0.6981317f &&
            collision_hit_material() == -1) {
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

struct Moby *respawn_spawner_child(struct Moby *owner, Vec4 *pos) __asm__("FUN_L09_0030a778");

struct Moby *respawn_spawner_child(struct Moby *owner, Vec4 *pos) {
    struct Moby *spawned_moby;
    struct Moby *other;
    u8          *owner_vars;
    s16         *data;
    s32          left;
    Vec4         pos_copy;
    Vec4        *pos_ptr;
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
        update_entity(spawned_moby);
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

/* Spawns a moby of class 0x75E and initialises its data. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C6B30.c: func_L09_0030C658), where it is exact; names translated to the US level program. */
extern char *
spawn_spawner_egg(int, void *, void *, unsigned char, unsigned char) __asm__("FUN_L09_0030b218");

char *spawn_spawner_egg(int           owner,
                        void         *initial_position,
                        void         *initial_velocity,
                        unsigned char grow_held_moby,

                        unsigned char owner_slot_index) {
    char *spawned_moby = (char *)create_moby(0x75E);
    if (spawned_moby) {
        char *moby_vars = ((char *)MOBY(spawned_moby)->pvars);
        *(int *)(moby_vars + 0x10) = owner;
        qcopy(spawned_moby + 0x10, initial_position);
        qcopy(moby_vars, initial_velocity);
        MOBY(spawned_moby)->rot.x = random_float_between(-3.1415927f, 3.1415927f);
        MOBY(spawned_moby)->rot.y = random_float_between(-3.1415927f, 3.1415927f);
        *(int *)(moby_vars + 0x14) = 0;
        *(short *)(moby_vars + 0x18) = scale_game_frames(500);
        MOBY(spawned_moby)->update_dist = 0xFF;
        MOBY(spawned_moby)->draw_dist = 0x40;
        spawned_moby[0x31] = 1;
        spawned_moby[0x20] = 0;
        MOBY(spawned_moby)->scale = *(float *)(((char *)MOBY(spawned_moby)->pclass) + 0x24) * 0.3f;
        moby_vars[0x1C] = owner_slot_index;
        moby_vars[0x1A] = grow_held_moby;
        moby_vars[0x1B] = 0;
        update_entity(spawned_moby);
    }
    return spawned_moby;
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002C6B30.c: func_L09_0030C790), where it is exact; names translated to the US level program. */
void remove_spawner_egg(void *moby, char *data) __asm__("FUN_L09_0030b350");

void remove_spawner_egg(void *moby, char *data) {
    char *other = *(char **)(data + 0x10);
    if (other != 0 && *(short *)(other + 0xA6) == 0x75D && (unsigned char)other[0x20] != 0xFE &&
        (unsigned char)other[0x20] != 0xFD) {
        int   index = (unsigned char)data[0x1C];
        char *base = *(char **)(other + 0x78);
        base += index * 4;
        *(int *)(base + 0xB0) = 0;
    }
    mark_moby_for_removal(moby);
}

/* Bubble update: drifts, bounces off level geometry and other bubbles, settles on the ground, then shrinks away. */
void spawner_egg_update(struct Moby *moby) __asm__("FUN_L09_0030b3b0");

void spawner_egg_update(struct Moby *moby) {
    enum {
        SPAWNER_EGG_STATE_MOVING = 0,
        SPAWNER_EGG_STATE_RESPAWN_CHILD = 1,
        SPAWNER_EGG_STATE_SHRINKING = 2
    };
    BubbleVars      *vars = (BubbleVars *)moby->pvars;
    Vec4             previous_position;
    Vec4             step;
    Vec4             away;
    BubbleOwnerVars *owner;
    struct Moby     *other;
    struct Moby     *held;
    s32              i;
    f32              other_bubble_distance_xy;
    f32              rate;

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
                    fast_vec_scale(&away,
                                   &away,
                                   (1.8f - other_bubble_distance_xy) *
                                       (frame_scale * -0.5f + 1.0f));
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
